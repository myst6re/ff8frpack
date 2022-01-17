/****************************************************************************
 ** FF8frPack FF8.fr Pack configurator and installer
 ** Copyright (C) 2022 Arzel Jérôme <myst6re@gmail.com>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "window.h"

#include <QFileDialog>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedLayout>
#include <QStyle>

#include "core/ffnxinstallation.h"
#include "core/process.h"
#include "core/provisionner.h"
#include "pages/configpage.h"
#include "pages/errorpage.h"
#include "pages/welcomepage.h"
#include "widgets/helpwidget.h"
#include "widgets/resizablelabel.h"
#include "widgets/flatcombobox.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    _image = new ResizableLabel(this);
    _image->setPixmap(QPixmap(":/images/background.jpg"));
    _currentInstallation = new FlatComboBox(this);
    _currentInstallation->setDefaultText(style()->standardIcon(QStyle::SP_MessageBoxWarning), tr("Not found"));
    QPushButton *changeInstallationButton = new QPushButton(tr("Set path..."));
    _provisionButton = new QPushButton(tr("Patch the game"));
    _provisionButton->setIcon(style()->standardPixmap(QStyle::SP_VistaShield));
    _provisionButton->setDisabled(true);

    _welcomePage = new WelcomePage(this);
    _configPage = new ConfigPage(this);
    _errorPage = new ErrorPage(height() / 4, this);

    _stackedLayout = new QStackedLayout;
    _stackedLayout->addWidget(_welcomePage);
    _stackedLayout->addWidget(_configPage);
    _stackedLayout->addWidget(_errorPage);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(_image, 0, 0, 3, 1, Qt::AlignTop);
    layout->addWidget(new QLabel(tr("FF8 Installation:")), 0, 1);
    layout->addWidget(_currentInstallation, 0, 2);
    layout->addWidget(changeInstallationButton, 0, 3);
    layout->addWidget(_provisionButton, 0, 4);
    layout->addLayout(_stackedLayout, 1, 1, 1, 4);
    layout->setRowStretch(1, 1);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(2, 1);

    connect(_configPage, &ConfigPage::saveClicked, this, &Window::save);
    connect(_configPage, &ConfigPage::resetClicked, this, &Window::reset);
    connect(_errorPage, &ErrorPage::closeClicked, this, &Window::closeError);
    connect(changeInstallationButton, &QPushButton::clicked, this, &Window::customiseFF8Installation);
    connect(_provisionButton, &QPushButton::clicked, this, &Window::patchFF8Installation);
    connect(_currentInstallation, &FlatComboBox::currentIndexChanged, this, &Window::setCurrentInstallation);

    load();
}

void Window::load()
{
    _installations = FF8Installation::installations();

    if (_installations.isEmpty()) {
        _configPage->setEnabled(false);
        return;
    }

    QFileIconProvider iconProvider;
    for (const FF8Installation &inst : _installations) {
        _currentInstallation->addItem(
                    iconProvider.icon(QFileInfo(inst.exePath())),
                    inst.typeString(),
                    inst.appPath()
                    );
    }
}

void Window::save()
{
    _configPage->save(_config);

    if (! _config.save()) {
        showError(tr("An error occured when saving the FFNx.toml file:\n") + _config.errorString());
        return;
    }
}

void Window::reset()
{
    load();
}

void Window::customiseFF8Installation()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("FF8 Installation directory"), QString(), QFileDialog::DontConfirmOverwrite);

    if (directory.isNull()) {
        return;
    }

    FF8Installation inst = FF8Installation::fromPath(directory);

    if (inst.isValid()) {
        QFileIconProvider iconProvider;
        _currentInstallation->addItem(
                    iconProvider.icon(QFileInfo(inst.exePath())),
                    tr("%1 (manual)").arg(inst.typeString()),
                    inst.appPath());
    } else {
        QFileIconProvider iconProvider;
        _currentInstallation->addItem(
                    iconProvider.icon(QFileInfo(inst.exePath())),
                    tr("Manual"),
                    inst.appPath());
    }
}

void Window::patchFF8Installation()
{
    bool ok = false;
    QString ff8Dir(_currentInstallation->currentData().toString());
    FF8frPackInstallation ff8frPackInst = FF8frPackInstallation::localInstallation();
    if (! Provisionner(ff8frPackInst, ff8Dir).provision()) {
        // Retry in admin mode
        Process::ExecuteStatus status = Process::executeElevated(ff8frPackInst.patcherFileName(), QStringList() << ff8frPackInst.dirName() << ff8Dir);
        if (status == Process::Cancelled) {
            showError(tr("Patching cancelled"));
        } else if (status != Process::Ok) {
            showError(tr("Cannot patch the game, maybe you don't have enough permission"));
        } else {
            ok = true;
        }
    } else {
        ok = true;
    }

    if (ok) {
        _provisionButton->setText(tr("Unpatch the game"));
    }
}

void Window::setCurrentInstallation(int index)
{
    Q_UNUSED(index)

    QString dir = _currentInstallation->currentData().toString();
    FFNxInstallation ffnxInstallation(dir);
    bool isValid = ffnxInstallation.isValid();

    _provisionButton->setEnabled(! dir.isEmpty());

    if (isValid) {
        _config.setFileName(ffnxInstallation.configFileName());

        if (! _config.load()) {
            showError(tr("An error occured when parsing the FFNx.toml file:\n") + _config.errorString());
            return;
        }

        _configPage->load(_config);

        _stackedLayout->setCurrentWidget(_configPage);
    } else {
        _stackedLayout->setCurrentWidget(_welcomePage);
    }

    _configPage->setEnabled(isValid);
}

void Window::showError(const QString &errorStr)
{
    _errorPage->setText(errorStr);
    _stackedLayout->setCurrentWidget(_errorPage);
    _configPage->setEnabled(false);
}

void Window::closeError()
{
    if (_configPage->isEnabled()) {
        _stackedLayout->setCurrentWidget(_configPage);
    } else {
        _stackedLayout->setCurrentWidget(_welcomePage);
    }
}

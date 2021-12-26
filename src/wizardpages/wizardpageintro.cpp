/****************************************************************************
 ** FF8frPack FF8.fr Pack configurator and installer
 ** Copyright (C) 2021 Arzel Jérôme <myst6re@gmail.com>
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
#include "wizardpageintro.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#include "../wizard.h"

WizardPageIntro::WizardPageIntro(QWidget *parent) :
    QWizardPage(parent), _installations(FF8Installation::installations())
{
    setTitle(tr("Installation"));
    setCommitPage(true);

    _lineEdit = new QLineEdit(this);
    registerField(MANDATORY_FIELD(FieldPath), _lineEdit);
    QPushButton *select = new QPushButton(tr("Choose..."), this);

    QHBoxLayout *selectFileLayout = new QHBoxLayout;
    selectFileLayout->addWidget(_lineEdit, 1);
    selectFileLayout->addWidget(select);
    selectFileLayout->setContentsMargins(QMargins());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(1);
    layout->addWidget(new QLabel(tr("Final Fantasy VIII directory:")));
    layout->addLayout(selectFileLayout);

    connect(select, &QPushButton::clicked, this, &WizardPageIntro::chooseFile);
}

void WizardPageIntro::initializePage()
{
    if (_lineEdit->text().isEmpty()) {
        if (_installations.contains(FF8Installation::Steam)) {
            _lineEdit->setText(_installations[FF8Installation::Steam].appPath());
        } else if (_installations.contains(FF8Installation::Standard)) {
            _lineEdit->setText(_installations[FF8Installation::Standard].appPath());
        } else {
            _lineEdit->setText(QCoreApplication::applicationDirPath());
        }
    }
}

void WizardPageIntro::chooseFile()
{
    const QString directory = QFileDialog::getExistingDirectory(
                this,
                tr("Choose Final Fantasy VIII installation directory"),
                _lineEdit->text(),
                QFileDialog::Options());

    if (directory.isNull()) {
        return;
    }

    _lineEdit->setText(directory);
}

bool WizardPageIntro::validatePage()
{
    if (!QFileInfo(_lineEdit->text()).isDir()) {
        QMessageBox::warning(
                    this,
                    tr("Wrong path"),
                    tr("Please set an existing directory where Final Fantasy VIII is installed"));
        return false;
    }

    return true;
}

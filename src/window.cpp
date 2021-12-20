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
#include "window.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QStackedLayout>

#include "helpwidget.h"

Window::Window(const FFNxInstallation &ffnxInstallation, QWidget *parent)
    : QWidget(parent), _installation(ffnxInstallation)
{
    _stackedLayout = new QStackedLayout(this);
    _stackedLayout->addWidget(buildConfigPage());
    _stackedLayout->addWidget(buildErrorPage());

    load();
}

QWidget *Window::buildConfigPage()
{
    QWidget *configPage = new QWidget(this);

    _rendering = new QComboBox(configPage);
    _rendering->addItem(tr("Auto (recommended)"), FFNxConfig::RendererAuto);
    _rendering->addItem(tr("OpenGL"), FFNxConfig::OpenGL);
    _rendering->addItem(tr("Direct3D9 (not recommended)"), FFNxConfig::Direct3D9);
    _rendering->addItem(tr("Direct3D11"), FFNxConfig::Direct3D11);
    _rendering->addItem(tr("Direct3D12 (not recommended)"), FFNxConfig::Direct3D12);
    _rendering->addItem(tr("Vulkan"), FFNxConfig::Vulkan);

    _fullscreen = new QComboBox(configPage);
    _fullscreen->addItem(tr("Enabled"), true);
    _fullscreen->addItem(tr("Disabled"), false);

    _highResFont = new QComboBox(configPage);
    _highResFont->addItem(tr("High Resolution"), true);
    _highResFont->addItem(tr("Low Resolution"), false);

    _externalMusic = new QComboBox(configPage);
    _externalMusic->addItem(tr("Enabled"), true);
    _externalMusic->addItem(tr("Disabled"), false);

    _buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Save | QDialogButtonBox::Reset | QDialogButtonBox::RestoreDefaults,
                configPage);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Rendering"), _rendering);
    formLayout->addRow(tr("Fullscreen"), _fullscreen);
    formLayout->addRow(tr("Font"), _highResFont);
    formLayout->addRow(tr("Improved music"), _externalMusic);

    QVBoxLayout *layout = new QVBoxLayout(configPage);
    layout->addLayout(formLayout);
    layout->addWidget(_buttonBox);

    connect(_buttonBox, &QDialogButtonBox::clicked, this, [&](QAbstractButton *button) {
        switch (_buttonBox->standardButton(button)) {
        case QDialogButtonBox::Save:
            save();
            break;
        case QDialogButtonBox::Reset:
            reset();
            break;
        case QDialogButtonBox::RestoreDefaults:
            restoreDefaults();
            break;
        default:
            qWarning() << "Button pressed" << _buttonBox->buttonRole(button);
            break;
        }
    });

    return configPage;
}

QWidget *Window::buildErrorPage()
{
    QWidget *errorPage = new QWidget(this);

    _helpWidget = new HelpWidget(height() / 4,
                                            QString(),
                                            HelpWidget::IconWarning,
                                            errorPage);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Close,
                errorPage);

    QVBoxLayout *layout = new QVBoxLayout(errorPage);
    layout->addWidget(_helpWidget);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::clicked, this, &Window::close);

    return errorPage;
}

void Window::load()
{
    _config.setFileName(_installation.configFileName());

    if (! _config.load()) {
        showError(tr("An error occured when parsing the FFNx.toml file:\n") + _config.errorString());
        return;
    }

    _stackedLayout->widget(0)->setEnabled(true);
    _stackedLayout->setCurrentIndex(0);

    bool ok = false;
    FFNxConfig::Renderer renderer = _config.renderer(ok);
    int index = _rendering->findData(renderer);
    if (!ok || index < 0) {
        _rendering->addItem(tr("Unknown"), renderer);
        if (index < 0) {
            index = _rendering->count() - 1;
        }
    }

    _rendering->setCurrentIndex(index);

    _fullscreen->setCurrentIndex(_fullscreen->findData(_config.isFullscreen()));
    _externalMusic->setCurrentIndex(_externalMusic->findData(_config.isExternalMusicEnabled()));
}

void Window::save()
{
    _config.setRenderer(FFNxConfig::Renderer(_rendering->currentData().toInt()));
    _config.setFullscreen(_fullscreen->currentData().toBool());
    _config.setExternalMusicEnabled(_externalMusic->currentData().toBool());

    if (! _config.save()) {
        showError(tr("An error occured when save the FFNx.toml file:\n") + _config.errorString());
        return;
    }
}

void Window::reset()
{
    load();
}

void Window::restoreDefaults()
{
    _rendering->setCurrentIndex(0);
    _fullscreen->setCurrentIndex(0);
    _externalMusic->setCurrentIndex(0);
}

void Window::showError(const QString &errorStr)
{
    _helpWidget->setText(errorStr);
    _stackedLayout->setCurrentIndex(1);
    _stackedLayout->widget(0)->setEnabled(false);
}

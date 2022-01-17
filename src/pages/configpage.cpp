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
#include "configpage.h"

#include <QAbstractButton>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

#include "../core/ffnxconfig.h"
#include "../core/ffnxinstallation.h"

ConfigPage::ConfigPage(QWidget *parent) :
    QWidget(parent)
{
    _rendering = new QComboBox(this);
    _rendering->addItem(tr("Auto (recommended)"), FFNxConfig::RendererAuto);
    _rendering->addItem(tr("OpenGL"), FFNxConfig::OpenGL);
    _rendering->addItem(tr("Direct3D9 (not recommended)"), FFNxConfig::Direct3D9);
    _rendering->addItem(tr("Direct3D11"), FFNxConfig::Direct3D11);
    _rendering->addItem(tr("Direct3D12 (not recommended)"), FFNxConfig::Direct3D12);
    _rendering->addItem(tr("Vulkan"), FFNxConfig::Vulkan);

    _fullscreen = new QComboBox(this);
    _fullscreen->addItem(tr("Enabled"), true);
    _fullscreen->addItem(tr("Disabled"), false);

    _highResFont = new QComboBox(this);
    _highResFont->addItem(tr("High Resolution"), true);
    _highResFont->addItem(tr("Low Resolution"), false);

    _externalMusic = new QComboBox(this);
    _externalMusic->addItem(tr("Enabled"), true);
    _externalMusic->addItem(tr("Disabled"), false);

    _buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Save | QDialogButtonBox::Reset | QDialogButtonBox::RestoreDefaults,
                this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Rendering"), _rendering);
    formLayout->addRow(tr("Fullscreen"), _fullscreen);
    formLayout->addRow(tr("Font"), _highResFont);
    formLayout->addRow(tr("Improved music"), _externalMusic);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(formLayout);
    layout->addWidget(_buttonBox);

    connect(_buttonBox, &QDialogButtonBox::clicked, this, [&](QAbstractButton *button) {
        switch (_buttonBox->standardButton(button)) {
        case QDialogButtonBox::Save:
            emit saveClicked();
            break;
        case QDialogButtonBox::Reset:
            emit resetClicked();
            break;
        case QDialogButtonBox::RestoreDefaults:
            restoreDefaults();
            break;
        default:
            qWarning() << "Button pressed" << _buttonBox->buttonRole(button);
            break;
        }
    });
}

void ConfigPage::load(const FFNxConfig &config)
{
    bool ok = false;
    FFNxConfig::Renderer renderer = config.renderer(ok);
    int index = _rendering->findData(renderer);
    if (!ok || index < 0) {
        _rendering->addItem(tr("Unknown"), renderer);
        if (index < 0) {
            index = _rendering->count() - 1;
        }
    }

    _rendering->setCurrentIndex(index);

    _fullscreen->setCurrentIndex(_fullscreen->findData(config.isFullscreen()));
    _externalMusic->setCurrentIndex(_externalMusic->findData(config.isExternalMusicEnabled()));
}

void ConfigPage::save(FFNxConfig &config) const
{
    config.setRenderer(FFNxConfig::Renderer(_rendering->currentData().toInt()));
    config.setFullscreen(_fullscreen->currentData().toBool());
    config.setExternalMusicEnabled(_externalMusic->currentData().toBool());
}

void ConfigPage::restoreDefaults()
{
    _rendering->setCurrentIndex(0);
    _fullscreen->setCurrentIndex(0);
    _externalMusic->setCurrentIndex(0);
}

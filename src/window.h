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
#pragma once

#include <QWidget>

#include "ffnxconfig.h"
#include "ffnxinstallation.h"

class HelpWidget;
class QComboBox;
class QDialogButtonBox;
class QStackedLayout;

class Window : public QWidget
{
    Q_OBJECT

    enum Page {
        Default = 0,
        Error
    };

public:
    Window(const FFNxInstallation &ffnxInstallation, QWidget *parent = nullptr);
public slots:
    void reset();
    void save();
    void restoreDefaults();
private:
    QWidget *buildConfigPage();
    QWidget *buildErrorPage();
    void load();
    void showError(const QString &errorStr);

    QDialogButtonBox *_buttonBox;

    QComboBox *_rendering;
    QComboBox *_fullscreen;
    QComboBox *_highResFont;
    QComboBox *_externalMusic;

    QStackedLayout *_stackedLayout;
    HelpWidget *_helpWidget;

    const FFNxInstallation &_installation;
    FFNxConfig _config;
};

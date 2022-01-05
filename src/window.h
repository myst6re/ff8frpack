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
#pragma once

#include <QList>
#include <QWidget>

#include "core/ff8installation.h"
#include "core/ffnxconfig.h"

class ConfigPage;
class ErrorPage;
class FlatComboBox;
class HelpWidget;
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QPushButton;
class QStackedLayout;
class ResizableLabel;
class WelcomePage;

class Window : public QWidget
{
    Q_OBJECT

    enum Page {
        Default = 0,
        Error
    };

public:
    Window(QWidget *parent = nullptr);
public slots:
    void reset();
    void save();
    void customiseFF8Installation();
    void patchFF8Installation();
    void setCurrentInstallation(int index);
    void closeError();
private:
    void load();
    void showError(const QString &errorStr);

    WelcomePage *_welcomePage;
    ConfigPage *_configPage;
    ErrorPage *_errorPage;

    ResizableLabel *_image;
    QPushButton *_provisionButton;

    FlatComboBox *_currentInstallation;
    QStackedLayout *_stackedLayout;

    QList<FF8Installation> _installations;
    FFNxConfig _config;
};

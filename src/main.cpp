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
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

#include "ffnxinstallation.h"
#include "window.h"
#include "wizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        if (translator.load(QLocale(locale), "FF8frPack", "_", ":/i18n/")) {
            a.installTranslator(&translator);
            break;
        }
    }

    FFNxInstallation ffnxInstallation = FFNxInstallation::localInstallation();
    if (true || !ffnxInstallation.isValid()) {
        Wizard w(FFNxInstallation::autoDetectInstallation());
        w.show();

        return a.exec();
    }

    Window w(ffnxInstallation);
    w.show();

    return a.exec();
}

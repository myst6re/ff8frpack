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
#include <QCoreApplication>
#include <QTimer>

#include "arguments.h"
#include "argumentsconfigure.h"
#include "argumentsprovision.h"
#include "../core/provisionner.h"
#include "../core/ff8frpackinstallation.h"

int commandProvision()
{
    ArgumentsProvision args;
    if (! Provisionner(FF8frPackInstallation(args.ff8frPackDirectory()), args.ff8Directory()).provision()) {
        qWarning() << qPrintable("Error when provisionning");
        return 2;
    }

    return 0;
}

int commandConfigure()
{
    ArgumentsConfigure args;


    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

    Arguments args;
    if (args.help()) {
        args.showHelp();
    }

    int ret = 0;

    switch (args.command()) {
    case Arguments::None:
        args.showHelp();
    case Arguments::Provision:
        ret = commandProvision();
        break;
    case Arguments::Configure:
        ret = commandConfigure();
        break;
    }

    if (ret != 0) {
        return ret;
    }

    if (args.isValid()) {

    } else {
        qWarning() << qPrintable("Invalid arguments");
        return 1;
    }

    QTimer::singleShot(0, &a, &QCoreApplication::quit);

    return a.exec();
}

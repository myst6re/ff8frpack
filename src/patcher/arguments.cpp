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
#include "arguments.h"

#include <QDir>
#include <QStringList>

Arguments::Arguments() :
    _valid(false)
{
    _parser.setApplicationDescription(APPLICATION_DESCRIPTION);
    _parser.addVersionOption();

    _parser.addPositionalArgument(
        "command", "Command to execute", "<command>"
    );
    _parser.addPositionalArgument("args", "", "[<args>]");
    _parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);

    parse();
}

void Arguments::parse()
{
    _valid = false;
    _parser.process(*qApp);

    QStringList args = _parser.positionalArguments();

    if (args.isEmpty()) {
        qWarning() << qPrintable("Please specify a command");
        return;
    }

    const QString &command = args.first();

    if (command == "provision") {
        _command = Provision;
    } else if (command == "configure") {
        _command = Configure;
    } else {
        qWarning() << qPrintable("Unknown command type:") << qPrintable(command);
        return;
    }

    _valid = true;
}

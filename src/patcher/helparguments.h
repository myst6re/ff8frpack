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

#include <QCommandLineParser>

#define PATCHER_ADD_ARG(names, description, valueName, defaultValue) \
    _parser.addOption(QCommandLineOption(names, description, valueName, defaultValue))

#define PATCHER_ADD_FLAG(names, description) \
    _parser.addOption(QCommandLineOption(names, description))

#define PATCHER_OPT_NAMES(shortName, fullName) \
    (QStringList() << shortName << fullName)

class HelpArguments
{
public:
    HelpArguments();
    [[ noreturn ]] void showHelp(int exitCode = EXIT_SUCCESS);
    bool help() const;
protected:
    QCommandLineParser _parser;
};


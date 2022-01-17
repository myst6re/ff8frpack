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
#include "helparguments.h"

#include <QFileInfo>
#include <QRegularExpression>

HelpArguments::HelpArguments()
{
    PATCHER_ADD_FLAG(PATCHER_OPT_NAMES("h", "help"), "Displays help.");
}

bool HelpArguments::help() const
{
    return _parser.isSet("help");
}

[[ noreturn ]] void HelpArguments::showHelp(int exitCode)
{
    QRegularExpression usage("Usage: .* \\[options\\]");
    qInfo("%s", qPrintable(_parser.helpText().replace(usage, "Usage: %1 [options]")
                         .arg(QFileInfo(qApp->arguments().first()).fileName())));
    ::exit(exitCode);
}

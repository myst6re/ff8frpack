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
#include "process.h"

#include <QProcess>
#include <QRegularExpression>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shellapi.h>
#endif

Process::ExecuteStatus Process::executeElevated(const QString &program, const QStringList &arguments)
{
    QStringList args = arguments;
#ifdef Q_OS_MACOS
    args.prepend(program);
#endif
    args.replaceInStrings("'", "'\"'\"'"); // Escape '
    args.replaceInStrings(QRegularExpression("^(.*)$"), "'\\1'"); // Enclose by '

#ifdef Q_OS_WIN32
    SHELLEXECUTEINFOW shellExecuteInfo = SHELLEXECUTEINFOW();
    shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
    shellExecuteInfo.lpVerb = L"runas";
    shellExecuteInfo.lpFile = program.toStdWString().c_str();
    shellExecuteInfo.lpParameters = args.join(" ").toStdWString().c_str();
    shellExecuteInfo.nShow = SW_NORMAL;
    if (ShellExecuteExW(&shellExecuteInfo)) {
        return Ok;
    }

    switch (GetLastError()) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_DDE_FAIL:
    case ERROR_NO_ASSOCIATION:
    case ERROR_ACCESS_DENIED:
    case ERROR_DLL_NOT_FOUND:
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_SHARING_VIOLATION:
        return NotStarted;
    case ERROR_CANCELLED:
        return Cancelled;
    }

    return Error;
#else
#ifdef Q_OS_MACOS
    QString command = args.join(" "),
             script = QString("do shell script \"%1\" with administrator privileges")
            .arg(command.replace("\"", "\\\"")); // Escape "
    switch (QProcess::execute("osascript", QStringList() << "-e" << script)) {
    case -2:
        return NotStarted;
    case -1:
        return Crashed;
    case 0:
        return Ok;
    default:
        return Error;
    }
#endif
#endif
}

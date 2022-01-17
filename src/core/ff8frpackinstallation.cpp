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
#include "ff8frpackinstallation.h"

#include <QCoreApplication>
#include <QDir>

FF8frPackInstallation::FF8frPackInstallation(const QString &dirName) :
    _dirName(dirName)
{
}

const QString &FF8frPackInstallation::dirName() const
{
    return _dirName;
}

QString FF8frPackInstallation::ffnxDirName() const
{
    return QString("%1/%2").arg(_dirName, "FFNx");
}

QString FF8frPackInstallation::patcherFileName() const
{
#ifdef Q_OS_WIN32
    return QString("%1/%2").arg(_dirName, "FF8frPackPatcher.exe");
#else
    return QString("%1/%2").arg(_dirName, "FF8frPackPatcher");
#endif
}

FF8frPackInstallation FF8frPackInstallation::localInstallation()
{
    QString applicationDirPath(QCoreApplication::applicationDirPath());
#ifdef Q_OS_MACOS
    // If we are in a bundle
    if (applicationDirPath.endsWith("MacOS", Qt::CaseInsensitive)) {
        // Change directory ../../../
        applicationDirPath = applicationDirPath.left(
                    applicationDirPath.lastIndexOf("/",
                                                   applicationDirPath.lastIndexOf("/",
                                                                                  applicationDirPath.lastIndexOf("/") - 1
                                                                                  ) - 1
                                                   )
                    );
    }
#endif
    return FF8frPackInstallation(applicationDirPath);

}

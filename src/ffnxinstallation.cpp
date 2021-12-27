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
#include "ffnxinstallation.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>

#include "geturl.h"

FFNxInstallation::FFNxInstallation(const QString &dirName) :
    _dirName(dirName)
{
}

QString FFNxInstallation::configFileName() const
{
    return QString("%1/FFNx.toml").arg(_dirName);
}

const QString &FFNxInstallation::dirName() const
{
    return _dirName;
}

QString FFNxInstallation::ff8frPackFileName() const
{
    return QString("%1/FF8.fr Pack.exe").arg(_dirName);
}

bool FFNxInstallation::isValid() const
{
    return QFile::exists(configFileName());
}

FFNxInstallation FFNxInstallation::localInstallation()
{
    FFNxInstallation currentDir(QCoreApplication::applicationDirPath());

    if (currentDir.isValid()) {
        return currentDir;
    }

    FFNxInstallation parentDir(QString("%1/..").arg(QCoreApplication::applicationDirPath()));

    if (parentDir.isValid()) {
        return parentDir;
    }

    return currentDir;
}

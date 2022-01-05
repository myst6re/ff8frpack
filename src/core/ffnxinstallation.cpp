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
#include "ffnxinstallation.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>

FFNxInstallation::FFNxInstallation(const QString &dirName) :
    _dirName(dirName)
{
}

QString FFNxInstallation::patcherFileName() const
{
#ifdef Q_OS_WIN32
    return QString("%1/%2").arg(_dirName, "FF8frPackPatcher.exe");
#else
    return QString("%1/%2").arg(_dirName, "FF8frPackPatcher");
#endif
}

QString FFNxInstallation::configFileName() const
{
    return QString("%1/FFNx.toml").arg(_dirName);
}

QString FFNxInstallation::installedFlagFileName() const
{
    return QString("%1/FF8frPack.installed").arg(_dirName);
}

const QString &FFNxInstallation::dirName() const
{
    return _dirName;
}

bool FFNxInstallation::isValid() const
{
    return QFile::exists(configFileName());
}

bool FFNxInstallation::isEnabled() const
{
    return QFile::exists(configFileName());
}

FFNxInstallation FFNxInstallation::fromFF8Installation(const FF8Installation &ff8Installation)
{
    return FFNxInstallation(ff8Installation.appPath());
}

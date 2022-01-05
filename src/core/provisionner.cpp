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
#include "provisionner.h"

#include <QDir>
#include <QFile>

#include "ffnxinstallation.h"

Provisionner::Provisionner(const FF8frPackInstallation &ff8frPackInstallation, const QString &ff8DirName) :
    _ff8frPackInstallation(ff8frPackInstallation), _ff8DirName(ff8DirName)
{
}

bool Provisionner::provision() const
{
    // Install FFNx
    if (! backupFiles(QStringList() << "AF3DN.P")) {
        return false;
    }

    if (! copyDirectory(_ff8frPackInstallation.ffnxDirName(), _ff8DirName)) {
        unprovision();
        return false;
    }

    FFNxInstallation ffnxInstallation(_ff8DirName);

    if (! touchFile(ffnxInstallation.installedFlagFileName())) {
        unprovision();
        return false;
    }

#ifdef Q_OS_WIN32
    if (! copyFile(_ff8frPackInstallation.patcherFileName(), ffnxInstallation.patcherFileName())) {
        unprovision();
        return false;
    }
#endif

    return true;
}

bool Provisionner::isProvisionned() const
{
    FFNxInstallation ffnxInstallation(_ff8DirName);
    return QFile::exists(ffnxInstallation.installedFlagFileName());
}

bool Provisionner::unprovision() const
{
    if (! unbackupFiles(QStringList() << "AF3DN.P")) {
        return false;
    }

    FFNxInstallation ffnxInstallation(_ff8DirName);

    if (! QFile::remove(ffnxInstallation.installedFlagFileName())) {
        return false;
    }

    return true;
}

bool Provisionner::copyDirectory(const QString &sourceDirName, const QString &destinationDirName) const
{
    QDir sourceDir(sourceDirName), destinationDir(destinationDirName);
    if (! sourceDir.exists()) {
        return false;
    }

    if (! destinationDir.exists() && ! destinationDir.mkdir(".")) {
        return false;
    }

    for (const QString &d : sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (! copyDirectory(sourceDir.filePath(d), destinationDir.filePath(d))) {
            return false;
        }
    }

    for (const QString &f : sourceDir.entryList(QDir::Files)) {
        if (! QFile::copy(sourceDir.filePath(f), destinationDir.filePath(f))) {
            return false;
        }
    }

    return true;
}

bool Provisionner::backupFiles(const QStringList &names) const
{
    for (const QString &name : names) {
        QString fileName = QString("%1/%2").arg(_ff8DirName, name),
                backupFileName = QString("%1/%2.backup").arg(_ff8DirName, name);
        if (QFile::exists(fileName) && ! QFile::exists(backupFileName)
                && ! QFile::copy(fileName, backupFileName)) {
            return false;
        }
    }

    return true;
}

bool Provisionner::unbackupFiles(const QStringList &names) const
{
    for (const QString &name : names) {
        QString fileName = QString("%1/%2").arg(_ff8DirName, name),
                backupFileName = QString("%1/%2.backup").arg(_ff8DirName, name);
        if (QFile::exists(backupFileName) && QFile::remove(fileName)
                && ! QFile::copy(backupFileName, fileName)) {
            return false;
        }
    }

    return true;
}

bool Provisionner::touchFile(const QString &fileName) const
{
    return QFile(fileName).open(QIODevice::WriteOnly);
}

bool Provisionner::copyFile(const QString &sourceFileName, const QString &destinationFileName) const
{
    if (QFile::exists(destinationFileName) && ! QFile::remove(destinationFileName)) {
        return false;
    }
    return QFile::copy(sourceFileName, destinationFileName);
}

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

#include <QObject>
#include <QString>
#include <QStringList>

#include "ff8frpackinstallation.h"

class Provisionner : public QObject
{
    Q_OBJECT
public:
    explicit Provisionner(const FF8frPackInstallation &ff8frPackInstallation, const QString &ff8DirName);
    bool provision() const;
    bool isProvisionned() const;
    bool unprovision() const;
signals:
    void progress(qsizetype progression, qsizetype max);
private:
    bool copyDirectory(const QString &sourceDirName, const QString &destinationDirName) const;
    bool backupFiles(const QStringList &names) const;
    bool unbackupFiles(const QStringList &names) const;
    bool touchFile(const QString &fileName) const;
    bool copyFile(const QString &sourceFileName, const QString &destinationFileName) const;

    FF8frPackInstallation _ff8frPackInstallation;
    QString _ff8DirName;
};

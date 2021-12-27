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
#pragma once

#include <QDir>
#include <QMap>
#include <QObject>

#include "ffnxinstallation.h"
#include "installprogression.h"

class QIODevice;
class GetUrl;

class Provisionner : public QObject
{
    Q_OBJECT
public:
    Provisionner(const FFNxInstallation &installation, QObject *parent = nullptr);
    bool startProvision();
signals:
    void progress(qint64 value, qint64 max);
    void finished(const QString &error);
private slots:
    void setDownloadProgression(QIODevice *destination, qint64 bytesReceived, qint64 bytesTotal);
    void downloadEnd(QIODevice *destination, const QString &error);
private:
    bool unzipPackage(QIODevice *destination, const QDir &dir);
    bool installPackage(const QDir &dir);
    const FFNxInstallation &_installation;
    QMap<QIODevice *, GetUrl *> _getUrls;
};

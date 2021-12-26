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
#include "provisionner.h"

#include <QTemporaryFile>
#include <QTemporaryDir>

#include "geturl.h"
#include "package.h"

Provisionner::Provisionner(const FFNxInstallation &installation, QObject *parent) :
    QObject(parent), _installation(installation)
{
}

bool Provisionner::startProvision()
{
    QTemporaryFile *temp = new QTemporaryFile("FF8Fr-pack-XXXXXX.zip", this);
    temp->open();
    qDebug() << temp->fileName();
    GetUrl *getUrl = new GetUrl(this);
    _getUrls.insert(temp, getUrl);
    getUrl->startDownloadFile(QUrl("https://www.ff8.fr/download/programs/FFNx-pack-steam-v1.2.zip"), temp);

    connect(getUrl, &GetUrl::finished, this, &Provisionner::downloadEnd);
    connect(getUrl, &GetUrl::downloadProgress, this, &Provisionner::setDownloadProgression);

    return false;
}

void Provisionner::setDownloadProgression(QIODevice *destination, qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "Provisionner::setDownloadProgression" << bytesReceived << bytesTotal;
    emit progress(bytesReceived, bytesTotal < 0 ? -1 : bytesTotal * 3);
}

void Provisionner::downloadEnd(QIODevice *destination, const QString &error)
{
    if (!error.isEmpty()) {
        emit finished(error);
        return;
    }
    QTemporaryDir tempDir("FF8Fr-pack-XXXXXX");
    tempDir.setAutoRemove(true);
    QDir dir(tempDir.path());
    if (!unzipPackage(destination, dir) || !installPackage(dir)) {
        return;
    }

    emit finished(QString());
}


bool Provisionner::unzipPackage(QIODevice *destination, const QDir &dir)
{
    Package *package = new Package(destination, this);
    connect(package, &Package::progress, this, [&](qint64 value, qint64 max) {
        emit progress(max + value, max * 3);
    });
    if (!package->unzip(dir, 0xee316dc8)) {
        emit finished(package->errorString());
        return false;
    }

    return true;
}

bool Provisionner::installPackage(const QDir &dir)
{
    return false;
}

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
#include "geturl.h"

GetUrl::GetUrl(QObject *parent) :
    QObject(parent), _networkAccess(this), _downloadReply(nullptr),
    _destination(nullptr)
{
}

bool GetUrl::startDownloadFile(const QUrl &url, QIODevice *destination)
{
    if (!destination || !destination->isOpen()) {
        return false;
    }

    _destination = destination;

    _downloadReply = _networkAccess.get(QNetworkRequest(url));

    connect(_downloadReply, &QIODevice::readyRead, this, &GetUrl::downloadReadyRead);
    connect(_downloadReply, &QNetworkReply::finished, this, &GetUrl::downloadSuccess);
    connect(_downloadReply, &QNetworkReply::downloadProgress, this, [&](qint64 bytesReceived, qint64 bytesTotal) {
        emit downloadProgress(_destination, bytesReceived, bytesTotal);
    });
    connect(_downloadReply, &QNetworkReply::errorOccurred, this, &GetUrl::downloadError);
    connect(_downloadReply, &QNetworkReply::sslErrors, this, &GetUrl::downloadSslErrors);

    return true;
}

void GetUrl::downloadReadyRead()
{
    if (_downloadReply == nullptr) {
        return;
    }

    _destination->write(_downloadReply->readAll());
}

void GetUrl::downloadSuccess()
{
    _destination->write(_downloadReply->readAll());
    _destination->close();
    _downloadReply->close();

    emit finished(_destination, QString());

    _downloadReply->deleteLater();
}

void GetUrl::downloadError(QNetworkReply::NetworkError code)
{
    emit finished(_destination, tr("Error occurred"));

    _destination->close();

    _downloadReply->deleteLater();
}

void GetUrl::downloadSslErrors(const QList<QSslError> &errors)
{
    emit finished(_destination, tr("SSL Error occurred"));

    _destination->close();

    _downloadReply->deleteLater();
}

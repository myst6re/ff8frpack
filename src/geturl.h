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

#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSslError>
#include <QIODevice>

class QNetworkAccessManager;

class GetUrl : public QObject
{
    Q_OBJECT

public:
    explicit GetUrl(QObject *parent = nullptr);
    bool startDownloadFile(const QUrl &url, QIODevice *destination);
signals:
    void finished(QIODevice *destination, const QString &error);
    void downloadProgress(QIODevice *destination, qint64 bytesReceived, qint64 bytesTotal);
private slots:
    void downloadReadyRead();
    void downloadSuccess();
    void downloadError(QNetworkReply::NetworkError code);
    void downloadSslErrors(const QList<QSslError> &errors);
private:
    QNetworkAccessManager _networkAccess;
    QNetworkReply *_downloadReply;
    QIODevice *_destination;
};

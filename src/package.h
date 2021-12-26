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
#include <QObject>

#include "zip.h"
#include "installprogression.h"

class Package : public QObject
{
    Q_OBJECT
public:
    Package(QIODevice *in, QObject *parent = nullptr);
    bool unzip(const QDir &dir, quint32 crc);
    const QString &errorString() const {
        return _unzip.errorString();
    }
signals:
    void progress(qint64 value, qint64 max);
private:
    UnZip _unzip;
};

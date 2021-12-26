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
#include "package.h"

#include <zlib.h>

Package::Package(QIODevice *in, QObject *parent) :
    QObject(parent), _unzip(in)
{

}

bool Package::unzip(const QDir &dir, quint32 crc)
{
    _unzip.setOutDirectory(dir);
    if (!_unzip.open()) {
        return false;
    }

    if (!_unzip.checkZippedFileIntegrity(crc)) {
        return false;
    }

    qint64 max = _unzip.size();

    UnZip::ReadError r;
    do {
        emit progress(_unzip.pos(), max);

        r = _unzip.read();
    } while (r == UnZip::Ok);

    _unzip.close();

    if (r == UnZip::Error) {
        return false;
    }

    emit progress(max, max);

    return true;
}

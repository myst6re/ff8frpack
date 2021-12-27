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

#include <QObject>
#include <QDir>

extern "C" {
#include "minizip/unzip.h"
}

class QIODevice;

class UnZip : QObject
{
    Q_OBJECT
public:
    enum ReadError {
        Ok,
        EndOfList,
        Error
    };
    UnZip(QIODevice *in = nullptr, const QDir &out = QDir());
    inline void setInDevice(QIODevice *in) {
        _in = in;
    }
    inline const QDir &outDirectory() const {
        return _out;
    }
    inline void setOutDirectory(const QDir &out) {
        _out = out;
    }
    bool isOpen() const;
    bool open();
    // Read one file
    ReadError read();
    // Read all files
    bool readAll();
    bool reset();
    bool seek(const QString &fileName);
    qint64 pos();
    qint64 currentFile();
    qint64 size();
    qint64 fileCount();
    bool close();
    bool checkZippedFileIntegrity(unsigned long crc);
    inline const QString &errorString() const {
        return _lastError;
    }
private:
    QString _lastError;
    QIODevice *_in;
    QDir _out;
    zlib_filefunc64_def _ffunc;
    unzFile _unzFile;
};

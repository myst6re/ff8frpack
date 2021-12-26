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
#include "zip.h"

#include <QIODevice>

#define STRCMPCASENOSENTIVEFUNCTION(str1, str2) \
    QString::compare(str1, str2, Qt::CaseInsensitive)

#define STRCMPCUSTOM 2

extern "C" {
voidpf ZCALLBACK qiodevice_open64_file_func(voidpf opaque, const void* filename, int mode)
{
    Q_UNUSED(filename)
    QIODevice *io = (QIODevice *)opaque;
    QIODevice::OpenMode openMode = QIODevice::ReadOnly;

    if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER) == ZLIB_FILEFUNC_MODE_READ) {
        openMode = QIODevice::ReadOnly;
    } else if (mode & ZLIB_FILEFUNC_MODE_EXISTING) {
        openMode = QIODevice::ReadOnly | QIODevice::Append;
    } else if (mode & ZLIB_FILEFUNC_MODE_CREATE) {
        openMode = QIODevice::WriteOnly | QIODevice::Truncate;
    }

    if (!io->open(openMode)) {
        return nullptr;
    }

    return io;
}

uLong ZCALLBACK qiodevice_read_file_func(voidpf opaque, voidpf stream, void* buf, uLong size)
{
    Q_UNUSED(opaque)
    QIODevice *io = (QIODevice *)stream;

    if (io == nullptr) {
        return 0;
    }

    qint64 s = io->read((char *)buf, size);

    return s < 0 ? 0 : uLong(s);
}

uLong ZCALLBACK qiodevice_write_file_func(voidpf opaque, voidpf stream, const void* buf, uLong size)
{
    Q_UNUSED(opaque)
    QIODevice *io = (QIODevice *)stream;

    if (io == nullptr) {
        return 0;
    }

    qint64 s = io->write((const char *)buf, size);

    return s < 0 ? 0 : uLong(s);
}

ZPOS64_T ZCALLBACK qiodevice_tell64_file_func(voidpf opaque, voidpf stream)
{
    Q_UNUSED(opaque)
    QIODevice *io = (QIODevice *)stream;

    if (io == nullptr) {
        return 0;
    }

    qint64 p = io->pos();

    return p < 0 ? 0 : ZPOS64_T(p);
}

long ZCALLBACK qiodevice_seek64_file_func(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin)
{
    Q_UNUSED(opaque)
    QIODevice *io = (QIODevice *)stream;

    if (io == nullptr) {
        return 0;
    }

    switch (origin) {
    case ZLIB_FILEFUNC_SEEK_CUR:
        offset += io->pos();
        break;
    case ZLIB_FILEFUNC_SEEK_END:
        offset += io->size();
        break;
    case ZLIB_FILEFUNC_SEEK_SET:
        break;
    default:
        return -1;
    }

    return io->seek(offset) ? 0 : -1;
}

int ZCALLBACK qiodevice_close_file_func(voidpf opaque, voidpf stream)
{
    Q_UNUSED(opaque)
    QIODevice *io = (QIODevice *)stream;

    if (io == nullptr) {
        return 0;
    }

    io->close();

    return 0;
}

int ZCALLBACK qiodevice_error_file_func(voidpf opaque, voidpf stream)
{
    Q_UNUSED(opaque)
    QIODevice *io = (QIODevice *)stream;

    if (io == nullptr) {
        return 0;
    }

    return io->errorString().isEmpty() ? 0 : 1;
}
};

UnZip::UnZip(QIODevice *in, const QDir &out) :
    _in(in), _out(out), _unzFile(nullptr)
{
    _ffunc.zopen64_file = qiodevice_open64_file_func;
    _ffunc.zread_file = qiodevice_read_file_func;
    _ffunc.zwrite_file = qiodevice_write_file_func;
    _ffunc.ztell64_file = qiodevice_tell64_file_func;
    _ffunc.zseek64_file = qiodevice_seek64_file_func;
    _ffunc.zclose_file = qiodevice_close_file_func;
    _ffunc.zerror_file = qiodevice_error_file_func;
}

bool UnZip::isOpen() const
{
    return _unzFile != nullptr;
}

bool UnZip::open()
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return false;
    }

    if (isOpen()) {
        close();
    }

    _ffunc.opaque = _in;
    _unzFile = unzOpen2_64(nullptr, &_ffunc);

    return _unzFile != nullptr;
}

UnZip::ReadError UnZip::read()
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return Error;
    }

    char filename_inzip[256] = "";
    unz_file_info64 file_info;
    int err = unzGetCurrentFileInfo64(_unzFile, &file_info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0);

    if (UNZ_OK != err) {
        _lastError = _in->errorString();
        return Error;
    }

    QString name(filename_inzip);

    QString outFileName = _out.filePath(name),
        outDirName = outFileName.left(outFileName.lastIndexOf('/')),
        outName = name.mid(name.lastIndexOf('/') + 1);

    if (outName.isEmpty()) {
        if (!QFileInfo(outDirName).isDir() && !_out.mkpath(name)) {
            _lastError = tr("Cannot create \"%1\" directory").arg(outDirName);
            return Error;
        }

        err = unzGoToNextFile(_unzFile);

        if (UNZ_END_OF_LIST_OF_FILE != err && UNZ_OK != err) {
            _lastError = _in->errorString();
            return Error;
        }

        return read();
    }

    if (!QFileInfo(outDirName).isDir() && !_out.mkpath(name.left(name.lastIndexOf('/')))) {
        _lastError = tr("Cannot create \"%1\" directory").arg(outDirName);
        return Error;
    }

    QFile outFile(outFileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        _lastError = outFile.errorString();
        return Error;
    }

    err = unzOpenCurrentFile(_unzFile);

    if (UNZ_OK != err) {
        _lastError = _in->errorString();
        return Error;
    }

    QByteArray buffer(16384, '\0');

    int r;

    do {
        r = unzReadCurrentFile(_unzFile, buffer.data(), buffer.size());

        if (r < 0) {
            _lastError = _in->errorString();
            break;
        }

        if (r > 0) {
            if (outFile.write(buffer.constData(), r) < 0) {
                r = UNZ_ERRNO;
                _lastError = outFile.errorString();
                break;
            }
        }

    } while (r > 0);

    outFile.close();
    err = unzCloseCurrentFile(_unzFile);

    if (UNZ_OK != r || UNZ_OK != err) {
        if (r == UNZ_OK) {
            _lastError = _in->errorString();
        }
        return Error;
    }

    err = unzGoToNextFile(_unzFile);

    if (UNZ_END_OF_LIST_OF_FILE != err && UNZ_OK != err) {
        _lastError = _in->errorString();
        return Error;
    }

    return UNZ_END_OF_LIST_OF_FILE == err ? EndOfList : Ok;
}

bool UnZip::readAll()
{
    ReadError r;

    do {
        r = read();

        if (r == Error) {
            return false;
        }

    } while (r == Ok);

    return true;
}

bool UnZip::reset()
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return false;
    }

    int err = unzGoToFirstFile(_unzFile);

    if (UNZ_OK != err) {
        _lastError = _in->errorString();
        return false;
    }

    return true;
}

bool UnZip::seek(const QString &fileName)
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return false;
    }

    QByteArray data = fileName.toLatin1();

    if (UNZ_OK != unzLocateFile(_unzFile, data.constData(), STRCMPCUSTOM)) {
        _lastError = _in->errorString();
        return false;
    }

    return true;
}


qint64 UnZip::pos()
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return -1;
    }

    return _in->pos();
}

qint64 UnZip::currentFile()
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return false;
    }

    return unzGetOffset64(_unzFile);
}

qint64 UnZip::size()
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return -1;
    }

    return _in->size();
}

qint64 UnZip::fileCount()
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return false;
    }

    unz_global_info64 gi;

    if (UNZ_OK != unzGetGlobalInfo64(_unzFile, &gi)) {
        _lastError = _in->errorString();
        return false;
    }

    return gi.number_entry;
}

bool UnZip::checkZippedFileIntegrity(unsigned long crc)
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return false;
    }

    qint64 pos = _in->pos();
    _in->reset();
    QByteArray data = _in->readAll();
    unsigned long res = crc32(0, (const unsigned char *)data.constData(), data.size());
    _in->seek(pos);

    return crc == res;
}

bool UnZip::close()
{
    if (_in == nullptr) {
        _lastError = tr("Input device not set");
        return false;
    }

    if (UNZ_OK != unzClose(_unzFile)) {
        _lastError = _in->errorString();
        return false;
    }

    _unzFile = nullptr;

    return true;
}

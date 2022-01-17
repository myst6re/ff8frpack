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
#include "ff8installation.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#include <winbase.h>
#include <winreg.h>
#endif

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QRegularExpression>
#include <QStandardPaths>

FF8Installation::FF8Installation() :
    _type(Standard)
{
}

FF8Installation::FF8Installation(Type type) :
    _type(type)
{
    switch (type) {
    case Standard:
        _appPath = standardFF8AppPath("Square Soft, Inc/Final Fantasy VIII/1.00");
        _savePaths = standardFF8DataPaths(_appPath);
        break;
    case Demo:
        _appPath = standardFF8AppPath("Square Soft, Inc/Final Fantasy VIII Demo/1.00");
        _savePaths = standardFF8DataPaths(_appPath);
        break;
    case Steam:
        _appPath = steamFF8AppPath();
        _savePaths = steamFF8UserDataPaths();
        break;
    case Remaster:
        _appPath = steamFF8RemasterAppPath();
        _savePaths = steamFF8RemasterUserDataPaths();
        break;
    }
}

FF8Installation::FF8Installation(Type type, const QString &appPath) :
    _appPath(appPath), _type(type)
{
    switch (type) {
    case Standard:
        _savePaths = standardFF8DataPaths(_appPath);
        break;
    case Demo:
        _savePaths = standardFF8DataPaths(_appPath);
        break;
    case Steam:
        _savePaths = steamFF8UserDataPaths();
        break;
    case Remaster:
        _savePaths = steamFF8RemasterUserDataPaths();
        break;
    }
}

bool FF8Installation::isValid() const
{
    return !_appPath.isEmpty() && QFile::exists(_appPath) && QFile::exists(exePath());
}

bool FF8Installation::hasSlots() const
{
    return !_savePaths.isEmpty();
}

QString FF8Installation::savePath(int slot) const
{
    if (_savePaths.isEmpty()) {
        return QString();
    }
    QString path = _savePaths.first();
    switch (_type) {
    case Standard:
    case Demo:
        return QString("%1/Slot%2").arg(path).arg(slot);
    case Steam:
    case Remaster:
        return path;
    }
    Q_ASSERT(false);
    return QString();
}

QString FF8Installation::exePath() const
{
    switch (_type) {
    case Standard:
    case Demo:
        return _appPath + "/FF8.exe";
    case Steam:
        return _appPath + "/FF8_Launcher.exe";
    case Remaster:
        return _appPath + "/FFVIII_LAUNCHER.exe";
    }
    Q_ASSERT(false);
    return QString();
}

QString FF8Installation::typeString() const
{
    switch (_type) {
    case Standard:
        return qApp->translate("FF8Installation", "FF8 Standard");
    case Demo:
        return qApp->translate("FF8Installation", "FF8 Demo");
    case Steam:
        return qApp->translate("FF8Installation", "FF8 Steam");
    case Remaster:
        return qApp->translate("FF8Installation", "FF8 Remaster");
    }
    Q_ASSERT(false);
    return QString();
}

QString FF8Installation::saveNamePattern(quint8 slot) const
{
    switch (_type) {
    case Standard:
    case Demo:
        return "save{num}";
    case Steam:
    case Remaster:
        return QString("slot%1_save{num}.ff8").arg(slot);
    }
    Q_ASSERT(false);
    return QString();
}

bool FF8Installation::hasMetadata() const
{
    return _type == Steam;
}

QList<FF8Installation> FF8Installation::installations()
{
    QList<FF8Installation::Type> types;
    types.append(Steam);
    types.append(Standard);
    QList<FF8Installation> ret;

    for (FF8Installation::Type type: types) {
        FF8Installation installation(type);
        if (installation.isValid()) {
            ret.append(installation);
        }
    }

    return ret;
}

FF8Installation FF8Installation::fromPath(const QString &path)
{
    QList<FF8Installation::Type> types;
    types.append(Steam);
    types.append(Standard);

    for (FF8Installation::Type type: types) {
        FF8Installation installation(type, path);
        if (installation.isValid()) {
            return installation;
        }
    }

    return FF8Installation(Steam, path);
}

QStringList FF8Installation::standardFF8DataPaths(const QString &appPath)
{
    QString path;
    QStringList appDataLocations = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);

    if (!appDataLocations.isEmpty()) {
        path = appDataLocations.first().append("/VirtualStore/").append(QString(appPath).replace(QRegularExpression("^\\w+:/"), ""));
        if (!QFile::exists(path)) {
            path = QString();
        }
    }

    if (path.isEmpty()) {
        path = appPath;
    }

    return QStringList(path + "/save");
}

QString FF8Installation::standardFF8AppPath(const QString &path)
{
    return QDir::cleanPath(QDir::fromNativeSeparators(regValue(path, "AppPath")));
}

QString FF8Installation::steamFF8AppPath()
{
    QStringList apps = searchInstalledApps("FINAL FANTASY VIII", "SQUARE ENIX", 1);
    return apps.isEmpty() ? QString() : apps.first();
}

QString FF8Installation::steamFF8RemasterAppPath()
{
    QStringList apps = searchInstalledApps("FINAL FANTASY VIII - REMASTERED", "Square Enix", 1);
    return apps.isEmpty() ? QString() : apps.first();
}

QStringList FF8Installation::steamFF8UserDataPaths(int max)
{
    QStringList ff8UserDataPaths;

    for (const QString &documentsPath : QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)) {
        QDir ff8UserDataDir(QString("%1/Square Enix").arg(documentsPath));

        for (const QString &dir : ff8UserDataDir.entryList(QStringList("FINAL FANTASY VIII*"), QDir::Dirs)) {
            QDirIterator it(ff8UserDataDir.absoluteFilePath(dir),
                            QStringList("user_*"),
                            QDir::Dirs,
                            QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

            while (it.hasNext()) {
                it.next();

                ff8UserDataPaths.append(it.filePath());

                if (max > 0 && ff8UserDataPaths.size() >= max) {
                    return ff8UserDataPaths;
                }
            }
        }
    }

    return ff8UserDataPaths;
}

QStringList FF8Installation::steamFF8RemasterUserDataPaths(int max)
{
    QStringList ff8UserDataPaths;

    for (const QString &documentsPath : QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)) {
        QDir ff8UserDataDir(documentsPath + "/My Games");

        for (const QString &dir : ff8UserDataDir.entryList(QStringList("FINAL FANTASY VIII*"), QDir::Dirs)) {
            QDirIterator it(ff8UserDataDir.absoluteFilePath(dir + "/Steam"),
                            QStringList("*"),
                            QDir::Dirs,
                            QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

            while (it.hasNext()) {
                it.next();

                QString path = it.filePath() + "/game_data/user/saves";

                if (QFile::exists(path)) {
                    ff8UserDataPaths.append(path);

                    if (max > 0 && ff8UserDataPaths.size() >= max) {
                        return ff8UserDataPaths;
                    }
                }
            }
        }
    }

    return ff8UserDataPaths;
}

QString FF8Installation::regValue(const QString &regPath, const QString &regKey, RegTarget target)
{
#ifdef Q_OS_WIN32
    HKEY phkResult;
    LONG error;
    REGSAM flags = KEY_READ;

    if (target == Target32) {
#ifdef KEY_WOW64_32KEY
        flags |= KEY_WOW64_32KEY; // if you compile in 64-bit, force reg search into 32-bit entries
#endif
    } else if (target == Target64) {
#ifdef KEY_WOW64_64KEY
        flags |= KEY_WOW64_64KEY;
#endif
    }

    // Open regPath relative to HKEY_LOCAL_MACHINE
    error = RegOpenKeyExW(HKEY_LOCAL_MACHINE, QDir::toNativeSeparators("SOFTWARE/" + regPath).toStdWString().c_str(), 0, flags, &phkResult);
    if (ERROR_SUCCESS == error) {
        BYTE value[MAX_PATH];
        DWORD cValue = MAX_PATH, type;

        // Open regKey which must is a string value (REG_SZ)
        RegQueryValueExW(phkResult, regKey.toStdWString().c_str(), nullptr, &type, value, &cValue);
        if (ERROR_SUCCESS == error && type == REG_SZ) {
            RegCloseKey(phkResult);
            return QString::fromUtf16((char16_t *)value);
        }
        RegCloseKey(phkResult);
    }
#else
    Q_UNUSED(regPath)
    Q_UNUSED(regKey)
    Q_UNUSED(target)
#endif

    return QString();
}

QString FF8Installation::regValue(const QString &regPath, const QString &regKey)
{
    QString ret;

#if defined(KEY_WOW64_32KEY) || defined(KEY_WOW64_64KEY)
    ret = regValue(regPath, regKey, Target32);
    if (ret.isEmpty()) {
        ret = regValue(regPath, regKey, Target64);
    }
#else
    ret = regValue(regPath, regKey, TargetNone);
#endif

    return ret;
}

QStringList FF8Installation::searchInstalledApps(const QString &appName, const QString &publisher, int max)
{
    QStringList ret;
#ifdef Q_OS_WIN32
    HKEY phkResult, phkResult2;
    LONG error;
    REGSAM flags = KEY_READ;

#ifdef KEY_WOW64_64KEY
    flags |= KEY_WOW64_64KEY; // if you compile in 64-bit, force reg search into 64-bit entries
#endif

    error = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\", 0, flags, &phkResult);

    if (ERROR_SUCCESS == error) {
        DWORD index = 0;
        WCHAR subKeyName[MAX_PATH];
        DWORD subKeyCName = MAX_PATH;

        while (ERROR_SUCCESS == RegEnumKeyExW(phkResult, index, subKeyName, &subKeyCName, nullptr, nullptr, nullptr, nullptr)) {
            QString subKeyNameStr = QString::fromUtf16((char16_t *)subKeyName);
            error = RegOpenKeyExW(phkResult, QString("%1\\").arg(subKeyNameStr).toStdWString().c_str(), 0, KEY_READ, &phkResult2);

            if (ERROR_SUCCESS == error) {
                BYTE value[MAX_PATH];
                DWORD cValue = MAX_PATH, type;
                error = RegQueryValueExW(phkResult2, L"DisplayName", nullptr, &type, value, &cValue);

                if (ERROR_SUCCESS == error && REG_SZ == type) {
                    QString softwareNameStr = QString::fromUtf16((char16_t *)value);

                    if (softwareNameStr.compare(appName, Qt::CaseInsensitive) == 0) {
                        error = RegQueryValueExW(phkResult2, L"Publisher", nullptr, &type, value, &cValue);

                        if (ERROR_SUCCESS == error && REG_SZ == type) {
                            QString publisherStr = QString::fromUtf16((char16_t *)value);

                            if (publisherStr.compare(publisher, Qt::CaseInsensitive) == 0) {
                                cValue = MAX_PATH;
                                error = RegQueryValueExW(phkResult2, L"InstallLocation", nullptr, &type, value, &cValue);

                                if (ERROR_SUCCESS == error && REG_SZ == type) {
                                    QString appPath = QDir::fromNativeSeparators(QDir::cleanPath(QString::fromUtf16((char16_t *)value)));

                                    if (QFile::exists(appPath)) {
                                        ret.append(appPath);

                                        if (max > 0 && ret.size() >= max) {
                                            RegCloseKey(phkResult2);
                                            RegCloseKey(phkResult);
                                            return ret;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            RegCloseKey(phkResult2);
            ++index;
            subKeyCName = MAX_PATH;
        }

        RegCloseKey(phkResult);
    }
#else
    Q_UNUSED(appName)
    Q_UNUSED(publisher)
    Q_UNUSED(max)
#endif
    return ret;
}

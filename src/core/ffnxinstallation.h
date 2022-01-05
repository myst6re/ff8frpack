#pragma once

#include <QString>

#include "ff8installation.h"

class FFNxInstallation
{
public:
    explicit FFNxInstallation(const QString &dirName);
    QString patcherFileName() const;
    QString configFileName() const;
    QString installedFlagFileName() const;
    const QString &dirName() const;
    bool isValid() const;
    bool isEnabled() const;
    static FFNxInstallation localInstallation();
    static FFNxInstallation fromFF8Installation(const FF8Installation &ff8Installation);
private:
    QString _dirName;
};

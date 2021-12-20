#pragma once

#include <QString>

#include "installprogression.h"

class FFNxInstallation
{
public:
    explicit FFNxInstallation(const QString &dirName);
    QString configFileName() const;
    const QString &dirName() const;
    bool isValid() const;
    bool provision(InstallProgression *progression = nullptr) const;
    static FFNxInstallation localInstallation();
    static FFNxInstallation autoDetectInstallation();
private:
    QString _dirName;
};

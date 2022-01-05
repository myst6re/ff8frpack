#pragma once

#include <QString>

#include "helparguments.h"

class CommonArguments : public HelpArguments
{
public:
    CommonArguments();
    inline const QString &ff8Directory() const {
        return _ff8Directory;
    }
    inline const QString &ff8frPackDirectory() const {
        return _ff8frPackDirectory;
    }
protected:
    QString _ff8frPackDirectory;
    QString _ff8Directory;
};


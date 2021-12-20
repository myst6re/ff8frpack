#pragma once

#include <QString>

class InstallProgression
{
public:
    InstallProgression();
    virtual ~InstallProgression();
    virtual bool observerWasCanceled() const = 0;
    virtual void setObserverMaximum(int max) = 0;
    virtual void setObserverValue(int value) = 0;
    virtual void setObserverMessage(const QString &message) = 0;
    virtual bool observerRetry(const QString &message) = 0;
};


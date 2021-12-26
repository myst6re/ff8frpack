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


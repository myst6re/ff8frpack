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

#include <QWizard>

#include "ffnxinstallation.h"

#define FIELD_NAME_private(field) \
    #field

#define FIELD_NAME(field) \
    FIELD_NAME_private(Wizard::field)

#define MANDATORY_FIELD(field) \
    FIELD_NAME(field)"*"

class Wizard : public QWizard
{
    Q_OBJECT
public:
    enum {
        PageIntro,
        PageInstall
    };
    enum Field {
        FieldPath,
    };

    explicit Wizard(const FFNxInstallation &installation, QWidget *parent = nullptr);

signals:

private:
    const FFNxInstallation &_installation;
};


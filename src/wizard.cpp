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
#include "wizard.h"

#include "wizardpages/wizardpageintro.h"
#include "wizardpages/wizardpageinstall.h"

Wizard::Wizard(const FFNxInstallation &installation, QWidget *parent)
    : QWizard(parent), _installation(installation)
{
    setWindowTitle("FF8.Fr's Pack");
    setPage(PageIntro, new WizardPageIntro(installation, this));
    setPage(PageInstall, new WizardPageInstall(this));
    setOptions(options() | QWizard::NoBackButtonOnStartPage | QWizard::NoBackButtonOnLastPage);
    setButtonText(QWizard::CommitButton, tr("Install"));
}

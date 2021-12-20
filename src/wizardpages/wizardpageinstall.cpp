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
#include "wizardpageinstall.h"

#include <QCoreApplication>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QTimer>
#include <QVBoxLayout>

#include "../ffnxinstallation.h"
#include "../wizard.h"

WizardPageInstall::WizardPageInstall(QWidget *parent) :
    QWizardPage(parent), _isComplete(false)
{
    setTitle(tr("Installation"));

    _timer = new QTimer(this);

    _progressBar = new QProgressBar(this);
    _progressBar->setRange(0, 10000);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Installation in progress...")));
    layout->addWidget(_progressBar);
    layout->addStretch(1);

    connect(_timer, &QTimer::timeout, this, &WizardPageInstall::processEvents);
}

void WizardPageInstall::processEvents()
{
    if (_progressBar->value() != _value) {
        _progressBar->setValue(_value);
    }

    QCoreApplication::processEvents();
}

void WizardPageInstall::initializePage()
{
    /* Provision can be long, the timer will keep
     * the interface responsive during the installation process */
    _timer->start(700);

    QString dirName = field(FIELD_NAME(FieldPath)).toString();
    FFNxInstallation(dirName).provision(this);

    _timer->stop();

    // Ensure progress bar is up to date
    processEvents();

    // Unlock button
    _isComplete = true;
}

bool WizardPageInstall::isComplete() const
{
    return _isComplete;
}

bool WizardPageInstall::observerWasCanceled() const
{
    return false;
}

void WizardPageInstall::setObserverMaximum(int max)
{
    _max = max;
}

void WizardPageInstall::setObserverValue(int value)
{
    _value = value * 10000 / _max;
}

void WizardPageInstall::setObserverMessage(const QString &message)
{
    _message = message;
}

bool WizardPageInstall::observerRetry(const QString &message)
{
    QMessageBox::StandardButton response =  QMessageBox::question(this, tr("Installation Error"), message);

    return QMessageBox::Yes == response;
}

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

#include "../provisionner.h"
#include "../wizard.h"

WizardPageInstall::WizardPageInstall(QWidget *parent) :
    QWizardPage(parent), _isComplete(false), _installation(QString())
{
    setTitle(tr("Installation"));

    _timer = new QTimer(this);

    _progressBar = new QProgressBar(this);
    _progressBar->setRange(0, 1000);

    _label = new QLabel(tr("Installation in progress..."));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(_label);
    layout->addWidget(_progressBar);
    layout->addStretch(1);

    connect(_timer, &QTimer::timeout, this, &WizardPageInstall::processEvents);
}

void WizardPageInstall::processEvents()
{
    qDebug() << "WizardPageInstall::processEvents" << _value;
    if (_progressBar->value() != _value) {
        _progressBar->setValue(_value);
    }

    if (!_message.isEmpty()) {
        _label->setText(_message);
    }

    QCoreApplication::processEvents();
}

void WizardPageInstall::initializePage()
{
    /* Provision can be long, the timer will keep
     * the interface responsive during the installation process */
    _timer->start(700);

    QString dirName = field(FIELD_NAME(FieldPath)).toString();
    _installation = FFNxInstallation(dirName);
    Provisionner *provisionner = new Provisionner(_installation, this);

    connect(provisionner, &Provisionner::progress, this, &WizardPageInstall::setProgression);
    connect(provisionner, &Provisionner::finished, this, &WizardPageInstall::setFinished);

    provisionner->startProvision();
}

void WizardPageInstall::setProgression(qint64 value, qint64 max)
{
    _value = value * 1000 / max;
    qDebug() << "WizardPageInstall::setProgression" << value << max << _value;
}

void WizardPageInstall::setFinished(const QString &error)
{
    qDebug() << "WizardPageInstall::setFinished" << error;
    _message = error;
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

/*
bool WizardPageInstall::observerRetry(const QString &message)
{
    QMessageBox::StandardButton response =  QMessageBox::question(this, tr("Installation Error"), message);

    return QMessageBox::Yes == response;
}
*/

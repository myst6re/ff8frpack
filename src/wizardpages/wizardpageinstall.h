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

#include <QWizardPage>

#include "../installprogression.h"

class QProgressBar;

class WizardPageInstall : public QWizardPage, public InstallProgression
{
    Q_OBJECT
public:
    WizardPageInstall(QWidget *parent = nullptr);
    void initializePage() override;
    bool isComplete() const override;
    bool observerWasCanceled() const override;
    void setObserverMaximum(int max) override;
    void setObserverValue(int value) override;
    void setObserverMessage(const QString &message) override;
    bool observerRetry(const QString &message) override;
private slots:
    void processEvents();
private:
    QProgressBar *_progressBar;
    QTimer *_timer;
    int _max, _value;
    bool _isComplete;
    QString _message;
};

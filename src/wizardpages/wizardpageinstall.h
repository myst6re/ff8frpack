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

#include "../ffnxinstallation.h"
#include "../installprogression.h"

class QLabel;
class QProgressBar;

class WizardPageInstall : public QWizardPage
{
    Q_OBJECT
public:
    WizardPageInstall(QWidget *parent = nullptr);
    void initializePage() override;
    bool isComplete() const override;
private slots:
    void processEvents();
    void setProgression(qint64 value, qint64 max);
    void setFinished(const QString &error);
private:
    QProgressBar *_progressBar;
    QLabel *_label;
    QTimer *_timer;
    qint64 _max, _value;
    bool _isComplete;
    QString _message;
    FFNxInstallation _installation;
};

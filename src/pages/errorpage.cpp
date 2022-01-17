/****************************************************************************
 ** FF8frPack FF8.fr Pack configurator and installer
 ** Copyright (C) 2022 Arzel Jérôme <myst6re@gmail.com>
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
#include "errorpage.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "../widgets/helpwidget.h"

ErrorPage::ErrorPage(int iconSize, QWidget *parent)
    : QWidget{parent}
{
    _helpWidget = new HelpWidget(iconSize,
                                            QString(),
                                            HelpWidget::IconWarning,
                                            this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Close,
                this);
    buttonBox->button(QDialogButtonBox::Close)->setText(tr("Back"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(_helpWidget);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::clicked, this, &ErrorPage::closeClicked);
}

void ErrorPage::setText(const QString &text)
{
    _helpWidget->setText(text);
}

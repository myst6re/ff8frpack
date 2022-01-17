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
#include "flatcombobox.h"

#include <QStackedLayout>
#include <QComboBox>
#include <QLabel>
#include <QStackedLayout>

FlatComboBox::FlatComboBox(QWidget *parent) :
    QWidget(parent)
{
    QWidget *labelWidget = new QWidget(this);
    _icon = new QLabel(labelWidget);

    _label = new QLabel(labelWidget);
    _label->setTextFormat(Qt::PlainText);
    _comboBox = new QComboBox(this);

    QHBoxLayout *labelLayout = new QHBoxLayout(labelWidget);
    labelLayout->addWidget(_icon);
    labelLayout->addWidget(_label, 1);
    labelLayout->setContentsMargins(QMargins());

    _layout = new QStackedLayout(this);
    _layout->addWidget(_comboBox);
    _layout->addWidget(labelWidget);

    updateFrame();

    connect(_comboBox, &QComboBox::currentIndexChanged, this, &FlatComboBox::currentIndexChanged);
}

void FlatComboBox::updateFrame()
{
    _layout->setCurrentIndex(int(_comboBox->count() <= 1));
    if(_comboBox->count() == 1) {
        _label->setText(_comboBox->itemText(0));
        _icon->setPixmap(_comboBox->itemIcon(0).pixmap(16));
    } else if(_comboBox->count() <= 0) {
        if (_comboBox->currentIndex() != -1) {
            emit currentIndexChanged(-1);
        }
        _label->setText(_defaultText);
        _icon->setPixmap(_defaultIcon.pixmap(16));
    }
}

void FlatComboBox::setDefaultText(const QIcon &defaultIcon, const QString &defaultText)
{
    _defaultIcon = defaultIcon;
    _defaultText = defaultText;

    updateFrame();
}

void FlatComboBox::addItem(const QIcon &icon, const QString &text, const QVariant &userData)
{
    _comboBox->addItem(icon, text, userData);
    updateFrame();
}

QVariant FlatComboBox::currentData() const
{
    return _comboBox->currentData(Qt::UserRole);
}

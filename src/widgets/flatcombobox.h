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
#pragma once

#include <QIcon>
#include <QVariant>
#include <QWidget>

class QComboBox;
class QLabel;
class QStackedLayout;

class FlatComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit FlatComboBox(QWidget *parent = nullptr);
    void addItem(const QIcon &icon, const QString &text, const QVariant &userData);
    QVariant currentData() const;
    void setDefaultText(const QIcon &defaultIcon, const QString &defaultText);
signals:
    void currentIndexChanged(int index);
private:
    void updateFrame();
    QString _defaultText;
    QIcon _defaultIcon;
    QStackedLayout *_layout;
    QComboBox *_comboBox;
    QLabel *_icon, *_label;
};

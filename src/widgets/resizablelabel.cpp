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
#include "resizablelabel.h"

ResizableLabel::ResizableLabel(QWidget *parent) :
    QLabel(parent)
{
}

void ResizableLabel::setPixmap(const QPixmap &pixmap)
{
    _background = pixmap;

    drawBackground();
}

void ResizableLabel::drawBackground()
{
    QSize contentsSize = contentsRect().size();

    if (_background.size() != contentsSize) {
        QLabel::setPixmap(_background.scaled(contentsSize, Qt::KeepAspectRatioByExpanding));
    } else {
        QLabel::setPixmap(_background);
    }
}

void ResizableLabel::resizeEvent(QResizeEvent *e)
{
    if (!_background.isNull() &&
            (pixmap().isNull() || contentsRect().size() != pixmap().size())) {
        setUpdatesEnabled(false);
        drawBackground(); // Call setPixmap() -> update() which must be prevented
        setUpdatesEnabled(true);
    }

    QLabel::resizeEvent(e);
}

QSize ResizableLabel::minimumSizeHint() const
{
    return QSize(0, 0);
}

/*
    Ultimate Webshots Converter 2.0
    Copyright (C) 2006  Hervé "Setaou" BRY <uwc at apinc dot org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include "PreviewWidget.h"
// Qt
#include <QtGui>

PreviewWidget::PreviewWidget(CollectionModel *mdlCollection, QWidget *parent) : QWidget(parent)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
	updateGeometry();

	this->mdlCollection = mdlCollection;

	connect(mdlCollection, SIGNAL(rowsRemoved(QModelIndex, int, int)), this, SLOT(rowsRemoved(QModelIndex, int, int)));
	connect(mdlCollection, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(dataChanged(QModelIndex, QModelIndex)));
}

void PreviewWidget::setIndex(const QModelIndex &index)
{
	if (!index.isValid())
		return clear();

	this->index = index;

	cWebshots::Picture *picture = mdlCollection->picture(index);

	pmPicture.loadFromData(picture->getPicturePtr(), picture->getPictureSize());
	update();
}


void PreviewWidget::clear()
{
	index = QModelIndex();

	pmPicture = QPixmap();
	update();
}

void PreviewWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QString legend;

	// If there is a picture to display
	if (!pmPicture.isNull())
	{
		int offsetX, offsetY;

		QBrush brush(QColor(255, 255, 255, 200));
		painter.setBrush(brush);
			
		// Picture size > Widget size
		if ((pmPicture.size().height() > size().height()) || (pmPicture.size().width() > size().width()))
		{
			QSize newsize = pmPicture.size();
			newsize.scale(size(),Qt::KeepAspectRatio);
			
			offsetX = (size().width() - newsize.width()) / 2;
			offsetY = (size().height() - newsize.height()) / 2;
			painter.drawPixmap(offsetX, offsetY, pmPicture.scaled(size(),Qt::KeepAspectRatio));


			legend = "Scale : " + QString::number((int)(newsize.width() * 100 / pmPicture.size().width())) + "%";
			painter.setPen(Qt::NoPen);
			painter.drawRect(QRect(offsetX + newsize.width() - painter.fontMetrics().width(legend), offsetY, painter.fontMetrics().width(legend), painter.fontMetrics().height()));
			painter.setPen(Qt::SolidLine);
			painter.drawText(offsetX + newsize.width() - painter.fontMetrics().width(legend), offsetY + painter.fontMetrics().height() - painter.fontMetrics().descent(), legend);
		}
		// Picture size <= widget size
		else
		{
			offsetX = (size().width() - pmPicture.size().width()) / 2;
			offsetY = (size().height() - pmPicture.size().height()) / 2;
			painter.drawPixmap(offsetX, offsetY, pmPicture);
		}

		legend = "Size : " + QString::number(pmPicture.size().width()) + "x" + QString::number(pmPicture.size().height());
		painter.setPen(Qt::NoPen);
		painter.drawRect(QRect(offsetX, offsetY, painter.fontMetrics().width(legend), painter.fontMetrics().height()));
		painter.setPen(Qt::SolidLine);
		painter.drawText(offsetX, offsetY + painter.fontMetrics().height() - painter.fontMetrics().descent(), legend);
	}
}

void PreviewWidget::resizeEvent(QResizeEvent *event)
{
}

void PreviewWidget::rowsRemoved(const QModelIndex &parent, int start, int end)
{
	if ((index.row() >= start) || (index.row() <= end))
		clear();
}

void PreviewWidget::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	if ((index.row() >= topLeft.row()) || (index.row() <= bottomRight.row()))
		setIndex(index);
}

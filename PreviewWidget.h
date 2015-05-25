/*
    Ultimate Webshots Converter 2.0
    Copyright (C) 2006  Herve "Setaou" BRY <uwc at apinc dot org>

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


#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "CollectionModel.h"

// Qt
#include <QWidget>
#include <QPixmap>


class PreviewWidget : public QWidget
{
    Q_OBJECT

private slots:
	void rowsRemoved(const QModelIndex &parent, int start, int end);
	void dataChanged(const QModelIndex &topLeft, const QModelIndex& bottomRight);

private:
	CollectionModel *mdlCollection;
	QModelIndex index;
	QPixmap pmPicture;

public:
	PreviewWidget(CollectionModel *mdlCollection, QWidget *parent = 0);

public slots:
	void setIndex(const QModelIndex &index);
	void clear();

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
};

#endif

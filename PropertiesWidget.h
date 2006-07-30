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


#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include "CollectionModel.h"
#include "MetaDataWidget.h"

// Qt
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QGroupBox>

class PropertiesWidget : public QWidget
{
    Q_OBJECT

private slots:
	void updateProperty();
	void rowsRemoved(const QModelIndex &parent, int start, int end);
	void dataChanged(const QModelIndex &topLeft, const QModelIndex& bottomRight);

private:
	CollectionModel *mdlCollection;
	QModelIndex index;

	MetaDataWidget *metadata;
	QGroupBox *grpBox;

public:
	PropertiesWidget(CollectionModel *mdlCollection, QWidget *parent = 0);

public slots:
	void setIndex(const QModelIndex &index);
	void clear();
};

#endif

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


#ifndef COLLECTIONMODEL_H
#define COLLECTIONMODEL_H

// Qt
#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>
#include <QObject>
#include <QStringList>
#include <QUrl>
// cWebshots
#include "cWebshots/Collection.h"

class CollectionModel : public QAbstractListModel
{
	Q_OBJECT
	
public:
	CollectionModel(cWebshots::Collection &collection, QObject *parent = 0);
	
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	
	void addFiles(QStringList files);
	void addFiles(QList<QUrl> urls);
	void add(cWebshots::Collection &collection);
	void remove(const QModelIndex &index);
	void update(const QModelIndex &index);

	bool changed();
	void resetChanged();
	
	cWebshots::Picture* picture(const QModelIndex &index);
		
private:
	cWebshots::Collection *ptrCollection;

	bool collectionHasChanged;
};

#endif

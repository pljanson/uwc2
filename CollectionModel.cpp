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


#include "CollectionModel.h"
#include "LoadingDialog.h"
// Qt
#include <QPixmap>
#include <QIcon>
#include <QBuffer>
#include <QByteArray>

CollectionModel::CollectionModel(cWebshots::Collection &collection, QObject *parent) : QAbstractListModel(parent)
{
	ptrCollection = &collection;

	resetChanged();
}

int CollectionModel::rowCount(const QModelIndex &parent) const
{
	return ptrCollection->pictureCount();
}

QVariant CollectionModel::data(const QModelIndex &index, int role) const
{
	// If the index is invalid or out of bounds
    if ((!index.isValid()) || (index.row() >= (int)ptrCollection->pictureCount()) || (index.column() != 0))
		return QVariant();

	switch (role)
	{
	case Qt::DecorationRole:
		// Thumbnail
		{
			QPixmap pm;
			
			// If there is no thumbnail
			if (ptrCollection->picture(index.row()).getThumbnailSize() == 0)
			{
				// Create one
				pm.loadFromData((unsigned char *)ptrCollection->picture(index.row()).getPicturePtr(), (unsigned int)ptrCollection->picture(index.row()).getPictureSize());
				pm = pm.scaled(100, 75, Qt::KeepAspectRatio);
				
				// Update the collection
				QByteArray bytes;
				QBuffer buffer(&bytes);
				buffer.open(QIODevice::WriteOnly);
				pm.save(&buffer, "JPG");
				ptrCollection->picture(index.row()).setThumbnail((unsigned char *)bytes.data(), bytes.size());
				
				// Return the thumbnail
				return pm;
			}
			else
			{
				// Load the thumbnail into a pixmap
				pm.loadFromData((unsigned char *)ptrCollection->picture(index.row()).getThumbnailPtr(), (unsigned int)ptrCollection->picture(index.row()).getThumbnailSize());
				// Resize and return it
				return pm.scaled(100, 75, Qt::KeepAspectRatio);
			}
		}
	case Qt::ToolTipRole:
	case Qt::StatusTipRole:
		// Picture title
		return QString::fromStdString(ptrCollection->picture(index.row()).metaData().get("title").toString());
	case Qt::DisplayRole:
	default:
		return QVariant();
	}
}

QVariant CollectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void CollectionModel::add(cWebshots::Collection &collection)
{
	beginInsertRows(QModelIndex(), ptrCollection->pictureCount(), ptrCollection->pictureCount());
	
	*ptrCollection += collection;
	collectionHasChanged = true;
	
	endInsertRows();	
}

// TODO : verifier la validite l'index
void CollectionModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	
	ptrCollection->removePicture(index.row());
	collectionHasChanged = true;
	
	endRemoveRows();
}

// TODO : Verifier la validite l'index
void CollectionModel::update(const QModelIndex &index)
{
	collectionHasChanged = true;
	emit dataChanged(index, index);
}

// TODO : Verifier la validite l'index
cWebshots::Picture* CollectionModel::picture(const QModelIndex &index)
{
	return &(ptrCollection->picture(index.row()));
}

bool CollectionModel::changed()
{
	return collectionHasChanged;
}

void CollectionModel::resetChanged()
{
	collectionHasChanged = false;
}

void CollectionModel::addFiles(QList<QUrl> urls)
{
	QStringList files;

	for (int i = 0; i < urls.size(); i++)
		files.push_back(urls[i].toLocalFile());

	return addFiles(files);
}

void CollectionModel::addFiles(QStringList files)
{
	LoadingDialog *dialog;

	dialog = new LoadingDialog();
	dialog->init(files.size());
	dialog->show();

	for (int i = 0; i < files.size(); i++)
	{
		try
		{
			cWebshots::Collection collection(files[i].toStdString());
			add(collection);

			dialog->addFile(true, files[i]);
		}
		catch (cWebshots::Exceptions::Exception e)
		{
			dialog->addFile(false, QString::fromStdString(e.Description()));
		}
	}

	dialog->conclude();
}

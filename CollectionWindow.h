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


#ifndef COLLECTIONWINDOW_H
#define COLLECTIONWINDOW_H

// Qt
#include <QMainWindow>
#include <QToolBar>
#include <QListWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTableWidget>
#include <QSplitter>
#include <QListView>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QMenu>

#include "cWebshots/Collection.h"

#include "CollectionModel.h"
#include "PreviewWidget.h"
#include "PropertiesWidget.h"
#include "QLinkedAction.h"

class CollectionWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	CollectionWindow(const QString filename = "");
	
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void closeEvent(QCloseEvent *event);

private slots:
	void save();
	void saveAs();
	void savePicture();
	void saveAllPictures();
	void addPicture();
	void removePicture();
	void collectionMetaData();

	void updateControls();
	void updateTitle();

private:
	QWidget *wdgMain;
	
	QGroupBox *grpPicture;
	PreviewWidget *preview;
	
	PropertiesWidget *properties;

	QSplitter *splPicture;
	QSplitter *splMain;
	
	QLinkedAction *actAddPicture;
	QLinkedAction *actRemovePicture;
	QLinkedAction *actSavePicture;
	QLinkedAction *actSaveAllPictures;
	
	QAction *actSaveCollection;
	QAction *actSaveCollectionAs;
	QAction *actClose;
	QAction *actCollectionMetaData;

	QPushButton *btnAddPicture;
	QPushButton *btnRemovePicture;
	QPushButton *btnSavePicture;
	QPushButton *btnSaveAllPictures;

	CollectionModel *mdlCollection;
	QListView *vwCollection;

	QMenu *mnuFile;
	QMenu *mnuPicture;
	QMenu *mnuMetadata;

	cWebshots::Collection Collection;
	cWebshots::Webshots_Formats fileFormat;
	QString fileName;
};

#endif

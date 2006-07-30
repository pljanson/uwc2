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


#include <QtGui>
#include "CollectionWindow.h"
#include "Misc.h"
#include "MetaDataDialog.h"

CollectionWindow::CollectionWindow(const QString filename)
{
	// Window
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowIcon(QIcon(":/images/picture.png"));
	resize(QSize(750,550));
	setAcceptDrops(true);
	
	// Central Widget
	wdgMain = new QWidget;

	if (!filename.isEmpty())
	{
		fileName = filename;
		fileFormat = cWebshots::identifyFile(filename.toStdString());
		Collection.loadFromFile(filename.toStdString());
	}
	mdlCollection = new CollectionModel(Collection, wdgMain);

	updateTitle();

	//---------------------------

	// Collection Model & View
	vwCollection = new QListView(wdgMain);
	vwCollection->setMinimumWidth(130);
	vwCollection->setFlow(QListView::LeftToRight);
	vwCollection->setWrapping(true);
	vwCollection->setViewMode(QListView::IconMode);
	vwCollection->setIconSize(QSize(100, 75));
	vwCollection->setSpacing(5);
	vwCollection->setResizeMode(QListView::Adjust);
	vwCollection->setModel(mdlCollection);
	
	// Buttons
	btnAddPicture = new QPushButton(this);
	btnAddPicture->setIcon(QIcon(":/images/add.png"));
	btnAddPicture->setToolTip("Add pictures");
	btnRemovePicture = new QPushButton(this);
	btnRemovePicture->setIcon(QIcon(":/images/remove.png"));
	btnRemovePicture->setToolTip("Remove selected picture");
	btnSavePicture = new QPushButton(this);
	btnSavePicture->setIcon(QIcon(":/images/filesave.png"));
	btnSavePicture->setToolTip("Save selected picture");
	btnSaveAllPictures = new QPushButton(this);	
	btnSaveAllPictures->setIcon(QIcon(":/images/filesaveall.png"));
	btnSaveAllPictures->setToolTip("Save all pictures");
	
	QHBoxLayout *layBtn = new QHBoxLayout;
	layBtn->addWidget(btnAddPicture);
	layBtn->addWidget(btnRemovePicture);
	layBtn->addWidget(btnSavePicture);
	layBtn->addWidget(btnSaveAllPictures);

	QVBoxLayout *layLeft = new QVBoxLayout;
	layLeft->addItem(layBtn);
	layLeft->addWidget(vwCollection);
	layLeft->setMargin(0);

	QWidget *wdgLeft = new QWidget(this);
	wdgLeft->setLayout(layLeft);

	//---------------------------

	// Preview
	grpPicture = new QGroupBox(this);
	grpPicture->setTitle("Picture preview");
	preview = new PreviewWidget(mdlCollection);
	grpPicture->setLayout(new QHBoxLayout);
	grpPicture->layout()->addWidget(preview);
		
	// Properties
	properties = new PropertiesWidget(mdlCollection);

	//---------------------------

	// Splitter Preview||Properties
	splPicture = new QSplitter(Qt::Vertical, this);
	grpPicture->resize(0,10000);
	properties->resize(0,0);
	splPicture->addWidget(grpPicture);
	splPicture->addWidget(properties);

	// Splitter Collection||(Preview||Properties)
	splMain = new QSplitter(Qt::Horizontal, wdgMain);
	vwCollection->resize(130,0);
	splPicture->resize(10000,0);
	splMain->addWidget(wdgLeft); //vwCollection);
	splMain->addWidget(splPicture);
	
	//---------------------------

	// Main Layout
	wdgMain->setLayout(new QHBoxLayout);
	wdgMain->layout()->addWidget(splMain);
	
	setCentralWidget(wdgMain);

	//---------------------------

	// Actions
	actAddPicture = new QLinkedAction(QIcon(":/images/add.png"), "Add pictures", this);
	actRemovePicture = new QLinkedAction(QIcon(":/images/remove.png"), "Remove selected picture", this);
	actSavePicture = new QLinkedAction(QIcon(":/images/filesave.png"), "Save selected picture", this);
	actSaveAllPictures = new QLinkedAction(QIcon(":/images/filesaveall.png"), "Save all pictures", this);
	actSaveCollection = new QAction(QIcon(":/images/filesave.png"), "Save file", this);
	actSaveCollectionAs = new QAction(QIcon(":/images/filesaveas.png"), "Save file as", this);
	actClose = new QAction(QIcon(":/images/exit.png"), "Close", this);
	actCollectionMetaData = new QAction(QIcon(":/images/edit.png"), "Collection metadata", this);

	actAddPicture->linkToPushButton(btnAddPicture);
	actRemovePicture->linkToPushButton(btnRemovePicture);
	actSavePicture->linkToPushButton(btnSavePicture);
	actSaveAllPictures->linkToPushButton(btnSaveAllPictures);

	//---------------------------

	// Menus
	mnuFile = new QMenu("File", this);
	mnuPicture = new QMenu("Picture", this);
	mnuMetadata = new QMenu("Metadata", this);

	menuBar()->addMenu(mnuFile);
	menuBar()->addMenu(mnuPicture);
	menuBar()->addMenu(mnuMetadata);
	mnuFile->addAction(actSaveCollection);
	mnuFile->addAction(actSaveCollectionAs);
	mnuFile->addAction(actClose);
	mnuPicture->addAction(actAddPicture);
	mnuPicture->addAction(actRemovePicture);
	mnuPicture->addAction(actSavePicture);
	mnuPicture->addAction(actSaveAllPictures);
	mnuMetadata->addAction(actCollectionMetaData);

	//---------------------------

	connect(mdlCollection, SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(updateTitle()));
	connect(mdlCollection, SIGNAL(rowsRemoved(QModelIndex, int, int)), this, SLOT(updateTitle()));
	connect(mdlCollection, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(updateTitle()));

	connect(mdlCollection, SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(updateControls()));
	connect(mdlCollection, SIGNAL(rowsRemoved(QModelIndex, int, int)), this, SLOT(updateControls()));
	connect(vwCollection->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(updateControls()));
	connect(vwCollection->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), properties, SLOT(setIndex(QModelIndex)));
	connect(vwCollection->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), preview, SLOT(setIndex(QModelIndex)));
	
	connect(actAddPicture, SIGNAL(triggered(bool)), this, SLOT(addPicture()));
	connect(actRemovePicture, SIGNAL(triggered(bool)), this, SLOT(removePicture()));
	connect(actSavePicture, SIGNAL(triggered(bool)), this, SLOT(savePicture()));
	connect(actSaveAllPictures, SIGNAL(triggered(bool)), this, SLOT(saveAllPictures()));
	connect(actSaveCollection, SIGNAL(triggered(bool)), this, SLOT(save()));
	connect(actSaveCollectionAs, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
	connect(actClose, SIGNAL(triggered(bool)), this, SLOT(close()));
	connect(actCollectionMetaData, SIGNAL(triggered(bool)), this, SLOT(collectionMetaData()));

	//---------------------------

	updateControls();
}

void CollectionWindow::savePicture()
{
	QModelIndex selection = vwCollection->selectionModel()->currentIndex();

	if (selection.isValid())
	{
		QString filename = QFileDialog::getSaveFileName(
								this,
								"Save selected picture",
								QString::fromStdString(Collection[selection.row()].metaData().get("title").toString()),
								"JPEG File (*.jpg)");

		if (!filename.isEmpty())
		{
			if (!filename.endsWith(".jpg", Qt::CaseInsensitive))
				filename.append(".jpg");

			Collection[selection.row()].saveToFile(filename.toStdString(), cWebshots::JPG);
		}
	}
}

// TODO : gerer le fait qu'il n'y ait pas de titre
void CollectionWindow::saveAllPictures()
{
	if (Collection.pictureCount() > 0)
	{
		QString path = QFileDialog::getExistingDirectory(
						this,
						"Save all pictures",
						"",
						QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

		if (!path.isEmpty())
		{
			for (int i = 0; i < Collection.pictureCount(); i++)
			{
				QString filename;

				filename = QDir(path).filePath(QString::fromStdString(Collection[i].metaData().get("title").toString()) + ".jpg");

				Collection[i].saveToFile(filename.toStdString(), cWebshots::JPG);
			}
		}
	}
}

void CollectionWindow::addPicture()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
                            this,
                            "Add pictures",
                            "",
                            "All pictures (*.wbc *.wbp *.wbz *.wb0 *.wb1 *.wbd *.jpg *.jpeg)\nWebshots Files (*.wbc *.wbp *.wbz *.wb0 *.wb1 *.wbd)\nCollections (*.wbc *.wbp *.wbz)\nPictures (*.wbd *.wb0 *.wb1)\nJPEG Files (*.jpg *.jpeg)");

	if (!filenames.isEmpty())
		mdlCollection->addFiles(filenames);
}

void CollectionWindow::removePicture()
{
	mdlCollection->remove(vwCollection->selectionModel()->currentIndex());
	
	// <Ugly Hack>
	// At this point, the selection should have changed, and the selection model should
	// have emitted a currentChanged signal. Since this is not the case (probably a bug of Qt
	// or I don't have correctly understood how it works), we have to update views manually.
	preview->setIndex(vwCollection->selectionModel()->currentIndex());
	properties->setIndex(vwCollection->selectionModel()->currentIndex());
	// </Ugly Hack>
}


void CollectionWindow::updateControls()
{
	QModelIndex selection = vwCollection->selectionModel()->currentIndex();

	actSaveAllPictures->setEnabled(Collection.pictureCount() > 0);
	actRemovePicture->setEnabled(selection.isValid());
	actSavePicture->setEnabled(selection.isValid());
}

void CollectionWindow::save()
{
	// The file has never been saved or the format was not a collection
	if ((fileName.isEmpty()) || !((fileFormat == cWebshots::WBC) || (fileFormat == cWebshots::WBZ)))
		saveAs();
	// The file has already been saved, just overwrite
	else
	{
		Collection.saveToFile(fileName.toStdString(), fileFormat);
		mdlCollection->resetChanged();
		updateTitle();
	}
}

void CollectionWindow::saveAs()
{
	QString selectedFilter;
	QString filename = QFileDialog::getSaveFileName(
							this,
							"Save as",
							"",
							"WBZ (*.wbz)\nWBC (*.wbc *.wbd)",
							&selectedFilter
							);

	if (!filename.isEmpty())
	{
		if (selectedFilter.startsWith("WBZ"))
		{
			if (!filename.endsWith(".wbz", Qt::CaseInsensitive))
				filename.append(".wbz");

			fileName = filename;
			fileFormat = cWebshots::WBZ;
		}
		else if (selectedFilter.startsWith("WBC"))
		{
			if ((!filename.endsWith(".wbc", Qt::CaseInsensitive)) && (!filename.endsWith(".wbd", Qt::CaseInsensitive)))
				filename.append(".wbc");

			fileName = filename;
			fileFormat = cWebshots::WBC;
		}

		Collection.saveToFile(fileName.toStdString(), fileFormat);

		mdlCollection->resetChanged();
		updateTitle();
	}
}


void CollectionWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if ((event->mimeData()->hasUrls()) && (checkFiles(event->mimeData()->urls())))
		event->acceptProposedAction();
	else
		event->ignore();
}

void CollectionWindow::dropEvent(QDropEvent *event)
{
	mdlCollection->addFiles(event->mimeData()->urls());

	event->acceptProposedAction();
}

void CollectionWindow::updateTitle()
{
	QString title = "UWC 2 - ";

	if (fileName.isEmpty())
		title += "New Collection";
	else
		title += fileName;

	if (mdlCollection->changed())
		title += " *";

	setWindowTitle(title);
}

void CollectionWindow::closeEvent(QCloseEvent *event)
{
	if (mdlCollection->changed())
	{
		int choice = QMessageBox::question(this, "Save the collection ?", "Changes have been made to the collection.\nIf you close the window, they will be lost.\nDo you want to save the collection before ?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);

		switch (choice)
		{
		case QMessageBox::Yes:
			save();
			event->accept();
			break;
		case QMessageBox::No:
			event->accept();
			break;
		case QMessageBox::Cancel:
			event->ignore();
			break;
		}
	}
	else
		event->accept();

}

void CollectionWindow::collectionMetaData()
{
	MetaDataDialog *dia = new MetaDataDialog("Collection Metadata", this);

	dia->setAvailableKeys(cWebshots::Collection::availableKeys());
	dia->setMetaData(Collection.metaData());

	if (dia->exec() == MetaDataDialog::Accepted)
		Collection.setMetaData(dia->metaData());

	delete dia;
}

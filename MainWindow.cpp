/*
    Ultimate Webshots Converter 2.0
    Copyright (C) 2006  Herv� "Setaou" BRY <uwc at apinc dot org>

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


// Qt
#include <QtGui>
// Window
#include "MainWindow.h"
#include "CollectionWindow.h"
#include "LoadingDialog.h"
#include "Misc.h"

MainWindow::MainWindow()
{
	setWindowTitle("UW 2.0");
	setWindowIcon(QIcon(":/images/uwc.png"));
	layout()->setSizeConstraint(QLayout::SetFixedSize);
	setAttribute(Qt::WA_DeleteOnClose);
	setAcceptDrops(true);
	
	// Background
	lblBackground = new QLabel(this);
	QPixmap bg;
	bg.load(":/images/main-back.png");
	lblBackground->setPixmap(bg);
	setCentralWidget(lblBackground);
	
	// Actions
	actOpen = new QAction(QIcon(":/images/fileopen.png"), "&Open", this);
	connect(actOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	actNew = new QAction(QIcon(":/images/filenew.png"), "&New", this);
	connect(actNew, SIGNAL(triggered()), this, SLOT(newFile()));
	actConvert = new QAction(QIcon(":/images/convert.png"), "Batch &Conversion", this);
	actLanguage = new QAction(QIcon(":/images/configure.png"), "&Language", this);
	actQuit = new QAction(QIcon(":/images/exit.png"), "&Quit", this);
	connect(actQuit, SIGNAL(triggered()), this, SLOT(quit()));
	actAbout = new QAction(QIcon(":/images/info.png"), "&About", this);
		
	// Toolbar
	tlbMain = addToolBar("Main");
	tlbMain->setMovable(false);
	tlbMain->addAction(actNew);
	tlbMain->addAction(actOpen);
	tlbMain->addSeparator();
	tlbMain->addAction(actConvert);
	tlbMain->addSeparator();
	tlbMain->addAction(actAbout);
	
	// Menubar
	mnuFile = menuBar()->addMenu("&File");
	mnuFile->addAction(actNew);
	mnuFile->addAction(actOpen);
	mnuFile->addSeparator();
	mnuFile->addAction(actQuit);	
	mnuConvert = menuBar()->addMenu("&Convert");
	mnuConvert->addAction(actConvert);
	mnuSettings = menuBar()->addMenu("&Settings");
	mnuSettings->addAction(actLanguage);
	mnuAbout = menuBar()->addMenu("?");
	mnuAbout->addAction(actAbout);
}

void MainWindow::newFile()
{
	CollectionWindow *wndCollection = new CollectionWindow();
	wndCollection->show();
}

void MainWindow::openFile()
{
	QStringList filenames = QFileDialog::getOpenFileNames(this,"Open","","Webshots Files (*.wbc *.wbp *.wbz *.wb0 *.wb1 *.wbd)\nCollections (*.wbc *.wbp *.wbz)\nPictures (*.wbd *.wb0 *.wb1)");

	if (!filenames.isEmpty())
		loadFiles(QStringList(filenames));
}

void MainWindow::showConversionWnd()
{
}

void MainWindow::showLanguageDlg()
{
}

void MainWindow::quit()
{
	qApp->closeAllWindows();
}

void MainWindow::showAboutDlg()
{
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if ((event->mimeData()->hasUrls()) && (checkFiles(event->mimeData()->urls())))
		event->acceptProposedAction();
	else
		event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)
{
	loadFiles(event->mimeData()->urls());

	event->acceptProposedAction();
}

void MainWindow::loadFiles(QList<QUrl> urls)
{
	QStringList files;

	for (int i = 0; i < urls.size(); i++)
		files.push_back(urls[i].toLocalFile());

	return loadFiles(files);
}

void MainWindow::loadFiles(QStringList files)
{
	LoadingDialog *dialog;

	dialog = new LoadingDialog();
	dialog->init(files.size());
	dialog->show();

	for (int i = 0; i < files.size(); i++)
	{
		try
		{
			CollectionWindow *wndCollection = new CollectionWindow(files[i]);
			wndCollection->show();

			dialog->addFile(true, files[i]);
		}
		catch (cWebshots::Exceptions::Exception e)
		{
			dialog->addFile(false, QString::fromStdString(e.Description()));
		}
	}

	dialog->conclude();
}

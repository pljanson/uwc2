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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMenuBar>

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
	MainWindow();

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	
	void loadFiles(QStringList files);
	void loadFiles(QList<QUrl> urls);

	private slots:
	void newFile();
	void openFile();
    void exportArchive();
    void batchConvert();
	void showConversionWnd();
	void showLanguageDlg();
	void quit();
	void showAboutDlg();
	
	private:
	QLabel *lblBackground;
	
	QAction *actOpen;
	QAction *actNew;
	QAction *actConvert;
    QAction *actExport;
	QAction *actLanguage;
	QAction *actQuit;
	QAction *actAbout;
	
	QMenu *mnuFile;
	QMenu *mnuConvert;
	QMenu *mnuSettings;
	QMenu *mnuAbout;
	
	QToolBar *tlbMain;
};

#endif

/*
    ExportWindow.cpp
    based on the existing CollectionWindow
    The export window requires setting the source and destination;
    and these must be different. Only then is the export button enabled.
    Clicking in the path also opens the browse window (ClickableLabel).

    Copyright (C) 2015 PLJ

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
#include <QFileDialog>
#include "ExportWindow.h"
#include "ExportArchive.h"

#include <QDebug>

//#define FASTTEST

#ifndef FASTTEST
ExportWindow::ExportWindow() : sourceSelected(false), destinationSelected(false)
#else
ExportWindow::ExportWindow() : sourceSelected(true), destinationSelected(true) //for fast testing
#endif
{
    // Window
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/images/picture.png"));
    resize(QSize(600,600));

    // Central Widget
    wdgMain = new QWidget;

    //---------------------------
    lblExportTitle = new QLabel(this);
    lblExportTitle->setText("<H1>Export Webshots Archive</H1>");

    //- - -

    lblSrc = new QLabel(this);
    lblSrc->setText("Source:");

    lblSrcPath = new ClickableLabel("", this);
    lblSrcPath->setFrameStyle(QFrame::Panel);
#ifndef FASTTEST
    lblSrcPath->setText("select a directory");
#else
    lblSrcPath->setText("E:\\Webshots Data2"); //for fast testing
#endif
    btnBrowseSrc = new QPushButton(this);
    btnBrowseSrc->setIcon(QIcon(":/images/fileopen.png"));
    btnBrowseSrc->setToolTip("Browse Source Directory");

    //- - -

    lblDest = new QLabel(this);
    lblDest->setText("Destination:");

    lblDestPath = new ClickableLabel("", this);
    lblDestPath->setFrameStyle(QFrame::Panel);
#ifndef FASTTEST
    lblDestPath->setText("select a directory");
#else
    lblDestPath->setText("E:\\WebshotsOut"); //for fast testing
#endif
    btnBrowseDest = new QPushButton(this);
    btnBrowseDest->setIcon(QIcon(":/images/fileopen.png"));
    btnBrowseDest->setToolTip("Browse Destination Directory");

    //- - -

    cbDoThumbs = new QCheckBox("Do Process &Thumbnails", this);
    cbDoThumbs->setChecked(1);
    cbDoAlbumInfo = new QCheckBox("Do &Copy album.txt and photo.txt", this);
    cbDoAlbumInfo->setChecked(1);
    cbStopOnError= new QCheckBox("&Stop on errors", this);
    cbStopOnError->setChecked(0);

    //- - -

    btnExport = new QPushButton(this);
    btnExport->setIcon(QIcon(":/images/export.png"));
    btnExport->setToolTip("Export Webshots");

    // - - -

    txtEdit = new QTextEdit(this);
    txtEdit->setStyleSheet("font:9pt \"Courier\";");

    //---------------------------

    QGridLayout *layout = new QGridLayout();
    layout->setSpacing(10);

    layout->addWidget(lblExportTitle,0, 1, 1, 3);
    layout->addWidget(lblSrc,        1, 0, 1, 1);
    layout->addWidget(lblSrcPath,    1, 1, 1, 3);
    layout->addWidget(btnBrowseSrc,  1, 4, 1, 1);

    layout->addWidget(lblDest,       2, 0, 1, 1);
    layout->addWidget(lblDestPath,   2, 1, 1, 3);
    layout->addWidget(btnBrowseDest, 2, 4, 1, 1);

    //layout->addWidget(btnExport,   3, 0, 1, 5);
    layout->addWidget(cbDoThumbs,    3, 1, 1, 3);
    layout->addWidget(cbDoAlbumInfo, 4, 1, 1, 3);
    layout->addWidget(cbStopOnError, 5, 1, 1, 3);

    layout->addWidget(btnExport,     6, 0, 1, 5);
    layout->addWidget(txtEdit,       7, 0, 5, 5);

    // Main Layout
    wdgMain->setLayout(layout);
    setCentralWidget(wdgMain);

    //---------------------------

    // Actions
    connect(btnBrowseSrc, SIGNAL(clicked()),this, SLOT(browseSrc()));
    connect(lblSrcPath, SIGNAL(clicked()), this, SLOT(browseSrc()));

    connect(btnBrowseDest, SIGNAL(clicked()),this, SLOT(browseDest()));
    connect(lblDestPath, SIGNAL(clicked()), this, SLOT(browseDest()));

    connect(btnExport, SIGNAL(clicked()),this, SLOT(exportArchive()));

    //---------------------------
    updateControls();
}

void ExportWindow::browseSrc()
{
    QString dirSrcPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    QDir dir(dirSrcPath);
    if (dir.exists() && !dirSrcPath.isEmpty())
    {
        lblSrcPath->setText(dirSrcPath);
        sourceSelected = true;
    }
    else
    {
        lblSrcPath->setText("select a directory");
        qDebug() << "lblSrcPath [" << dirSrcPath << "] does not exists";
    }
    updateControls();
}

void ExportWindow::browseDest()
{
    QString dirDestPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    QDir dir(dirDestPath);
    if (dir.exists() && !dirDestPath.isEmpty())
    {
        lblDestPath->setText(dirDestPath);
        destinationSelected = true;
    }
    else
    {
        lblDestPath->setText("select a directory");
        qDebug() << "lblDestPath [" << dirDestPath << "] does not exists";
    }
    updateControls();
}

void ExportWindow::exportArchive()
{
    //check if source nd destination are different
    if(lblSrcPath->text() == lblDestPath->text())
    {
        //we do not allow the same directory for source and destination, disable destination.
        lblDestPath->setText("Select a different directory than the source");
        destinationSelected = false;
        updateControls();
    }
    else
    {
        //feedback
        txtEdit->clear();
        txtEdit->append("start ExportWebshotsArchive");
        txtEdit->append("Source      [" + lblSrcPath->text() + "]");
        txtEdit->append("Destination [" + lblDestPath->text() + "]");
        QString optionMsg = "options  : ";
        if (cbDoThumbs->isChecked())
        {
            optionMsg.append(" DoThumbs");
        }
        if (cbDoAlbumInfo->isChecked())
        {
            optionMsg.append(" DoAlbumInfo");
        }
        if (cbStopOnError->isChecked())
        {
            optionMsg.append(" StopOnError");
        }
        txtEdit->append(optionMsg);
        txtEdit->append("");

        //setup
        ExportArchive::ExportOptions options(
          this,                  //for use of callback function logMessage(...)
          lblSrcPath->text(),
          lblDestPath->text(),
          cbDoThumbs->isChecked(),
          cbDoAlbumInfo->isChecked(),
          cbStopOnError->isChecked()
        );

        //action
        bool succes = ExportArchive::ExportWebshotsArchive(options);

        //reporting
        QString msg = "Export of [" + lblSrcPath->text() + "] was " + QString(succes?"Succesful":"NOT succesful");
        qDebug() << msg;
        txtEdit->append(msg);

        //close if completely successful; is this useful? or only of StopOnError?
        //if (succes)
        //{
        //    this->close();
        //}
    }
}

//---------------------------

void ExportWindow::updateControls()
{
    btnExport->setEnabled(sourceSelected && destinationSelected);
}

// callback function to put addtional loggin into the QTextEdit widget
void ExportWindow::logMessage(QString aMsg)
{
    //if(m_LogEdit && aMsg)
    txtEdit->append(aMsg);
}
//---------------------------


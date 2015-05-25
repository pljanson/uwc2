/*
    Ultimate Webshots Converter 2.0

    ExportWindow.h
    based on  CollectionWindow
    The export window requires setting the source and desrtination,
    and these must be different. Only then is the export button enabled.
    Clicking in the path also opens the browse window.

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
#ifndef EXPORTWINDOW
#define EXPORTWINDOW

// Qt
#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include "ClickableLabel.h"
#include <QTextEdit>

class ExportWindow : public QMainWindow
{
    Q_OBJECT

public:
    ExportWindow();
    void logMessage(QString aMsg);

private slots:
    void exportArchive();
    void browseSrc();
    void browseDest();

private:
    QWidget *wdgMain;

    QLabel *lblExportTitle;

    bool sourceSelected;
    QLabel *lblSrc;
    ClickableLabel *lblSrcPath;
    QPushButton *btnBrowseSrc;

    bool destinationSelected;
    QLabel *lblDest;
    ClickableLabel *lblDestPath;
    QPushButton *btnBrowseDest;

    QCheckBox *cbDoThumbs;
    QCheckBox *cbDoAlbumInfo;
    QCheckBox *cbStopOnError;

    QPushButton *btnExport;

    QTextEdit *txtEdit;

    void updateControls();
};

#endif // EXPORTWINDOW

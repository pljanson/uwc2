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


#ifndef METADATADIALOG_H
#define METADATADIALOG_H

// Qt
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "MetaDataWidget.h"

class MetaDataDialog : public QDialog
{
	Q_OBJECT

	private:
		MetaDataWidget *wdgMetadata;
		QPushButton *btnOk;
		QPushButton *btnCancel;
	
	public:
		MetaDataDialog(QString title = "", QWidget *parent = 0);

		void setMetaData(cWebshots::MetaData metadata);
		cWebshots::MetaData metaData();

		void setAvailableKeys(std::vector<cWebshots::Datum::KeyType> keys);
		std::vector<cWebshots::Datum::KeyType> availableKeys();

	public slots:
		void accept();
		void reject();
};

#endif

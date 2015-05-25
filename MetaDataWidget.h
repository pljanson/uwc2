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


#ifndef METADATAWIDGET_H
#define METADATAWIDGET_H

#include "cWebshots/MetaData.h"

// Qt
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>

class MetaDataWidget : public QWidget
{
    Q_OBJECT

private slots:
	void updateProperty();
	void addClicked();
	void removeClicked();
	void listChanged(int index);

private:
	QTableWidget *tblProperties;
	QPushButton *btnPlus, *btnMinus;
	QComboBox *cmbList;

	cWebshots::MetaData metadata;
	std::vector<cWebshots::Datum::KeyType> keys;

public:
	MetaDataWidget(QWidget *parent = 0);

	void setMetaData(cWebshots::MetaData metadata);
	cWebshots::MetaData metaData();

	void setAvailableKeys(std::vector<cWebshots::Datum::KeyType> keys);
	std::vector<cWebshots::Datum::KeyType> availableKeys();

public slots:
	void clear();

signals:
	void dataChanged();
};

#endif

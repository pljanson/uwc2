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

#include "MetaDataDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

MetaDataDialog::MetaDataDialog(QString title, QWidget *parent) : QDialog(parent, 0)
{
	setWindowTitle(title);

	wdgMetadata = new MetaDataWidget(this);
	btnOk = new QPushButton("Ok", this);
	btnCancel = new QPushButton("Cancel", this);

	QVBoxLayout *layout1 = new QVBoxLayout();
	QHBoxLayout *layout2 = new QHBoxLayout();

	QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);

	layout2->addItem(spacer);
	layout2->addWidget(btnOk);
	layout2->addWidget(btnCancel);

	layout1->addWidget(wdgMetadata);
	layout1->addLayout(layout2);

	this->setLayout(layout1);

	connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(accept()));
	connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

void MetaDataDialog::setMetaData(cWebshots::MetaData metadata)
{
	this->wdgMetadata->setMetaData(metadata);
}

cWebshots::MetaData MetaDataDialog::metaData()
{
	return wdgMetadata->metaData();
}

void MetaDataDialog::setAvailableKeys(std::vector<cWebshots::Datum::KeyType> keys)
{
	wdgMetadata->setAvailableKeys(keys);
}

std::vector<cWebshots::Datum::KeyType> MetaDataDialog::availableKeys()
{
	return wdgMetadata->availableKeys();
}

void MetaDataDialog::accept()
{
	hide();
	setResult(QDialog::Accepted);
}

void MetaDataDialog::reject()
{
	hide();
	setResult(QDialog::Rejected);
}
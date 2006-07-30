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


#include "MetaDataWidget.h"
// Qt
#include <QtGui>

MetaDataWidget::MetaDataWidget(QWidget *parent) : QWidget(parent)
{
	// Table
	tblProperties = new QTableWidget(0, 1, this);
	tblProperties->setSelectionMode(QAbstractItemView::NoSelection);
	tblProperties->verticalHeader()->setClickable(false);
	tblProperties->verticalHeader()->setResizeMode(QHeaderView::Custom);
	tblProperties->horizontalHeader()->setClickable(false);
	tblProperties->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	tblProperties->horizontalHeader()->hide();

	// Combobox and buttons
	btnMinus = new QPushButton(QIcon(":/images/remove.png"), "", this);
	btnPlus = new QPushButton(QIcon(":/images/add.png"), "", this);
	cmbList = new QComboBox(this);

	cmbList->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	btnMinus->setEnabled(false);
	btnPlus->setEnabled(false);

	QHBoxLayout *layBottom = new QHBoxLayout;
	layBottom->addWidget(cmbList);
	layBottom->addWidget(btnPlus);
	layBottom->addWidget(btnMinus);

	// Main Layout
	QVBoxLayout *layMain = new QVBoxLayout;
	setLayout(layMain);
	layMain->setMargin(0);
	layMain->addWidget(tblProperties);
	layMain->addLayout(layBottom);

	// Connections
	connect(btnPlus, SIGNAL(clicked(bool)), this, SLOT(addClicked()));
	connect(btnMinus, SIGNAL(clicked(bool)), this, SLOT(removeClicked()));
	connect(cmbList, SIGNAL(currentIndexChanged(int)), this, SLOT(listChanged(int)));
}

void MetaDataWidget::setMetaData(cWebshots::MetaData metadata)
{
	// If nothing has changed, return
	if (this->metadata == metadata)
		return;

	this->metadata = metadata;

	std::vector<std::string> v = metadata.listKeys();

	// Adjust the number of rows
	for (int i = tblProperties->rowCount(); i < v.size(); i++)
		tblProperties->insertRow(tblProperties->rowCount());
	for (int i = tblProperties->rowCount(); i > v.size(); i--)
		tblProperties->removeRow(tblProperties->rowCount() - 1);

	// Populate rows
	QStringList headers;
	for (int i = 0; i < v.size(); i++)
	{
		// Add the row header to the list
		headers << QString::fromStdString(v.at(i));

		// Delete the cell widget
		if (tblProperties->cellWidget(i, 0) != NULL)
			tblProperties->cellWidget(i, 0)->deleteLater();

		// Create the new cell widget according to the type of the Datum
		cWebshots::Datum dat = metadata.get(v.at(i));
		switch (dat.getType())
		{
		case cWebshots::Datum::type_string:
			{
				QLineEdit* widget = new QLineEdit(this);
				widget->setFrame(false);
				widget->setText(QString::fromStdString(dat.toString()));
				tblProperties->setCellWidget(i, 0,  widget);
				connect(widget, SIGNAL(editingFinished()), this, SLOT(updateProperty()));
				break;
			}
		case cWebshots::Datum::type_integer:
			{
				QSpinBox* widget = new QSpinBox(this);
				widget->setFrame(false);
				widget->setValue(dat.toInt());
				tblProperties->setCellWidget(i, 0,  widget);
				connect(widget, SIGNAL(editingFinished()), this, SLOT(updateProperty()));
				break;
			}
		case cWebshots::Datum::type_date:
			{
				QDateEdit* widget = new QDateEdit(this);
				widget->setFrame(false);
				QDateTime date;
				date.setTime_t(dat.toDate());
				widget->setDate(date.date());
				widget->setDisplayFormat("dd MMMM yyyy");
				widget->setMinimumDate(QDate(1970, 01, 01));
				tblProperties->setCellWidget(i, 0,  widget);
				connect(widget, SIGNAL(editingFinished()), this, SLOT(updateProperty()));

				// Hack : Updates the date to reflect the value rounded by the widget
				this->metadata.setDate(dat.getKey(), QDateTime(widget->date()).toTime_t());

				break;
			}
		case cWebshots::Datum::type_bool:
			{
				QComboBox* widget = new QComboBox(this);
				widget->setFrame(false);
				widget->addItem("True");
				widget->addItem("False");
				widget->setCurrentIndex(dat.toBool() ? 0 : 1);
				tblProperties->setCellWidget(i, 0,  widget);
				connect(widget, SIGNAL(currentIndexChanged(int)), this, SLOT(updateProperty()));
			}
		}
	}

	// Set rows labels and height
	tblProperties->setVerticalHeaderLabels(headers);
	tblProperties->resizeRowsToContents();

	// Update buttons
	listChanged(cmbList->currentIndex());
}

cWebshots::MetaData MetaDataWidget::metaData()
{
	return metadata;
}

void MetaDataWidget::updateProperty()
{
	std::vector<std::string> v = metadata.listKeys();
	bool changed = false;

	// Check if the value of the widgets have changed and update metadata if necessary
	for (int i = 0; i < v.size(); i++)
	{
		cWebshots::Datum dat = metadata.get(v.at(i));
		switch (dat.getType())
		{
		case cWebshots::Datum::type_string:
			{
				QLineEdit* widget = (QLineEdit*)tblProperties->cellWidget(i, 0);
				if (dat.toString() != widget->text().toStdString())
				{
					changed = true;
					metadata.setString(dat.getKey(), widget->text().toStdString());
				}
				break;
			}
		case cWebshots::Datum::type_integer:
			{
				QSpinBox* widget = (QSpinBox*)tblProperties->cellWidget(i, 0);
				if (dat.toInt() !=  widget->value())
				{
					changed = true;
					metadata.setInt(dat.getKey(), widget->value());
				}
				break;
			}
		case cWebshots::Datum::type_date:
			{
				QDateEdit* widget = (QDateEdit*)tblProperties->cellWidget(i, 0);
				if (dat.toDate() != QDateTime(widget->date()).toTime_t())
				{
					changed = true;
					metadata.setDate(dat.getKey(), QDateTime(widget->date()).toTime_t());
				}
				break;
			}
		case cWebshots::Datum::type_bool:
			{
				QComboBox* widget = (QComboBox*)tblProperties->cellWidget(i, 0);
				if (dat.toBool() != (widget->currentIndex() == 0))
				{
					changed = true;
					metadata.setBool(dat.getKey(), widget->currentIndex() == 0);
				}
			}
		}		
	}

	// Emit the signal dataChanged if at least one value has changed
	if (changed)
		emit dataChanged();
}

void MetaDataWidget::clear()
{
	// Clear the metadata
	metadata.clear();
	// Delete all lines
	for (int i = tblProperties->rowCount() - 1; i >= 0; i--)
		tblProperties->removeRow(i);
	// Emit the signal dataChanged
	emit dataChanged();
}

void MetaDataWidget::setAvailableKeys(std::vector<cWebshots::Datum::KeyType> keys)
{
	this->keys = keys;
	cmbList->clear();

	// Populate the list
	for (int i = 0; i < keys.size(); i++)
		cmbList->addItem(QString::fromStdString(keys.at(i).key));
}

std::vector<cWebshots::Datum::KeyType> MetaDataWidget::availableKeys()
{
	return keys;
}

void MetaDataWidget::addClicked()
{
	cWebshots::MetaData meta = metadata;

	switch (keys.at(cmbList->currentIndex()).type)
	{
	case cWebshots::Datum::type_bool:
		meta.setBool(keys.at(cmbList->currentIndex()).key, false);
		break;
	case cWebshots::Datum::type_date:
		meta.setDate(keys.at(cmbList->currentIndex()).key, QDateTime::currentDateTime().toTime_t());
		break;
	case cWebshots::Datum::type_integer:
		meta.setInt(keys.at(cmbList->currentIndex()).key, 0);
		break;
	case cWebshots::Datum::type_string:
		meta.setString(keys.at(cmbList->currentIndex()).key, "");
		break;
	}

	setMetaData(meta);
	listChanged(cmbList->currentIndex());
	emit dataChanged();
}

void MetaDataWidget::removeClicked()
{
	cWebshots::MetaData meta = metadata;
	meta.clear(keys.at(cmbList->currentIndex()).key);

	setMetaData(meta);
	listChanged(cmbList->currentIndex());
	emit dataChanged();
}

void MetaDataWidget::listChanged(int index)
{
	if (index == -1)
	{
		btnMinus->setEnabled(false);
		btnPlus->setEnabled(false);
	}
	else if (metadata.exists(keys.at(index).key))
	{
		btnMinus->setEnabled(true);
		btnPlus->setEnabled(false);
	}
	else
	{
		btnMinus->setEnabled(false);
		btnPlus->setEnabled(true);
	}
}

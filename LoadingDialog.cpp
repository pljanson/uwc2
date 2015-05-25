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

#include "LoadingDialog.h"

LoadingDialog::LoadingDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	setWindowFlags(Qt::WindowStaysOnTopHint | windowFlags());
	setAttribute(Qt::WA_DeleteOnClose);

	nbOk = 0;
	nbErr = 0;
}

void LoadingDialog::addFile(bool ok, QString text)
{
	QListWidgetItem *item = new QListWidgetItem();

	if (ok)
	{
		item->setIcon(QIcon(":/images/right.png"));
		nbOk++;
	}
	else
	{
		item->setIcon(QIcon(":/images/wrong.png"));
		nbErr++;
	}

	item->setText(text);

	list->addItem(item);
	list->setCurrentItem(item);
	QApplication::processEvents();

	progress->setValue(progress->value() + 1);
}

void LoadingDialog::conclude()
{
	// If there has been no errors, close the window
	if (nbErr == 0)
		close();

	// Otherwise, add a summary
	QListWidgetItem *item1 = new QListWidgetItem(QIcon(":/images/info.png"), QString::number(nbOk) + " file(s) loaded successfully.");
	QListWidgetItem *item2 = new QListWidgetItem(QIcon(":/images/info.png"), QString::number(nbErr) + " error(s).");
	list->addItem(item1);
	list->addItem(item2);

	list->setCurrentItem(item2);
}

void LoadingDialog::init(int nbFiles)
{
	progress->setMaximum(nbFiles);		
}

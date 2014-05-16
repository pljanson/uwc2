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

#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include "ui_LoadingDialog.h"

class LoadingDialog : public QDialog, private Ui::LoadingDialog
{
    Q_OBJECT

public:
    LoadingDialog(QWidget *parent = 0);
	void addFile(bool ok, QString text);
	void init(int nbFiles);
	void conclude();

private:
	int nbOk, nbErr;
};

#endif
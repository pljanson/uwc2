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

#include "QLinkedAction.h"

void QLinkedAction::setEnabled(bool state)
{
	QAction::setEnabled(state);
	emit stateChanged(state);
}

void QLinkedAction::linkToPushButton(QPushButton *button)
{
	QObject::connect(button, SIGNAL(clicked(bool)), this, SLOT(trigger()));
	QObject::connect(button, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)));
	QObject::connect(this, SIGNAL(stateChanged(bool)), button, SLOT(setEnabled(bool)));
}

QLinkedAction::QLinkedAction (QObject *parent) : QAction(parent)
{
}

QLinkedAction::QLinkedAction ( const QString &text, QObject *parent ) : QAction(text, parent)
{
}

QLinkedAction::QLinkedAction ( const QIcon &icon, const QString & text, QObject *parent ) : QAction(icon, text, parent)
{
}

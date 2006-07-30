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


#ifndef QLINKEDACTION_H
#define QLINKEDACTION_H

// Qt
#include <QPushButton>
#include <QAction>

class QLinkedAction : public QAction
{
	Q_OBJECT

public:
	QLinkedAction ( QObject * parent );
	QLinkedAction ( const QString & text, QObject * parent );
	QLinkedAction ( const QIcon & icon, const QString & text, QObject * parent );

	void setEnabled(bool state);
	void linkToPushButton(QPushButton *button);

signals:
	void stateChanged(bool state);
};

#endif

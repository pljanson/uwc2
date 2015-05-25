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


// Qt
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

// Window
#include "MainWindow.h"

// cWebshots
#include "cWebshots/Exceptions.h"

#define UWC_DEBUG

/*
//debug to logfile by
// http://www.qt-coding.com/2013/08/06/tip-of-the-day-redirect-qdebug-to-a-file/
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1] ").arg(dt);

   switch (type)
   {
      case QtDebugMsg:
         txt += QString("{Debug} \t\t %1").arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("{Warning} \t %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("{Critical} \t %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("{Fatal} \t\t %1").arg(msg);
         abort();
         break;
   }

   QFile outFile("LogFile.log");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
}
*/

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(customMessageHandler);

	QApplication app(argc, argv);

	MainWindow *wndMain = new MainWindow;
	wndMain->show();

#ifndef UWC_DEBUG
	try
	{
#endif
		return app.exec();
#ifndef UWC_DEBUG
	}
	catch (std::runtime_error e)
	{
		QMessageBox::critical(wndMain, "UWC - Critical error !", QString::fromAscii(e.what()) + "\n\nThe program will now close.");
	}
	catch (cWebshots::Exceptions::Exception e)
	{
		QMessageBox::critical(wndMain, "UWC - Critical error !", QString::fromStdString(e.Description()) + "\n\nThe program will now close.");
	}
#endif
}

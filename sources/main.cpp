/*
* This file is part of QLumEdit, an ultimate Eulumdat file editor
* Copyright (C) 2007  Krzysztof Struginski
*
* "qt_pl.ts" Copyright (C) 2006-2007  ARISE  http://qtdev.arise.pl/
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Krzysztof Struginski <cagrin@gmail.com>
*   Program page: http://sourceforge.net/projects/qlumedit
*/

#include <QApplication>
#include <QtGui>

#include "mainwindow.h"
#include "eulumdat.h"

#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);

    QSettings settings("Cagrinlabs", "QLumEdit");
    QString language = settings.value("lang", QString(QLocale::system().name())).toString();
    qApp->processEvents();

    QTranslator main_translator;
    main_translator.load(":/translations/qt_" + language + ".qm");
    app.installTranslator( &main_translator );

    QTranslator translator;
    translator.load(":/translations/qlumedit_" + language + ".qm");
    app.installTranslator( &translator );

    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1250"));

    if (language == "pl")
    {
        QLocale::setDefault(QLocale::Polish);
    }

    QStringList files;
    QStringList args = app.arguments();

    for (int i = 1; i < args.count(); ++i)
    {
        QString argument = args.at(i);
        if (!files.contains(argument))
        {
            files.append(argument);
        }
    }

    MainWindow mainWin;
    mainWin.setWindowIcon(QIcon(":/images/socket.png"));
    mainWin.show();

    if (files.count())
    {
        mainWin.loadFile(files.last());
    }

    return app.exec();
}

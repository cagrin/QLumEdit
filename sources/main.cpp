#include <QApplication>
#include <QtWidgets>

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
    if (main_translator.load(":/translations/qtbase_" + language + ".qm")) {
        app.installTranslator( &main_translator );
    }

    QTranslator translator;
    if (translator.load(":/translations/qlumedit_" + language + ".qm")) {
        app.installTranslator( &translator );
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

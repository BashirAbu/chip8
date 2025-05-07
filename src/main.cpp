#include "mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <qapplication.h>
#include <qfontdatabase.h>
int main(int argc, char *argv[])

{
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();
    return app.exec();
}

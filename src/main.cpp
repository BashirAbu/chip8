#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <qcoreevent.h>
#include <qmainwindow.h>

int main(int argc, char *argv[])

{
    QApplication app(argc, argv);
    QMainWindow mw;
    mw.show();
    return app.exec();
}

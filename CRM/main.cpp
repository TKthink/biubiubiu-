#include "mainwindow.h"
#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    client c;
//    MainWindow w;
//    w.show();
    c.setGeometry(800,300,380,320);
    c.show();
    return a.exec();
}

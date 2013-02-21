#include <QApplication>
//#include "mainwindow.h"
#include "mainform.hpp"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;

    //w.show();
    MainForm f;
    f.show();
    return a.exec();
}

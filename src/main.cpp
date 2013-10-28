#include <QApplication>
#include <mainform.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;

    //w.show();
    MainForm f;
    f.show();
    //f.getFramesLoop();
    a.exec();

    return 0;
}

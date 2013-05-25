#include "simplerunner.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleRunner w;
    w.show();
    return a.exec();
}

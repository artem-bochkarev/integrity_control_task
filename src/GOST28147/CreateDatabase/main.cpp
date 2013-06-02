#include "createdatabase.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CreateDatabase w;
    w.show();
    return a.exec();
}

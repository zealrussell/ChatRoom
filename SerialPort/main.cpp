#include "myport.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyPort w;
    w.show();
    return a.exec();
}

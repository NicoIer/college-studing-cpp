#include "widget.h"
#include "drawer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Drawer w;
    w.resize(250,700);
    w.show();
    return a.exec();
}

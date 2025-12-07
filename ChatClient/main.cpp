#include "mainwidget.h"

#include <QApplication>

#include "model/data.h"

void totest(){
    LOG()<<"hello world";
}

int main(int argc, char *argv[])
{
    totest();
    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    return a.exec();
}

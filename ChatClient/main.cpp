#include "mainwidget.h"

#include <QApplication>

#include "model/data.h"

// 测试日志
void totest(){
    LOG()<<"hello world";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 获取单例
    MainWidget *instance = MainWidget::getInstance();
    instance->show();
    return a.exec();
}

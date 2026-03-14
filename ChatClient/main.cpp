#include "mainwidget.h"

#include <QApplication>

#include <QFile>

#include <QDebug>

#include "model/data.h"

#include "debug.h"

// 测试日志
void totest(){
    LOG()<<"hello world";

}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QFile qss(":/resource/style.qss");
    if(qss.open(QFile::ReadOnly)){
        // QString style = QLatin1StringView(qss.readAll());
        QString style = QString::fromUtf8(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
        qDebug()<<"Global QSS loaded sucessfully!!";
    }else{
        qDebug() << "Failed to load QSS: "<<qss.errorString();
    }

#if TEST_UI
    InfoWidget *testWidget = new InfoWidget(nullptr, WindowModal::IS_MAIN);
    testWidget->setTitleText("测试登录窗口");
    testWidget->show();
#endif

    // 获取单例
    MainWidget *instance = MainWidget::getInstance();
    instance->show();
    return a.exec();
}

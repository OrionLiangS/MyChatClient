#include <QWidget>

#include <QEvent>

#include <QVBoxLayout>

#include "infowidget.h"

InfoWidget::InfoWidget(QWidget*parent):QDialog(parent) {

    // =========================================
    // 0. 基础窗口属性设置
    // =========================================
    this->setObjectName("selfInfoWidget");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setFixedSize(320, 360);
    this->move(QCursor::pos());


    // =========================================
    // 1. 创建内部容器与特效
    // =========================================
    mainFrame = new QFrame(this);
    mainFrame->setObjectName("mainFrame");
    mainFrame->setStyleSheet(
        "#mainFrame {"
        "   background-color: #FFFFFF;"
        "   border-radius: 10px;"
        "}"
        );

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setBlurRadius(20);
    mainFrame->setGraphicsEffect(shadow);

    QVBoxLayout* globalLayout = new QVBoxLayout(this);
    globalLayout->setContentsMargins(20, 20, 20, 20);
    globalLayout->addWidget(mainFrame);


}

bool InfoWidget::event(QEvent *event)
{
    // 监听窗口激活状态改变
    // 当你点击窗口外部时，当前窗口会变成“非激活”状态 (isActiveWindow() 为 false)
    if (event->type() == QEvent::ActivationChange)
    {
        if (!this->isActiveWindow())
        {
            this->close(); // 失去焦点，自己关闭自己
        }
    }
    return QDialog::event(event);
}

void InfoWidget::showEvent(QShowEvent *event)
{
    // 调用父类默认行为
    QDialog::showEvent(event);

    // 显示时立即激活窗口
    this->activateWindow();
    this->setFocus();
}

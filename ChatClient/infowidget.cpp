#include <QWidget>

#include <QEvent>

#include <QVBoxLayout>

#include "infowidget.h"

InfoWidget::InfoWidget(QWidget*parent, bool isModal):QDialog(parent),m_isModal(isModal) {



    this->setObjectName("selfInfoWidget");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // =========================================
    // 0. 基础窗口属性设置
    // =========================================

    // 模态状态
    if(m_isModal){
        this->setWindowModality(Qt::ApplicationModal); // 设置模态属性
        this->setFixedSize(667,533);

        if(parent){
            QWidget* topLevel = parent->window();
            QPoint parentGlobalPos = topLevel->mapToGlobal(QPoint(0, 0));
            int xOffset = (topLevel->width() - this->width())/2;
            int yOffset = (topLevel->height() - this->height())/2;
            this->move(parentGlobalPos.x() + xOffset, parentGlobalPos.y() + yOffset);
        }
    }
    else{

        this->setAttribute(Qt::WA_DeleteOnClose);
        this->setFixedSize(320, 360);
        this->move(QCursor::pos()); ///< 可进行重写为头像右侧, 改基类即可
    }

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
    // 非模态时调用
    if(!m_isModal)
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

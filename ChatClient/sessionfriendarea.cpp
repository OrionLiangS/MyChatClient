#include "sessionfriendarea.h"

#include <QVBoxLayout>

#include <QPushButton>


#include<QScrollArea>

#include<QScrollBar>

SessionFriendArea::SessionFriendArea(QWidget *parent)
    : QScrollArea{parent}
{

    // =========================
    //  设置滚动效果
    // =========================

    this->setObjectName("SfArea");
    // 取消控件四周的边界
    this->setFrameShape(QFrame::NoFrame);

    // 开启滚动
    this->setWidgetResizable(true);

    // 禁止水平滚动
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 垂直方向滚动自适应(少无滚动 多滚动)
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // =========================
    // 初始化容器
    // =========================
    container = new QWidget();
    container->setObjectName("container");
    // =========================
    // 设置容器
    // =========================
    this->setWidget(container); // 容器设置进滚动区 - 会自适应

    // 设置布局管理器
    QVBoxLayout* layout = new QVBoxLayout(container);

    // 布局管理器无四周边界
    layout->setContentsMargins(0,0,0,0);

    // 空间间距为0
    layout->setSpacing(0);

    // 布局内元素置顶
    layout->setAlignment(Qt::AlignTop);

    // ================ test ================

    for(int i=0;i<30;++i){
        QPushButton* btn = new QPushButton(container);
        btn->setFixedHeight(45);
        btn->setText("测试好友 " + QString::number(i));
        btn->setStyleSheet("border: 1px solid #1296DB; background-color: white; margin: 0px; border-radius: 0px;");
        layout->addWidget(btn);
    }

    this->AreaSetStyle();

}

void SessionFriendArea::AreaSetStyle()
{
    // ============================================
    // QSS无法渲染, 此处直接硬编码StyleSheet
    // ============================================
    this->setStyleSheet(R"(
        /* 针对 SfArea 下的垂直滚动条 */
        QScrollArea#SfArea QScrollBar:vertical {
            width: 7px;
            background: transparent;
            margin: 0px;
            border: none;
        }

        /* 滑块 */
        QScrollArea#SfArea QScrollBar::handle:vertical {
            background: rgba(180, 180, 180, 150);
            border-radius: 3px;
            min-height: 20px;
        }

        QScrollArea#SfArea QScrollBar::handle:vertical:hover {
            background: rgba(150, 150, 150, 200);
        }

        /* 隐藏箭头 (最关键的部分，漏了就不渲染) */
        QScrollArea#SfArea QScrollBar::add-line:vertical,
        QScrollArea#SfArea QScrollBar::sub-line:vertical {
            height: 0px;
            width: 0px;
            background: transparent;
            subcontrol-position: top;
        }

        /* 隐藏轨道背景 */
        QScrollArea#SfArea QScrollBar::add-page:vertical,
        QScrollArea#sessionFriendArea QScrollBar::sub-page:vertical {
            background: transparent;
            border: none;
        }
    )");

}

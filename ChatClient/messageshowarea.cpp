#include "messageshowarea.h"

#include <QScrollBar>

#include <QVBoxLayout>

#include<QGridLayout>

/**
 * @brief MessageShowArea::MessageShowArea
 * @details
 * 构造一个MessageShowArea对象
 */
MessageShowArea::MessageShowArea() {

    // 设置对象名字
    this->setObjectName("messageShowArea");

    // 创建容器
    container = new QWidget(this);
    container->setObjectName("messageShowContainer");

    // 设置容器可resize
    this->setWidgetResizable(true);

    // 将容器设置进MessageShowArea - (继承自QScrollArea)中
    this->setWidget(container);

    // 硬编码设置样式
    this->AreaSetStyle();

    // 为容器设置布局
    QVBoxLayout *messageContainerLayout = new QVBoxLayout(container);

    // 为容器设置布局
    container->setLayout(messageContainerLayout);


}


/**
 * @brief MessageShowArea::AreaSetStyle
 *
 */
void MessageShowArea::AreaSetStyle()
{
    // ============================================
    // QSS无法渲染, 此处直接硬编码StyleSheet
    // ============================================
    this->setStyleSheet(R"(

        QScrollArea#messageShowArea {
            /* 1. 先把所有边框清零，告诉 Qt "别画默认的那个坑了" */
            border: none;

            /* 2. 单独画出右侧的分割线 */
            border-top: 1px solid #D5D5D5;
            border-bottom: 1px solid #D5D5D5;


            /* 3. 建议加上背景透明，防止默认底色干扰 */
            /*background-color: #EDEDED;*/
            background: transparent;
        }

        /* 针对 SfArea 下的垂直滚动条 */
        QScrollArea#messageShowArea QScrollBar:vertical {
            width: 7px;
            background: transparent;
            margin: 0px;
            border: none;
        }

        /* 滑块 */
        QScrollArea#messageShowArea QScrollBar::handle:vertical {
            background: rgba(180, 180, 180, 150);
            border-radius: 3px;
            min-height: 20px;
        }

        QScrollArea#messageShowArea QScrollBar::handle:vertical:hover {
            background: rgba(150, 150, 150, 200);
        }

        /* 隐藏箭头 (最关键的部分，漏了就不渲染) */
        QScrollArea#messageShowArea QScrollBar::add-line:vertical,
        QScrollArea#messageShowArea QScrollBar::sub-line:vertical {
            height: 0px;
            width: 0px;
            background: transparent;
            subcontrol-position: top;
        }

        /* 隐藏轨道背景 */
        QScrollArea#messageShowArea QScrollBar::add-page:vertical,
        QScrollArea#messageShowArea QScrollBar::sub-page:vertical {
            background: transparent;
            border: none;
            /* border-right: 1px solid #D5D5D5;*/
        }
    )");
}


/**
 * @brief MessageItem::MessageItem
 * @param isLeft 判断当前消息为左侧消息还是右侧消息
 * @details
 * 构造函数
 */
MessageItem::MessageItem(bool isLeft)
    :isLeft(isLeft)
{

}

/**
 * @brief MessageItem::makeMessageItem
 * @param isLeft 判断当前是否为左侧消息
 * @param message message消息对象
 * @return 返回一个构造好的message对象
 * @details
 * 建造者模式, 用来构造对应不同的消息对象
 */
MessageItem *MessageItem::makeMessageItem(bool isLeft, const Message &message)
{
    // 1) 创建布局并设置
    MessageItem* item = new MessageItem(isLeft);
    QGridLayout* layout = new QGridLayout(item);
    item->setLayout(layout);

    // 设置最小高度
    item->setMinimumHeight(53);

    // 2) 创建头像
    QPushButton *messageAvatar = new QPushButton(item);
    messageAvatar->setFixedSize(33,33);
    messageAvatar->setIconSize(QSize(33,33));
    messageAvatar->setIcon(message.sender.avatar);
    messageAvatar->setObjectName("messageAvatar");

    // 3) 设置头像进布局(根据情况设置)
    if(isLeft){
        layout->addWidget(messageAvatar, 0,0,2,1, Qt::AlignLeft|Qt::AlignTop);
    }else{
        layout->addWidget(messageAvatar, 0,1,2,1, Qt::AlignRight|Qt::AlignTop);
    }






    // 返回消息
    return item;
}

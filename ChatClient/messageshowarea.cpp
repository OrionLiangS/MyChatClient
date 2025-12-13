#include "messageshowarea.h"

#include <QScrollBar>

#include <QVBoxLayout>

#include <QGridLayout>

#include <QLabel>

using namespace model;

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

    // 头像设置固定大小
    messageAvatar->setFixedSize(33,33);
    // 头像Icon固定大小
    messageAvatar->setIconSize(QSize(33,33));
    // 设置头像(从message对象中获取)
    messageAvatar->setIcon(message.sender.avatar);
    // 设置ObjectName方便设置QSS样式
    messageAvatar->setObjectName("messageAvatar");

    // 3) 设置头像进布局(根据情况设置)
    if(isLeft){
        layout->addWidget(messageAvatar, 0,0,2,1, Qt::AlignLeft|Qt::AlignTop);
    }else{
        layout->addWidget(messageAvatar, 0,1,2,1, Qt::AlignRight|Qt::AlignTop);
    }


    // 4) 设置名字和时间
    QLabel *nameAndTimeLabel = new QLabel();
    nameAndTimeLabel->setObjectName("nameAndTimeLabel");
    // 设置字符串
    nameAndTimeLabel->setText(message.sender.nickname + " | " + message.time);
    nameAndTimeLabel->setAlignment(Qt::AlignBottom);

    // 根据左右设置在Layout中的位置
    if(isLeft){
        layout->addWidget(nameAndTimeLabel, 0,1, Qt::AlignLeft);
    }
    else{
        layout->addWidget(nameAndTimeLabel,0,0, Qt::AlignRight);
    }

    // 5) 创建消息体 (调用工厂函数 - 创建不同消息类型的消息体)
    QWidget *contentWidget = nullptr;
    switch(message.messageType){
        case TEXT_TYPE:
            contentWidget = makeTextMessageItem();
            break;
        case IMAGE_TYPE:
            contentWidget = makeImageMessageItem();
            break;
        case FILE_TYPE:
            contentWidget = makeFileMessageItem();
            break;
        case SPEECH_TYPE:
            contentWidget = makeSpeechMessageItem();

            break;
        default:
            LOG()<<"未知消息类型 messageType:"<<message.messageType;
    }
        if(isLeft){
            layout->addWidget(contentWidget, 1, 1);
        }
        else{
            layout->addWidget(contentWidget, 1, 0);
        }

    // 返回消息
    return item;
}


MessageItem *MessageItem::makeTextMessageItem()
{
    /**
     * @todo
     */
    return nullptr;
}

MessageItem *MessageItem::makeImageMessageItem()
{
    /**
     * @todo
     */
    return nullptr;
}

MessageItem *MessageItem::makeFileMessageItem()
{
    /**
     * @todo
     */
    return nullptr;
}


MessageItem *MessageItem::makeSpeechMessageItem()
{
    /**
     * @todo
     */
    return nullptr;
}


// ################################################
// 创建消息体(文本消息的气泡框与其对应的内容显示)
// ###############################################
MessageContentLabel::MessageContentLabel(const QString &text, bool isLeft)
    :isLeft(isLeft)
{

    // 1) 设置Label与对应样式
    QFont font;
    // 此处所设置的字体需要使用 因此暂时不考虑使用QSS(需要计算字体的大小)
    font.setFamily("微软雅黑");
    font.setPixelSize(16);
    this->messageContentLabel = new QLabel(text, this);
    this->messageContentLabel->setObjectName("messageContentLabel");
    // 设置文本靠左, 高度居中
    this->messageContentLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    // 设置文本自动换行
    this->messageContentLabel->setWordWrap(true);
}



























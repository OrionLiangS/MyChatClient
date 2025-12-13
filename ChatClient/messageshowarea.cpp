#include "messageshowarea.h"

#include <QScrollBar>

#include <QVBoxLayout>

#include <QGridLayout>

#include <QLabel>

#include <QFontMetrics>

#include <QPainter> ///< 画家对象

#include <QPainterPath> ///< 用于绘制不规则形状

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


// ################################################
// 从MessageShowArea中插入一条消息(尾部插入)
// ###############################################
void MessageShowArea::addMessage(bool isLeft, const Message &message)
{
    MessageItem* messageItem = MessageItem::makeMessageItem(isLeft, message);
    this->container->layout()->addWidget(messageItem);
}


// ################################################
// 从MessageShowArea中插入一条消息(头部插入)
// ###############################################
void MessageShowArea::addFrontMessage(bool isLeft, const Message &message)
{
    MessageItem* messageItem = MessageItem::makeMessageItem(isLeft, message);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(this->container->layout());
    layout->insertWidget(0, messageItem);
}

// ################################################
// 清空MessageShowArea中的所有消息数据
// ###############################################
void MessageShowArea::clearMessage()
{
    QLayout *layout = this->container->layout();
    for(int i = layout->count()-1;i>=0;--i){
        QLayoutItem* item = layout->takeAt(i);
        if(item && item->widget()){
            delete item->widget();
        }
    }
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
            // 此处传入的是一个QByteArray参数, 不一定需要手动转换 本质上QString存在传入QByteArray的构造函数
            contentWidget = makeTextMessageItem(isLeft, message.content);
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

/**
 * @brief MessageItem::makeTextMessageItem
 * @param isLeft - 判断当前消息为左侧消息还是右侧消息
 * @param message - 消息体
 * @return 返回一个QWidget(主要是为了能够更好的适配其他返回值类型)
 * @details
 * 通过该函数返回一个QWidget主要是通过适应多态 父类指针调用子类方法形成多态
 */
QWidget *MessageItem::makeTextMessageItem(bool isLeft, const QString& message)
{
    MessageContentLabel *label = new MessageContentLabel(message, isLeft);
    return label;
}

QWidget *MessageItem::makeImageMessageItem()
{
    /**
     * @todo
     */
    return nullptr;
}

QWidget *MessageItem::makeFileMessageItem()
{
    /**
     * @todo
     */
    return nullptr;
}


QWidget *MessageItem::makeSpeechMessageItem()
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



// ################################################
// 重写paintEvent进行消息气泡的绘制
// ###############################################

// 该函数将会在控件被显示时调用, 无需手动调用
void MessageContentLabel::paintEvent(QPaintEvent *event)
{

    // 处理Warning

    (void)event;
    // 1) 获取父元素的宽度
    // 文本需要在占据父元素60%的位置进行换行
    // 因此需要先获取父元素的宽度
    // 其父元素为消息框
    QObject* parentWidget = this->parent();
    if(!parentWidget->isWidgetType()){
        // 父元素的类型为一个QWidget类型(MessageItem) 因此若是不是Widget类型则说明不是父元素
        return;
    }
    QWidget *parent = qobject_cast<QWidget*>(parentWidget);
    // 获取宽度
    int width = parent->width()*0.6;


    // 2) 计算当前文本一行放置有多宽
    // 获取字体的度量值
    QFontMetrics metric(this->messageContentLabel->font());
    // 通过度量值与单行字体的宽度计算一行的宽度
    int totalWidth = metric.horizontalAdvance(this->messageContentLabel->text());

    // 3) 计算行数
    // 减去40是因为其中总宽度包含左右间距
    // +1 表示确保至少有一行文本
    int rows = (totalWidth/(width-40))+1;
    if(rows == 1){
        // 当行数真的只有一行时 其宽度即为真正的文本宽度+40(40为margin宽度)
        width = totalWidth+40;
    }

    // 4) 根据行数计算得到高度
    // font().pixelSize()*1.2 为字体高度 其中1.2为系数
    // 20px为上下间距各为10px
    int height = rows * (this->messageContentLabel->font().pixelSize()*1.2)+20;



    // 5) 绘制气泡
    // 设置画家对象
    QPainter painter(this);

    // 设置对象用来绘制不规则图形
    QPainterPath path;
    // 设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    // 通过左右分别绘制对应的气泡
    if(isLeft){
        // 设置画笔颜色 (线条边框)
        painter.setPen(QPen(QColor(255, 255, 255)));
        // 设置画刷 (填充)
        painter.setBrush(QColor(255, 255, 255));

        // =====================
        // 绘制圆角矩形
        // =====================
        // 第一个参数设置为10, 其中这个绘画出来的控件是基于父元素的位置, 因此需要留出一定的位置给气泡尖尖
        painter.drawRoundedRect(10,0,width,height,5,5);

        // =====================
        // 绘制小尖尖
        // =====================
        // 移动画笔
        path.moveTo(10,15);
        // 划线
        path.lineTo(0,20);
        path.lineTo(10,25);
        // 闭合
        path.closeSubpath();

        // 移动Label的位置
        this->setGeometry(10,0,width,height);

    }else{
        // 右侧为当前登录用户(本人) 为蓝色色系
        painter.setPen(QPen(QColor(58, 188, 245)));
        painter.setBrush(QColor(58, 188, 245));


        // =====================
        // 换算坐标
        // =====================
        int leftPos = this->width()-width-10;
        int rightPos = this->width()-10;

        // =====================
        // 绘制圆角矩形
        // =====================
        painter.drawRoundedRect(leftPos,0,width,height,5,5);

        // =====================
        // 绘制小尖尖
        // =====================
        // 移动画笔
        path.moveTo(rightPos,15);
        // 划线
        path.lineTo(rightPos+10,20);
        path.lineTo(rightPos,25);
        // 闭合
        path.closeSubpath();

        // 移动Label的位置
        this->setGeometry(leftPos,0,width,height);

    }

    // 6) 将小三角进行绘画
    painter.drawPath(path);

    // 7) 重新设置父元素高度
    parent->setFixedHeight(height+30);
}



























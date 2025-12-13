#include "messageshowarea.h"

#include <QScrollBar>

#include <QVBoxLayout>

#include <QGridLayout>

#include <QLabel>

#include <QFontMetrics>

#include <QPainter> ///< 画家对象

#include <QPainterPath> ///< 用于绘制不规则形状

#include "debug.h"

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


    // #########################
    // 添加测试数据
    // #########################
#if TEST_UI
    for(int i=0;i<15;++i){
        UserInfo userinfo;
        userinfo.avatar = QIcon(":/resource/image/defaultAvatar.png");
        userinfo.nickname = "测试用户"+QString::number(i);
        QString text ="This is a test Message  This is a test Message This is a test Message This is a test Message This is a test Message This is a test Message This is a test Message This is a test Message";
        Message message = Message::makeMessage(TEXT_TYPE, QString::number(i), userinfo,text.toUtf8(),"");
        addMessage(true, message);
        addMessage(false, message);
    }

#endif

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
// 构造函数
MessageItem::MessageItem(bool isLeft)
    :isLeft(isLeft)
{
    // 这里的 contentWidget 初始化为空
}

// 【核心新增】响应窗口大小变化
void MessageItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // 如果当前持有的是文本气泡，通知它重新计算大小
    if (contentWidget) {
        // 尝试转换成 MessageContentLabel
        if (auto textBubble = qobject_cast<MessageContentLabel*>(contentWidget)) {
            // 传入当前 Item 的宽度，让气泡自己算该多宽
            textBubble->updateContentSize(this->width());

            // 重新设置 Item 的高度，因为气泡高度变了，Item 也要撑开
            // 70 是预留给头像、名字和上下边距的空间
            int newItemHeight = textBubble->height() + 40;
            if (newItemHeight < 80) newItemHeight = 80; // 最小高度
            this->setFixedHeight(newItemHeight);
        }
    }
}

MessageItem *MessageItem::makeMessageItem(bool isLeft, const Message &message)
{
    MessageItem* item = new MessageItem(isLeft);
    QGridLayout* layout = new QGridLayout(item);
    item->setLayout(layout);

    // 1. 布局设置 (紧凑一点)
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setVerticalSpacing(5);
    layout->setHorizontalSpacing(10); // 头像和气泡的间距

    // 2. 创建头像
    QPushButton *messageAvatar = new QPushButton(item);
    messageAvatar->setObjectName("messageAvatar");
    messageAvatar->setFixedSize(40,40); // 微信标准头像大小
    messageAvatar->setIconSize(QSize(40,40));
    messageAvatar->setIcon(message.sender.avatar);
    messageAvatar->setFlat(true); // 去掉按钮边框
    messageAvatar->setFocusPolicy(Qt::NoFocus);

    // 3. 创建名字和时间
    QLabel *nameLabel = new QLabel();
    nameLabel->setText(message.sender.nickname + " | " + message.time);
    nameLabel->setStyleSheet("color: #B2B2B2; font-size: 12px;");
    nameLabel->setFixedHeight(15); // 固定高度

    // 4. 创建消息体
    QWidget *contentWidget = nullptr;
    if (message.messageType == TEXT_TYPE) {
        contentWidget = makeTextMessageItem(isLeft, message.content);
    }
    // ... 其他类型 ...

    // 将创建好的 contentWidget 保存到 item 成员变量中，供 resizeEvent 使用
    item->contentWidget = contentWidget;

    // 5. 初始化一次大小 (防止刚出来是0)
    if (auto textBubble = qobject_cast<MessageContentLabel*>(contentWidget)) {
        textBubble->updateContentSize(600); // 先给个大概值
    }

    // ============================================================
    // 布局核心逻辑 (解决左右不对齐问题)
    // ============================================================

    if(isLeft){
        // === 左侧布局 ===
        // Row 0: 头像 | 名字 | 弹簧
        // Row 1: (空) | 气泡 | 弹簧

        // 头像 (第0列，跨2行，靠左上)
        layout->addWidget(messageAvatar, 0, 0, 2, 1, Qt::AlignTop | Qt::AlignLeft);

        // 名字 (第1列，靠左)
        layout->addWidget(nameLabel, 0, 1, Qt::AlignLeft | Qt::AlignTop);

        // 气泡 (第1列，靠左)
        layout->addWidget(contentWidget, 1, 1, Qt::AlignLeft | Qt::AlignTop);

        // 关键：设置第2列为弹簧，把内容往左挤
        layout->setColumnStretch(2, 1);
        layout->setColumnStretch(0, 0);
        layout->setColumnStretch(1, 0);
    }
    else{
        // === 右侧布局 ===
        // Row 0: 弹簧 | 名字 | 头像
        // Row 1: 弹簧 | 气泡 | (空)

        // 名字靠右
        nameLabel->setAlignment(Qt::AlignRight);

        // 弹簧 (第0列) - 必须设为 stretch 1
        layout->setColumnStretch(0, 1);
        layout->setColumnStretch(1, 0);
        layout->setColumnStretch(2, 0);

        // 名字 (第1列，靠右)
        layout->addWidget(nameLabel, 0, 1, Qt::AlignRight | Qt::AlignTop);

        // 气泡 (第1列，靠右)
        layout->addWidget(contentWidget, 1, 1, Qt::AlignRight | Qt::AlignTop);

        // 头像 (第2列，跨2行，靠右上)
        layout->addWidget(messageAvatar, 0, 2, 2, 1, Qt::AlignTop | Qt::AlignRight);
    }

    // 初始化高度
    int itemHeight = contentWidget->height() + 40;
    if (itemHeight < 80) itemHeight = 80;
    item->setFixedHeight(itemHeight);

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
// 构造函数
// ###############################################
MessageContentLabel::MessageContentLabel(const QString &text, bool isLeft)
    :isLeft(isLeft)
{
    // 改为 Fixed，我们要手动控制它的大小
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(16);
    this->messageContentLabel = new QLabel(text, this);
    this->messageContentLabel->setObjectName("messageContentLabel");
    this->messageContentLabel->setFont(font);
    this->messageContentLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    this->messageContentLabel->setWordWrap(true);
    // 让 Label 背景透明，不要遮挡我们画的气泡
    this->messageContentLabel->setStyleSheet("background: transparent; border: none;");
}

// ################################################
// 【核心修复】新增：专门计算大小，不画图
// ###############################################
void MessageContentLabel::updateContentSize(int parentWidth)
{
    // 1. 限制最大宽度 (比如父控件宽度的 55%，稍微窄一点更像微信)
    // 限制最小 200px
    int maxWidth = (parentWidth > 200) ? parentWidth * 0.55 : 200;

    // 2. 计算文本尺寸
    QFontMetrics metric(this->messageContentLabel->font());
    // 获取文本的理想矩形大小
    QRect textRect = metric.boundingRect(0, 0, maxWidth - 30, 0,
                                         Qt::AlignLeft | Qt::TextWordWrap,
                                         this->messageContentLabel->text());

    // 3. 计算气泡最终宽高
    // 宽度 = 文字宽 + 左右内边距 (30px)
    // 高度 = 文字高 + 上下内边距 (20px)
    m_contentWidth = textRect.width() + 30;
    m_contentHeight = textRect.height() + 20;

    // 4. 确保最小尺寸 (防止文字太少时气泡太小)
    if (m_contentWidth < 60) m_contentWidth = 60;
    if (m_contentHeight < 40) m_contentHeight = 40;

    // 设置自己的固定大小
    this->setFixedSize(m_contentWidth, m_contentHeight);

    // 5. 调整内部 Label 的位置 (解决文本居中问题)
    if(isLeft) {
        // 左侧消息：左边留 20px (10px尖角 + 10px空隙)，右边留 10px
        this->messageContentLabel->setGeometry(20, 10, textRect.width(), textRect.height());
    } else {
        // 右侧消息：左边留 10px，右边留 20px (10px尖角 + 10px空隙)
        this->messageContentLabel->setGeometry(10, 10, textRect.width(), textRect.height());
    }
}

// ################################################
// 【核心修复】纯净的绘图，绝不改大小
// ###############################################
void MessageContentLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPainterPath path;
    painter.setRenderHint(QPainter::Antialiasing);

    // 使用当前实际的控件宽高，保证不被截断
    int w = this->width();
    int h = this->height();

    if(isLeft){
        // === 左侧气泡 (对方) ===
        painter.setPen(QPen(QColor(255, 255, 255)));
        painter.setBrush(QColor(255, 255, 255));

        // 1. 绘制圆角矩形
        // x=10: 留出左边 10px 给尖角
        // w-10: 宽度减少 10px 防止右边出界
        painter.drawRoundedRect(10, 0, w - 10, h, 5, 5);

        // 2. 绘制小尖角 (在左边)
        path.moveTo(10, 15);
        path.lineTo(0, 20);   // 尖尖指向最左 (0, 20)
        path.lineTo(10, 25);
        path.closeSubpath();

    } else {
        // === 右侧气泡 (我) ===
        painter.setPen(QPen(QColor(125, 197, 235))); //
        painter.setBrush(QColor(125, 197, 235));

        // 1. 绘制圆角矩形
        // x=0: 从最左边开始
        // w-10: 右边留出 10px 给尖角
        painter.drawRoundedRect(0, 0, w - 10, h, 5, 5);

        // 2. 绘制小尖角 (在右边)
        path.moveTo(w - 10, 15);
        path.lineTo(w, 20);      // 尖尖指向最右 (w, 20)
        path.lineTo(w - 10, 25);
        path.closeSubpath();
    }

    // 绘制路径 (填充尖角)
    painter.drawPath(path);
}


























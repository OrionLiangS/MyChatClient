#include "sessionfriendarea.h"

#include <QVBoxLayout>

#include <QPushButton>

#include "model/data.h"

#include<QScrollArea>

#include<QScrollBar>

#include <QGridLayout>

#include "debug.h"


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

    // 渲染样式
    this->AreaSetStyle();

#if TEST_UI
    // ================ test begin ================

    // 测试添加Item
    for(int i=0;i<30;++i){
        // QString name = "测试用户"+QString::number(i);
        // QString text = "最后一条消息"+QString::number(i);
        // SessionFriendItem *item = new SessionFriendItem(this, QIcon(":/resource/image/defaultAvatar.png"), name, text);
        // this->container->layout()->addWidget(item);

        addItem(SessionItemType, QString::number(i),QIcon(":/resource/image/defaultAvatar.png"), "测试用户"+QString::number(i), "最后一条消息"+QString::number(i));

        // addItem(ApplyItemType, QString::number(i),QIcon(":/resource/image/defaultAvatar.png"), "测试用户"+QString::number(i), "最后一条消息"+QString::number(i));

    }

    // 测试点击首元素
    clickItem(0);

    // 测试清除Item
    // clear();

    // ================ test end ================
#endif
}

// ============================================
// 清除SessionFriendArea中的所有元素Item
// ============================================
void SessionFriendArea::clear()
{
    // 获取layout布局
    QLayout* layout = container->layout();

    // 获取计数
    int count = layout->count();

    // 由后往前遍历item并进行删除
    for(int i = count-1;i>=0;--i){
        QLayoutItem *item = layout->takeAt(i);
        // 判断是否释放内存
        if(item->widget()) delete item->widget();
    }
}


// ============================================
// 在SessionFriendArea中添加一个Item
// ============================================
void SessionFriendArea::addItem(ItemType itemType, const QString &itemId,const QIcon &avatar, const QString &name, const QString &text)
{
    SessionFriendItem* item = nullptr;
    switch(itemType){
    case SessionItemType:
        item = new SessionItem(this,itemId ,avatar, name, text);
        break;
    case FriendItemType:
        item = new FriendItem(this,itemId ,avatar, name, text);
        break;
    case ApplyItemType:
        item = new ApplyItem(this,itemId ,avatar, name);
        break;
    default:
        LOG()<<"未知的Item类型  itemType: "<<itemType;
    }
    container->layout()->addWidget(item);
}


// ============================================
// 通过index索引选中Item
// ============================================
void SessionFriendArea::clickItem(int index)
{
    // 获取判断索引是否越界
    if(index < 0 || index >= container->layout()->count()){
        LOG()<<"点击元素下标越界  index: "<<index <<"当前Item数: "<<container->layout()->count();
    }
    QLayoutItem *Layoutitem = container->layout()->itemAt(index);
    if(!Layoutitem || !Layoutitem->widget()){
        LOG()<<"指定元素不存在    index: "<<index;
    }
    SessionFriendItem* item = qobject_cast<SessionFriendItem*>(Layoutitem->widget());
    // 调用select进行选中
    item->select();
}


// ============================================
// 用于硬编码设置SessionFriendArea中的StyleSheet
// ============================================
void SessionFriendArea::AreaSetStyle()
{
    // ============================================
    // QSS无法渲染, 此处直接硬编码StyleSheet
    // ============================================
    this->setStyleSheet(R"(

        QScrollArea#SfArea {
            /* 1. 先把所有边框清零，告诉 Qt "别画默认的那个坑了" */
            border: none;

            /* 2. 单独画出右侧的分割线 */
            border-right: 1px solid #D5D5D5;

            /* 3. 建议加上背景透明，防止默认底色干扰 */
            background-color: transparent;
        }

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
        QScrollArea#SfArea QScrollBar::sub-page:vertical {
            background: transparent;
            border: none;
            /* border-right: 1px solid #D5D5D5;*/
        }
    )");

}


// SessionFriendItem的构造
SessionFriendItem::SessionFriendItem(QWidget *owner, const QIcon &avatar, const QString &name, const QString &text)
    :owner(owner) // 获取SessionFriendArea
{
    // ===========================
    // 基础样式设置
    // ===========================

    // 设置对象名
    this->setObjectName("sessionFriendItem");
    // 固定高度
    this->setFixedHeight(67);

    // ===========================
    // 设置布局
    // ===========================
    QGridLayout *layout = new QGridLayout(this);
    this->setLayout(layout);

    // 布局四周边界
    layout->setContentsMargins(10,8,15,8);

    // 布局内部空间间隔
    layout->setHorizontalSpacing(5);
    layout->setVerticalSpacing(2);

    // ===========================
    // 成员内容填充
    // ===========================

    // 设置头像(按钮控件)
    avatarBtn = new QPushButton(this);
    avatarBtn->setFixedSize(40,40);
    avatarBtn->setObjectName("avatarBtn"); // 对象ID
    avatarBtn->setIconSize(QSize(40,40));
    avatarBtn->setIcon(avatar);

    // 设置用户名
    nameLabel = new QLabel(name, this);
    nameLabel->setObjectName("nameLabel"); // 对象ID

    // 当继承为ApplyItem时需要对该控件进行移除
    messageLabel = new QLabel(text, this);
    messageLabel->setObjectName("messageLabel"); // 对象ID

    // ===========================
    // 设置进布局
    // ===========================
    layout->addWidget(avatarBtn, 0,0,2,1);
    layout->addWidget(nameLabel, 0,1,1,1);
    layout->addWidget(messageLabel, 1,1,1,1);

    // ===========================
    // 拉伸系数
    // ===========================
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,0);

}

void SessionFriendItem::mousePressEvent(QMouseEvent *event)
{
    switch(event->button()){
    case Qt::LeftButton:

#if TEST_UI
        LOG()<<"LeftButton pressed";
#endif
        select();
        break;
    case Qt::RightButton:
#if TEST_UI
        LOG()<<"RightButton pressed";
#endif
        break;
    default:
#if TEST_UI
        LOG()<<"Other Button pressed";
#endif
        break;
    }
}

/**
 * @brief SessionFriendItem::paintEvent
 * @param event
 * @details
 * 当需要一个空白的Widget自定义控件可以被QSS渲染时 需要重写其的paintEvent函数
 */
void SessionFriendItem::paintEvent(QPaintEvent *event)
{
    // 1. 初始化样式选项
    QStyleOption opt;
    opt.initFrom(this);

    // 2. 创建画家
    QPainter p(this);

    // 3. 核心代码：使用 Qt 的样式引擎绘制这个控件
    // PE_Widget 表示绘制一个通用的 Widget 背景
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


// ============================================
// 点击时的焦点聚焦功能 内部调用active来实行业务逻辑
// ============================================
void SessionFriendItem::select()
{
    // 获取父元素的孩子
    const QObjectList& childrens = this->parentWidget()->children();
    for(QObject* child:childrens){
        if(!child->isWidgetType()){
            continue;
        }
        SessionFriendItem* item = qobject_cast<SessionFriendItem*>(child);
        item->selected = false;

        // 刷新对应的样式
        item->setProperty("selected", false);
        // 丢弃原有样式
        item->style()->unpolish(item);
        // 设置新样式
        item->style()->polish(item);
        // 更新
        item->update();
    /**
     * @todo
     */
    }
    // 设置自己为选中
    if (!this->selected) { // 只有状态变了才刷新
        this->selected = true;
        this->setProperty("selected", true);

        this->style()->unpolish(this);
        this->style()->polish(this);
        this->update();
    }

    active();
}


// ============================================
// 不进行实现 主要为子类提供可重写的基本语法
// ============================================
void SessionFriendItem::active()
{
    // nothing todo
    // 只需要为子类提供可重写即可
}



// SessionItem 的构造函数
SessionItem::SessionItem(QWidget *owner, const QString &chatSessionId, const QIcon &avatar, const QString &name, const QString &message)
    :SessionFriendItem(owner, avatar, name, message), chatSessionId(chatSessionId)
{

}



// ============================================
// 调用SessionItem的点击逻辑
// ============================================
void SessionItem::active()
{
    LOG()<<"调用SessionItem的点击逻辑";
    /**
     * @todo
     */
}

// FriendItem 的构造
FriendItem::FriendItem(QWidget *owner, const QString &friendId, const QIcon &avatar, const QString &name, const QString &signature)
    :SessionFriendItem(owner, avatar, name, signature),friendId(friendId)
{

}

// ============================================
// 调用FriendItem的点击逻辑
// ============================================
void FriendItem::active()
{
    LOG()<<"调用FriendItem的点击逻辑";
    /**
     * @todo
     */
}

// ApplyItem 的构造
ApplyItem::ApplyItem(QWidget *owner, const QString &applyId, const QIcon &avatar, const QString &name)
:SessionFriendItem(owner, avatar, name, ""),applyId(applyId)
{
    // 获取布局
    QGridLayout* layout = qobject_cast<QGridLayout*>(this->layout());

    // ========================
    // 去除其中的Label
    // ========================

    // 从布局中去除
    layout->removeWidget(this->messageLabel);

    // 释放内存
    delete messageLabel;

    // ========================
    // 初始化PushButton按钮
    // ========================

    acceptBtn = new QPushButton();
    rejectBtn = new QPushButton();

    // 设置按钮大小
    // acceptBtn->setFixedSize(20,20);
    // rejectBtn->setFixedSize(20,20);



    // // 进入控件指针变成手指指向 - 暂不考虑(割裂感过强)
    // acceptBtn->setCursor(Qt::PointingHandCursor);
    // rejectBtn->setCursor(Qt::PointingHandCursor);


    // 设置对象名
    acceptBtn->setObjectName("acceptBtn");
    rejectBtn->setObjectName("rejectBtn");

    // 设置按钮布局
    layout->addWidget(acceptBtn, 0, 2, 2, 1);
    layout->addWidget(rejectBtn, 0, 3, 2, 1);
}



// ============================================
// 调用ApplyItem的点击逻辑
// ============================================
void ApplyItem::active()
{
    LOG()<<"调用ApplyItem的点击逻辑";
    /**
     * @todo
     */
}

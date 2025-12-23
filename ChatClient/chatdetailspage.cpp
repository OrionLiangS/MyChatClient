#include "chatdetailspage.h"

#include <QVBoxLayout>

#include <QFont>

#include <QFontMetrics>

#include <QStyle>

#include "debug.h"

#include <QStyleOption>

#include <QPainter>

#include <QStyle>

#include "choosefriendwidget.h"

#include "model/data.h"


// #########################################
// 会话容器 - 需要塞进抽屉中
// #########################################



GroupChatDetailsPage::GroupChatDetailsPage(QWidget *parent)
{
#if /*TEST_GROUP_CHAT*/0
    QLabel* label = new QLabel(this);
    label->setText("GroupChatDetailsPage");
#endif

    groupChatVlayout = new QVBoxLayout(this);
    this->setLayout(groupChatVlayout);
    groupChatVlayout->setSpacing(0);
    groupChatVlayout->setContentsMargins(0,0,0,0);


    QWidget *groupChatSearchWidget = new QWidget(this);
    groupChatSearchWidget->setFixedHeight(45);
    QHBoxLayout *groupChatSearchHlayout = new QHBoxLayout(groupChatSearchWidget);
    groupChatSearchWidget->setLayout(groupChatSearchHlayout);
    groupChatSearchHlayout->setContentsMargins(20,10,20,5);
    groupChatSearchHlayout->setSpacing(0);
    groupChatSearchHlayout->setAlignment(Qt::AlignHCenter);
    groupChatSearch = new QLineEdit(groupChatSearchWidget);
    groupChatSearch->setObjectName("groupChatSearch");
    groupChatSearch->setFixedHeight(25);
    groupChatSearch->setPlaceholderText("搜索群成员");
    groupChatSearchHlayout->addWidget(groupChatSearch);


    groupChatVlayout->addWidget(groupChatSearchWidget);


    groupChatScroll = new FloatingScrollArea(this);
    groupChatScroll->setObjectName("groupChatScroll");
    groupChatVlayout->addWidget(groupChatScroll);


    groupChatDetailsScrollContainer = new QWidget(this);
    groupChatDetailsScrollContainer->setObjectName("groupChatDetailsScrollContainer");
    groupChatScroll->setWidget(groupChatDetailsScrollContainer);
    groupChatScroll->setWidgetResizable(true);

    groupDetailsScrollContainerVlayout = new QVBoxLayout(groupChatDetailsScrollContainer);
    groupDetailsScrollContainerVlayout->setContentsMargins(25,0,25,0);
    groupChatDetailsScrollContainer->setLayout(groupDetailsScrollContainerVlayout);

    // ==========================================================
    // groupDetailsScrollContainerVlayout内布局
    // ==========================================================

    groupContainerAvatarItemWidget = new QWidget(groupChatDetailsScrollContainer);
    groupContainerAvatarItemWidget->setObjectName("groupContainerAvatarItemWidget");
    groupContainerAvatarItemLayout = new QGridLayout(groupContainerAvatarItemWidget);
    groupContainerAvatarItemWidget->setLayout(groupContainerAvatarItemLayout);

    groupContainerMoreAvatarItemBtn = new QPushButton(groupContainerAvatarItemWidget);
    groupContainerMoreAvatarItemBtn->setObjectName("groupContainerMoreAvatarItemBtn");

    groupDetailsScrollContainerVlayout->addWidget(groupContainerAvatarItemWidget);

    ContainerMoreAvatarItemBtnWidget = new QWidget(groupChatDetailsScrollContainer);
    QHBoxLayout *moreAvatarItemBtnHlayout = new QHBoxLayout(ContainerMoreAvatarItemBtnWidget);
    moreAvatarItemBtnHlayout->setObjectName("moreAvatarItemBtnHlayout");
    ContainerMoreAvatarItemBtnWidget->setLayout(moreAvatarItemBtnHlayout);

    groupDetailsScrollContainerVlayout->addWidget(ContainerMoreAvatarItemBtnWidget);

    groupAnnouncementTag = new QLabel(groupChatDetailsScrollContainer);
    groupAnnouncementTag->setObjectName("groupAnnouncementTag");

    groupAnnouncement = new QPushButton(groupChatDetailsScrollContainer);
    groupAnnouncement->setObjectName("groupAnnouncementTag");

    groupNameTag = new QLabel(groupChatDetailsScrollContainer);
    groupNameTag->setObjectName("groupAnnouncementTag");

    groupName = new QPushButton(groupChatDetailsScrollContainer);
    groupName->setObjectName("groupName");

    exitGroupChatBtn = new QPushButton(groupChatDetailsScrollContainer);
    exitGroupChatBtn->setObjectName("exitGroupChatBtn");

    clearChatHistory = new QPushButton(groupChatDetailsScrollContainer);
    clearChatHistory->setObjectName("clearChatHistory");



#if TEST_UI_
    for(int i =0;i<30;++i){
        QPushButton *btn = new QPushButton(groupChatDetailsScrollContainer);
        groupDetailsScrollContainerVlayout->addWidget(btn);
    }
#endif


}

void GroupChatDetailsPage::initSignalSlots()
{
    // 关联弹窗构造函数
    connect(this, &GroupChatDetailsPage::signalAddFriendClicked, this, [=](){
        ChooseFriendWidget *chooseFriendWidget = new ChooseFriendWidget(this);
        chooseFriendWidget->show();
    });
}



// #########################################
// 单个标签 主要存在一个头像和一个姓名
// #########################################
AvatarItem::AvatarItem(QWidget *parent, const QString& name,const QIcon& avatarIcon): QWidget{parent}
{

    // =========================
    // 初始化控件信息
    // =========================
    this->setFixedSize(45,60);
    avatarBtn = new QPushButton(this);
    avatarBtn->setObjectName("AvatarItem_avatarBtn");
    avatarBtn->setFixedSize(40,40);
    avatarBtn->setIconSize(QSize(40,40));


    nameLabel = new QLabel(this);
    nameLabel->setObjectName("AvatarItem_nameLabel");

    // 不为空再去添加头像
    if (!avatarIcon.isNull()) {
        avatarBtn->setIcon(avatarIcon);
    }

    // =========================
    // 设置名字
    // =========================
    QFont font("微软雅黑", 7);
    nameLabel->setText(name); // 设置名字
    nameLabel->setFont(font);
    nameLabel->setAlignment(Qt::AlignCenter);

    // 对名字进行截断(以头像宽度为最大值)
    QFontMetrics metrics(font);
    int totalWidth = metrics.horizontalAdvance(name);
    int name_max_width = 30;
    if(totalWidth>name_max_width){
        // 大于 需要截断
        QString ellipsis = "...";
        int ellipsisLen = metrics.horizontalAdvance(ellipsis);
        int avaiableWidth = name_max_width - ellipsisLen;
        int avaiableSize = name.size() * ((double)avaiableWidth/totalWidth);
        QString leftStr = name.left(avaiableSize);
        nameLabel->setText(leftStr + ellipsis);
    }

    // =========================
    // 创建布局管理器
    // =========================
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(2);
    this->setLayout(vlayout);

    // =========================
    // 添加至布局管理器
    // =========================
    vlayout->addWidget(avatarBtn, 0, Qt::AlignCenter);
    vlayout->addWidget(nameLabel, 0, Qt::AlignCenter);

    // =========================
    // 设置信号槽
    // =========================
    // 采用中转信号的方式
    connect(avatarBtn, &QPushButton::clicked, this, &AvatarItem::clicked);
}

// ==========================
// 允许自定义对应的ObjectName
// ==========================
void AvatarItem::setAvatarObjectName(const QString &objectName)
{
    if(avatarBtn){
        avatarBtn->setObjectName(objectName);

        // 强制刷新样式 (防止样式不生效)
        avatarBtn->style()->unpolish(avatarBtn);
        avatarBtn->style()->polish(avatarBtn);
    }
}

ChatDetailsPage *ChatDetailsPage::createChatDetailsPage(ChatDetailType type, QWidget *parent)
{
    /*
        CHAT_DETAIL_PRIVATE, // 私聊/单聊
        CHAT_DETAIL_GROUP    // 群聊
    */
    ChatDetailsPage* details_page;

        if (CHAT_DETAIL_PRIVATE == type){
            details_page = new PrivateChatDetailsPage(parent);
        }
        else if(CHAT_DETAIL_GROUP == type){
            details_page = new GroupChatDetailsPage(parent);
        }else{
            LOG()<<"未知的ChatDetailType类型, type: "<<type;
            details_page = new ChatDetailsPage();
        }
        return details_page ;
}

void ChatDetailsPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

ChatDetailsPage::ChatDetailsPage(QWidget *parent)
    :QWidget(parent)
{

}

PrivateChatDetailsPage::PrivateChatDetailsPage(QWidget *parent)
{
#if TEST_PRIVATE_CHAT

#endif
}

void PrivateChatDetailsPage::initSignalSlots()
{

}

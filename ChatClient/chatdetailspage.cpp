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
    : ChatDetailsPage(parent)
{
#if /*TEST_GROUP_CHAT*/0
    QLabel* label = new QLabel(this);
    label->setText("GroupChatDetailsPage");
#endif

    // 1. 基础布局 & 搜索框 & 滚动区
    initBaseLayout();

    // 2. 头像区域 (Grid + 按钮 + 线)
    initAvatarArea();

    // 3. 群信息 (公告 + 名称 + 线)
    initGroupInfo();

    // 4. 底部按钮 (清空 + 退出)
    initFooter();

    // 最后加弹簧
    groupDetailsScrollContainerVlayout->addStretch();


    // 初始化信号槽
    initSignalSlots();
}

// ==========================================
// 1. 基础布局初始化
// ==========================================
void GroupChatDetailsPage::initBaseLayout()
{
    // 外层 VLayout
    groupChatVlayout = new QVBoxLayout(this);
    this->setLayout(groupChatVlayout);
    groupChatVlayout->setSpacing(0);
    groupChatVlayout->setContentsMargins(0,0,0,0);

    // 搜索框区域
    QWidget *groupChatSearchWidget = new QWidget(this);
    groupChatSearchWidget->setFixedHeight(45);

    QHBoxLayout *groupChatSearchHlayout = new QHBoxLayout(groupChatSearchWidget);
    groupChatSearchWidget->setLayout(groupChatSearchHlayout); // 显式设置一下更稳妥
    groupChatSearchHlayout->setContentsMargins(20,10,20,5);
    groupChatSearchHlayout->setSpacing(0);
    groupChatSearchHlayout->setAlignment(Qt::AlignHCenter);

    groupChatSearch = new QLineEdit(groupChatSearchWidget);
    groupChatSearch->setObjectName("groupChatSearch");
    groupChatSearch->setFixedHeight(25);
    groupChatSearch->setPlaceholderText("搜索群成员");
    groupChatSearch->setClearButtonEnabled(true);
    groupChatSearch->setTextMargins(15,0,15,0);

    groupChatSearchHlayout->addWidget(groupChatSearch);
    groupChatVlayout->addWidget(groupChatSearchWidget);

    // 滚动区域
    groupChatScroll = new FloatingScrollArea(this);
    groupChatScroll->setObjectName("groupChatScroll");
    groupChatVlayout->addWidget(groupChatScroll);

    // 滚动内部容器
    groupChatDetailsScrollContainer = new QWidget(this);
    groupChatDetailsScrollContainer->setObjectName("groupChatDetailsScrollContainer");

    groupChatScroll->setWidget(groupChatDetailsScrollContainer);
    groupChatScroll->setWidgetResizable(true);

    // 滚动内部 VLayout
    groupDetailsScrollContainerVlayout = new QVBoxLayout(groupChatDetailsScrollContainer);
    groupDetailsScrollContainerVlayout->setContentsMargins(25,0,25,0);
    groupDetailsScrollContainerVlayout->setSpacing(7);
}

// ==========================================
// 2. 头像区域初始化
// ==========================================
void GroupChatDetailsPage::initAvatarArea()
{
    // --- 头像 Widget ---
    groupContainerAvatarItemWidget = new QWidget(groupChatDetailsScrollContainer);
    groupContainerAvatarItemWidget->setMinimumHeight(130);
    groupContainerAvatarItemWidget->setObjectName("groupContainerAvatarItemWidget");

    groupContainerAvatarItemLayout = new QGridLayout(groupContainerAvatarItemWidget);
    groupContainerAvatarItemWidget->setLayout(groupContainerAvatarItemLayout);

    // 【保留原布局】直接加 widget
    groupDetailsScrollContainerVlayout->addWidget(groupContainerAvatarItemWidget);

#if TEST_UI
    groupContainerAvatarItemWidget->setStyleSheet("background-color: red");
#endif

    // --- 查看更多按钮 ---
    groupContainerMoreAvatarItemBtn = new QPushButton(groupContainerAvatarItemWidget);
    groupContainerMoreAvatarItemBtn->setObjectName("groupContainerMoreAvatarItemBtn");
    groupContainerMoreAvatarItemBtn->setFixedHeight(30);

    // -----------------------------------------
    // 设置按钮内部的布局
    // -----------------------------------------
    QHBoxLayout *moreAvatarItemBtnHlayout = new QHBoxLayout(groupContainerMoreAvatarItemBtn);
    moreAvatarItemBtnHlayout->setSpacing(5);
    moreAvatarItemBtnHlayout->setContentsMargins(0,0,0,0);

    groupContainerMoreAvatarItemBtn->setLayout(moreAvatarItemBtnHlayout);
    // 先添加一个弹簧
    moreAvatarItemBtnHlayout->addStretch();

    // 添加左侧的Label
    QLabel *moreAvatarLabel = new QLabel(groupContainerMoreAvatarItemBtn);
    moreAvatarItemBtnHlayout->addWidget(moreAvatarLabel);
    moreAvatarLabel->setText("查看更多");
    moreAvatarLabel->setFixedHeight(25);
    moreAvatarLabel->setStyleSheet("font-size:12px;color:#9e9e9e;");

    // 添加右侧的Icon
    QPushButton *seeMoreAvatarIcon = new QPushButton(groupContainerAvatarItemWidget);
    seeMoreAvatarIcon->setStyleSheet("border:none;background: transparent;");
    seeMoreAvatarIcon->setFixedSize(15,15);
    seeMoreAvatarIcon->setIconSize(QSize(15,15));
    seeMoreAvatarIcon->setIcon(QIcon(":/resource/image/bottom.png"));
    moreAvatarItemBtnHlayout->addWidget(seeMoreAvatarIcon);

    // 在添加一个弹簧 把两个内容挤在一起
    moreAvatarItemBtnHlayout->addStretch();


#if TEST_UI_
    groupContainerMoreAvatarItemBtn->setStyleSheet("background-color: Black;");
#endif

    // 【保留原布局】
    groupDetailsScrollContainerVlayout->addWidget(groupContainerMoreAvatarItemBtn);

    // --- 分隔线 ---
    addSeparator();
}

// ==========================================
// 3. 群信息初始化
// ==========================================
void GroupChatDetailsPage::initGroupInfo()
{


    // --- 群名称 ---
    groupNameTag = new QLabel(groupChatDetailsScrollContainer);
    groupNameTag->setObjectName("groupNameTag");
    groupNameTag->setText("群名称");

    groupName = new QPushButton(groupChatDetailsScrollContainer);
    groupName->setObjectName("groupName");
    groupName->setFixedHeight(40);


    // 【严格保留你的布局逻辑】
    groupDetailsScrollContainerVlayout->addWidget(groupNameTag, 0, Qt::AlignLeft);
    groupDetailsScrollContainerVlayout->addWidget(groupName);


    // --------------------------------------------
    // 群名称的布局
    // --------------------------------------------
    // 创建布局
    QHBoxLayout* groupNameLayout = new QHBoxLayout(groupName);
    groupName->setLayout(groupNameLayout);
    groupNameLayout->setContentsMargins(0,0,0,0);
    groupNameLayout->setSpacing(5);
    // 设置左边的文本
    QLabel* groupNameLabel = new QLabel(groupName);
    groupNameLayout->addWidget(groupNameLabel,0,Qt::AlignLeft|Qt::AlignVCenter);
    groupNameLabel->setStyleSheet("font-size:12px;color:#9e9e9e;");
    groupNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // 穿透
    groupNameLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    // 设置右边的修改icon
    QPushButton *groupNameIcon = new QPushButton(groupName);
    groupNameIcon->setFixedSize(13,13);
    groupNameIcon->setIconSize(QSize(12,12));
    groupNameIcon->setIcon(QIcon(":/resource/image/editName.png"));
    groupNameLayout->addWidget(groupNameIcon,0,Qt::AlignLeft|Qt::AlignVCenter);
    groupNameIcon->setStyleSheet("border:none;background: transparent;");
    groupNameIcon->setAttribute(Qt::WA_TransparentForMouseEvents);

#if TEST_UI
    groupNameLabel->setText("群名称的内容 - 测试");
#endif



    // --- 群公告 ---
    groupAnnouncementTag = new QLabel(groupChatDetailsScrollContainer);
    groupAnnouncementTag->setObjectName("groupAnnouncementTag");
    groupAnnouncementTag->setText("群公告");


    groupAnnouncement = new QPushButton(groupChatDetailsScrollContainer);
    groupAnnouncement->setObjectName("groupAnnouncement");
    groupAnnouncement->setFixedHeight(40);
    groupAnnouncement->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    // 【严格保留你的布局逻辑】：左对齐Tag，居中对齐Btn
    groupDetailsScrollContainerVlayout->addWidget(groupAnnouncementTag, 0, Qt::AlignLeft);
    groupDetailsScrollContainerVlayout->addWidget(groupAnnouncement);


    // --------------------------------------------
    // 群公告的布局
    // --------------------------------------------
    // 创建布局
    QHBoxLayout* groupAnnouncementLayout = new QHBoxLayout(groupAnnouncement);
    groupAnnouncement->setLayout(groupAnnouncementLayout);
    groupAnnouncementLayout->setContentsMargins(0,0,0,0);
    groupAnnouncementLayout->setSpacing(0);
    // 设置左边的文本
    QLabel* announcementLabel = new QLabel(groupAnnouncement);
    groupAnnouncementLayout->addWidget(announcementLabel,0,Qt::AlignLeft|Qt::AlignVCenter);
    announcementLabel->setStyleSheet("font-size:12px;color:#9e9e9e;");
    announcementLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // 设置右边的更多icon
    QPushButton *announcementIcon = new QPushButton(groupAnnouncement);
    announcementIcon->setFixedSize(15,15);
    announcementIcon->setIconSize(QSize(15,15));
    announcementIcon->setIcon(QIcon(":/resource/image/more.png"));
    groupAnnouncementLayout->addWidget(announcementIcon,0,Qt::AlignRight|Qt::AlignVCenter);
    announcementIcon->setStyleSheet("border:none;background: transparent;");


#if TEST_UI
    announcementLabel->setText("群公告的内容 - 测试");
#endif


    // --- 分隔线 ---
    addSeparator();
}

// ==========================================
// 4. 底部按钮初始化
// ==========================================
void GroupChatDetailsPage::initFooter()
{
    // --- 清空聊天记录 ---
    clearChatHistory = new QPushButton(groupChatDetailsScrollContainer);
    clearChatHistory->setObjectName("clearChatHistory");
    clearChatHistory->setText("清空聊天记录");
    groupDetailsScrollContainerVlayout->addWidget(clearChatHistory);
    clearChatHistory->setFixedHeight(30);

    // --- 分隔线 ---
    addSeparator();

    // --- 退出群聊 ---
    exitGroupChatBtn = new QPushButton(groupChatDetailsScrollContainer);
    exitGroupChatBtn->setObjectName("exitGroupChatBtn");
    exitGroupChatBtn->setText("退出群聊");
    exitGroupChatBtn->setFixedHeight(30);

    groupDetailsScrollContainerVlayout->addWidget(exitGroupChatBtn);
}

// ==========================================
// 5. 辅助函数：只负责画线
// ==========================================
void GroupChatDetailsPage::addSeparator()
{
    QFrame *line = new QFrame(groupChatDetailsScrollContainer);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #EAEAEA; border: none; min-height: 1px; max-height: 1px;");
    groupDetailsScrollContainerVlayout->addWidget(line);
}



void GroupChatDetailsPage::initSignalSlots()
{
    // 关联弹窗构造函数
    connect(this, &GroupChatDetailsPage::signalAddFriendClicked, this, [=](){
        ChooseFriendWidget *chooseFriendWidget = new ChooseFriendWidget(this);
        chooseFriendWidget->show();
    });

    connect(groupContainerMoreAvatarItemBtn, &QPushButton::clicked, this, [=](){
#if TEST_UI
        LOG()<<"查看更多群友";
#endif
    });

    connect(groupName, &QPushButton::clicked, this ,[=](){
#if TEST_UI
        LOG()<<"修改群名";
#endif
    });

    connect(groupAnnouncement,&QPushButton::clicked, this ,[=](){
#if TEST_UI
        LOG()<<"打开群公告";
#endif
    });

    connect(clearChatHistory,&QPushButton::clicked, this ,[=](){
#if TEST_UI
        LOG()<<"清空聊天记录";
#endif
    });

    connect(exitGroupChatBtn,&QPushButton::clicked, this ,[=](){
#if TEST_UI
        LOG()<<"退出群聊";
#endif
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

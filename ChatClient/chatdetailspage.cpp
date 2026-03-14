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
#include "toolswidget.h"
#include "infowidget.h"

// ##############################################################################
//                               GroupChatDetailsPage
//                          群聊详情页 - 核心逻辑实现
// ##############################################################################

// ==============================================================================
//  构造函数
// ==============================================================================
GroupChatDetailsPage::GroupChatDetailsPage(QWidget *parent)
    : ChatDetailsPage(parent)
{
#if /*TEST_GROUP_CHAT*/0
    QLabel* label = new QLabel(this);
    label->setText("GroupChatDetailsPage");
#endif

    // -------------------------------------------------------
    // 1. 初始化基础框架
    //    (构建整体垂直布局、搜索框、滚动区域外壳)
    // -------------------------------------------------------
    initBaseLayout();

    // -------------------------------------------------------
    // 2. 初始化头像网格区域
    //    (构建 4列 Grid，并预置 [+] 和 [-] 按钮)
    // -------------------------------------------------------
    initAvatarArea();

    // -------------------------------------------------------
    // 3. 初始化群信息区域
    //    (群公告、群名称及其修改功能)
    // -------------------------------------------------------
    initGroupInfo();

    // -------------------------------------------------------
    // 4. 初始化底部功能区
    //    (清空聊天记录、退出群聊按钮)
    // -------------------------------------------------------
    initFooter();

    // -------------------------------------------------------
    // 5. 初始化信号槽连接
    //    (绑定按钮点击事件与业务逻辑)
    // -------------------------------------------------------
    initSignalSlots();

    // -------------------------------------------------------
    // 6. 初始化弹窗
    //    (预加载修改群名的确认弹窗)
    // -------------------------------------------------------
    initConfirmModifygroupName();

    // 【注意】: 这里不再添加 addStretch()，因为 initBaseLayout 中使用了 SetFixedSize 约束
    // groupDetailsScrollContainerVlayout->addStretch();
}

// ==============================================================================
//  功能函数: 添加群成员
//  原理: 基于 QGridLayout 的自动计数特性，计算行列坐标
// ==============================================================================
void GroupChatDetailsPage::addMemberItem(const QString& userId, const QString& name, const QIcon& avatar)
{
    // 1. 获取当前布局里已经有多少个控件了 (包含 +, -, 以及已存在的成员)
    int count = groupContainerAvatarItemLayout->count();

    // 2. [核心算法] 计算下一个空位的坐标
    //    无需维护复杂的 index 变量，直接根据 layout 现有数量推算
    int row = count / 4; // 行号: count / 列数
    int col = count % 4; // 列号: count % 列数

    // 3. 创建头像控件 AvatarItem
    AvatarItem* userItem = new AvatarItem(groupContainerAvatarItemWidget, name, avatar);

    // 4. 绑定点击信号 -> 转发为查看详情信号
    connect(userItem, &AvatarItem::clicked, this, [=](){
        emit signalShowUserDetail(userId);
    });

    // 5. 添加进 Grid 布局
    groupContainerAvatarItemLayout->addWidget(userItem, row, col);
}

// ==============================================================================
//  UI 初始化: 基础布局 (Base Layout)
//  重点: 解决 ScrollArea 底部按钮被裁剪的核心布局约束
// ==============================================================================
void GroupChatDetailsPage::initBaseLayout()
{
    // --- 1. 最外层垂直布局 ---
    groupChatVlayout = new QVBoxLayout(this);
    this->setLayout(groupChatVlayout);
    groupChatVlayout->setSpacing(0);
    groupChatVlayout->setContentsMargins(0,0,0,0);

    // --- 2. 顶部搜索框区域 ---
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
    groupChatSearch->setClearButtonEnabled(true);
    groupChatSearch->setTextMargins(15,0,15,0);

    groupChatSearchHlayout->addWidget(groupChatSearch);
    groupChatVlayout->addWidget(groupChatSearchWidget);

    // --- 3. 滚动区域 (FloatingScrollArea) ---
    groupChatScroll = new FloatingScrollArea(this);
    groupChatScroll->setObjectName("groupChatScroll");
    groupChatVlayout->addWidget(groupChatScroll);

    // --- 4. 滚动内容的实体容器 (Container) ---
    groupChatDetailsScrollContainer = new QWidget(this);
    groupChatDetailsScrollContainer->setObjectName("groupChatDetailsScrollContainer");

    groupChatScroll->setWidget(groupChatDetailsScrollContainer);
    groupChatScroll->setWidgetResizable(true);

    // --- 5. 滚动容器内部布局 (VLayout) ---
    groupDetailsScrollContainerVlayout = new QVBoxLayout(groupChatDetailsScrollContainer);

    // [关键修复]: 底部留出 150px 边距，防止滚动到底部时，最下方的按钮被 ScrollArea 的边框或轨道遮挡
    groupDetailsScrollContainerVlayout->setContentsMargins(20,0,20,150);
    groupDetailsScrollContainerVlayout->setSpacing(7);

    // [核心修复]: 强制容器高度 = 所有子控件高度之和
    // 使用 SetFixedSize 配合去掉了 addStretch()，确保 ScrollArea 能精确计算滚动范围
    groupDetailsScrollContainerVlayout->setSizeConstraint(QLayout::SetFixedSize);
}

// ==============================================================================
//  UI 初始化: 头像网格区域 (Avatar Grid)
//  重点: 4列布局，固定前两个位置为 [+] 和 [-]
// ==============================================================================
void GroupChatDetailsPage::initAvatarArea()
{
    // --- 1. 头像容器 Widget ---
    groupContainerAvatarItemWidget = new QWidget(groupChatDetailsScrollContainer);
    groupContainerAvatarItemWidget->setObjectName("groupContainerAvatarItemWidget");

    // [关键设置]: 垂直方向 Fixed (完全由 Grid 撑开)，防止被压缩
    groupContainerAvatarItemWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // --- 2. 网格布局 ---
    groupContainerAvatarItemLayout = new QGridLayout(groupContainerAvatarItemWidget);
    groupContainerAvatarItemWidget->setLayout(groupContainerAvatarItemLayout);
    groupContainerAvatarItemLayout->setSpacing(0);

    // [关键设置]: 强制 Grid 计算出精确的 SizeHint
    groupContainerAvatarItemLayout->setSizeConstraint(QLayout::SetFixedSize);

    // 添加到父布局
    groupDetailsScrollContainerVlayout->addWidget(groupContainerAvatarItemWidget);

#if TEST_UI
    // groupContainerAvatarItemWidget->setStyleSheet("background-color: red");
#endif

    // --- 3. 添加固定按钮 [+] ---
    AvatarItem* addBtn = new AvatarItem(groupContainerAvatarItemWidget, "添加", QIcon());
    addBtn->setAvatarObjectName("detailsPageAddFriendBtn"); // 设置特殊样式ID
    connect(addBtn, &AvatarItem::clicked, this, &GroupChatDetailsPage::signalAddFriendClicked);

    // 强制放在 (0, 0)
    groupContainerAvatarItemLayout->addWidget(addBtn, 0, 0);

    // --- 4. 添加固定按钮 [-] ---
    AvatarItem* removeBtn = new AvatarItem(groupContainerAvatarItemWidget, "移除", QIcon());
    removeBtn->setAvatarObjectName("detailsPageRemoveFriendBtn"); // 设置特殊样式ID
    connect(removeBtn, &AvatarItem::clicked, this, &GroupChatDetailsPage::signalRemoveFriendClicked);
    removeBtn->setHidden(true);

    // 强制放在 (0, 1)
    groupContainerAvatarItemLayout->addWidget(removeBtn, 0, 1);

    // --- 5. [测试用] 批量生成假数据 ---
#if TEST_UI
    for(int i=0; i<20; i++) {
        addMemberItem(QString::number(i), QString("这是一个群友%1").arg(i), QIcon(":/resource/image/defaultAvatar.png"));
    }
#endif

    // --- 分隔线 ---
    addSeparator();
}

// ==============================================================================
//  UI 初始化: 群信息 (名称 & 公告)
// ==============================================================================
void GroupChatDetailsPage::initGroupInfo()
{
    // --------------------------------------------------------
    // A. 群名称部分
    // --------------------------------------------------------
    groupNameTag = new QLabel(groupChatDetailsScrollContainer);
    groupNameTag->setObjectName("groupNameTag");
    groupNameTag->setText("群名称");

    groupName = new QPushButton(groupChatDetailsScrollContainer);
    groupName->setObjectName("groupName");
    groupName->setFixedHeight(20);

    // 群名称修改输入框 (默认隐藏)
    groupNameEdit = new LineEditFocus(groupChatDetailsScrollContainer);
    groupNameEdit->setObjectName("groupNameEdit");
    groupNameEdit->setHidden(true);
    groupNameEdit->setFixedHeight(20);

    // 添加到主布局
    groupDetailsScrollContainerVlayout->addWidget(groupNameTag, 0, Qt::AlignLeft);
    groupDetailsScrollContainerVlayout->addWidget(groupName);
    groupDetailsScrollContainerVlayout->addWidget(groupNameEdit);

    // [内部布局]: 群名称按钮内部 (左侧文字 + 右侧编辑图标)
    QHBoxLayout* groupNameLayout = new QHBoxLayout(groupName);
    groupName->setLayout(groupNameLayout);
    groupNameLayout->setContentsMargins(0,0,0,0);
    groupNameLayout->setSpacing(5);

    groupNameLabel = new QLabel(groupName);
    groupNameLayout->addWidget(groupNameLabel,0,Qt::AlignLeft|Qt::AlignVCenter);
    groupNameLabel->setStyleSheet("font-size:12px;color:#9e9e9e;");
    groupNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    groupNameLabel->setAttribute(Qt::WA_TransparentForMouseEvents); // 鼠标穿透，点击事件交给按钮

    QPushButton *groupNameIcon = new QPushButton(groupName);
    groupNameIcon->setFixedSize(13,13);
    groupNameIcon->setIconSize(QSize(12,12));
    groupNameIcon->setIcon(QIcon(":/resource/image/editName.png"));
    groupNameLayout->addWidget(groupNameIcon,0,Qt::AlignLeft|Qt::AlignVCenter);
    groupNameIcon->setStyleSheet("border:none;background: transparent;");
    groupNameIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    groupNameLabel->setObjectName("groupNameLabel");

#if TEST_UI
    groupNameLabel->setText("群名称的内容 - 测试");
#endif

    // --------------------------------------------------------
    // B. 群公告部分
    // --------------------------------------------------------
    groupAnnouncementTag = new QLabel(groupChatDetailsScrollContainer);
    groupAnnouncementTag->setObjectName("groupAnnouncementTag");
    groupAnnouncementTag->setText("群公告");

    groupAnnouncement = new QPushButton(groupChatDetailsScrollContainer);
    groupAnnouncement->setObjectName("groupAnnouncement");
    groupAnnouncement->setFixedHeight(40);
    groupAnnouncement->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    groupDetailsScrollContainerVlayout->addWidget(groupAnnouncementTag, 0, Qt::AlignLeft);
    groupDetailsScrollContainerVlayout->addWidget(groupAnnouncement);

    // [内部布局]: 群公告按钮内部 (左侧文字 + 右侧箭头)
    QHBoxLayout* groupAnnouncementLayout = new QHBoxLayout(groupAnnouncement);
    groupAnnouncement->setLayout(groupAnnouncementLayout);
    groupAnnouncementLayout->setContentsMargins(0,0,0,0);
    groupAnnouncementLayout->setSpacing(0);

    QLabel* announcementLabel = new QLabel(groupAnnouncement);
    groupAnnouncementLayout->addWidget(announcementLabel,0,Qt::AlignLeft|Qt::AlignVCenter);
    announcementLabel->setStyleSheet("font-size:12px;color:#9e9e9e;");
    announcementLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

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

// ==============================================================================
//  UI 初始化: 底部操作按钮
// ==============================================================================
void GroupChatDetailsPage::initFooter()
{
    // --- 1. 清空聊天记录 ---
    clearChatHistory = new QPushButton(groupChatDetailsScrollContainer);
    clearChatHistory->setObjectName("clearChatHistory");
    clearChatHistory->setText("清空聊天记录");
    groupDetailsScrollContainerVlayout->addWidget(clearChatHistory);
    clearChatHistory->setFixedHeight(30);

    // --- 分隔线 ---
    addSeparator();

    // --- 2. 退出群聊 (最底部) ---
    exitGroupChatBtn = new QPushButton(groupChatDetailsScrollContainer);
    exitGroupChatBtn->setObjectName("exitGroupChatBtn");
    exitGroupChatBtn->setText("退出群聊");
    exitGroupChatBtn->setFixedHeight(30);

    groupDetailsScrollContainerVlayout->addWidget(exitGroupChatBtn);
}

// ==============================================================================
//  辅助函数: 添加分割线
// ==============================================================================
void GroupChatDetailsPage::addSeparator()
{
    QFrame *line = new QFrame(groupChatDetailsScrollContainer);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #EAEAEA; border: none; min-height: 1px; max-height: 1px;");
    groupDetailsScrollContainerVlayout->addWidget(line);
}

// ==============================================================================
//  UI 初始化: 修改群名称确认弹窗 (InfoWidget 复用)
// ==============================================================================
void GroupChatDetailsPage::initConfirmModifygroupName()
{
    // 创建模态弹窗
    confirmModifyGroupName = new InfoWidget(this,WindowModal::IS_MODAL);
    confirmModifyGroupName->setFixedSize(320,200);

    // 获取通用容器并布局
    QFrame* frame = confirmModifyGroupName->getFrame();
    QGridLayout *layout = new QGridLayout(frame);
    layout->setContentsMargins(30,30,30,30);
    layout->setSpacing(7);
    frame->setLayout(layout);

    // 标题
    QLabel* label = new QLabel(frame);
    label->setText("修改群聊名称?");
    layout->addWidget(label,0,0,1,2);
    label->setStyleSheet("background: transparent; font-size: 15px; font-family:微软雅黑; font-width: auto;");
    label->setAlignment(Qt::AlignCenter);

    // 确认按钮
    acceptModifyNameBtn = new QPushButton(frame);
    acceptModifyNameBtn->setFocusPolicy(Qt::NoFocus);
    acceptModifyNameBtn->setFixedHeight(35);
    acceptModifyNameBtn->setObjectName("acceptModifyNameBtn");
    acceptModifyNameBtn->setText("修改");

    // 取消按钮
    cancelModifyNameBtn = new QPushButton(frame);
    cancelModifyNameBtn->setFocusPolicy(Qt::NoFocus);
    cancelModifyNameBtn->setFixedHeight(32);
    cancelModifyNameBtn->setObjectName("cancelModifyNameBtn");
    cancelModifyNameBtn->setText("取消");

    layout->addWidget(acceptModifyNameBtn, 1, 0,1,1);
    layout->addWidget(cancelModifyNameBtn, 1, 1,1,1);

    // 连接信号: 确认修改
    connect(acceptModifyNameBtn, &QPushButton::clicked, this,[=](){
        QString str = groupNameEdit->text();
        groupNameLabel->setText(str);
        confirmModifyGroupName->close();
    });

    // 连接信号: 取消 (关闭弹窗)
    connect(cancelModifyNameBtn, &QPushButton::clicked, confirmModifyGroupName, &InfoWidget::close);
}

// ==============================================================================
//  信号槽初始化
// ==============================================================================
void GroupChatDetailsPage::initSignalSlots()
{
    // 1. 添加好友
    connect(this, &GroupChatDetailsPage::signalAddFriendClicked, this, [=](){
        ChooseFriendWidget *chooseFriendWidget = new ChooseFriendWidget(this);
        chooseFriendWidget->exec();
    });

    // 2. 点击群名称 -> 切换到编辑模式
    connect(groupName, &QPushButton::clicked, this ,[=](){
        QString str= groupNameLabel->text();
        if(groupNameEdit->isHidden()){
            groupNameEdit->setHidden(false);
            groupNameEdit->setText(str);
            groupNameEdit->setFocus(); // 获得焦点
            groupNameEdit->setCursorPosition(str.length()); // 光标移至末尾
            groupName->setHidden(true);
        }
#if TEST_UI
        LOG()<<"修改群名";
#endif
    });

    // 3. 编辑框失去焦点 -> 恢复展示模式 & 检测是否修改
    connect(groupNameEdit, &LineEditFocus::signalLoseFocus, this,[=](){
#if TEST_UI
        LOG()<<"修改群名框失去了焦点";
#endif
        if(groupName->isHidden()){
            groupNameEdit->setHidden(true);
            groupName->setHidden(false);
        }
        // 如果内容变了，弹出确认框
        if(groupNameEdit->isModified()){
            confirmModifyGroupName->show();
        }
    } );

    // 4. 其他功能按钮 (目前仅打印日志)
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

// ##############################################################################
//                               AvatarItem
//                      群成员头像项 (头像 + 名字)
// ##############################################################################

// ==============================================================================
//  构造函数
// ==============================================================================
AvatarItem::AvatarItem(QWidget *parent, const QString& name,const QIcon& avatarIcon): QWidget{parent}
{
    // ------------------------------------
    // 1. 初始化尺寸
    // ------------------------------------
    // 适当增加宽度，给文字留点空间 (50x65)
    this->setFixedSize(50, 65);

    // 头像按钮
    avatarBtn = new QPushButton(this);
    avatarBtn->setObjectName("AvatarItem_avatarBtn");
    avatarBtn->setFixedSize(40,40);
    avatarBtn->setIconSize(QSize(40,40));

    if (!avatarIcon.isNull()) {
        avatarBtn->setIcon(avatarIcon);
    }

    // 名字标签
    nameLabel = new QLabel(this);
    nameLabel->setObjectName("AvatarItem_nameLabel");
    nameLabel->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    // ------------------------------------
    // 2. 字体与截断逻辑 (Smart Elide)
    // ------------------------------------
    // 使用 12px 字体 (需与 QSS 保持一致以保证计算准确)
    QFont font("Microsoft YaHei", 12);
    nameLabel->setFont(font);

    // 计算文字宽度，如果过长则自动截断为 "..."
    QFontMetrics metrics(font);
    int maxTextWidth = this->width() - 4; // 预留左右边距

    QString elidedText = metrics.elidedText(name, Qt::ElideRight, maxTextWidth);
    nameLabel->setText(elidedText);

    // 如果被截断，悬停显示全名
    if (elidedText != name) {
        nameLabel->setToolTip(name);
    }

    // ------------------------------------
    // 3. 布局管理
    // ------------------------------------
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(0);
    this->setLayout(vlayout);

    vlayout->addWidget(avatarBtn, 0, Qt::AlignHCenter|Qt::AlignBottom);
    vlayout->addWidget(nameLabel, 0, Qt::AlignHCenter|Qt::AlignTop);

    // 信号转发
    connect(avatarBtn, &QPushButton::clicked, this, &AvatarItem::clicked);
}

// ==============================================================================
//  辅助函数: 允许外部设置 AvatarBtn 的 ObjectName (用于加号/减号的特殊样式)
// ==============================================================================
void AvatarItem::setAvatarObjectName(const QString &objectName)
{
    if(avatarBtn){
        avatarBtn->setObjectName(objectName);

        // 强制刷新样式 (防止 QSS 不立即生效)
        avatarBtn->style()->unpolish(avatarBtn);
        avatarBtn->style()->polish(avatarBtn);
    }
}

// ##############################################################################
//                             ChatDetailsPage (工厂类)
// ##############################################################################

ChatDetailsPage *ChatDetailsPage::createChatDetailsPage(ChatDetailType type, QWidget *parent)
{
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

// ##############################################################################
//                           PrivateChatDetailsPage
//                      单聊详情页 - 核心逻辑实现
// ##############################################################################

// ==============================================================================
//  构造函数
// ==============================================================================
PrivateChatDetailsPage::PrivateChatDetailsPage(QWidget *parent)
{
    // -------------------------------------------------------
    // 1. 最外层布局初始化
    // -------------------------------------------------------
    initBaseLayout();

    // -------------------------------------------------------
    // 2. 头像区域初始化
    //    (横向布局: 对方头像 + 添加按钮)
    // -------------------------------------------------------
    initAvatarArea();

    // -------------------------------------------------------
    // 3. 初始化"查找聊天记录"按钮
    // -------------------------------------------------------
    initFindHistoryBtn();

    // -------------------------------------------------------
    // 4. 初始化底部功能区
    //    (清空聊天记录 / 删除好友)
    // -------------------------------------------------------
    initFooter();

    // -------------------------------------------------------
    // 5. 初始化信号槽
    // -------------------------------------------------------
    initSignalSlots();

    // 最后加弹簧 (顶起上方内容)
    privateChatVlayout->addStretch();
}

// ==============================================================================
//  信号槽初始化
// ==============================================================================
void PrivateChatDetailsPage::initSignalSlots()
{
    // 1. 添加好友
    connect(this, &PrivateChatDetailsPage::signalAddFriendClicked, this, [=](){
        ChooseFriendWidget *chooseFriendWidget = new ChooseFriendWidget(this);
        chooseFriendWidget->exec();
    });

    // 2. 查找聊天记录
    connect(privateChatFindHistory, &QPushButton::clicked, this, [=](){
#if TEST_UI
        LOG()<<"查找聊天记录";
#endif
    });

    // 3. 清空聊天记录
    connect(clearChatHistory, &QPushButton::clicked, this, [=](){
#if TEST_UI
        LOG()<<"清空聊天记录";
#endif
    });

    // 4. 删除好友
    connect(privateDeleteFriend, &QPushButton::clicked, this, [=](){
#if TEST_UI
        LOG()<<"删除好友";
#endif
    });
}

// ==============================================================================
//  UI 初始化: 头像区域 (横向布局)
// ==============================================================================
void PrivateChatDetailsPage::initAvatarArea()
{
    privateChatAvatarWidget = new QWidget(this);
    privateChatVlayout->addWidget(privateChatAvatarWidget);
    privateChatAvatarWidget->setFixedHeight(70);
#if TEST_UI
    // privateChatAvatarWidget->setStyleSheet("background-color: red;");
#endif

    // 内部使用横向布局
    privateChatAvatarHlayout = new QHBoxLayout(privateChatAvatarWidget);
    privateChatAvatarWidget->setLayout(privateChatAvatarHlayout);
    privateChatAvatarHlayout->setAlignment(Qt::AlignLeft);
    privateChatAvatarHlayout->setSpacing(0);
    privateChatAvatarHlayout->setContentsMargins(0,0,0,0);

    // --- 添加固定按钮 [+] ---
    AvatarItem* addBtn = new AvatarItem(privateChatAvatarWidget, "添加", QIcon());
    addBtn->setAvatarObjectName("detailsPageAddFriendBtn"); // 设置特殊样式ID
    connect(addBtn, &AvatarItem::clicked, this, &PrivateChatDetailsPage::signalAddFriendClicked);
    privateChatAvatarHlayout->addWidget(addBtn);

    // --- [测试用] 添加当前用户头像 ---
#if TEST_UI
    // AvatarItem::AvatarItem(QWidget *parent, const QString& name,const QIcon& avatarIcon): QWidget{parent}
    AvatarItem *curUser = new AvatarItem(privateChatAvatarWidget, "测试用户数据", QIcon(":/resource/image/defaultAvatar.png"));
    privateChatAvatarHlayout->addWidget(curUser);
#endif

    addSeparator();
}

// ==============================================================================
//  UI 初始化: 基础布局
// ==============================================================================
void PrivateChatDetailsPage::initBaseLayout()
{
    privateChatVlayout = new QVBoxLayout(this);
    // 设置布局
    this->setLayout(privateChatVlayout);
    // 初始化布局参数
    privateChatVlayout->setSpacing(10);
    privateChatVlayout->setContentsMargins(20,10,20,150);
}

// ==============================================================================
//  辅助函数: 添加分割线
// ==============================================================================
void PrivateChatDetailsPage::addSeparator()
{
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #EAEAEA; border: none; min-height: 1px; max-height: 1px;");
    privateChatVlayout->addWidget(line);
}

// ==============================================================================
//  UI 初始化: 查找聊天记录按钮
// ==============================================================================
void PrivateChatDetailsPage::initFindHistoryBtn()
{
    privateChatFindHistory = new QPushButton(this);
    privateChatFindHistory->setFixedHeight(35);
    privateChatFindHistory->setObjectName("privateChatFindHistory");
#if TEST_UI
    // privateChatFindHistory->setStyleSheet("background-color:red;");
#endif
    privateChatVlayout->addWidget(privateChatFindHistory);

    // 内部布局: 左侧文本 + 右侧图标
    QHBoxLayout *findHistoryLayout = new QHBoxLayout(privateChatFindHistory);
    privateChatFindHistory->setLayout(findHistoryLayout);
    findHistoryLayout->setSpacing(0);
    findHistoryLayout->setContentsMargins(0,0,0,0);

    QLabel *historyLabel = new QLabel(privateChatFindHistory);

    QPushButton *historyMoreIcon = new QPushButton(privateChatFindHistory);
    findHistoryLayout->addWidget(historyLabel, 0, Qt::AlignLeft);
    findHistoryLayout->addWidget(historyMoreIcon, 0, Qt::AlignRight);

    historyLabel->setText("查找聊天记录");
    historyLabel->setStyleSheet("font-size: 12px;");

    historyMoreIcon->setFixedSize(15,15);
    historyMoreIcon->setIconSize(QSize(15,15));
    historyMoreIcon->setIcon(QIcon(":/resource/image/more.png"));
    historyMoreIcon->setStyleSheet("border:none;background: transparent;");

#if TEST_UI
#endif

    addSeparator();
}

// ==============================================================================
//  UI 初始化: 底部操作按钮
// ==============================================================================
void PrivateChatDetailsPage::initFooter()
{
    // --- 1. 清空聊天记录 ---
    clearChatHistory = new QPushButton(this);
    clearChatHistory->setObjectName("clearChatHistory");
    clearChatHistory->setText("清空聊天记录");
    privateChatVlayout->addWidget(clearChatHistory);
    clearChatHistory->setFixedHeight(30);

    // --- 分隔线 ---
    addSeparator();

    // --- 2. 删除好友 (最底部) ---
    privateDeleteFriend = new QPushButton(this);
    privateDeleteFriend->setObjectName("privateDeleteFriend");
    privateDeleteFriend->setText("删除好友");
    privateDeleteFriend->setFixedHeight(30);

    privateChatVlayout->addWidget(privateDeleteFriend);
}

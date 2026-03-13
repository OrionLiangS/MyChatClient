#include "addfrienddialog.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "debug.h"
#include <QLabel>
#include <QLineEdit>
#include <QStyleOption>
#include <QStyle>
#include <QPainter>
#include <QStyle>
#include <QStackedWidget>

// ################################################################################
// 类: AddFriendDialog
// 描述: 查找好友弹窗
//       包含搜索框、结果展示区(StackedWidget)以及添加好友的核心交互逻辑
// ################################################################################

// ================================================================================
// 构造函数
// ================================================================================
AddFriendDialog::AddFriendDialog(QWidget *parent)
    :InfoWidget(parent, true)
{
    initMainWidget();

    initTitle();

    initSearchWidget();

    initStackedWidget();

    initSignalSlots();
}


// ================================================================================
// 函数: initMainWidget
// 描述: 初始化主窗口的基本布局与样式
// ================================================================================
void AddFriendDialog::initMainWidget()
{
    mainLayout = new QVBoxLayout(mainFrame);
    mainFrame->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(10);
    mainLayout->setAlignment(Qt::AlignTop);

    this->setFixedSize(365,490);

    this->setAttribute(Qt::WA_DeleteOnClose); // 修复了模态情况下关闭不会自动释放内存导致内存泄漏

    // -----------------------------------------------------------
    // 设置主窗口圆角与背景色
    // -----------------------------------------------------------
    mainFrame->setStyleSheet(
        "#mainFrame {"
        "   background-color: #F7F7F7;"
        "   border-radius: 10px;"
        "}"
        );
}

// ================================================================================
// 函数: initTitle
// 描述: 初始化自定义标题栏 (包含标题文本与关闭按钮)
// ================================================================================
void AddFriendDialog::initTitle()
{
    // 初始化标题Widget以及关闭按钮
    addFriendTitleWidget = new QWidget(mainFrame);
    addFriendTitleWidget->setFixedHeight(31);
    addFriendTitleWidget->setObjectName("addFriendTitleWidget");

    // -----------------------------------------------------------
    // 创建标题栏布局
    // -----------------------------------------------------------
    QHBoxLayout *titleLayout = new QHBoxLayout(addFriendTitleWidget);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0,0,0,0);
    addFriendTitleWidget->setLayout(titleLayout);

    // 左侧占位，保持标题居中
    titleLayout->addSpacing(50);
    titleLayout->addStretch();

    // 将TitleWidget添加进总布局
    mainLayout->addWidget(addFriendTitleWidget);

    // -----------------------------------------------------------
    // 创建标题文本 Label
    // -----------------------------------------------------------
    QLabel *addFriendTitle = new QLabel(addFriendTitleWidget);
    addFriendTitleWidget->setFixedHeight(31);
    addFriendTitle->setText("添加朋友");
    titleLayout->addWidget(addFriendTitle, 0, Qt::AlignHCenter);
    titleLayout->addStretch();
    addFriendTitle->setStyleSheet("font-size: 17px; font-family:\"思源黑体\"; font-weight: bold;");

    // -----------------------------------------------------------
    // 初始化关闭按钮
    // -----------------------------------------------------------
    closeAddFriendBtn = new QPushButton(mainFrame);
    closeAddFriendBtn->setFixedSize(50,31);
    closeAddFriendBtn->setObjectName("closeAddFriendBtn");
    closeAddFriendBtn->setText(QChar(0x00D7)); // '×' 符号

    // 将按钮添加进布局 (右对齐)
    titleLayout->addWidget(closeAddFriendBtn, 0, Qt::AlignRight);

// 测试：用于调试标题栏区域
#if TEST_UI
    // addFriendTitleWidget->setStyleSheet("background-color: red;");
#endif

}

// ================================================================================
// 函数: initSignalSlots
// 描述: 绑定窗口内的信号与槽
// ================================================================================
void AddFriendDialog::initSignalSlots()
{
    connect(closeAddFriendBtn, &QPushButton::clicked, this, &AddFriendDialog::close);
    connect(addFriendSearchBtn, &QPushButton::clicked, this, &AddFriendDialog::doSearch);
    // 绑定回车键触发搜索
    connect(addFriendSearchEdit, &QLineEdit::returnPressed, this, &AddFriendDialog::doSearch);
}

// ================================================================================
// 函数: initSearchWidget
// 描述: 初始化搜索区域 (输入框 + 搜索按钮)
// ================================================================================
void AddFriendDialog::initSearchWidget()
{
    // 初始化Widget容器
    addFriendSearchWidget = new QWidget(mainFrame);
    addFriendSearchWidget->setFixedHeight(50);
    QHBoxLayout *searchWidgetLayout = new QHBoxLayout(addFriendSearchWidget);
    addFriendSearchWidget->setLayout(searchWidgetLayout);
    searchWidgetLayout->setContentsMargins(0,0,0,0);
    searchWidgetLayout->setSpacing(0);

    // -----------------------------------------------------------
    // 初始化搜索输入框 (LineEditFocus)
    // -----------------------------------------------------------
    searchWidgetLayout->addSpacing(20);
    addFriendSearchEdit = new LineEditFocus(addFriendSearchWidget);
    addFriendSearchEdit->setFixedHeight(35);
    addFriendSearchEdit->setObjectName("addFriendSearchEdit");
    addFriendSearchEdit->setClearButtonEnabled(true);

    // 添加搜索图标 Action
    QAction *searchAction = new QAction(addFriendSearchWidget);
    searchAction->setIcon(QIcon(":/resource/image/search.svg"));

    addFriendSearchEdit->addAction(searchAction, QLineEdit::LeadingPosition);
    addFriendSearchEdit->setPlaceholderText("搜索手机号或者ID");

    // 将搜索LineEdit添加进布局
    searchWidgetLayout->addWidget(addFriendSearchEdit);

    // -----------------------------------------------------------
    // 初始化搜索按钮
    // -----------------------------------------------------------
    addFriendSearchBtn = new QPushButton(addFriendSearchWidget);
    addFriendSearchBtn->setText("搜索");
    addFriendSearchBtn->setObjectName("addFriendSearchBtn");
    searchWidgetLayout->addWidget(addFriendSearchBtn);
    addFriendSearchBtn->setFixedSize(70,40);
    addFriendSearchBtn->setFocusPolicy(Qt::NoFocus);
    searchWidgetLayout->addSpacing(20);

    // -----------------------------------------------------------
    // 将搜索整体Widget加入总布局
    // -----------------------------------------------------------
    mainLayout->addWidget(addFriendSearchWidget);

#if TEST_UI
    // addFriendSearchWidget->setStyleSheet("background-color:red;");
#endif
    // 将按钮设置为默认按钮, 防止回车直接关闭Dialog
    addFriendSearchBtn->setDefault(true);
}

// ================================================================================
// 函数: initStackedWidget
// 描述: 初始化多页展示区域 (控制 空白页/错误页/结果页 的切换)
// ================================================================================
void AddFriendDialog::initStackedWidget()
{
    // 初始化堆叠窗口
    m_stackedWidget = new QStackedWidget(mainFrame);
    mainLayout->addWidget(m_stackedWidget);
    // m_stackedWidget->setContentsMargins(20,20,20,20);

#if TEST_UI
    // QWidget *testWidget = new QWidget(mainFrame);
    // testWidget->setStyleSheet("background-color: red;");
#endif

    // Page 0: 空白页 (默认)
    m_stackedWidget->addWidget(new QWidget());

    // Page 1: 错误/查无此人提示页
    errorPage = new QLabel(m_stackedWidget);
    errorPage->setText("无法找到该用户, 请检查你填写的账号是否正确;");
    errorPage->setAlignment(Qt::AlignCenter);
    errorPage->setStyleSheet("font-size: 13px; color: rgb(158, 158, 158);");
    m_stackedWidget->addWidget(errorPage);

    // Page 2: 搜索结果卡片页
    resultCard = new FriendResultCard(m_stackedWidget);
    m_stackedWidget->addWidget(resultCard);

#if TEST_UI
    // m_stackedWidget->setCurrentIndex(1);
#endif
}

// ================================================================================
// 函数: setSearchInfo
// 描述: 根据传入的用户信息，判断关系并刷新 UI 状态
// ================================================================================
void AddFriendDialog::setSearchInfo(UserInfo *userinfo)
{
    // -----------------------------------------------------------
    // 1. 判空处理 (若为空则显示错误页)
    // -----------------------------------------------------------
    if(!userinfo) {
        m_stackedWidget->setCurrentIndex(1);
#if TEST_UI
        LOG()<<"setSearchInfo 接收到了空指针";
#endif
        return;
    }

    QString myUserId = "";
#if TEST_UI
    // 硬编码myUserId进行测试
    myUserId = "uuid-myself-000001";
#endif

    // -----------------------------------------------------------
    // 2. 关系判断逻辑 (自己/好友/陌生人)
    // -----------------------------------------------------------
    /**
        @todo
        需要接入 DataCenter 获取真实的 LoginUser ID
    */
    QString id = userinfo->userId;
    FriendRelationEnum relation = RELATION_STRANGER;

    if(id == /*查找自己的id*/myUserId){
        relation = RELATION_OWN;
    }
    // 逻辑在这里说明不是自己，继续判断是否为好友
    else if (checkIsFriend(id)){
        relation = RELATION_FRIEND;
    }
    else {
        relation = RELATION_STRANGER;
    }

    // -----------------------------------------------------------
    // 3. 刷新结果卡片并切换视图
    // -----------------------------------------------------------
    resultCard->setInfo(userinfo, relation);
    m_stackedWidget->setCurrentIndex(2);
}

// ================================================================================
// 函数: checkIsFriend
// 描述: 检查目标ID是否已经是好友
// ================================================================================
bool AddFriendDialog::checkIsFriend(const QString &id)
{
    /**
     * @todo
     * 接入 DataCenter::checkIsFriend(id)
     */
#if TEST_UI
    return false;
#endif
}

// ================================================================================
// 函数: doSearch
// 描述: 执行搜索逻辑 (点击按钮或回车触发)
// ================================================================================
void AddFriendDialog::doSearch()
{
    QString searchStr = addFriendSearchEdit->text();
    if(searchStr == "") return ;

    /**
     * @todo
     * 此处应当发起网络请求 NetClient::searchFriend
     * 目前的实现仅为 UI 测试用的 Mock 数据
    */
    UserInfo userinfo;
#if TEST_UI
    userinfo.userId = "123456";
    userinfo.avatar = QIcon(":/resource/image/defaultAvatar.png");
    userinfo.description = "测试签名测试签名测试签名测试签名测试签名测试签名测试签名";
    userinfo.phone = "18677777777";
    userinfo.nickname = "测试用户";
#endif

    // 模拟搜索回调
    setSearchInfo(&userinfo);
}

// ================================================================================
// 函数: setSearchKeyword
// 描述: 对外接口，设置搜索框文本
// ================================================================================
void AddFriendDialog::setSearchKeyword(const QString &key)
{
    addFriendSearchEdit->setText(key);
}


// ################################################################################
// 类: FriendResultCard
// 描述: 搜索结果卡片组件 (包含头像、昵称、操作按钮组)
// ################################################################################

// ================================================================================
// 构造函数
// ================================================================================
FriendResultCard::FriendResultCard(QWidget *parent)
    : QWidget{parent}
{
    initUI();

    initAvatarUI();

    initBtnGroupUI();

    initSignalSlots();
}

// ================================================================================
// 函数: paintEvent
// 描述: 重写绘图事件以支持 QSS 样式表 (针对 QWidget 子类)
// ================================================================================
void FriendResultCard::paintEvent(QPaintEvent *event)
{
    // 1. 初始化样式配置对象
    QStyleOption opt;
    opt.initFrom(this);

    // 2. 创建 Painter
    QPainter p(this);

    // 3. 使用当前的 Style 对象来绘制背景等 QSS 属性
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// ================================================================================
// 函数: setInfo
// 描述: 设置卡片数据并更新 UI
// ================================================================================
void FriendResultCard::setInfo(UserInfo *userInfo, FriendRelationEnum relation)
{
    if(!userInfo){
        return;
    }
    m_userInfo = *userInfo;
    m_relation = relation;

    // 必须调用更新函数以刷新界面
    updateUI();
}

// ================================================================================
// 函数: updateUI
// 描述: 根据当前数据与关系状态刷新界面元素 (头像文本、按钮显隐)
// ================================================================================
void FriendResultCard::updateUI()
{
    // -----------------------------------------------------------
    // 1. 刷新基础信息
    // -----------------------------------------------------------
    m_nickName->setText(m_userInfo.nickname);
    m_id->setText(m_userInfo.userId);
    friendResultCardAvatar->setIcon(m_userInfo.avatar);

    // -----------------------------------------------------------
    // 2. 根据关系状态切换按钮显隐 (状态机)
    // -----------------------------------------------------------
    if(m_relation == RELATION_OWN){ // 自己
        friendResultCardAddBtn->setHidden(true);
        friendResultCardDelBtn->setHidden(true);
        friendResultCardSendBtn->setHidden(true);
    }else if(m_relation == RELATION_FRIEND){ // 好友
        friendResultCardAddBtn->setHidden(true);
        friendResultCardDelBtn->setHidden(false);
        friendResultCardSendBtn->setHidden(false);
    }else{ // 非好友 (陌生人)
        friendResultCardAddBtn->setHidden(false);
        friendResultCardDelBtn->setHidden(true);
        friendResultCardSendBtn->setHidden(true);
    }
}

// ================================================================================
// 函数: initUI
// 描述: 初始化卡片基础容器布局
// ================================================================================
void FriendResultCard::initUI()
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25,10,25,20);
    this->setLayout(mainLayout);
    mainLayout->setAlignment(Qt::AlignTop);

    container = new QWidget(this);
    mainLayout->addWidget(container);
    container->setObjectName("friendResultCardDialog");
    container->setFixedHeight(200);
#if TEST_UI
    // container->setStyleSheet("background-color:red; border:none; border-radius: 10px");
#endif
    containerLayout = new QVBoxLayout(container);
    container->setLayout(containerLayout);
    containerLayout->setAlignment(Qt::AlignTop);
    containerLayout->setSpacing(10);
    containerLayout->setContentsMargins(25,15,25,15);

}

// ================================================================================
// 函数: initAvatarUI
// 描述: 初始化头像、昵称、ID 的布局
// ================================================================================
void FriendResultCard::initAvatarUI()
{
    // -----------------------------------------------------------
    // 1. 创建头像区域 Widget 与 Grid 布局
    // -----------------------------------------------------------
    QWidget* avatarWidget = new QWidget(container);
    avatarWidget->setFixedHeight(65);
    QGridLayout* layout = new QGridLayout(avatarWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    // -----------------------------------------------------------
    // 2. 头像 (Row 0-1, Col 0)
    // -----------------------------------------------------------
    friendResultCardAvatar = new QPushButton(avatarWidget);
    friendResultCardAvatar->setObjectName("friendResultCardAvatar");
    friendResultCardAvatar->setFixedSize(60, 60);
    friendResultCardAvatar->setIconSize(QSize(60, 60));
    layout->addWidget(friendResultCardAvatar, 0, 0, 2, 1);

    // -----------------------------------------------------------
    // 3. 昵称 (Row 0, Col 1)
    // -----------------------------------------------------------
    m_nickName = new QLabel(avatarWidget);
    m_nickName->setFixedHeight(20);
    layout->addWidget(m_nickName, 0, 1, Qt::AlignLeft | Qt::AlignBottom);
    m_nickName->setStyleSheet("font-size: 15px;font-weight: bold;");

    // -----------------------------------------------------------
    // 4. ID (Row 1, Col 1)
    // -----------------------------------------------------------
    m_id = new QLabel(avatarWidget);
    m_id->setFixedHeight(20);
    layout->addWidget(m_id, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    m_id->setStyleSheet("font-size: 12px;color: rgb(158, 158, 158);");

    // 设置列伸缩，让文字列占据剩余空间
    layout->setColumnStretch(1, 1);

    containerLayout->addWidget(avatarWidget);
    addSeparator();

#if TEST_UI
    // friendResultCardAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    // m_nickName->setText("测试用户用户名");
    // m_id->setText("testUserId1111111");
#endif
}

// ================================================================================
// 函数: initBtnGroupUI
// 描述: 初始化按钮组 (添加、删除、发消息)
//       注：采用自定义复合布局实现 "上图下文" 效果
// ================================================================================
void FriendResultCard::initBtnGroupUI()
{
    QWidget *btnsWidget = new QWidget(container);
    btnsWidget->setFixedHeight(80);
    QHBoxLayout *layout = new QHBoxLayout(btnsWidget);
    btnsWidget->setLayout(layout);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(3);

    // --------------------------------------------------------------
    // 1. 添加好友按钮 (Add)
    // --------------------------------------------------------------
    friendResultCardAddBtn = new QPushButton(btnsWidget);
    friendResultCardAddBtn->setObjectName("friendResultCardAddBtn");
    friendResultCardAddBtn->setFocusPolicy(Qt::NoFocus);
    friendResultCardAddBtn->setFixedSize(80, 60);

    // 1.1 设置内部垂直布局
    QVBoxLayout* addBtnLayout = new QVBoxLayout(friendResultCardAddBtn);
    addBtnLayout->setContentsMargins(0,0,0,0);
    addBtnLayout->setSpacing(0);
    friendResultCardAddBtn->setLayout(addBtnLayout);

    // 1.2 上部：图标 (设置为鼠标穿透，防止遮挡外层点击)
    QPushButton *addIcon = new QPushButton(friendResultCardAddBtn);
    addIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    addIcon->setFixedSize(40,40);
    addIcon->setIconSize(QSize(30,30));
    addIcon->setStyleSheet("border: none; background: transparent;");
    addIcon->setIcon(QIcon(":/resource/image/cardAddFriend.svg"));
    addIcon->setFocusPolicy(Qt::NoFocus);
    addBtnLayout->addWidget(addIcon, 0, Qt::AlignHCenter);

    // 1.3 下部：文字 Label
    QLabel *addLabel = new QLabel("添加好友", friendResultCardAddBtn);
    addLabel->setAlignment(Qt::AlignCenter);
    addLabel->setStyleSheet("border: none; background: transparent;color: #576b95;");
    addBtnLayout->addWidget(addLabel, 0, Qt::AlignHCenter);


    // --------------------------------------------------------------
    // 2. 删除好友按钮 (Del)
    // --------------------------------------------------------------
    friendResultCardDelBtn = new QPushButton(btnsWidget);
    friendResultCardDelBtn->setObjectName("friendResultCardDelBtn");
    friendResultCardDelBtn->setFocusPolicy(Qt::NoFocus);
    friendResultCardDelBtn->setFixedSize(80, 60);

    // 2.1 内部垂直布局
    QVBoxLayout* delBtnLayout = new QVBoxLayout(friendResultCardDelBtn);
    delBtnLayout->setContentsMargins(0,0,0,0);
    delBtnLayout->setSpacing(0);
    friendResultCardDelBtn->setLayout(delBtnLayout);

    // 2.2 上部：图标
    QPushButton *delIcon = new QPushButton(friendResultCardDelBtn);
    delIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    delIcon->setFixedSize(40,40);
    delIcon->setIconSize(QSize(30,30));
    delIcon->setStyleSheet("border: none; background: transparent;");
    delIcon->setIcon(QIcon(":/resource/image/cardDelFriend.svg"));
    delIcon->setFocusPolicy(Qt::NoFocus);
    delBtnLayout->addWidget(delIcon, 0, Qt::AlignHCenter);

    // 2.3 下部：文字 Label
    QLabel *delLabel = new QLabel("删除好友", friendResultCardDelBtn);
    delLabel->setAlignment(Qt::AlignCenter);
    delLabel->setStyleSheet("border: none; background: transparent;color: #576b95;");
    delBtnLayout->addWidget(delLabel, 0, Qt::AlignHCenter);


    // --------------------------------------------------------------
    // 3. 发消息按钮 (Send)
    // --------------------------------------------------------------
    friendResultCardSendBtn = new QPushButton(btnsWidget);
    friendResultCardSendBtn->setObjectName("friendResultCardSendBtn");
    friendResultCardSendBtn->setFocusPolicy(Qt::NoFocus);
    friendResultCardSendBtn->setFixedSize(80, 60);

    // 3.1 内部垂直布局
    QVBoxLayout* sendBtnLayout = new QVBoxLayout(friendResultCardSendBtn);
    sendBtnLayout->setContentsMargins(0,0,0,0);
    sendBtnLayout->setSpacing(0);
    friendResultCardSendBtn->setLayout(sendBtnLayout);

    // 3.2 上部：图标
    QPushButton *sendIcon = new QPushButton(friendResultCardSendBtn);
    sendIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    sendIcon->setFixedSize(40,40);
    sendIcon->setIconSize(QSize(30,30));
    sendIcon->setStyleSheet("border: none; background: transparent;");
    sendIcon->setIcon(QIcon(":/resource/image/cardSendFriend.svg"));
    sendIcon->setFocusPolicy(Qt::NoFocus);
    sendBtnLayout->addWidget(sendIcon, 0, Qt::AlignHCenter);

    // 3.3 下部：文字 Label
    QLabel *sendLabel = new QLabel("发消息", friendResultCardSendBtn);
    sendLabel->setAlignment(Qt::AlignCenter);
    sendLabel->setStyleSheet("border: none; background: transparent;color: #576b95;");
    sendBtnLayout->addWidget(sendLabel, 0, Qt::AlignHCenter);


    // --------------------------------------------------------------
    // 将三个按钮加入外层容器布局
    // --------------------------------------------------------------
    layout->addWidget(friendResultCardSendBtn, 0, Qt::AlignLeft);
    layout->addWidget(friendResultCardAddBtn, 0, Qt::AlignHCenter);
    layout->addWidget(friendResultCardDelBtn, 0, Qt::AlignRight);

    containerLayout->addWidget(btnsWidget);

#if TEST_UI
    friendResultCardSendBtn->setHidden(false);
    friendResultCardDelBtn->setHidden(false);
    friendResultCardAddBtn->setHidden(true);
#endif
}


// ================================================================================
// 函数: initSignalSlots
// 描述: 绑定卡片内部按钮的点击事件 (目前仅为日志输出)
// ================================================================================
void FriendResultCard::initSignalSlots()
{
    connect(friendResultCardAddBtn, &QPushButton::clicked, this, [=](){
#if TEST_UI
        LOG()<<"添加好友逻辑";
#endif
    });
    connect(friendResultCardDelBtn, &QPushButton::clicked, this, [=](){
#if TEST_UI
        LOG()<<"删除好友逻辑";
#endif
    });
    connect(friendResultCardSendBtn, &QPushButton::clicked, this, [=](){
#if TEST_UI
        LOG()<<"发送消息逻辑";
#endif
    });
}

// ================================================================================
// 函数: addSeparator
// 描述: 添加一条灰色分割线
// ================================================================================
void FriendResultCard::addSeparator()
{
    QFrame *line = new QFrame(container);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #EAEAEA; border: none; min-height: 1px; max-height: 1px;");
    containerLayout->addWidget(line);
}

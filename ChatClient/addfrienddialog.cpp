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

AddFriendDialog::AddFriendDialog(QWidget *parent)
    :InfoWidget(parent, true)
{
    initMainWidget();

    initTitle();


    initSearchWidget();

    initStackedWidget();

    initSignalSlots();

}

void AddFriendDialog::initMainWidget()
{
    mainLayout = new QVBoxLayout(mainFrame);
    mainFrame->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(10);
    mainLayout->setAlignment(Qt::AlignTop);

    this->setFixedSize(365,490);

    mainFrame->setStyleSheet(
        "#mainFrame {"
        "   background-color: #F7F7F7;"
        "   border-radius: 10px;"
        "}"
        );
}

void AddFriendDialog::initTitle()
{

    // 初始化标题Widget以及关闭按钮
    addFriendTitleWidget = new QWidget(mainFrame);
    addFriendTitleWidget->setFixedHeight(31);
    addFriendTitleWidget->setObjectName("addFriendTitleWidget");

    // 创建布局
    QHBoxLayout *titleLayout = new QHBoxLayout(addFriendTitleWidget);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0,0,0,0);
    addFriendTitleWidget->setLayout(titleLayout);

    titleLayout->addSpacing(50);

    titleLayout->addStretch();
    // 将TitleWidget添加进总布局
    mainLayout->addWidget(addFriendTitleWidget);

    // 创建TitleLabel
    QLabel *addFriendTitle = new QLabel(addFriendTitleWidget);
    addFriendTitleWidget->setFixedHeight(31);
    addFriendTitle->setText("添加朋友");
    titleLayout->addWidget(addFriendTitle, 0, Qt::AlignHCenter);
    titleLayout->addStretch();
    addFriendTitle->setStyleSheet("font-size: 17px; font-family:\"思源黑体\"; font-weight: bold;");

    // 初始化关闭按钮
    closeAddFriendBtn = new QPushButton(mainFrame);
    closeAddFriendBtn->setFixedSize(50,31);
    closeAddFriendBtn->setObjectName("closeAddFriendBtn");
    closeAddFriendBtn->setText(QChar(0x00D7));
    // 将按钮添加进布局
    titleLayout->addWidget(closeAddFriendBtn, 0, Qt::AlignRight);

// 测试
#if TEST_UI
    // addFriendTitleWidget->setStyleSheet("background-color: red;");
#endif

}

void AddFriendDialog::initSignalSlots()
{
    connect(closeAddFriendBtn, &QPushButton::clicked, this, &AddFriendDialog::close);
}

void AddFriendDialog::initSearchWidget()
{
    // 初始化Widget
    addFriendSearchWidget = new QWidget(mainFrame);
    addFriendSearchWidget->setFixedHeight(50);
    QHBoxLayout *searchWidgetLayout = new QHBoxLayout(addFriendSearchWidget);
    addFriendSearchWidget->setLayout(searchWidgetLayout);
    searchWidgetLayout->setContentsMargins(0,0,0,0);
    searchWidgetLayout->setSpacing(0);

    // --------------------------------
    // 搜索框
    // --------------------------------
    // 初始化搜索框LineEdit
    searchWidgetLayout->addSpacing(20);
    addFriendSearchEdit = new LineEditFocus(addFriendSearchWidget);
    addFriendSearchEdit->setFixedHeight(35);
    addFriendSearchEdit->setObjectName("addFriendSearchEdit");
    addFriendSearchEdit->setClearButtonEnabled(true);
    QAction *searchAction = new QAction(addFriendSearchWidget);
    searchAction->setIcon(QIcon(":/resource/image/search.svg"));

    addFriendSearchEdit->addAction(searchAction, QLineEdit::LeadingPosition);
    addFriendSearchEdit->setPlaceholderText("搜索手机号或者ID");
    // 将搜索LineEdit添加进布局
    searchWidgetLayout->addWidget(addFriendSearchEdit);

    // --------------------------------
    // 搜索按钮
    // --------------------------------
    // 初始化搜索按钮Btn
    addFriendSearchBtn = new QPushButton(addFriendSearchWidget);
    addFriendSearchBtn->setText("搜索");
    addFriendSearchBtn->setObjectName("addFriendSearchBtn");
    searchWidgetLayout->addWidget(addFriendSearchBtn);
    addFriendSearchBtn->setFixedSize(70,40);
    addFriendSearchBtn->setFocusPolicy(Qt::NoFocus);
    searchWidgetLayout->addSpacing(20);

    // --------------------------------
    // 搜索栏加入总体布局
    // --------------------------------
    mainLayout->addWidget(addFriendSearchWidget);

#if TEST_UI
    // addFriendSearchWidget->setStyleSheet("background-color:red;");
#endif
}

void AddFriendDialog::initStackedWidget()
{
    // 初始化相框
    m_stackedWidget = new QStackedWidget(mainFrame);
    mainLayout->addWidget(m_stackedWidget);
    // m_stackedWidget->setContentsMargins(20,20,20,20);
#if TEST_UI
    // QWidget *testWidget = new QWidget(mainFrame);
    // testWidget->setStyleSheet("background-color: red;");
#endif
    m_stackedWidget->addWidget(new QWidget());
    errorPage = new QLabel(m_stackedWidget);
    errorPage->setText("无法找到该用户, 请检查你填写的账号是否正确;");
    errorPage->setAlignment(Qt::AlignCenter);
    errorPage->setStyleSheet("font-size: 13px; color: rgb(158, 158, 158);");
    m_stackedWidget->addWidget(errorPage);

    resultCard = new FriendResultCard(m_stackedWidget);
    m_stackedWidget->addWidget(resultCard);
#if TEST_UI
    m_stackedWidget->setCurrentIndex(2);
#endif
}

void AddFriendDialog::setSearchInfo(UserInfo *userinfo)
{

}



FriendResultCard::FriendResultCard(QWidget *parent)
    : QWidget{parent}
{
    initUI();

    initAvatarUI();

    initBtnGroupUI();
}

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

void FriendResultCard::setInfo(UserInfo *userInfo, FriendRelationEnum relation)
{
    if(!userInfo){
        return;
    }
    m_userInfo = *userInfo;
}

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
    container->setFixedHeight(220);
#if TEST_UI
        // container->setStyleSheet("background-color:red; border:none; border-radius: 10px");
#endif
    containerLayout = new QVBoxLayout(container);
    container->setLayout(containerLayout);
    containerLayout->setAlignment(Qt::AlignTop);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(25,15,25,15);

}

void FriendResultCard::initAvatarUI()
{
    // 1. 明确父子关系：layout 属于 avatarWidget
    QWidget* avatarWidget = new QWidget(container);
    avatarWidget->setFixedHeight(65);
    QGridLayout* layout = new QGridLayout(avatarWidget); // 修改点：parent 设为 avatarWidget
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10); // 调整头像与文字的间距

    // 2. 头像：占据第0行第0列，跨2行1列
    friendResultCardAvatar = new QPushButton(avatarWidget);
    friendResultCardAvatar->setObjectName("friendResultCardAvatar");
    friendResultCardAvatar->setFixedSize(60, 60);
    friendResultCardAvatar->setIconSize(QSize(60, 60));
    layout->addWidget(friendResultCardAvatar, 0, 0, 2, 1); // 修改点：跨度改为 2行1列

    // 3. 名字：第0行第1列
    QLabel *friendResultCardNName = new QLabel(avatarWidget);
    friendResultCardNName->setFixedHeight(20);
    // 修改点：位置设为 (0, 1)
    layout->addWidget(friendResultCardNName, 0, 1, Qt::AlignLeft | Qt::AlignBottom);
    friendResultCardNName->setStyleSheet("font-size: 15px;font-weight: bold;");

    // 4. ID：第1行第1列
    QLabel *friendResultCardId = new QLabel(avatarWidget);
    friendResultCardId->setFixedHeight(20);
    // 修改点：位置设为 (1, 1)
    layout->addWidget(friendResultCardId, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    friendResultCardId->setStyleSheet("font-size: 12px;color: rgb(158, 158, 158);");

    // 设置列伸缩，让文字列占据剩余空间，避免间距过大
    layout->setColumnStretch(1, 1);

    containerLayout->addWidget(avatarWidget);
    addSeparator();

#if TEST_UI
    friendResultCardAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    friendResultCardNName->setText("测试用户用户名");
    friendResultCardId->setText("testUserId1111111");
#endif
}

void FriendResultCard::initBtnGroupUI()
{
    QWidget *btnsWidget = new QWidget(container);
    btnsWidget->setFixedHeight(80);
    QHBoxLayout *layout = new QHBoxLayout(btnsWidget);
    btnsWidget->setLayout(layout);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(3);

    // ==============================================================
    // 1. 添加好友按钮 (Add) - 按照你的范例
    // ==============================================================
    friendResultCardAddBtn = new QPushButton(btnsWidget);
    friendResultCardAddBtn->setObjectName("friendResultCardAddBtn");
    friendResultCardAddBtn->setFocusPolicy(Qt::NoFocus);
    friendResultCardAddBtn->setFixedSize(80, 60);
    // friendResultCardAddBtn->setText("添加好友"); // 有了内部Label，外层建议不设Text，防止重影

    // 1.1 设置垂直布局
    QVBoxLayout* addBtnLayout = new QVBoxLayout(friendResultCardAddBtn);
    addBtnLayout->setContentsMargins(0,0,0,0);
    addBtnLayout->setSpacing(0);
    friendResultCardAddBtn->setLayout(addBtnLayout);

    // 1.2 上面：内层 Button 显示图标
    QPushButton *addIcon = new QPushButton(friendResultCardAddBtn);
    addIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    addIcon->setFixedSize(40,40);
    addIcon->setIconSize(QSize(30,30));
    addIcon->setStyleSheet("border: none; background: transparent;"); // 透明背景
    addIcon->setIcon(QIcon(":/resource/image/cardAddFriend.svg")); // 注意路径拼写
    addIcon->setFocusPolicy(Qt::NoFocus);
    addBtnLayout->addWidget(addIcon, 0, Qt::AlignHCenter); // 居中添加

    // 1.3 下面：Label 显示文字
    QLabel *addLabel = new QLabel("添加好友", friendResultCardAddBtn);
    addLabel->setAlignment(Qt::AlignCenter);
    addLabel->setStyleSheet("border: none; background: transparent;color: #576b95;"); // 确保无边框
    addBtnLayout->addWidget(addLabel, 0, Qt::AlignHCenter);


    // ==============================================================
    // 2. 删除好友按钮 (Del) - 同样的套路
    // ==============================================================
    friendResultCardDelBtn = new QPushButton(btnsWidget);
    friendResultCardDelBtn->setObjectName("friendResultCardDelBtn");
    friendResultCardDelBtn->setFocusPolicy(Qt::NoFocus);
    friendResultCardDelBtn->setFixedSize(80, 60);

    // 2.1 垂直布局
    QVBoxLayout* delBtnLayout = new QVBoxLayout(friendResultCardDelBtn);
    delBtnLayout->setContentsMargins(0,0,0,0);
    delBtnLayout->setSpacing(0);
    friendResultCardDelBtn->setLayout(delBtnLayout);

    // 2.2 上面：内层 Button 显示图标
    QPushButton *delIcon = new QPushButton(friendResultCardDelBtn);
    delIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    delIcon->setFixedSize(40,40);
    delIcon->setIconSize(QSize(30,30));
    delIcon->setStyleSheet("border: none; background: transparent;");
    delIcon->setIcon(QIcon(":/resource/image/cardDelFriend.svg"));
    delIcon->setFocusPolicy(Qt::NoFocus);
    delBtnLayout->addWidget(delIcon, 0, Qt::AlignHCenter);

    // 2.3 下面：Label 显示文字
    QLabel *delLabel = new QLabel("删除好友", friendResultCardDelBtn);
    delLabel->setAlignment(Qt::AlignCenter);
    delLabel->setStyleSheet("border: none; background: transparent;color: #576b95;");
    delBtnLayout->addWidget(delLabel, 0, Qt::AlignHCenter);


    // ==============================================================
    // 3. 发消息按钮 (Send) - 同样的套路
    // ==============================================================
    friendResultCardSendBtn = new QPushButton(btnsWidget);
    friendResultCardSendBtn->setObjectName("friendResultCardSendBtn");
    friendResultCardSendBtn->setFocusPolicy(Qt::NoFocus);
    friendResultCardSendBtn->setFixedSize(80, 60);

    // 3.1 垂直布局
    QVBoxLayout* sendBtnLayout = new QVBoxLayout(friendResultCardSendBtn);
    sendBtnLayout->setContentsMargins(0,0,0,0);
    sendBtnLayout->setSpacing(0);
    friendResultCardSendBtn->setLayout(sendBtnLayout);

    // 3.2 上面：内层 Button 显示图标
    QPushButton *sendIcon = new QPushButton(friendResultCardSendBtn);
    sendIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    sendIcon->setFixedSize(40,40);
    sendIcon->setIconSize(QSize(30,30));
    sendIcon->setStyleSheet("border: none; background: transparent;");
    sendIcon->setIcon(QIcon(":/resource/image/cardSendFriend.svg"));
    sendIcon->setFocusPolicy(Qt::NoFocus);
    sendBtnLayout->addWidget(sendIcon, 0, Qt::AlignHCenter);

    // 3.3 下面：Label 显示文字
    QLabel *sendLabel = new QLabel("发消息", friendResultCardSendBtn);
    sendLabel->setAlignment(Qt::AlignCenter);
    sendLabel->setStyleSheet("border: none; background: transparent;color: #576b95;");
    sendBtnLayout->addWidget(sendLabel, 0, Qt::AlignHCenter);


    // ==============================================================
    // 放入外层容器布局
    // ==============================================================
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



void FriendResultCard::initSignalSlots()
{

}

void FriendResultCard::addSeparator()
{
    QFrame *line = new QFrame(container);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #EAEAEA; border: none; min-height: 1px; max-height: 1px;");
    containerLayout->addWidget(line);
}

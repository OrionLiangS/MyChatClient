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

AddFriendDialog::AddFriendDialog(QWidget *parent)
    :InfoWidget(parent, true)
{
    initMainWidget();

    initTitle();


    initSearchWidget();

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

void AddFriendDialog::initCardWidget()
{
    QWidget *cardWidget = new QWidget(mainFrame);
    cardHLayout = new QHBoxLayout(cardWidget);

    cardWidget->setLayout(cardHLayout);
}

FriendResultCard::FriendResultCard(const UserInfo &userInfo, QWidget *parent)
    : QWidget{parent}, userInfo(userInfo)
{
    this->setObjectName("friendResultCard");
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

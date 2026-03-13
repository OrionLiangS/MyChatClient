#include "mainwidget.h"

#include <QIcon>

#include <QFile>

#include <QHBoxLayout>

#include <QVBoxLayout>

#include <QSplitter>

#include "debug.h"

#include "selfinfowidget.h"

#include "sidebar.h"

#include "chatdetailspage.h"

#include "addfrienddialog.h"

/**
 * @brief MainWidget::instance 单例实例
 */
MainWidget* MainWidget::instance = nullptr;


/**
 * @brief MainWidget::getInstance 获取单例实例
 * @details
 * 若是单例不存在则创建一个单例
 * 若是单例存在则获取已有的单例
 * @return
 * 返回一个单例指针
 */
MainWidget *MainWidget::getInstance()
{
    if(instance == nullptr){
        // 不传入参数, 默认为nullptr - 表示以桌面作为父窗口
        instance = new MainWidget();
    }
    return instance;
}

/**
 * @brief MainWidget::MainWidget 构造函数
 * @param parent
 */
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("聊天室");
    // qDebug() << "Icon exists?" << QFile::exists(":/resource/image/logo.png");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));


    // ===========================
    //  初始化窗口
    // ===========================
    initMainWindow();   // 初始化主窗口
    initLeftWindow();   // 初始化左窗口
    initMidWindow();    // 初始化中间窗口
    initRightWindow();  // 初始化右窗口

    // ===========================
    //  初始化信号槽
    // ===========================
    initSignalSlots();
    initSetWindowFocus();

}

/**
 * @brief MainWidget::initMainWindow 初始化全局窗口
 */
void MainWidget::initMainWindow()
{
    // 1. 窗口基础设置
    this->resize(900, 700);
    this->setMinimumSize(627, 570);
    // 隐藏标题栏 - 暂不考虑实现
    // this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    // this->setAttribute(Qt::WA_TranslucentBackground);
    this->setObjectName("MainWidget");
    this->setStyleSheet("MainWidget");



    // A. 创建外层的大布局 (水平)
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); //设置Layout内控件四周的空白
    mainLayout->setSpacing(0); // 设置布局中各个控件紧密布局

    // B. 创建左侧窗口 (独立于 Splitter 之外)
    leftWindow = new QWidget(this);
    leftWindow->setFixedWidth(70); // 彻底固定
    leftWindow->setObjectName("leftWindow");
    leftWindow->setStyleSheet("leftWindow");

    // C. 创建右侧的 Splitter (只用来管理 中间 和 右侧)
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1);
    splitter->setObjectName("mainSplitter");
    splitter->setStyleSheet("mainSplitter::handle"); // 透明把手

    // D. 创建中间和右侧窗口
    midWindow = new QWidget(this);
    midWindow->setObjectName("midWindow");
    rightWindow = new QWidget(this);
    rightWindow->setObjectName("rightWindow");

    // 样式设置
    midWindow->setStyleSheet("midWindow");
    rightWindow->setStyleSheet("rightWindow");

    // ============================================================
    //  组装核心
    // ============================================================

    // 1. 把 中、右 塞进 Splitter
    splitter->addWidget(midWindow);
    splitter->addWidget(rightWindow);

    // 2. 设置 Splitter 行为 (防止中间窗口消失)
    splitter->setCollapsible(0, false); // 中间窗口不能折叠
    splitter->setCollapsible(1, false); // 右侧窗口不能折叠

    midWindow->setMinimumWidth(200);    // 中间最小宽
    midWindow->setMaximumWidth(400);    // 中间最大宽
    rightWindow->setMinimumWidth(200);  // 右侧最小宽

    // 拉伸策略：拖动窗口时，增量全给右侧
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    // 3. 把 左窗口 和 Splitter 塞进主布局
    // 结构： [ Fixed Left ] + [ Splitter (Mid | Right) ]
    mainLayout->addWidget(leftWindow);
    mainLayout->addWidget(splitter);
}

/**
 * @brief MainWidget::initLeftWindow 初始化左窗口
 */
void MainWidget::initLeftWindow()
{
    // 设置布局管理器
    QVBoxLayout *vlayout = new QVBoxLayout(leftWindow);
    leftWindow->setLayout(vlayout);
    vlayout->setSpacing(20);
    vlayout->setContentsMargins(0,30,0,0);

    // 标签大小
    QSize icon_size(26, 26);

    // 头像大小
    QSize default_size(45, 45);

    // ============================
    // 1) 创建头像
    // ============================

    userAvatar = new QPushButton(leftWindow);
    userAvatar->setFixedSize(default_size);
    userAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    userAvatar->setIconSize(default_size);
    // 设置ObjectName
    userAvatar->setObjectName("userAvatar");
    // 设置样式
    userAvatar->setStyleSheet("userAvatar");



    // ============================
    // 2) 创建标签按钮
    // ============================

    // lamdba 表达式 - 创建QPushButton对象
    auto createBtn = [&](const QString& objName, const QString& iconPath = "")->QPushButton*{
        QPushButton* btn = new QPushButton(leftWindow);
        btn->setFixedSize(default_size);
        btn->setIconSize(icon_size);

        btn->setObjectName(objName);
        btn->setIcon(QIcon(iconPath));
        btn->setStyleSheet(objName);
        return btn;
    };


    sessionTabButton = createBtn("sessionTab", ":/resource/image/sessionTabTargeted.png");
    friendTabButton = createBtn("friendTab", ":/resource/image/friendTab.png");
    applyTabButton = createBtn("applyTab", ":/resource/image/applyTab.png");


    // ============================
    // 3) 添加到布局
    // ============================
    vlayout->addWidget(userAvatar, 0, Qt::AlignHCenter);
    vlayout->addWidget(sessionTabButton, 0, Qt::AlignHCenter);
    vlayout->addWidget(friendTabButton, 0, Qt::AlignHCenter);
    vlayout->addWidget(applyTabButton, 0, Qt::AlignHCenter);

    // 添加弹簧
    vlayout->addStretch();

}

/**
 * @brief MainWidget::initMidWindow 初始化中间窗口
 * @details
 * 布局方式为:
 * 1) 外层Widget(默认midWindow)
 * 2) 内层垂直布局管理器(QVBoxLayout)
 * 3) 分上下两个Widget(searchWidget, sessionListWidget)
 * 4) searchWidget固定高度 , sessionListWidget享有剩下的所有高度
 * 5) 设置Icon与QSS
 */
void MainWidget::initMidWindow()
{
    // ============================
    // 1) 创建垂直布局管理
    // ============================
    // 垂直布局管理器创建
    QVBoxLayout* layout = new QVBoxLayout(midWindow);
    midWindow->setLayout(layout);
    // 四周空白为0
    layout->setContentsMargins(0,0,0,0);
    // 控件间隔为0
    layout->setSpacing(0);

    // ============================
    // 2) 实例化searchWidget并设置进layout中
    // ============================
    searchWidget = new QWidget(midWindow);
    searchWidget->setObjectName("searchWidget");
    searchWidget->setFixedHeight(66); // 固定widget高度
    // searchWidget->setAttribute(Qt::WA_StyledBackground); // 强制描绘背景(用于测试)
    layout->addWidget(searchWidget);

    // ========================================================
    // 3) searchWidget中创建水平布局并设置QLineEdit与Button
    // ========================================================
    // 创建横向布局 放置搜索与按钮
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchWidget->setLayout(searchLayout);
    searchLayout->setContentsMargins(20,5,20,5);

    // 实例化搜索框
    searchEdit = new QLineEdit(searchWidget);
    searchEdit->setObjectName("searchEdit");
    searchEdit->setFixedHeight(30);
    searchEdit->setPlaceholderText("此处输入进行搜索....");

    // 实例化搜索按钮
    addFriendBtn = new QPushButton(searchWidget);
    addFriendBtn->setObjectName("addFriendBtn");
    addFriendBtn->setFixedSize(30,30);
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(addFriendBtn);
    addFriendBtn->setIconSize(QSize(20,20));
    addFriendBtn->setIcon(QIcon(":/resource/image/addFriend.png"));


    // ========================================================
    // 4) 创建FriendlistWidget(QScrollArea)
    // ========================================================
    sessionFriendArea = new SessionFriendArea(midWindow);
    // sessionFriendArea->setAttribute(Qt::WA_StyledBackground);  // 强制描绘背景(用于测试)
    layout->addWidget(sessionFriendArea);


}

/**
 * @brief MainWidget::initRightWindow 初始化右侧窗口
 */
void MainWidget::initRightWindow()
{

    // ============================
    // 1) 整体布局
    // ============================
    //

    QVBoxLayout *rightWindowlayout = new QVBoxLayout(rightWindow);
    rightWindowlayout->setSpacing(0);
    rightWindowlayout->setContentsMargins(0,0,0,0);
    rightWindow->setLayout(rightWindowlayout);

    // 标题Widget
    titleWidget = new QWidget(rightWindow);
    titleWidget->setObjectName("titleWidget");
    titleWidget->setFixedHeight(67);

    // 下部分割器
    rightWindowSplitter = new QSplitter(Qt::Vertical, rightWindow);
    rightWindowSplitter->setObjectName("rightWindowSplitter");
    rightWindowSplitter->setHandleWidth(1);



    // 消息展示区
    messageShowArea = new MessageShowArea();

    // 消息编辑区
    messageEditArea = new MessageEditArea();
    // messageEditArea->setObjectName("messageEidtArea");


    // 设置布局
    rightWindowlayout->addWidget(titleWidget);
    rightWindowlayout->addWidget(rightWindowSplitter);

    // 分割器增加内容
    rightWindowSplitter->addWidget(messageShowArea);
    rightWindowSplitter->addWidget(messageEditArea);
    rightWindowSplitter->setCollapsible(1,false); // 防止折叠
    rightWindowSplitter->setCollapsible(0,false); // 防止折叠



    // 设置启动时的默认比例
    rightWindowSplitter->setStretchFactor(0, 5); // MessageShowArea
    rightWindowSplitter->setStretchFactor(1, 5); // MessageEditArea


    // ============================
    // 2) TitleWidget 设置布局
    // ============================
    QHBoxLayout* titleLayout = new QHBoxLayout(titleWidget);
    titleExtraBtn = new QPushButton(titleWidget);
    titleExtraBtn->setObjectName("titleExtraBtn");
    titleWidgetTitle = new QLabel(titleWidget);
    titleWidgetTitle->setObjectName("titleWidgetTitle");

    // 1. 设置按钮固定大小
    titleExtraBtn->setFixedSize(40,40);
    titleExtraBtn->setIconSize(QSize(20,20));
    titleExtraBtn->setIcon(QIcon(":/resource/image/TitleExtraBtn.png"));
    titleExtraBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    // 如果不加这一行，当文字很长时（如 "For Test Long Text..."），Label 依然会因为不想被截断而把按钮挤出去。
    titleWidgetTitle->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    titleWidgetTitle->setMinimumWidth(0);

#if TEST_UI
    titleWidgetTitle->setText("For Test Long Text Long Text Long Text");
    // titleExtraBtn->setText("...");
#endif

    titleWidget->setLayout(titleLayout);
    titleLayout->setContentsMargins(10,0,10,0);

    // Label 权重为 1 (拿走所有剩余空间)
    titleLayout->addWidget(titleWidgetTitle, 1);

    // Button 权重为 0 (只占用固定大小，不参与拉伸)
    titleLayout->addWidget(titleExtraBtn, 0);

    // 创建抽屉并将抽屉设置进去
    sidebar = new Sidebar(rightWindow, titleWidget->height());
    sidebar->raise();

    /**
    * @todo - rebuilding ....
    */
    // 创建内容, 并将内容设置进抽屉
    // 测试数据
    ChatDetailType type;
#if TEST_GROUP_CHAT
    type = CHAT_DETAIL_GROUP;
#elif TEST_PRIVATE_CHAT
    type = CHAT_DETAIL_PRIVATE;
#endif
    chatDetailsPage = ChatDetailsPage::createChatDetailsPage(type);
    sidebar->setContent(chatDetailsPage);
}


/**
 * @brief MainWidget::initSignalSlots
 * @details
 * 初始化信号槽(切换标签页)
 */
void MainWidget::initSignalSlots()
{
    // 绑定信号槽
    connect(sessionTabButton, &QPushButton::clicked, this, &MainWidget::switchTabToSession);
    connect(friendTabButton, &QPushButton::clicked, this, &MainWidget::switchTabToFriend);
    connect(applyTabButton, &QPushButton::clicked, this, &MainWidget::switchTabToApply);

    connect(userAvatar, &QPushButton::clicked, this,[=](){
        SelfInfoWidget* selfInfo = new SelfInfoWidget(this);
        selfInfo->show();
    });

    connect(titleExtraBtn, &QPushButton::clicked, rightWindowSplitter, [=](){
        sidebar->toggle();
#if TEST_UI
        // QWidget *tempWidget = new QWidget(this);
        // tempWidget->setStyleSheet("background-color: red;");
        // sidebar->setContent(tempWidget);
#endif
    });

    connect(addFriendBtn, &QPushButton::clicked, this, [=](){
        AddFriendDialog* addFriendDialog = new AddFriendDialog(this);
        addFriendDialog->exec();
    });

    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWidget::onSearchEditReturnPressed);
}

void MainWidget::onSearchEditReturnPressed()
{
    // 1. 获取用户输入的内容
    QString text = searchEdit->text().trimmed();

    // 如果是空的，可能只是想过滤列表(本地搜索)，或者什么都不做
    if (text.isEmpty()) {
        // 这里可以写清除搜索/恢复列表的逻辑
        return;
    }

    // 2. 核心联动：打开 AddFriendDialog 并自动搜索
    // 假设你有一个成员变量 addFriendDialog 指向那个弹窗
    // 如果没有，可能需要 new 一个，或者用单例，或者从某处获取
    // 这里假设你是在点击 addFriendBtn 时创建的它，建议把它变为 MainWidget 的成员变量以便复用

    AddFriendDialog* addFriendDialog = new AddFriendDialog(this);


    // 3. 设置弹窗里的搜索框内容 (我们需要在 AddFriendDialog 里加个接口，或者直接操作)
    // 比较优雅的方式是在 AddFriendDialog 里加一个 public 函数：setSearchKeyword(const QString& text)
    addFriendDialog->setSearchKeyword(text);

    // 4. 展示弹窗
    addFriendDialog->show();

    // 5. 自动触发搜索 (可选)
    // 如果 setKeyword 里已经包含触发搜索的逻辑，这里就不用写了
    // 或者手动调用 addFriendDialog->doSearch();
    addFriendDialog->doSearch();

    // 6. (可选) 清空主界面的输入框，防止误解
    searchEdit->clear();
}



/**
 * @brief MainWidget::switchTabToSession
 * @details
 * 以下三个switchTab函数相同:
 * 用于切换标签页至会话页
 * 记录当前标签页
 */
void MainWidget::switchTabToSession()
{
    // 1) 记录当前标签页
    activeTab = SESSION_LIST;
    // 2) 设置Icon变化
    sessionTabButton->setIcon(QIcon(":/resource/image/sessionTabTargeted.png"));
    friendTabButton->setIcon(QIcon(":/resource/image/friendTab.png"));
    applyTabButton->setIcon(QIcon(":/resource/image/applyTab.png"));
    // 3) midWindow部分加载会话列表
    loadSessionList();

}

/**
 * @brief MainWidget::switchTabToFriend
 * @details
 * 用于切换标签页至好友页
 */
void MainWidget::switchTabToFriend()
{
    // 1) 记录当前标签页
    activeTab = FRIEND_LIST;
    // 2) 设置Icon变化
    sessionTabButton->setIcon(QIcon(":/resource/image/sessionTab.png"));
    friendTabButton->setIcon(QIcon(":/resource/image/friendTabTargeted.png"));
    applyTabButton->setIcon(QIcon(":/resource/image/applyTab.png"));
    // 3) midWindow部分加载好友列表
    loadFriendList();

}


/**
 * @brief MainWidget::switchTabToApply
 * @details
 * 用于切换标签页至好友申请页
 */
void MainWidget::switchTabToApply()
{
    // 1) 记录当前标签页
    activeTab = APPLY_LIST;
    // 2) 设置Icon变化
    sessionTabButton->setIcon(QIcon(":/resource/image/sessionTab.png"));
    friendTabButton->setIcon(QIcon(":/resource/image/friendTab.png"));
    applyTabButton->setIcon(QIcon(":/resource/image/applyTabTargeted.png"));
    // 3) midWindow部分加载好友申请列表
    loadApplyList();

}




/**
 * @brief MainWidget::loadSessionList
 * @details
 * midWindow部分加载会话列表
 */
void MainWidget::loadSessionList()
{
    /**
     * @todo
     */
    LOG()<<"INFO - loadSessionList()";
}


/**
 * @brief MainWidget::loadFriendList
 * @details
 * midWindow部分加载好友列表
 */
void MainWidget::loadFriendList()
{
    /**
     * @todo
     */
    LOG()<<"INFO - loadFriendList()";

}


/**
 * @brief MainWidget::loadApplyList
 * @details
 * midWindow部分加载好友申请列表
 */
void MainWidget::loadApplyList()
{
    /**
     * @todo
     */
    LOG()<<"INFO - loadApplyList()";

}

void MainWidget::initSetWindowFocus()
{
    leftWindow->setFocusPolicy(Qt::ClickFocus);
    midWindow->setFocusPolicy(Qt::ClickFocus);
    rightWindow->setFocusPolicy(Qt::ClickFocus);
}

MainWidget::~MainWidget() {}

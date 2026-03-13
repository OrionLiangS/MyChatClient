#include "historymessagewidget.h"

#include "debug.h"

using model::MessageType;


// ################################################################################
// 类: HistoryMessageWidget
// 描述: 历史聊天记录主窗口，继承自 InfoWidget（模态模式）。
//       界面从上到下：标题栏 -> 搜索/日期切换区 -> 消息滚动列表。
// ################################################################################


// ================================================================================
// 函数: HistoryMessageWidget (构造函数)
// 描述: 依次初始化各区域，并在 TEST_UI 模式下填充测试数据
// ================================================================================
HistoryMessageWidget::HistoryMessageWidget(QWidget* parent) :InfoWidget(parent, true){
    // 1) 外框初始化
    initHistoryMsgWidget();

    // 2) 初始化标题栏
    initTitle();

    // 3) 初始化搜索栏
    initSearch();

    // 4) 初始化滚动区域
    initScrollarea();

#if TEST_UI
    // 构造测试数据
    Message msg;
    msg.messageType = model::TEXT_TYPE;
    msg.content = "这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息";
    msg.sender.avatar = QIcon(":/resource/image/defaultAvatar.png");
    msg.sender.nickname = "测试用户";
    msg.time = "2023-03-13 22:56:30";

    for(int i = 0;i<30;++i){
        this->addHistoryMsg(msg);
    }
#endif

}


// ================================================================================
// 函数: addHistoryMsg
// 描述: 通过工厂方法构造 HistoryItem，追加到滚动区域布局末尾
// ================================================================================
void HistoryMessageWidget::addHistoryMsg(const Message &msg)
{
    HistoryItem* item = HistoryItem::makeHistoryItem(msg);
    historyMsgScrollWidget->layout()->addWidget(item);
}


// ================================================================================
// 函数: clear
// 描述: 从布局末尾向前遍历，逐个移除并销毁所有 HistoryItem
// ================================================================================
void HistoryMessageWidget::clear()
{
    for(int i = historyMsgScrollLayout->count()-1;i>=0;--i){
        QWidget *w = historyMsgScrollLayout->itemAt(i)->widget();
        if(!w){
            historyMsgScrollLayout->removeWidget(w);
            w->deleteLater();
        }
    }
}


// ================================================================================
// 函数: initHistoryMsgWidget
// 描述: 初始化窗口外框（固定尺寸、背景样式、主布局）
// ================================================================================
void HistoryMessageWidget::initHistoryMsgWidget()
{
    this->setFixedSize(450, 600);
    this->setAttribute(Qt::WA_DeleteOnClose); // 模态对话框需要设置删除逻辑

    // 设置窗口背景颜色和圆角
    mainFrame->setStyleSheet(
        "#mainFrame {"
        "   background-color: #F7F7F7;"
        "   border-radius: 10px;"
        "}"
        );
    mainLayout = new QVBoxLayout();
    mainFrame->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(10);
    mainLayout->setAlignment(Qt::AlignTop);
}


// ================================================================================
// 函数: initTitle
// 描述: 初始化标题栏（居中标题文字 + 右侧关闭按钮）
// ================================================================================
void HistoryMessageWidget::initTitle()
{
    // -------------------------------------------
    // 1. 初始化标题栏容器
    // -------------------------------------------
    historyMsgWidgetTitle = new QWidget(mainFrame);
    historyMsgWidgetTitle->setFixedHeight(31);
    historyMsgWidgetTitle->setObjectName("historyMsgWidgetTitle");
    mainLayout->addWidget(historyMsgWidgetTitle);

    // -------------------------------------------
    // 2. 初始化标题栏布局（左弹簧 + 标题 + 右弹簧 + 关闭按钮）
    // -------------------------------------------
    QHBoxLayout *titleLayout = new QHBoxLayout(mainFrame);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0,0,0,0);
    historyMsgWidgetTitle->setLayout(titleLayout);
    titleLayout->addSpacing(50);
    titleLayout->addStretch();

    // -------------------------------------------
    // 3. 初始化标题文字
    // -------------------------------------------
    QLabel *historyMsgTitleLab = new QLabel(historyMsgWidgetTitle);
    historyMsgTitleLab->setFixedHeight(31);
    historyMsgTitleLab->setText("聊天记录");
    titleLayout->addWidget(historyMsgTitleLab, 0, Qt::AlignHCenter);
    titleLayout->addStretch();
    historyMsgTitleLab->setStyleSheet("font-size: 17px; font-family:\"思源黑体\"; font-weight: bold;");

    // -------------------------------------------
    // 4. 初始化关闭按钮
    // -------------------------------------------
    closeHistoryMsgBtn = new QPushButton(mainFrame);
    closeHistoryMsgBtn->setFixedSize(50,31);
    closeHistoryMsgBtn->setObjectName("closeHistoryMsgBtn");
    closeHistoryMsgBtn->setText(QChar(0x00D7));
    titleLayout->addWidget(closeHistoryMsgBtn, 0, Qt::AlignRight);

    // 关闭窗口功能
    connect(closeHistoryMsgBtn, &QPushButton::clicked, this, &QWidget::close);

#if TEST_UI
    // historyMsgWidgetTitle->setStyleSheet("background-color: red;");
#endif

}


// ================================================================================
// 函数: initSearch
// 描述: 初始化搜索区域（QStackedWidget 双模式切换 + RadioButton 选择器）
// ================================================================================
void HistoryMessageWidget::initSearch()
{
    // -------------------------------------------
    // 1. 初始化搜索区外层容器
    // -------------------------------------------
    searchWidget = new QWidget(mainFrame);
    searchWidget ->setFixedHeight(90);
#if TEST_UI
    // searchWidget->setStyleSheet("background-color: blue;");
#endif

    mainLayout->addWidget(searchWidget);
    QVBoxLayout *searchLayout = new QVBoxLayout(searchWidget);
    searchLayout->setSpacing(5);
    searchLayout->setContentsMargins(10,2,10,5);
    searchWidget->setLayout(searchLayout);
    searchLayout->setAlignment(Qt::AlignTop);

    // -------------------------------------------
    // 2. 初始化 QStackedWidget（搜索/日期模式切换）
    // -------------------------------------------
    searchModeStack = new QStackedWidget(searchWidget);
    searchModeStack->setFixedHeight(50);
    searchLayout->addWidget(searchModeStack, 2);

    // -------------------------------------------
    // 2a. Page 0: 搜索模式
    // -------------------------------------------
    QWidget *searchPage = new QWidget();
    QHBoxLayout *searchPageLayout = new QHBoxLayout(searchPage);
    searchPageLayout->setSpacing(10);
    searchPageLayout->setContentsMargins(0,0,0,0);

    searchEdit = new LineEditFocus(searchPage);
    searchEdit->setObjectName("HistoryMessageSearchEdit");
    searchEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    searchEdit->setPlaceholderText("搜索");
    searchPageLayout->addWidget(searchEdit);

    searchModeStack->addWidget(searchPage); // index 0

    // -------------------------------------------
    // 2b. Page 1: 日期模式
    // -------------------------------------------
    QWidget *datePage = new QWidget();
    QHBoxLayout *datePageLayout = new QHBoxLayout(datePage);
    datePageLayout->setSpacing(10);
    datePageLayout->setContentsMargins(0,0,0,0);

    bgnSel = new QDateEdit(datePage);
    bgnSel->setObjectName("dateBgnSel");
    endSel = new QDateEdit(datePage);
    endSel->setObjectName("dateEndSel");
    dateConfirmBtn = new QPushButton(datePage);
    dateConfirmBtn->setText("确认");
    dateConfirmBtn->setObjectName("historyDateConfirmBtn");

    bgnSel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    endSel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dateConfirmBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    datePageLayout->addWidget(bgnSel, 2);
    datePageLayout->addWidget(endSel, 2);
    datePageLayout->addWidget(dateConfirmBtn, 1);

    searchModeStack->addWidget(datePage); // index 1

    // 默认显示搜索模式
    searchModeStack->setCurrentIndex(0);

    // -------------------------------------------
    // 3. 初始化搜索/日期模式选择器（RadioButton）
    // -------------------------------------------
    QWidget *selBtnWidget = new QWidget(searchWidget);
    searchSelBtn = new QRadioButton(selBtnWidget);
    dateSelBtn = new QRadioButton(selBtnWidget);
    searchSelBtn->setText("搜索查找");
    dateSelBtn->setText("日期查找");

    QHBoxLayout *selLayout = new QHBoxLayout(selBtnWidget);
    selLayout->setSpacing(10);
    selLayout->setContentsMargins(0,0,0,0);
    selBtnWidget->setLayout(selLayout);

    selLayout->addWidget(searchSelBtn);
    selLayout->addWidget(dateSelBtn);

    searchLayout->addWidget(selBtnWidget,1);

#if TEST_UI
    // selBtnWidget->setStyleSheet("background-color: pink;");
#endif

    // 设置默认查找 - 搜索查找为默认
    searchSelBtn->setChecked(true);
    dateSelBtn->setChecked(false);

    // -------------------------------------------
    // 4. 连接信号槽 — 通过 QStackedWidget 切换模式
    // -------------------------------------------
    connect(searchSelBtn, &QRadioButton::toggled, this, [=](bool checked){
        if (checked) searchModeStack->setCurrentIndex(0);
    });

    connect(dateSelBtn, &QRadioButton::toggled, this, [=](bool checked){
        if (checked) searchModeStack->setCurrentIndex(1);
    });
}


// ================================================================================
// 函数: initScrollarea
// 描述: 初始化滚动区域（FloatingScrollArea + 内部容器 + 垂直布局）
// ================================================================================
void HistoryMessageWidget::initScrollarea()
{
    historyMsgScrollArea = new FloatingScrollArea(mainFrame);
    mainLayout->addWidget(historyMsgScrollArea);

#if TEST_UI
    // historyMsgScrollArea->setStyleSheet("background-color: #ededed; ");
#endif

    // -------------------------------------------
    // 创建滚动区域内部容器和布局
    // -------------------------------------------
    historyMsgScrollWidget = new QWidget(historyMsgScrollArea);
    historyMsgScrollArea->setWidget(historyMsgScrollWidget);
    historyMsgScrollArea->setWidgetResizable(true);
    historyMsgScrollLayout = new QVBoxLayout(historyMsgScrollWidget);
    historyMsgScrollWidget->setLayout(historyMsgScrollLayout);
    historyMsgScrollLayout->setSpacing(0);
    historyMsgScrollLayout->setContentsMargins(0,0,0,0);

#if TEST_UI
    // for(int i = 0;i<20;++i){
    //     historyMsgScrollLayout->addWidget(new QPushButton(historyMsgScrollWidget));
    // }
#endif
}


// ################################################################################
// 类: HistoryItem
// 描述: 单条历史消息的容器控件。
//       使用 QGridLayout 布局：
//       Row 0: [头像(col0)] [昵称(col1)] [-----(col2)] [时间(col3)]
//       Row 1: [头像续  ]  [消息内容 -------- 跨 col1~col3 ----------]
//       通过工厂方法 makeHistoryItem() 按消息类型构造内容控件。
// ################################################################################


// ================================================================================
// 函数: HistoryItem (构造函数)
// ================================================================================
HistoryItem::HistoryItem(){}


// ================================================================================
// 函数: makeHistoryItem (静态工厂方法)
// 描述: 根据消息数据构造完整的历史消息条目
// ================================================================================
HistoryItem *HistoryItem::makeHistoryItem(const Message &msg)
{
    // -------------------------------------------
    // 1. 初始化 Item 容器
    // -------------------------------------------
    HistoryItem *item = new HistoryItem();
    item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // -------------------------------------------
    // 2. 设置 GridLayout 布局
    // -------------------------------------------
    QGridLayout *itemLayout = new QGridLayout(item);
    itemLayout->setSpacing(5);
    itemLayout->setContentsMargins(10,10,20,10);
    item->setLayout(itemLayout);
    itemLayout->setAlignment(Qt::AlignTop);

    // -------------------------------------------
    // 3. 创建子控件
    // -------------------------------------------

    // 头像
    QPushButton *historyMsgAvatar = new QPushButton(item);
    historyMsgAvatar->setObjectName("historyMsgAvatar");
    historyMsgAvatar->setFixedSize(40,40);
    historyMsgAvatar->setIconSize(QSize(40,40));
    historyMsgAvatar->setIcon(msg.sender.avatar);
    historyMsgAvatar->setFlat(true);
    historyMsgAvatar->setFocusPolicy(Qt::NoFocus);

    // 昵称和时间
    QLabel *nameLabel = new QLabel(item);
    QLabel *timeLabel = new QLabel(item);
    nameLabel ->setFixedHeight(20);
    timeLabel ->setFixedHeight(20);
    nameLabel->setText(msg.sender.nickname);
    timeLabel ->setText(msg.time);
    nameLabel->setStyleSheet("font-size: 13px;background: transparent; border: none;color:#9e9e9e;");
    timeLabel->setStyleSheet("font-size: 13px;background: transparent; border: none;color:#9e9e9e;");

    // -------------------------------------------
    // 3a. 根据消息类型构造内容控件
    // -------------------------------------------
    QWidget *contentWidget = nullptr;

    if(msg.messageType == model::TEXT_TYPE){// 文本消息
        QLabel *messageText = new QLabel(item);
        messageText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        messageText->setWordWrap(true);
        messageText->setText(QString(msg.content));
        messageText->adjustSize();
        contentWidget = messageText;

    }else if(msg.messageType == model::IMAGE_TYPE){ // 图片消息

    }else if(msg.messageType == model::SPEECH_TYPE){ // 语音消息

    }else if(msg.messageType == model::FILE_TYPE){ // 文件消息

    }else{ // 未知消息
        if(msg.messageType == model::UNKNOWN_TYPE){
            LOG()<<"类型: UNKNOW_TYPE 未知错误";
        }
        else{
            LOG()<<"其他未知错误";
        }
    }

    // -------------------------------------------
    // 4. 添加至 GridLayout
    // -------------------------------------------
    // 布局结构:
    //   (0,0) 头像，跨2行1列    (0,1) 昵称    (0,3) 时间
    //   (1,1) 消息内容，跨1行3列（col1~col3）
    itemLayout->addWidget(historyMsgAvatar,0,0,2,1);
    itemLayout->addWidget(nameLabel, 0,1,1,1);
    itemLayout->addWidget(timeLabel, 0,3,1,1);
    itemLayout->addWidget(contentWidget,1,1,1,3);
    itemLayout->setColumnStretch(1, 1);
    return item;
}

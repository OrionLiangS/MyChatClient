#include "choosefriendwidget.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include "debug.h"
#include <QPushButton>
#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QLabel>
#include "model/data.h"


/**
 * @brief ChooseFriendWidget::ChooseFriendWidget 构造函数
 * @param parent 父窗口指针
 * @details
 * 初始化选择好友窗口，构建左右两个主要容器，并设置布局。
 * 该窗口继承自 InfoWidget，且设置为模态 (true)。
 */
ChooseFriendWidget::ChooseFriendWidget(QWidget* parent)
    :InfoWidget(parent, true)
{
    // ==================
    // 创建最外层布局管理器
    // ==================
    QHBoxLayout *mainLayout = new QHBoxLayout(mainFrame);
    mainLayout->setContentsMargins(0,10,0,10);
    mainLayout->setSpacing(0); // 左右容器紧贴

    // =============================
    // 初始化左右侧container
    // =============================
    initTotalContainer(mainLayout);
    initSelectedContainer(mainLayout);

#if TEST_UI
    // totalContainer->setStyleSheet("background-color: #afd473;");
    // selectedContainer->setStyleSheet("background-color: #aa83f1;");
#endif
    // // =============================
    // // 初始化ObjectName
    // // =============================


    initSignalSlots();
}

/**
 * @brief ChooseFriendWidget::initTotalContainer 初始化左侧布局
 * @param mainLayout 主布局指针
 * @details
 * 构建左侧区域，结构如下：
 * 1. 顶部搜索框 (totalContSearchWidget)
 * 2. 中间好友列表滚动区 (totalScrollArea)
 */
void ChooseFriendWidget::initTotalContainer(QHBoxLayout *mainLayout)
{
    // =============================
    // 1. 初始化容器并立即命名
    // =============================
    totalContainer = new QWidget(mainFrame);

    //  核心修复：出生即命名
    totalContainer->setObjectName("totalContainer");

    mainLayout->addWidget(totalContainer);
    totalContainer->setAttribute(Qt::WA_StyledBackground); // 命名后开启背景绘制，确保变白

    QVBoxLayout *vlayout = new QVBoxLayout(totalContainer);
    totalContainer->setLayout(vlayout);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(10);
    vlayout->setAlignment(Qt::AlignTop);

    // =============================
    // 2. 创建顶端搜索框区域
    // =============================
    QWidget* totalContSearchWidget = new QWidget(totalContainer);
    totalContSearchWidget->setFixedHeight(50);

    QVBoxLayout* searchLayout = new QVBoxLayout(totalContSearchWidget);
    totalContSearchWidget->setLayout(searchLayout);
    searchLayout->setContentsMargins(20,10,20,0);
    searchLayout->setSpacing(0);
    searchLayout->setAlignment(Qt::AlignTop);

    // 初始化搜索框并立即命名
    totalContSearchEdit = new QLineEdit(totalContainer);

    //  归位：搜索框命名
    totalContSearchEdit->setObjectName("totalContSearchEdit");

    totalContSearchEdit->setFixedHeight(35);
    searchLayout->addWidget(totalContSearchEdit);
    totalContSearchEdit->setPlaceholderText("搜索");

    vlayout->addWidget(totalContSearchWidget);

    // =============================
    // 3. 创建滚动区域
    // =============================
    totalScrollArea = new FloatingScrollArea(totalContainer);

    //  归位：滚动区命名
    totalScrollArea->setObjectName("totalScrollArea");

    totalScrollWidget = new QWidget(totalContainer);

    // 归位：滚动内容容器命名
    totalScrollWidget->setObjectName("totalScrollWidget");

    vlayout->addWidget(totalScrollArea);
    totalScrollArea->setWidget(totalScrollWidget);
    totalScrollArea->setWidgetResizable(true);

    // 设置滚动区布局
    totalScrollVlayout = new QVBoxLayout(totalScrollWidget);
    totalScrollWidget->setLayout(totalScrollVlayout);
    totalScrollVlayout->setSpacing(0);
    totalScrollVlayout->setContentsMargins(0,0,0,0);

#if TEST_UI
    for(int i =0;i<30;++i){
        // 使用封装好的添加函数生成测试数据
        addItemFroTotalContainer(QIcon(":/resource/image/defaultAvatar.png"),"测试用户"+QString::number(i),false);
    }
#endif

}


/**
 * @brief ChooseFriendWidget::initSelectedContainer 初始化右侧布局
 * @param mainLayout 主布局指针
 * @details
 * 构建右侧区域，结构如下：
 * 1. 顶部标题栏 (selectedContTitleWidget): 显示"发起群聊"和已选人数
 * 2. 中间已选列表 (selectedScrollArea): 显示已勾选的头像
 * 3. 底部按钮组 (selectedBottomGropWidget): 包含"完成"和"取消"按钮
 */
void ChooseFriendWidget::initSelectedContainer(QHBoxLayout *mainLayout)
{
    // =============================
    // 1. 初始化容器并立即命名
    // =============================
    selectedContainer = new QWidget(mainFrame);

    //  核心修复：必须在这里命名，StyleSheet 才能生效！
    selectedContainer->setObjectName("selectedContainer");

    mainLayout->addWidget(selectedContainer);

    // 命名之后再开启背景绘制
    selectedContainer->setAttribute(Qt::WA_StyledBackground);

    QVBoxLayout *vlayout = new QVBoxLayout(selectedContainer);
    selectedContainer->setLayout(vlayout);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(10);
    vlayout->setAlignment(Qt::AlignTop);

    // =============================
    // 2. 创建标题栏
    // =============================
    QWidget* selectedContTitleWidget = new QWidget(selectedContainer);
    selectedContTitleWidget->setFixedHeight(55);

    QHBoxLayout* titleLayout = new QHBoxLayout(selectedContTitleWidget);
    selectedContTitleWidget->setLayout(titleLayout);
    titleLayout->setContentsMargins(20,10,20,0);
    titleLayout->setSpacing(0);

    selectedContTitleTag = new QLabel(selectedContTitleWidget);
    selectedContNumLabel = new QLabel(selectedContTitleWidget);

    //  归位：Label 命名
    selectedContTitleTag->setObjectName("selectedContTitleTag");
    selectedContNumLabel->setObjectName("selectedContNumLabel");

    selectedContTitleTag->setText("发起群聊");
    selectedContNumLabel->setText("已选择人数(todo)");
    selectedContNumLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    titleLayout->addWidget(selectedContTitleTag, Qt::AlignLeft | Qt::AlignVCenter);
    titleLayout->addWidget(selectedContNumLabel, Qt::AlignRight | Qt::AlignVCenter);

    vlayout->addWidget(selectedContTitleWidget);

    // =============================
    // 3. 创建滚动区域
    // =============================
    selectedScrollArea = new FloatingScrollArea(selectedContainer);
    // 滚动区本身通常是透明的，可以不命名，或者也加上
    // selectedScrollArea->setObjectName("selectedScrollArea");

    selectedScrollWidget = new QWidget(selectedContainer);

    //  归位：右侧滚动内容容器命名 (防止这里也变灰)
    selectedScrollWidget->setObjectName("selectedScrollWidget");

    vlayout->addWidget(selectedScrollArea);
    selectedScrollArea->setWidget(selectedScrollWidget);
    selectedScrollArea->setWidgetResizable(true);

    // =============================
    // 4. 创建底部按钮组
    // =============================
    selectedBottomGropWidget = new QWidget(selectedContainer);

    //  归位：底部容器命名
    selectedBottomGropWidget->setObjectName("selectedBottomGropWidget");

    selectedBottomGropWidget->setFixedHeight(45);
    QHBoxLayout* selectedBottomGropHLayout = new QHBoxLayout(selectedBottomGropWidget);
    selectedBottomGropHLayout->setContentsMargins(10,0,10,5);
    selectedBottomGropHLayout->setSpacing(0);

    selectedFinishBtn = new QPushButton(selectedBottomGropWidget);
    selectedCancelBtn = new QPushButton(selectedBottomGropWidget);

    //  归位：按钮命名
    selectedFinishBtn->setObjectName("selectedFinishBtn");
    selectedCancelBtn->setObjectName("selectedCancelBtn");

    selectedFinishBtn->setFixedSize(110, 35);
    selectedFinishBtn->setText("完成");
    selectedFinishBtn->setFocusPolicy(Qt::NoFocus);

    selectedCancelBtn->setFixedSize(110, 35);
    selectedCancelBtn->setText("取消");
    selectedCancelBtn->setFocusPolicy(Qt::NoFocus);

    selectedBottomGropHLayout->addWidget(selectedFinishBtn, 0,Qt::AlignBottom|Qt::AlignLeft);
    selectedBottomGropHLayout->addStretch();
    selectedBottomGropHLayout->addWidget(selectedCancelBtn,0, Qt::AlignBottom|Qt::AlignRight);

    vlayout->addWidget(selectedBottomGropWidget, Qt::AlignBottom);

}



/**
 * @brief ChooseFriendWidget::addItemFroTotalContainer 添加好友条目
 * @param avatar 头像图标
 * @param name 昵称
 * @param isChecked 初始选中状态
 * @details 辅助函数，用于向左侧好友列表添加一个新的 ChooseFriendItem
 */
void ChooseFriendWidget::addItemFroTotalContainer(const QIcon &avatar, const QString &name, bool isChecked)
{
    ChooseFriendItem *new_item = new ChooseFriendItem(avatar,name,isChecked);
    totalScrollVlayout->addWidget(new_item);
}

void ChooseFriendWidget::initSignalSlots()
{
    connect(selectedCancelBtn, &QPushButton::clicked, this, &ChooseFriendWidget::close);

    connect(selectedFinishBtn, &QPushButton::clicked, this, [=](){
        LOG()<<"执行添加至群聊操作";
    });
}


/**
 * @brief ChooseFriendItem::ChooseFriendItem 构造函数
 * @param avatar
 * @param name
 * @param isChecked
 * @param parent
 * @details
 * 初始化单个好友条目，包含 CheckBox、头像和昵称。
 * 使用 HBoxLayout 进行横向排列。
 */
ChooseFriendItem::ChooseFriendItem(const QIcon &avatar, const QString &name, bool isChecked, QWidget *parent)
{
    this->setObjectName("chooseFriendItem");
    this->setFixedHeight(51);
    chooseFriendAvatar = new QPushButton(this);
    chooseFriendAvatar->setFixedSize(31,31);
    chooseFriendAvatar->setIconSize(QSize(31,31));
    chooseFriendAvatar->setFocusPolicy(Qt::NoFocus);
    chooseFriendNikeName = new QLabel(this);
    chooseFriendCheckBox = new QCheckBox(this);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    this->setLayout(hlayout);

    hlayout->setContentsMargins(10,0,10,0);
    hlayout->setSpacing(10);

    hlayout->addWidget(chooseFriendCheckBox);
    hlayout->addWidget(chooseFriendAvatar);
    hlayout->addWidget(chooseFriendNikeName);



    // ==========================
    // 初始化对象名字
    // ==========================
    initObjectNames();


    // ==========================
    // 设置内容
    // ==========================
    chooseFriendNikeName->setText(name);
    chooseFriendAvatar->setIcon(avatar);
    chooseFriendCheckBox->setChecked(isChecked);

    // ==========================
    // 初始化信号槽
    // ==========================
    initSignalSlot();
#if TEST_UI

#endif
}

/**
 * @brief ChooseFriendItem::clickHandler 点击事件处理
 * @details
 * 当 Item 被点击时触发，反转 CheckBox 的选中状态。
 * 这使得用户点击条目的任意位置都能切换选中。
 */
void ChooseFriendItem::clickHandler()
{
    bool checkedStatus = chooseFriendCheckBox->isChecked();
    chooseFriendCheckBox->setChecked(!checkedStatus);
}

/**
 * @brief ChooseFriendItem::initObjectNames 初始化Item内部控件名
 */
void ChooseFriendItem::initObjectNames()
{
    chooseFriendAvatar->setObjectName("chooseFriendAvatar");

    chooseFriendNikeName->setObjectName("chooseFriendNikeName");

    chooseFriendCheckBox->setObjectName("chooseFriendCheckBox");
}

/**
 * @brief ChooseFriendItem::initSignalSlot 初始化信号槽
 * @details 将自身的 clicked 信号连接到 clickHandler
 */
void ChooseFriendItem::initSignalSlot()
{
    connect(this, &QPushButton::clicked, this, &ChooseFriendItem::clickHandler);
}



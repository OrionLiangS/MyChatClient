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


// ################################################################################
// 类: ChooseFriendWidget
// 描述: 选择好友窗口，继承自 InfoWidget。
//       界面主要包含左侧的好友列表(TotalContainer)和右侧的已选列表(SelectedContainer)。
// ################################################################################


// ================================================================================
// 函数: ChooseFriendWidget (构造函数)
// 描述: 初始化窗口，构建左右布局，并设置模态状态
// ================================================================================
ChooseFriendWidget::ChooseFriendWidget(QWidget* parent)
    :InfoWidget(parent, true)
{
    // -------------------------------------------
    // 1) 创建最外层布局管理器
    // -------------------------------------------
    QHBoxLayout *mainLayout = new QHBoxLayout(mainFrame);
    mainLayout->setContentsMargins(0,10,0,10);
    mainLayout->setSpacing(0); // 左右容器紧贴

    // -------------------------------------------
    // 2) 初始化左右侧容器
    // -------------------------------------------
    initTotalContainer(mainLayout);
    initSelectedContainer(mainLayout);

#if TEST_UI
    // totalContainer->setStyleSheet("background-color: #afd473;");
    // selectedContainer->setStyleSheet("background-color: #aa83f1;");
#endif

    // -------------------------------------------
    // 3) 初始化信号槽
    // -------------------------------------------
    initSignalSlots();
}


// ================================================================================
// 函数: initTotalContainer
// 描述: 初始化左侧布局 (顶部搜索框 + 中间好友列表)
// ================================================================================
void ChooseFriendWidget::initTotalContainer(QHBoxLayout *mainLayout)
{
    // -------------------------------------------
    // 1. 初始化容器并立即命名
    // -------------------------------------------
    totalContainer = new QWidget(mainFrame);

    // 核心修复：出生即命名，确保QSS样式生效
    totalContainer->setObjectName("totalContainer");

    mainLayout->addWidget(totalContainer);
    totalContainer->setAttribute(Qt::WA_StyledBackground); // 命名后开启背景绘制，确保变白

    QVBoxLayout *vlayout = new QVBoxLayout(totalContainer);
    totalContainer->setLayout(vlayout);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(10);
    vlayout->setAlignment(Qt::AlignTop);

    // -------------------------------------------
    // 2. 创建顶端搜索框区域
    // -------------------------------------------
    QWidget* totalContSearchWidget = new QWidget(totalContainer);
    totalContSearchWidget->setFixedHeight(50);

    QVBoxLayout* searchLayout = new QVBoxLayout(totalContSearchWidget);
    totalContSearchWidget->setLayout(searchLayout);
    searchLayout->setContentsMargins(20,10,20,0);
    searchLayout->setSpacing(0);
    searchLayout->setAlignment(Qt::AlignTop);

    // 初始化搜索框并立即命名
    totalContSearchEdit = new QLineEdit(totalContainer);

    // 归位：搜索框命名
    totalContSearchEdit->setObjectName("totalContSearchEdit");

    totalContSearchEdit->setFixedHeight(35);
    searchLayout->addWidget(totalContSearchEdit);
    totalContSearchEdit->setPlaceholderText("搜索");

    vlayout->addWidget(totalContSearchWidget);

    // -------------------------------------------
    // 3. 创建滚动区域 (悬浮滚动条)
    // -------------------------------------------
    totalScrollArea = new FloatingScrollArea(totalContainer);

    // 归位：滚动区命名
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


// ================================================================================
// 函数: initSelectedContainer
// 描述: 初始化右侧布局 (标题栏 + 已选头像列表 + 底部按钮组)
// ================================================================================
void ChooseFriendWidget::initSelectedContainer(QHBoxLayout *mainLayout)
{
    // -------------------------------------------
    // 1. 初始化容器并立即命名
    // -------------------------------------------
    selectedContainer = new QWidget(mainFrame);

    // 核心修复：必须在这里命名，StyleSheet 才能生效！
    selectedContainer->setObjectName("selectedContainer");

    mainLayout->addWidget(selectedContainer);

    // 命名之后再开启背景绘制
    selectedContainer->setAttribute(Qt::WA_StyledBackground);

    QVBoxLayout *vlayout = new QVBoxLayout(selectedContainer);
    selectedContainer->setLayout(vlayout);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(10);
    vlayout->setAlignment(Qt::AlignTop);

    // -------------------------------------------
    // 2. 创建标题栏 ("发起群聊")
    // -------------------------------------------
    QWidget* selectedContTitleWidget = new QWidget(selectedContainer);
    selectedContTitleWidget->setFixedHeight(55);

    QHBoxLayout* titleLayout = new QHBoxLayout(selectedContTitleWidget);
    selectedContTitleWidget->setLayout(titleLayout);
    titleLayout->setContentsMargins(20,10,20,0);
    titleLayout->setSpacing(0);

    selectedContTitleTag = new QLabel(selectedContTitleWidget);
    selectedContNumLabel = new QLabel(selectedContTitleWidget);

    // 归位：Label 命名
    selectedContTitleTag->setObjectName("selectedContTitleTag");
    selectedContNumLabel->setObjectName("selectedContNumLabel");

    selectedContTitleTag->setText("发起群聊");
    selectedContNumLabel->setText("已选择人数(todo)");
    selectedContNumLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    titleLayout->addWidget(selectedContTitleTag, Qt::AlignLeft | Qt::AlignVCenter);
    titleLayout->addWidget(selectedContNumLabel, Qt::AlignRight | Qt::AlignVCenter);

    vlayout->addWidget(selectedContTitleWidget);

    // -------------------------------------------
    // 3. 创建滚动区域 (已选列表)
    // -------------------------------------------
    selectedScrollArea = new FloatingScrollArea(selectedContainer);
    // 滚动区本身通常是透明的，可以不命名，或者也加上
    // selectedScrollArea->setObjectName("selectedScrollArea");

    selectedScrollWidget = new QWidget(selectedContainer);

    // 归位：右侧滚动内容容器命名 (防止这里也变灰)
    selectedScrollWidget->setObjectName("selectedScrollWidget");

    vlayout->addWidget(selectedScrollArea);
    selectedScrollArea->setWidget(selectedScrollWidget);
    selectedScrollArea->setWidgetResizable(true);

    // -------------------------------------------
    // 4. 创建底部按钮组 ("完成"/"取消")
    // -------------------------------------------
    selectedBottomGropWidget = new QWidget(selectedContainer);

    // 归位：底部容器命名
    selectedBottomGropWidget->setObjectName("selectedBottomGropWidget");

    selectedBottomGropWidget->setFixedHeight(45);
    QHBoxLayout* selectedBottomGropHLayout = new QHBoxLayout(selectedBottomGropWidget);
    selectedBottomGropHLayout->setContentsMargins(10,0,10,5);
    selectedBottomGropHLayout->setSpacing(0);

    selectedFinishBtn = new QPushButton(selectedBottomGropWidget);
    selectedCancelBtn = new QPushButton(selectedBottomGropWidget);

    // 归位：按钮命名
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


    selectedScrollVlayout = new QVBoxLayout(selectedScrollWidget);
    selectedScrollVlayout->setAlignment(Qt::AlignTop);

#if TEST_UI
    for(int i =0;i<20;++i){
        addItemForSelectedContainer(QIcon(":/resource/image/defaultAvatar.png"),"测试用户"+QString::number(i));
    }
#endif
}


// ================================================================================
// 函数: addItemFroTotalContainer
// 描述: 辅助函数，向左侧好友列表添加一个新的条目(ChooseFriendItem)
// ================================================================================
void ChooseFriendWidget::addItemFroTotalContainer(const QIcon &avatar, const QString &name, bool isChecked)
{
    ChooseFriendItem *new_item = new ChooseFriendItem(avatar,name,isChecked);
    totalScrollVlayout->addWidget(new_item);
}


// ================================================================================
// 函数: addItemForSelectedContainer
// 描述: 辅助函数，向右侧已选列表添加一个新的条目(SelectedFriendItem)
// ================================================================================
void ChooseFriendWidget::addItemForSelectedContainer(const QIcon &avatar, const QString &name)
{
    SelectedFriendItem *new_item = new SelectedFriendItem(avatar,name, selectedScrollWidget);
    selectedScrollVlayout->addWidget(new_item);
}


// ================================================================================
// 函数: initSignalSlots
// 描述: 初始化 ChooseFriendWidget 的信号槽连接
// ================================================================================
void ChooseFriendWidget::initSignalSlots()
{
    connect(selectedCancelBtn, &QPushButton::clicked, this, &ChooseFriendWidget::close);

    connect(selectedFinishBtn, &QPushButton::clicked, this, [=](){
        LOG()<<"执行添加至群聊操作";
    });
}


// ################################################################################
// 类: ChooseFriendItem
// 描述: 左侧好友列表中的单个条目 (复选框 + 头像 + 名字)
// ################################################################################


// ================================================================================
// 函数: ChooseFriendItem (构造函数)
// 描述: 初始化条目UI，设置内容和样式
// ================================================================================
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

    ChooseFriendItemHLayout = new QHBoxLayout(this);
    ChooseFriendItemHLayout->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    this->setLayout(ChooseFriendItemHLayout);

    ChooseFriendItemHLayout->setContentsMargins(10,0,10,0);
    ChooseFriendItemHLayout->setSpacing(10);

    ChooseFriendItemHLayout->addWidget(chooseFriendCheckBox);
    ChooseFriendItemHLayout->addWidget(chooseFriendAvatar);
    ChooseFriendItemHLayout->addWidget(chooseFriendNikeName);

    // -------------------------------------------
    // 初始化对象名字 (用于QSS)
    // -------------------------------------------
    initObjectNames();

    // -------------------------------------------
    // 设置内容
    // -------------------------------------------
    chooseFriendNikeName->setText(name);
    chooseFriendAvatar->setIcon(avatar);
    chooseFriendCheckBox->setChecked(isChecked);

    // -------------------------------------------
    // 初始化信号槽
    // -------------------------------------------
    initSignalSlot();

#if TEST_UI
#endif
}


// ================================================================================
// 函数: clickHandler
// 描述: 响应条目点击，反转复选框的选中状态
// ================================================================================
void ChooseFriendItem::clickHandler()
{
    bool checkedStatus = chooseFriendCheckBox->isChecked();
    chooseFriendCheckBox->setChecked(!checkedStatus);
}


// ================================================================================
// 函数: initObjectNames
// 描述: 设置子控件的对象名，便于QSS样式匹配
// ================================================================================
void ChooseFriendItem::initObjectNames()
{
    chooseFriendAvatar->setObjectName("chooseFriendAvatar");
    chooseFriendNikeName->setObjectName("chooseFriendNikeName");
    chooseFriendCheckBox->setObjectName("chooseFriendCheckBox");
}


// ================================================================================
// 函数: initSignalSlot
// 描述: 绑定自身点击信号
// ================================================================================
void ChooseFriendItem::initSignalSlot()
{
    connect(this, &QPushButton::clicked, this, &ChooseFriendItem::clickHandler);
}


// ################################################################################
// 类: SelectedFriendItem
// 描述: 右侧已选列表中的单个条目 (继承自 ChooseFriendItem，隐藏复选框，增加删除按钮)
// ################################################################################


// ================================================================================
// 函数: SelectedFriendItem (构造函数)
// ================================================================================
SelectedFriendItem::SelectedFriendItem(const QIcon &avatar, const QString &name, QWidget *parent)
    :ChooseFriendItem(avatar, name, false, parent)
{
    setFixedHeight(35);

    this->setObjectName("selectedFriendItem");

    // -------------------------------------------
    // 初始化删除按钮
    // -------------------------------------------
    SelectedFriendItemDelBtn = new QPushButton(this);
    SelectedFriendItemDelBtn->setObjectName("SelectedFriendItemDelBtn");
    SelectedFriendItemDelBtn->setFixedSize(15,15);
    SelectedFriendItemDelBtn->setFocusPolicy(Qt::NoFocus);

    // 隐藏复选框 (左侧才需要)
    chooseFriendCheckBox->setHidden(true);

    SelectedFriendItemDelBtn->setIconSize(QSize(12,12));
    SelectedFriendItemDelBtn->setIcon(QIcon(":/resource/image/selectedFriendDelete.png"));

    // 在右侧添加弹簧和删除按钮
    ChooseFriendItemHLayout->addStretch();
    ChooseFriendItemHLayout->addWidget(SelectedFriendItemDelBtn, 0,Qt::AlignRight);

    initSignalSlots();
}


// ================================================================================
// 函数: initSignalSlots
// 描述: 绑定删除按钮的逻辑
// ================================================================================
void SelectedFriendItem::initSignalSlots()
{
    connect(SelectedFriendItemDelBtn, &QPushButton::clicked, this, [=](){
        LOG()<<"接入删除当前Item逻辑";
    });
}

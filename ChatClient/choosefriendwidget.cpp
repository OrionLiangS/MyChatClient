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
//       核心联动逻辑: 通过 m_selectedMap 映射和信号槽，实现左右列表的同步增删。
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
    totalContainer->setObjectName("totalContainer");

    mainLayout->addWidget(totalContainer);
    totalContainer->setAttribute(Qt::WA_StyledBackground);

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

    totalContSearchEdit = new QLineEdit(totalContainer);
    totalContSearchEdit->setObjectName("totalContSearchEdit");
    totalContSearchEdit->setFixedHeight(35);
    searchLayout->addWidget(totalContSearchEdit);
    totalContSearchEdit->setPlaceholderText("搜索");

    vlayout->addWidget(totalContSearchWidget);

    // -------------------------------------------
    // 3. 创建滚动区域
    // -------------------------------------------
    totalScrollArea = new FloatingScrollArea(totalContainer);
    totalScrollArea->setObjectName("totalScrollArea");

    totalScrollWidget = new QWidget(totalContainer);
    totalScrollWidget->setObjectName("totalScrollWidget");

    vlayout->addWidget(totalScrollArea);
    totalScrollArea->setWidget(totalScrollWidget);
    totalScrollArea->setWidgetResizable(true);

    totalScrollVlayout = new QVBoxLayout(totalScrollWidget);
    totalScrollWidget->setLayout(totalScrollVlayout);
    totalScrollVlayout->setSpacing(0);
    totalScrollVlayout->setContentsMargins(0,0,0,0);

#if TEST_UI
    for(int i =0;i<30;++i){
        // 生成测试数据：ID使用 "user_i" 格式
        addItemFroTotalContainer(QString("user_%1").arg(i), QIcon(":/resource/image/defaultAvatar.png"),"测试用户"+QString::number(i));
    }
#endif
}


// ================================================================================
// 函数: initSelectedContainer
// 描述: 初始化右侧布局 (标题栏 + 已选列表 + 底部按钮)
// ================================================================================
void ChooseFriendWidget::initSelectedContainer(QHBoxLayout *mainLayout)
{
    // -------------------------------------------
    // 1. 初始化容器并立即命名
    // -------------------------------------------
    selectedContainer = new QWidget(mainFrame);
    selectedContainer->setObjectName("selectedContainer");

    mainLayout->addWidget(selectedContainer);
    selectedContainer->setAttribute(Qt::WA_StyledBackground);

    QVBoxLayout *vlayout = new QVBoxLayout(selectedContainer);
    selectedContainer->setLayout(vlayout);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(10);
    vlayout->setAlignment(Qt::AlignTop);

    // -------------------------------------------
    // 2. 创建标题栏
    // -------------------------------------------
    QWidget* selectedContTitleWidget = new QWidget(selectedContainer);
    selectedContTitleWidget->setFixedHeight(55);

    QHBoxLayout* titleLayout = new QHBoxLayout(selectedContTitleWidget);
    selectedContTitleWidget->setLayout(titleLayout);
    titleLayout->setContentsMargins(20,10,20,0);
    titleLayout->setSpacing(0);

    selectedContTitleTag = new QLabel(selectedContTitleWidget);
    selectedContNumLabel = new QLabel(selectedContTitleWidget);
    selectedContTitleTag->setObjectName("selectedContTitleTag");
    selectedContNumLabel->setObjectName("selectedContNumLabel");

    selectedContTitleTag->setText("发起群聊");
    selectedContNumLabel->setText("已选择 0 人");
    selectedContNumLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    titleLayout->addWidget(selectedContTitleTag, Qt::AlignLeft | Qt::AlignVCenter);
    titleLayout->addWidget(selectedContNumLabel, Qt::AlignRight | Qt::AlignVCenter);

    vlayout->addWidget(selectedContTitleWidget);

    // -------------------------------------------
    // 3. 创建滚动区域
    // -------------------------------------------
    selectedScrollArea = new FloatingScrollArea(selectedContainer);
    selectedScrollWidget = new QWidget(selectedContainer);
    selectedScrollWidget->setObjectName("selectedScrollWidget");

    vlayout->addWidget(selectedScrollArea);
    selectedScrollArea->setWidget(selectedScrollWidget);
    selectedScrollArea->setWidgetResizable(true);

    selectedScrollVlayout = new QVBoxLayout(selectedScrollWidget);
    selectedScrollVlayout->setAlignment(Qt::AlignTop);
    selectedScrollVlayout->setContentsMargins(0,0,0,0);
    selectedScrollVlayout->setSpacing(10);

    // 不允许挤压子控件
    selectedScrollVlayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    // -------------------------------------------
    // 4. 创建底部按钮组
    // -------------------------------------------
    selectedBottomGropWidget = new QWidget(selectedContainer);
    selectedBottomGropWidget->setObjectName("selectedBottomGropWidget");
    selectedBottomGropWidget->setFixedHeight(45);

    QHBoxLayout* selectedBottomGropHLayout = new QHBoxLayout(selectedBottomGropWidget);
    selectedBottomGropHLayout->setContentsMargins(10,0,10,5);
    selectedBottomGropHLayout->setSpacing(0);

    selectedFinishBtn = new QPushButton(selectedBottomGropWidget);
    selectedCancelBtn = new QPushButton(selectedBottomGropWidget);
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


// ================================================================================
// 函数: addItemFroTotalContainer
// 描述: 向左侧列表添加好友条目，并连接信号
// ================================================================================
void ChooseFriendWidget::addItemFroTotalContainer(const QString& userId, const QIcon &avatar, const QString &name)
{
    // 构造时传入 false (默认不选中)
    ChooseFriendItem *new_item = new ChooseFriendItem(userId, avatar, name, false);

    // 连接信号：左侧 Item 状态改变 -> 通知 Widget 处理
    connect(new_item, &ChooseFriendItem::signalStatusChanged, this, &ChooseFriendWidget::onItemStatusChanged);

    totalScrollVlayout->addWidget(new_item);
}


// ================================================================================
// 函数: onItemStatusChanged
// 描述: [核心槽函数] 处理左侧状态变化，同步增删右侧 Item
// ================================================================================
void ChooseFriendWidget::onItemStatusChanged(ChooseFriendItem *item)
{
    // 1. 获取当前 Item 的选中状态
    bool isChecked = item->isChecked();

    if (isChecked) {
        // === 增加：左侧勾选 -> 右侧添加 ===

        // 防抖：避免重复添加
        if (m_selectedMap.contains(item)) return;

        // 构造右侧 Item (传入左侧指针 item 作为数据源和关联句柄)
        // 这里会调用 item->getIcon()，所以必须保证 item 已经初始化完毕
        SelectedFriendItem *selectedItem = new SelectedFriendItem(item, selectedScrollWidget);
        selectedScrollVlayout->addWidget(selectedItem);

        // 记入账本
        m_selectedMap.insert(item, selectedItem);
    }
    else {
        // === 删除：左侧取消 -> 右侧移除 ===

        if (m_selectedMap.contains(item)) {
            // 从 Map 移除并取出指针
            SelectedFriendItem *selectedItem = m_selectedMap.take(item);
            if (selectedItem) {
                // 销毁 UI 对象
                selectedItem->deleteLater();
            }
        }
    }

    // 更新顶部计数标签
    selectedContNumLabel->setText(QString("已选择 %1 人").arg(m_selectedMap.size()));
}


// ================================================================================
// 函数: getSelectedUserIds
// 描述: 获取当前所有选中用户的 ID 列表
// ================================================================================
QList<QString> ChooseFriendWidget::getSelectedUserIds() const
{
    QList<QString> ids;
    // 遍历 Map 的 Key (左侧 Item 指针)，获取其 ID
    for (auto item : m_selectedMap.keys()) {
        if (item) {
            ids.append(item->getUserId());
        }
    }
    return ids;
}


// ================================================================================
// 函数: initSignalSlots
// 描述: 初始化窗口级信号槽 (底部按钮)
// ================================================================================
void ChooseFriendWidget::initSignalSlots()
{
    connect(selectedCancelBtn, &QPushButton::clicked, this, &ChooseFriendWidget::close);

    connect(selectedFinishBtn, &QPushButton::clicked, this, [=](){
        QList<QString> ids = getSelectedUserIds();
        LOG() << "点击完成，选中的用户ID列表: " << ids;
        signalFinished(ids); // 发送信号并将ids进行传出
        this->close();
    });
}


// ################################################################################
// 类: ChooseFriendItem
// 描述: 左侧好友列表中的单个条目
// ################################################################################


// ================================================================================
// 函数: ChooseFriendItem (构造函数)
// ================================================================================
ChooseFriendItem::ChooseFriendItem(const QString& userId, const QIcon &avatar, const QString &name, bool isChecked, QWidget *parent)
    : m_userId(userId) // 保存 ID
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

    // 初始化控件名
    initObjectNames();

    // 设置内容
    chooseFriendNikeName->setText(name);
    chooseFriendAvatar->setIcon(avatar);

    // 设置初始状态 (注意：这会触发 toggled 信号，但此时外部可能还没 connect，所以是安全的)
    chooseFriendCheckBox->setChecked(isChecked);

    // 初始化信号槽
    initSignalSlot();
}


// ================================================================================
// 函数: clickHandler
// ================================================================================
void ChooseFriendItem::clickHandler()
{
    bool checkedStatus = chooseFriendCheckBox->isChecked();
    chooseFriendCheckBox->setChecked(!checkedStatus);
}


// ================================================================================
// 函数: initObjectNames
// ================================================================================
void ChooseFriendItem::initObjectNames()
{
    chooseFriendAvatar->setObjectName("chooseFriendAvatar");
    chooseFriendNikeName->setObjectName("chooseFriendNikeName");
    chooseFriendCheckBox->setObjectName("chooseFriendCheckBox");
}


// ================================================================================
// 函数: initSignalSlot
// 描述: 绑定自身信号
// ================================================================================
void ChooseFriendItem::initSignalSlot()
{
    // 1. 点击 Item -> 切换 CheckBox
    connect(this, &QPushButton::clicked, this, &ChooseFriendItem::clickHandler);

    // 2. 监听 CheckBox 变化 -> 发送信号 (带 this 指针)
    connect(chooseFriendCheckBox, &QCheckBox::toggled, this, [=](bool checked){
        emit signalStatusChanged(this);
    });
}


// ================================================================================
// 函数: 公开接口实现 (Getter / Setter)
// ================================================================================
QIcon ChooseFriendItem::getIcon() const {
    return chooseFriendAvatar->icon();
}

QString ChooseFriendItem::getName() const {
    return chooseFriendNikeName->text();
}

QString ChooseFriendItem::getUserId() const {
    return m_userId;
}

bool ChooseFriendItem::isChecked() const {
    return chooseFriendCheckBox->isChecked();
}

// 供右侧调用：修改状态
void ChooseFriendItem::setChecked(bool checked) {
    chooseFriendCheckBox->setChecked(checked);
}


// ################################################################################
// 类: SelectedFriendItem
// 描述: 右侧已选列表中的单个条目
// ################################################################################


// ================================================================================
// 函数: SelectedFriendItem (构造函数)
// 描述: 接收左侧指针，复用数据并建立关联
// ================================================================================
SelectedFriendItem::SelectedFriendItem(ChooseFriendItem* item, QWidget *parent)
    :ChooseFriendItem(item->getUserId(), item->getIcon(), item->getName(), false, parent), // 复用左侧数据
    m_refItem(item) // 保存关联
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

    // 隐藏 CheckBox (右侧不需要)
    chooseFriendCheckBox->setHidden(true);

    SelectedFriendItemDelBtn->setIconSize(QSize(12,12));
    SelectedFriendItemDelBtn->setIcon(QIcon(":/resource/image/selectedFriendDelete.png"));

    ChooseFriendItemHLayout->addStretch();
    ChooseFriendItemHLayout->addWidget(SelectedFriendItemDelBtn, 0,Qt::AlignRight);

    // 【关键修复】
    // 使用 &SelectedFriendItem::clickHandler 而不是父类名。
    // 这解决了 C2248 protected 成员访问错误。
    disconnect(this, &QPushButton::clicked, this, &SelectedFriendItem::clickHandler);

    initSignalSlots();
}


// ================================================================================
// 函数: initSignalSlots
// 描述: 绑定删除按钮逻辑
// ================================================================================
void SelectedFriendItem::initSignalSlots()
{
    connect(SelectedFriendItemDelBtn, &QPushButton::clicked, this, [=](){
        // 核心联动：点击右侧删除 -> 遥控左侧取消勾选
        if (m_refItem) {
            // 这会触发:
            // 1. 左侧 CheckBox 变为 false
            // 2. 左侧 emit signalStatusChanged(this)
            // 3. Widget::onItemStatusChanged(item)
            // 4. Widget 查表删除当前的 SelectedFriendItem (即 this)
            m_refItem->setChecked(false);
        }
    });
}

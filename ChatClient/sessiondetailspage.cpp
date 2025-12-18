#include "sessiondetailspage.h"

#include <QVBoxLayout>

#include <QFont>

#include <QFontMetrics>

#include <QStyle>

#include "debug.h"

#include <QStyleOption>

#include <QPainter>

#include <QStyle>

// #########################################
// 会话容器 - 需要塞进抽屉中
// #########################################

SessionDetailsPage::SessionDetailsPage(QWidget *parent)
    : QWidget{parent}
{
    // 创建布局管理器
    this->setObjectName("SessionDetailsPage");

    // 1) 设置网格布局
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(10, 10, 10, 10); // 加上边距好看点
    gridLayout->setSpacing(5);

    // 2) 创建“添加”按钮 (局部变量)
    AvatarItem* addBtn = new AvatarItem(this, "添加");
    AvatarItem* rmBtn = new AvatarItem(this, "移除");
    // 3) 改名 (让它变样式)
    addBtn->setAvatarObjectName("detailsPageAddFriendBtn");
    rmBtn->setAvatarObjectName("detailsPageRemoveFriendBtn");

    // 4) 连信号 (分流业务)
    connect(addBtn, &AvatarItem::clicked, this, &SessionDetailsPage::signalAddFriendClicked);
    connect(rmBtn, &AvatarItem::clicked, this, &SessionDetailsPage::signalRemoveFriendClicked);

    // 5) 上架 (放入布局)
    gridLayout->addWidget(addBtn, 0, 0);
    gridLayout->addWidget(rmBtn, 0, 1);

// =======================
// 测试数据
// =======================

#if TEST_UI
    AvatarItem*testUser = new AvatarItem(this, "测试用户", QIcon(":/resource/image/defaultAvatar.png"));
    gridLayout->addWidget(testUser, 0, 2);
#endif


}

void SessionDetailsPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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

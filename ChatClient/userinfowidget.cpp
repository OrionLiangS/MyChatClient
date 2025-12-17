#include "userinfowidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include "debug.h"

UserInfoWidget::UserInfoWidget(const UserInfo &userinfo, QWidget* parent) : InfoWidget(parent), userinfo(userinfo)
{
    // ======================
    // 初始化大小
    // ======================
    this->setFixedSize(320, 230);

    // ======================
    // 初始化控件
    // ======================
    userInfoAvatar = new QPushButton(mainFrame);
    userInfoAvatar->setFixedSize(65, 65);
    userInfoAvatar->setIconSize(QSize(65, 65));
    userInfoAvatar->setFlat(true);

    userInfoApplyFriendBtn = new QPushButton(mainFrame);
    userInfoSendMessageBtn = new QPushButton(mainFrame);
    userInfoDeleteFriendBtn = new QPushButton(mainFrame);

    userInfoApplyFriendBtn->setText("申请好友");
    userInfoSendMessageBtn->setText("发送信息");
    userInfoDeleteFriendBtn->setText("删除好友");

    userInfoId = new QLabel(mainFrame);
    userInfoTel = new QLabel(mainFrame);
    userInfoNikeName = new QLabel(mainFrame);

    // 1. 初始化 Tag 标签并设置文字
    userInfoIdTag = new QLabel(mainFrame);
    userInfoTelTag = new QLabel(mainFrame);
    userInfoIdTag->setText("ID: ");
    userInfoTelTag->setText("手机号: ");


    // ======================
    // 主布局
    // ======================
    QVBoxLayout *mainUinfoLayout = new QVBoxLayout(mainFrame);
    mainUinfoLayout->setContentsMargins(20, 20, 20, 20);
    mainUinfoLayout->setSpacing(10);

    // ======================
    // Top布局 (头像 + 右侧信息)
    // ======================
    QWidget *topUinfoWidget = new QWidget(mainFrame);
    QHBoxLayout *topUinfoLayout = new QHBoxLayout(topUinfoWidget);
    topUinfoLayout->setContentsMargins(0, 0, 0, 0);
    topUinfoLayout->setSpacing(15);
    topUinfoWidget->setFixedHeight(80);

    // 1. 左侧头像
    topUinfoLayout->addWidget(userInfoAvatar, 0, Qt::AlignTop);

    // 2. 右侧信息容器 (使用 Grid 布局)
    QWidget *topUinfoRightWidget = new QWidget(topUinfoWidget);
    QGridLayout *topUinfoRightLayout = new QGridLayout(topUinfoRightWidget); // 改用 QGridLayout
    topUinfoRightLayout->setContentsMargins(0, 0, 0, 0);
    topUinfoRightLayout->setVerticalSpacing(5);   // 行间距
    topUinfoRightLayout->setHorizontalSpacing(5); // Tag和内容之间的间距

    // 3. 添加文字到网格中
    //     // Row 0: 昵称 (占用 2 列)
    topUinfoRightLayout->addWidget(userInfoNikeName, 0, 0, 1, 2, Qt::AlignLeft | Qt::AlignVCenter);

    // Row 1: ID Tag + ID 内容
    topUinfoRightLayout->addWidget(userInfoIdTag, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
    topUinfoRightLayout->addWidget(userInfoId,    1, 1, Qt::AlignLeft | Qt::AlignVCenter);

    // Row 2: Tel Tag + Tel 内容
    topUinfoRightLayout->addWidget(userInfoTelTag, 2, 0, Qt::AlignLeft | Qt::AlignVCenter);
    topUinfoRightLayout->addWidget(userInfoTel,    2, 1, Qt::AlignLeft | Qt::AlignVCenter);

    // 4. 加个垂直弹簧，把所有内容往上推
    topUinfoRightLayout->setRowStretch(3, 1);

    // 将右侧容器加入 Top 布局
    topUinfoLayout->addWidget(topUinfoRightWidget);

    // 右侧水平弹簧
    topUinfoLayout->addStretch();

    // ======================
    // 中间分隔线
    // ======================
    QFrame *line = new QFrame(mainFrame);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #EAEAEA; border: none; min-height: 1px; max-height: 1px;");

    // ======================
    // Bottom布局
    // ======================
    QWidget *bottomUinfoWidget = new QWidget(mainFrame);
    QHBoxLayout *bottomUinfoLayout = new QHBoxLayout(bottomUinfoWidget);
    bottomUinfoLayout->setContentsMargins(0, 0, 0, 0);
    bottomUinfoLayout->setSpacing(10);

    userInfoApplyFriendBtn->setFixedHeight(35);
    userInfoSendMessageBtn->setFixedHeight(35);
    userInfoDeleteFriendBtn->setFixedHeight(35);

    bottomUinfoLayout->addWidget(userInfoApplyFriendBtn);
    bottomUinfoLayout->addWidget(userInfoSendMessageBtn);
    bottomUinfoLayout->addWidget(userInfoDeleteFriendBtn);

    // 组装到主布局
    mainUinfoLayout->addWidget(topUinfoWidget);
    mainUinfoLayout->addWidget(line);
    mainUinfoLayout->addWidget(bottomUinfoWidget, 0, Qt::AlignTop);

    // ==========================
    //  设置对象名 & 赋值
    // ==========================
    setObjectNameForSelf();
    selfInfoSetFocusPolicy();

    userInfoAvatar->setIcon(userinfo.avatar);
    userInfoTel->setText(userinfo.phone);
    userInfoNikeName->setText(userinfo.nickname);
    userInfoId->setText(userinfo.userId); // 给 ID 赋值 (假设 UserInfo 结构体里有 userId)
}

void UserInfoWidget::setObjectNameForSelf()
{
    userInfoId->setObjectName("userInfoId");
    userInfoTel->setObjectName("userInfoTel");
    userInfoNikeName->setObjectName("userInfoNikeName");
    userInfoAvatar->setObjectName("userInfoAvatar");
    userInfoApplyFriendBtn->setObjectName("userInfoApplyFriendBtn");
    userInfoSendMessageBtn->setObjectName("userInfoSendMessageBtn");
    userInfoDeleteFriendBtn->setObjectName("userInfoDeleteFriendBtn");
    userInfoIdTag->setObjectName("userInfoIdTag");
    userInfoTelTag->setObjectName("userInfoTelTag");
}

void UserInfoWidget::selfInfoSetFocusPolicy()
{
    userInfoAvatar->setFocusPolicy(Qt::NoFocus);
    userInfoApplyFriendBtn->setFocusPolicy(Qt::NoFocus);
    userInfoSendMessageBtn->setFocusPolicy(Qt::NoFocus);
    userInfoDeleteFriendBtn->setFocusPolicy(Qt::NoFocus);

}

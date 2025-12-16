#include "userinfowidget.h"

#include <QPushButton>

#include <QHBoxLayout>

#include <QVBoxLayout>

#include <QFrame>

#include "debug.h"

UserInfoWidget::UserInfoWidget(QWidget* parent) : InfoWidget(parent)
{

    // ======================
    // 初始化大小
    // ======================
    this->setFixedSize(320, 230);

    // ======================
    // 初始化控件 (注意父对象是 mainFrame)
    // ======================
    userInfoAvatar = new QPushButton(mainFrame);
    userInfoAvatar->setFixedSize(65, 65); //  对应图中蓝色 Avatar 大小
    userInfoAvatar->setIconSize(QSize(65, 65));
    userInfoAvatar->setFlat(true);

    userInfoApplyFriendBtn = new QPushButton(mainFrame);
    userInfoSendMessageBtn = new QPushButton(mainFrame);
    userInfoDeleteFriendBtn = new QPushButton(mainFrame);

    // 设置按钮文字方便调试
    userInfoApplyFriendBtn->setText("申请好友");
    userInfoSendMessageBtn->setText("发送信息");
    userInfoDeleteFriendBtn->setText("删除好友");

    userInfoId = new QLabel(mainFrame);
    userInfoTel = new QLabel(mainFrame);
    userInfoNikeName = new QLabel(mainFrame); // 也就是 NickName


    // ======================
    // 主布局
    // ======================
    QVBoxLayout *mainUinfoLayout = new QVBoxLayout(mainFrame);
    mainUinfoLayout->setContentsMargins(20, 20, 20, 20); // 留出内部边距
    mainUinfoLayout->setSpacing(10); // 上下区域间距

    // ======================
    // Top布局 (头像 + 右侧信息)
    // ======================
    QWidget *topUinfoWidget = new QWidget(mainFrame);
    QHBoxLayout *topUinfoLayout = new QHBoxLayout(topUinfoWidget);
    topUinfoLayout->setContentsMargins(0, 0, 0, 0);
    topUinfoLayout->setSpacing(15); // 头像和右侧文字的距离
    topUinfoWidget->setFixedHeight(80);

    // 1. 左侧头像
    topUinfoLayout->addWidget(userInfoAvatar, Qt::AlignTop);

    // 2. 右侧信息容器
    QWidget *topUinfoRightWidget = new QWidget(topUinfoWidget);
    QVBoxLayout *topUinfoRightLayout = new QVBoxLayout(topUinfoRightWidget);
    topUinfoRightLayout->setContentsMargins(0, 0, 0, 0);
    topUinfoRightLayout->setSpacing(5); // 文字行间距

    // 3. 添加文字
    topUinfoRightLayout->addWidget(userInfoId);
    topUinfoRightLayout->addWidget(userInfoNikeName);
    topUinfoRightLayout->addWidget(userInfoTel); // 补回缺失的 Tel

    // 4. 加个弹簧，让文字靠上 (可选)
    topUinfoRightLayout->addStretch();

    topUinfoLayout->addWidget(topUinfoRightWidget);

    // 右侧水平弹簧，防止被拉伸
    topUinfoLayout->addStretch();


    // ======================
    // 中间分隔线 (图中绿色的线)
    // ======================
    QFrame *line = new QFrame(mainFrame);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #EAEAEA; border: none; min-height: 1px; max-height: 1px;");


    // ======================
    // Bottom布局 (按钮组)
    // ======================
    QWidget *bottomUinfoWidget = new QWidget(mainFrame);
    QHBoxLayout *bottomUinfoLayout = new QHBoxLayout(bottomUinfoWidget);
    bottomUinfoLayout->setContentsMargins(0, 0, 0, 0);
    bottomUinfoLayout->setSpacing(10); // 按钮之间的间距

    userInfoApplyFriendBtn->setFixedHeight(35);
    userInfoSendMessageBtn->setFixedHeight(35);
    userInfoDeleteFriendBtn->setFixedHeight(35);

    bottomUinfoLayout->addWidget(userInfoApplyFriendBtn);
    bottomUinfoLayout->addWidget(userInfoSendMessageBtn);
    bottomUinfoLayout->addWidget(userInfoDeleteFriendBtn);
    // 1. 添加顶部区域 (头像+信息)
    mainUinfoLayout->addWidget(topUinfoWidget);

    // 2. 添加中间分隔线
    mainUinfoLayout->addWidget(line);


    // 3. 添加底部区域 (按钮组)
    mainUinfoLayout->addWidget(bottomUinfoWidget,Qt::AlignTop);

#if TEST_UI
    // 测试文字
    userInfoId->setText("ID: 123456");
    userInfoNikeName->setText("测试用户");
    userInfoTel->setText("Tel: 13800000000");
    userInfoAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
#endif

    // ==========================
    //  设置对象名
    // ==========================
    setObjectNameForSelf();

    // ==========================
    //  禁用抖动
    // ==========================
    selfInfoSetFocusPolicy();
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
}

void UserInfoWidget::selfInfoSetFocusPolicy()
{
    userInfoAvatar->setFocusPolicy(Qt::NoFocus);
    userInfoApplyFriendBtn->setFocusPolicy(Qt::NoFocus);
    userInfoSendMessageBtn->setFocusPolicy(Qt::NoFocus);
    userInfoDeleteFriendBtn->setFocusPolicy(Qt::NoFocus);

}

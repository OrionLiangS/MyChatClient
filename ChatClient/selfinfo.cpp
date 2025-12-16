#include "selfinfo.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include <QIcon>
#include <QSize>
#include "debug.h"
#include "model/data.h"

SelfInfo::SelfInfo(QWidget* parent) : InfoWidget(parent)
{

    // =========================================
    // 2. 初始化控件 (高度统一锁死 30px)
    // =========================================

    // --- 顶部模块 ---
    selfInfoAvatar = new QPushButton(mainFrame);
    selfInfoAvatar->setFlat(true);

    nameShow      = new QLabel(mainFrame);
    nameEdit      = new QLineEdit(mainFrame);
    nameEditBtn   = new QPushButton(mainFrame);
    nameSubmitBtn = new QPushButton(mainFrame);

    nameShow->setFixedHeight(30);
    nameEdit->setFixedHeight(30);

    IdTag = new QLabel(mainFrame);
    Id    = new QLabel(mainFrame);
    IdTag->setFixedHeight(30);
    Id->setFixedHeight(30);

    // --- 中部模块 ---
    signTag       = new QLabel(mainFrame);
    signShow      = new QLabel(mainFrame);
    signEdit      = new QLineEdit(mainFrame);
    signEditBtn   = new QPushButton(mainFrame);
    signSubmitBtn = new QPushButton(mainFrame);

    signTag->setFixedHeight(30);
    signShow->setFixedHeight(30);
    signEdit->setFixedHeight(30);

    // --- 底部模块 ---
    telTag        = new QLabel(mainFrame);
    telShow       = new QLabel(mainFrame);
    telEdit       = new QLineEdit(mainFrame);
    telEditBtn    = new QPushButton(mainFrame);
    telSubmitBtn  = new QPushButton(mainFrame);
    telVerifyCode = new QLineEdit(mainFrame);
    getVerifyCode = new QPushButton(mainFrame);
    getVerifyCode->setText("获取验证码");
    telVerifyCode->setPlaceholderText("请输入验证码.....");

    telTag->setFixedHeight(30);
    telShow->setFixedHeight(30);
    telEdit->setFixedHeight(30);
    telVerifyCode->setFixedHeight(30);
    getVerifyCode->setFixedHeight(35);

    // --- 设置对象名 ---
    setObjectNameForSelf();

    // --- 统一设置按钮大小 ---
    QSize btnSize(30, 30);
    nameEditBtn->setFixedSize(btnSize);
    nameSubmitBtn->setFixedSize(btnSize);
    signEditBtn->setFixedSize(btnSize);
    signSubmitBtn->setFixedSize(btnSize);
    telEditBtn->setFixedSize(btnSize);
    telSubmitBtn->setFixedSize(btnSize);

    // =========================================
    // 3. 主布局框架
    // =========================================
    QVBoxLayout* mainLayout = new QVBoxLayout(mainFrame);
    mainLayout->setContentsMargins(20, 15, 20, 15);
    mainLayout->setSpacing(5);

    QWidget* selfTopWidget    = new QWidget(mainFrame);
    QWidget* selfMidWidget    = new QWidget(mainFrame);
    QWidget* selfBottomWidget = new QWidget(mainFrame);

    selfTopWidget->setObjectName("selfTopWidget");
    selfMidWidget->setObjectName("selfMidWidget");
    selfBottomWidget->setObjectName("selfBottomWidget");

    // 容器高度设置
    selfTopWidget->setFixedHeight(80);
    selfMidWidget->setFixedHeight(80);
    selfBottomWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 分隔线
    QFrame* line1 = new QFrame(mainFrame);
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("background: transparent; border:none; border-bottom: 1px solid #f2f2f2;");
    line1->setFixedHeight(1);

    QFrame* line2 = new QFrame(mainFrame);
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("background: transparent; border:none; border-bottom: 1px solid #f2f2f2;");
    line2->setFixedHeight(1);

    mainLayout->addWidget(selfTopWidget);
    mainLayout->addWidget(line1);
    mainLayout->addWidget(selfMidWidget);
    mainLayout->addWidget(line2);
    mainLayout->addWidget(selfBottomWidget);
    mainLayout->addStretch();

    // =========================================
    // 4. 顶部布局 (Top Layout)
    // =========================================
    QHBoxLayout* topMainLayout = new QHBoxLayout(selfTopWidget);
    topMainLayout->setContentsMargins(0, 0, 0, 0);
    topMainLayout->setSpacing(15);

    selfInfoAvatar->setFixedSize(65, 65);
    selfInfoAvatar->setIconSize(QSize(65, 65));
    topMainLayout->addWidget(selfInfoAvatar, 0, Qt::AlignTop);

    QWidget* topRightWidget = new QWidget(this);
    topRightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topMainLayout->addWidget(topRightWidget);

    QGridLayout* topRightLayout = new QGridLayout();
    topRightLayout->setContentsMargins(0, 0, 0, 0);
    topRightLayout->setSpacing(5);
    topRightLayout->setRowMinimumHeight(0, 40);

    topRightLayout->addWidget(nameShow,      0, 0, 2, 2, Qt::AlignLeft | Qt::AlignVCenter);
    topRightLayout->addWidget(nameEdit,      0, 0, 2, 2, Qt::AlignVCenter);
    topRightLayout->addWidget(nameEditBtn,   0, 2, 2, 1, Qt::AlignRight | Qt::AlignVCenter);
    topRightLayout->addWidget(nameSubmitBtn, 0, 2, 2, 1, Qt::AlignRight | Qt::AlignVCenter);

    nameEdit->setHidden(true);
    nameSubmitBtn->setHidden(true);

    topRightLayout->addWidget(IdTag, 2, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    topRightLayout->addWidget(Id,    2, 1, 1, 2, Qt::AlignLeft | Qt::AlignVCenter);

    topRightLayout->setColumnStretch(0, 0);
    topRightLayout->setColumnStretch(1, 1);
    topRightLayout->setColumnStretch(2, 0);

    topRightWidget->setLayout(topRightLayout);

    // =========================================
    // 5. 中部布局 (Mid Layout) - 修复重点
    // =========================================
    QGridLayout* selfMidLayout = new QGridLayout(selfMidWidget);
    selfMidLayout->setContentsMargins(0, 0, 0, 0);
    selfMidLayout->setHorizontalSpacing(5);
    selfMidLayout->setVerticalSpacing(5);
    selfMidWidget->setLayout(selfMidLayout);

    //  [修复重点A] 减小硬性限制，创造缓冲空间
    // 原来是 40，现在改回 30 (刚好装下控件)。这样 30+5+30=65，容器75，剩余10px。
    selfMidLayout->setRowMinimumHeight(0, 30); // Tag Row
    selfMidLayout->setRowMinimumHeight(1, 30); // Content Row

    //  [修复重点B] 使用 Row Stretch 定向吸收剩余空间
    // Row 0 Stretch = 0: 保持最小高度，不许动
    // Row 1 Stretch = 1: 吸收那 10px 剩余空间，实际高度变成 40px
    selfMidLayout->setRowStretch(0, 0);
    selfMidLayout->setRowStretch(1, 1);

    // 这样，即使 LineEdit 变胖，也只是在 Row 1 的 40px 空间里折腾，绝不会挤压 Row 0

    selfMidLayout->addWidget(signTag, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

    selfMidLayout->addWidget(signShow,      1, 0, 1, 2, Qt::AlignLeft | Qt::AlignVCenter);
    selfMidLayout->addWidget(signEdit,      1, 0, 1, 2,  Qt::AlignVCenter);
    selfMidLayout->addWidget(signEditBtn,   1, 2, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
    selfMidLayout->addWidget(signSubmitBtn, 1, 2, 1, 1, Qt::AlignRight | Qt::AlignVCenter);

    signEdit->setHidden(true);
    signSubmitBtn->setHidden(true);

    selfMidLayout->setColumnStretch(0, 0);
    selfMidLayout->setColumnStretch(1, 1);
    selfMidLayout->setColumnStretch(2, 0);

    // =========================================
    // 6. 底部布局 (Bottom Layout) - 同步修复
    // =========================================
    QGridLayout* selfBottomLayout = new QGridLayout(selfBottomWidget);
    selfBottomLayout->setContentsMargins(0, 0, 0, 0);
    selfBottomLayout->setVerticalSpacing(5);
    selfBottomWidget->setLayout(selfBottomLayout);

    //  [同步修复] 底部也应用相同的逻辑，防止未来抖动
    selfBottomLayout->setRowMinimumHeight(0, 30);
    selfBottomLayout->setRowMinimumHeight(1, 30); // 手机号
    selfBottomLayout->setRowMinimumHeight(2, 30); // 验证码

    selfBottomLayout->setRowStretch(0, 0);
    selfBottomLayout->setRowStretch(1, 1); // 手机号行吸收空间
    selfBottomLayout->setRowStretch(2, 0); // 验证码行保持紧凑

    // Row 0: 标签
    selfBottomLayout->addWidget(telTag, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

    // Row 1: 手机号
    selfBottomLayout->addWidget(telShow,      1, 0, 1, 4, Qt::AlignLeft | Qt::AlignVCenter);
    selfBottomLayout->addWidget(telEdit,      1, 0, 1, 4, Qt::AlignVCenter);
    selfBottomLayout->addWidget(telEditBtn,   1, 4, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
    selfBottomLayout->addWidget(telSubmitBtn, 1, 4, 1, 1, Qt::AlignRight | Qt::AlignVCenter);

    telEdit->setHidden(true);
    telSubmitBtn->setHidden(true);

    // Row 2: 验证码
    selfBottomLayout->addWidget(telVerifyCode, 2, 0, 1, 3, Qt::AlignVCenter);
    selfBottomLayout->addWidget(getVerifyCode, 2, 3, 1, 2, Qt::AlignCenter);

    telVerifyCode->setHidden(true);
    getVerifyCode->setHidden(true);

    selfBottomLayout->setColumnStretch(0, 0);
    selfBottomLayout->setColumnStretch(1, 1);
    selfBottomLayout->setColumnStretch(2, 0);
    selfBottomLayout->setColumnStretch(3, 0);
    selfBottomLayout->setColumnStretch(4, 0);

    // =========================================
    // 7. 文本与图标设置 (保持不变)
    // =========================================
    IdTag->setText("ID: ");
    signTag->setText("个性签名: ");
    telTag->setText("手机号:");

    QSize iconSizeEdit(15, 15);
    nameEditBtn->setIconSize(iconSizeEdit);
    signEditBtn->setIconSize(iconSizeEdit);
    telEditBtn->setIconSize(iconSizeEdit);

    nameEditBtn->setIcon(QIcon(":/resource/image/selfInfoEdit.png"));
    signEditBtn->setIcon(QIcon(":/resource/image/selfInfoEdit.png"));
    telEditBtn->setIcon(QIcon(":/resource/image/selfInfoEdit.png"));

    QSize iconSizeSubmit(16, 16);
    nameSubmitBtn->setIconSize(iconSizeSubmit);
    signSubmitBtn->setIconSize(iconSizeSubmit);
    telSubmitBtn->setIconSize(iconSizeSubmit);

    nameSubmitBtn->setIcon(QIcon(":/resource/image/selfInfoSubmit.png"));
    signSubmitBtn->setIcon(QIcon(":/resource/image/selfInfoSubmit.png"));
    telSubmitBtn->setIcon(QIcon(":/resource/image/selfInfoSubmit.png"));

#if TEST_UI
    selfInfoAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    nameShow->setText("测试用户");
    signShow->setText("这是一条测试的个性签名");
    telShow->setText("18688888888");
    Id->setText("isIdTest0001");
#endif

    selfInfoSetFocusPolicy();
    initSignalSlots();
}



void SelfInfo::setObjectNameForSelf()
{
    // =========================================
    // 设置ObjectName
    // =========================================
    selfInfoAvatar->setObjectName("selfInfoAvatar");


    // 类比微信号
    IdTag->setObjectName("selfInfoIdTag");
    Id->setObjectName("selfInfoId");


    // 类比网名
    nameShow ->setObjectName("selfInfoNameShow");
    nameEdit->setObjectName("selfInfoNameEdit");
    nameSubmitBtn->setObjectName("selfInfoNameSubmitBtn");
    nameEditBtn->setObjectName("selfInfoNameEditBtn");

    // 个签
    signTag->setObjectName("selfInfoSignTag");
    signShow->setObjectName("selfInfoSignShow");
    signEdit->setObjectName("selfInfoSignEdit");
    signEditBtn->setObjectName("selfInfoSignEditBtn");
    signSubmitBtn->setObjectName("selfInfoSubmitBtn");

    // 手机号
    telTag->setObjectName("selfInfoTelTag");
    telShow->setObjectName("selfInfoTelShow");
    telEdit->setObjectName("selfInfoTelEdit");
    telEditBtn->setObjectName("selfInfoEditBtn");
    telSubmitBtn->setObjectName("selfInfoSubmitBtn");
    telVerifyCode->setObjectName("selfInfoTelVerifyCode");
    getVerifyCode->setObjectName("selfInfoGetVerifyCode");
}

void SelfInfo::initSignalSlots()
{

    // ==== NikeName ====
    connect(nameEditBtn, &QPushButton::clicked, this, [=](){
        if(nameEdit->isHidden()){
            QString nname = nameShow->text();
            nameEdit->setHidden(false);
            nameEditBtn->setHidden(true);
            nameSubmitBtn->setHidden(false);
            nameShow->setHidden(true);
            nameEdit->setText(nname);
        }
    });

    connect(nameSubmitBtn, &QPushButton::clicked, this, [=](){
        if(nameShow->isHidden()){
            nameEdit->setHidden(true);
            nameEditBtn->setHidden(false);
            nameSubmitBtn->setHidden(true);
            nameShow->setHidden(false);
            QString newname = nameEdit->text();
            nameShow->setText(newname);
            LOG()<<"触发 NikeName 修改提交操作";
        }
    });

    // ==== Sign ====

    connect(signEditBtn, &QPushButton::clicked, this, [=](){
        if(signEdit->isHidden()){
            signShow->setHidden(true);
            signEditBtn->setHidden(true);
            signSubmitBtn->setHidden(false);
            signEdit->setHidden(false);
            QString sign = signShow->text();
            signEdit->setText(sign);
        }
    });

    connect(signSubmitBtn, &QPushButton::clicked, this, [=](){
        if(signShow->isHidden()){
            signEdit->setHidden(true);
            signEditBtn->setHidden(false);
            signSubmitBtn->setHidden(true);
            signShow->setHidden(false);
            QString newsign = signEdit->text();
            signShow->setText(newsign);
            LOG()<<"触发 Sign 修改提交操作";
        }
    });



    // ==== Tel ====

    connect(telEditBtn, &QPushButton::clicked, this, [=](){
        if(telEdit->isHidden()){
            telShow->setHidden(true);
            telEditBtn->setHidden(true);
            telSubmitBtn->setHidden(false);
            telEdit->setHidden(false);
            getVerifyCode->setHidden(false);
            telVerifyCode->setHidden(false);
            QString tel = telShow->text();
            telEdit->setText(tel);
        }
    });

    connect(telSubmitBtn, &QPushButton::clicked, this, [=](){
        if(telShow->isHidden()){
            telEdit->setHidden(true);
            telEditBtn->setHidden(false);
            telSubmitBtn->setHidden(true);
            telShow->setHidden(false);
            getVerifyCode->setHidden(true);
            telVerifyCode->setHidden(true);
            QString newtel = telEdit->text();
            telShow->setText(newtel);
            LOG()<<"触发 tel 修改提交操作";
        }
    });


}

void SelfInfo::selfInfoSetFocusPolicy()
{
    selfInfoAvatar->setFocusPolicy(Qt::NoFocus);

    nameEditBtn->setFocusPolicy(Qt::NoFocus);

    nameSubmitBtn->setFocusPolicy(Qt::NoFocus);

    signEditBtn->setFocusPolicy(Qt::NoFocus);

    signSubmitBtn->setFocusPolicy(Qt::NoFocus);

    telSubmitBtn->setFocusPolicy(Qt::NoFocus);

    telEditBtn->setFocusPolicy(Qt::NoFocus);

    getVerifyCode->setFocusPolicy(Qt::NoFocus);

    // nameEdit->setFocusPolicy(Qt::NoFocus);

    // signEdit->setFocusPolicy(Qt::NoFocus);

    // telEdit->setFocusPolicy(Qt::NoFocus);

    // telVerifyCode->setFocusPolicy(Qt::NoFocus);
}

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

SelfInfo::SelfInfo(QWidget*parent):QDialog(parent) {

    this->setObjectName("selfInfoWidget");

    // 1. 窗口属性设置
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setAttribute(Qt::WA_TranslucentBackground); // 背景透明
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 关键：窗口尺寸要比实际内容稍微大一点，留出画阴影的空间
    this->setFixedSize(310, 330);
    this->move(QCursor::pos());

    // =========================================
    // 2. 创建内部容器 (mainFrame)
    // =========================================
    QFrame *mainFrame = new QFrame(this);
    mainFrame->setObjectName("mainFrame");

    // 使用 QSS 设置圆角和白色背景
    mainFrame->setStyleSheet(
        "#mainFrame {"
        "   background-color: #FFFFFF;"  // 白底
        "   border-radius: 10px;"        // 圆角
        "}"
        );

    // =========================================
    // 3. 添加阴影特效
    // =========================================
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);          // 0偏移
    shadow->setColor(QColor(0, 0, 0, 80)); // 黑色，透明度
    shadow->setBlurRadius(25);        // 模糊半径

    mainFrame->setGraphicsEffect(shadow);

    // =========================================
    // 4. 设置外层布局 (为了留出阴影空隙)
    // =========================================
    QVBoxLayout *globalLayout = new QVBoxLayout(this);
    globalLayout->setContentsMargins(10, 10, 10, 10); // 四周留 10px 给阴影
    globalLayout->addWidget(mainFrame);


    // =========================================
    // 初始化控件
    // =========================================

    // 头像
    selfInfoAvatar = new QPushButton(mainFrame);
    selfInfoAvatar->setFlat(true);
    // 类比微信号
    IdTag = new QLabel(mainFrame);
    Id = new QLabel(mainFrame);

    // 类比网名
    nameShow = new QLabel(mainFrame);
    nameEdit = new QLineEdit(mainFrame);
    nameSubmitBtn = new QPushButton(mainFrame);
    nameEditBtn = new QPushButton(mainFrame);


    // 个签
    signTag = new QLabel(mainFrame);
    signShow = new QLabel(mainFrame);
    signEdit = new QLineEdit(mainFrame);
    signEditBtn = new QPushButton(mainFrame);
    signSubmitBtn = new QPushButton(mainFrame);

    // 手机号
    telTag = new QLabel(mainFrame);
    telShow = new QLabel(mainFrame);
    telEdit = new QLineEdit(mainFrame);
    telEditBtn = new QPushButton(mainFrame);
    telSubmitBtn = new QPushButton(mainFrame);
    telVerifyCode = new QLineEdit(mainFrame);
    getVerifyCode = new QPushButton(mainFrame);


    // 设置ObjectNames
    setObjectNameForSelf();

    // =========================================
    // 布局
    // =========================================

    QVBoxLayout* mainLayout = new QVBoxLayout(mainFrame);


    mainLayout->setContentsMargins(25,20,25,20);

    // 创建上中下部Widget
    QWidget* selfTopWidget = new QWidget(mainFrame);
    selfTopWidget->setObjectName("selfTopWidget");

    QWidget* selfMidWidget = new QWidget(mainFrame);
    selfMidWidget->setObjectName("selfMidWidget");

    QWidget* selfBottomWidget = new QWidget(mainFrame);
    selfBottomWidget->setObjectName("selfBottomWidget");

    // 创建分隔线
    QFrame *line1 = new QFrame(mainFrame); // [修改]
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("background: transparent; border:none; border-bottom: 1px solid #f2f2f2;");
    line1->setFixedHeight(1);

    QFrame *line2 = new QFrame(mainFrame); // [修改]
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("background: transparent; border:none; border-bottom: 1px solid #f2f2f2;");
    line2->setFixedHeight(1);

    // 添加子Widget与分隔线 (mainLayout 现在是管理 mainFrame 内部的)
    mainLayout->addWidget(selfTopWidget);
    mainLayout->addWidget(line1);
    mainLayout->addWidget(selfMidWidget);
    mainLayout->addWidget(line2);
    mainLayout->addWidget(selfBottomWidget);

    // =========================================
    // selfTopWidget
    // =========================================
    QHBoxLayout *topMainLayout = new QHBoxLayout(selfTopWidget);
    topMainLayout->setContentsMargins(0, 0, 0, 0);
    topMainLayout->setSpacing(15);

    // --- A. 左侧：头像 ---
    selfInfoAvatar->setFixedSize(65, 65);
    selfInfoAvatar->setIconSize(QSize(65, 65));
    topMainLayout->addWidget(selfInfoAvatar, 0, Qt::AlignTop);


    // --- B. 右侧：网格布局
    QWidget* topRightWidget = new QWidget(this);
    topMainLayout->addWidget(topRightWidget);
    topRightWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed );
    QGridLayout *topRightLayout = new QGridLayout();
    topRightLayout->setContentsMargins(0, 0, 0, 0);
    topRightLayout->setSpacing(5);
    topRightLayout->setRowMinimumHeight(0, 30); // 强制第一行至少 30px (配合按钮高度)

    topRightLayout->addWidget(nameShow, 0,0,2,2,Qt::AlignLeft);
    topRightLayout->addWidget(nameEditBtn, 0,2,2,1,Qt::AlignVCenter|Qt::AlignRight);
    nameEditBtn->setFixedSize(30,30);

    topRightLayout->addWidget(nameEdit, 0,0,2,2);
    nameEdit->setHidden(true);
    topRightLayout->addWidget(nameSubmitBtn, 0,2,2,1,Qt::AlignVCenter|Qt::AlignRight);
    nameSubmitBtn->setHidden(true);
    nameEditBtn->setFixedSize(30,30);

    topRightLayout->addWidget(IdTag,2,0,1,1);
    topRightLayout->addWidget(Id,2,1,1,2);


    topRightWidget->setLayout(topRightLayout);

    // =========================================
    // selfMidWidget
    // =========================================
    QGridLayout *selfMidLayout = new QGridLayout(selfMidWidget);
    selfMidWidget->setLayout(selfMidLayout);
    selfMidLayout->setContentsMargins(0,0,0,0);
    selfMidLayout->setHorizontalSpacing(5);

    selfMidLayout->addWidget(signTag, 0,0,1,1,Qt::AlignLeft);
    selfMidLayout->addWidget(signShow, 1,0,1,2,Qt::AlignLeft);
    selfMidLayout->addWidget(signEdit, 1,0,1,2,Qt::AlignLeft);
    signEdit->setHidden(true);

    selfMidLayout->addWidget(signEditBtn, 1,2,1,1,Qt::AlignVCenter|Qt::AlignRight);
    selfMidLayout->addWidget(signSubmitBtn, 1,2,1,1,Qt::AlignVCenter|Qt::AlignRight);
    signEditBtn->setFixedSize(30,30);
    signSubmitBtn->setFixedSize(30,30);
    signSubmitBtn->setHidden(true);

    // =========================================
    // selfBottomWidget
    // =========================================
    QGridLayout *selfBottomLayout = new QGridLayout(selfBottomWidget);
    selfBottomWidget->setLayout(selfBottomLayout);
    selfBottomLayout->setContentsMargins(0,0,0,0);
    selfBottomLayout->setVerticalSpacing(5);

    selfBottomLayout->addWidget(telTag,0,0,1,1,Qt::AlignLeft|Qt::AlignVCenter);
    selfBottomLayout->addWidget(telShow,1,0,1,4);
    selfBottomLayout->addWidget(telEdit,1,0,1,4);
    telEdit->setHidden(true);
    selfBottomLayout->addWidget(telEditBtn,1,4,1,1);
    selfBottomLayout->addWidget(telSubmitBtn,1,4,1,1);
    telSubmitBtn->setHidden(true);
    telEditBtn->setFixedSize(30,30);
    telSubmitBtn->setFixedSize(30,30);

    selfBottomLayout->addWidget(telVerifyCode,2,0,1,3);
    selfBottomLayout->addWidget(getVerifyCode,2,3,1,2);
    telVerifyCode->setHidden(true);
    getVerifyCode->setHidden(true);


    // ======================================
    // 设置标签
    // ======================================
    IdTag->setText("ID: ");
    signTag->setText("个性签名: ");
    telTag->setText("手机号:");

    // ======================================
    // 设置按钮Icon样式(Edit)
    // ======================================
    // 大小
    nameEditBtn->setIconSize(QSize(15,15));
    signEditBtn->setIconSize(QSize(15,15));
    telEditBtn->setIconSize(QSize(15,15));

    // 设置Icon
    nameEditBtn->setIcon(QIcon(":/resource/image/selfInfoEdit.png"));
    signEditBtn->setIcon(QIcon(":/resource/image/selfInfoEdit.png"));
    telEditBtn->setIcon(QIcon(":/resource/image/selfInfoEdit.png"));


    // ======================================
    // 测试
    // ======================================
#if TEST_UI
    selfInfoAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    nameShow->setText("测试用户");
    signShow->setText("这是一条测试的个性签名");
    telShow->setText("18688888888");
    Id->setText("isIdTest0001");
#endif



    // ======================================
    // 禁用抖动
    // ======================================
    selfInfoAvatar->setFocusPolicy(Qt::NoFocus);

    nameEditBtn->setFocusPolicy(Qt::NoFocus);

    nameSubmitBtn->setFocusPolicy(Qt::NoFocus);

    signEditBtn->setFocusPolicy(Qt::NoFocus);

    signSubmitBtn->setFocusPolicy(Qt::NoFocus);
}

bool SelfInfo::event(QEvent *event)
{
    // 监听窗口激活状态改变
    // 当你点击窗口外部时，当前窗口会变成“非激活”状态 (isActiveWindow() 为 false)
    if (event->type() == QEvent::ActivationChange)
    {
        if (!this->isActiveWindow())
        {
            this->close(); // 失去焦点，自己关闭自己
        }
    }
    return QDialog::event(event);
}

void SelfInfo::showEvent(QShowEvent *event)
{
    // 调用父类默认行为
    QDialog::showEvent(event);

    // 显示时立即激活窗口
    this->activateWindow();
    this->setFocus();
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

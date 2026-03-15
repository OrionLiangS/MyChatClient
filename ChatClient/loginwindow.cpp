#include "loginwindow.h"

#include "debug.h"

#include "toast.h"

// ################################################################################
// 类: LoginWindow
// 描述: 登录/注册主窗口，继承自 InfoWidget（IS_MAIN 模式）。
//       界面从上到下：头像/二维码区 -> 表单切换区 -> 底部按钮组。
//       通过两个 bool 标志位（isAccountMode、isLogin）组合出 4 种场景。
// ################################################################################


// ================================================================================
// 函数: LoginWindow (构造函数)
// 描述: 调用 initLoginWindow() 初始化所有组件
// ================================================================================
LoginWindow::LoginWindow(QWidget *parent):InfoWidget(parent, WindowModal::IS_MAIN) {
    initLoginWindow();
}

// ================================================================================
// 函数: initLoginWindow
// 描述: 依次初始化各区域，并在最后调用 updateUI() 设置初始状态
// ================================================================================
void LoginWindow::initLoginWindow()
{
    this->setTitleText("账号登录");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));

    // 1) 初始化头像部分
    initAvatarOrQRcode();

    // 2) 初始化变化卡片部分
    initStackedWidget();

    // 3) 初始化按钮组
    initBtnGroup();

    // 4) 初始化一次界面
    updateUI();

    // 5) 初始化信号
    initSignal();
}


// ================================================================================
// 函数: initAvatarOrQRcode
// 描述: 初始化头像/二维码区域（显示上次登录用户头像，预留二维码切换功能）
// ================================================================================
void LoginWindow::initAvatarOrQRcode()
{
    // -------------------------------------------
    // 1. 初始化头像区容器
    // -------------------------------------------
    QWidget *aboutAvatarWidget = new QWidget(mainFrame);
    mainFrameLayout->addWidget(aboutAvatarWidget);

    // -------------------------------------------
    // 2. 设置头像区布局
    // -------------------------------------------
    QHBoxLayout *avatarLayout = new QHBoxLayout(aboutAvatarWidget);
    aboutAvatarWidget->setLayout(avatarLayout);
    avatarLayout->setContentsMargins(15,0,15,0);
    avatarLayout->setSpacing(0);
    aboutAvatarWidget->setFixedHeight(88);

    // -------------------------------------------
    // 3. 初始化头像按钮
    // -------------------------------------------
    avatarOrQRcode = new QPushButton(aboutAvatarWidget);
    avatarOrQRcode->setFixedSize(88,88);
    avatarOrQRcode->setObjectName("loginAvatarOrQRcode");
    avatarOrQRcode->setIconSize(QSize(88,88));
    avatarOrQRcode->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    avatarOrQRcode->setToolTip("点击展开二维码");

    avatarLayout->addWidget(avatarOrQRcode,0,Qt::AlignHCenter);
}


// ================================================================================
// 函数: initStackedWidget
// 描述: 初始化表单切换区域（QStackedWidget 双页面切换）
// ================================================================================
void LoginWindow::initStackedWidget()
{
    // -------------------------------------------
    // 1. 初始化 QStackedWidget 容器
    // -------------------------------------------
    stackWidget = new QStackedWidget(mainFrame);
    stackWidget->setFixedHeight(180);
    mainFrameLayout->addWidget(stackWidget);
#if TEST_UI
    // stackWidget->setStyleSheet("background-color: red;");
#endif

    // -------------------------------------------
    // 2. 初始化两个页面
    // -------------------------------------------
    initAccountPage();  // Page 0: 账号登录/注册
    initPhonePage();    // Page 1: 手机登录/注册

    // -------------------------------------------
    // 3. 设置默认显示页面
    // -------------------------------------------
    stackWidget->setCurrentIndex(1);

#if TEST_UI
    // accountLoginPage->setStyleSheet("background-color: green;");
    // phoneLoginPage->setStyleSheet("background-color: blue");
#endif
}


// ================================================================================
// 函数: initAccountPage
// 描述: 初始化账号登录/注册页面（Page 0）
// ================================================================================
void LoginWindow::initAccountPage()
{
    // -------------------------------------------
    // 1. 创建页面容器
    // -------------------------------------------
    accountLoginPage = new QWidget(stackWidget);

    // -------------------------------------------
    // 2. 创建账号输入框
    // -------------------------------------------
    loginAccountEdti = new QLineEdit(accountLoginPage);
    loginAccountEdti->setFixedHeight(40);
    loginAccountEdti->setPlaceholderText("请输入账号");
    loginAccountEdti->setObjectName("loginAccountEdti");

    // -------------------------------------------
    // 3. 创建密码输入框（带眼睛图标切换显示/隐藏）
    // -------------------------------------------
    loginPasswdEdit = new QLineEdit(accountLoginPage);
    loginPasswdEdit->setFixedHeight(40);
    loginPasswdEdit->setPlaceholderText("请输入密码");
    loginPasswdEdit->setObjectName("loginPasswdEdit");
    loginPasswdEdit->setEchoMode(QLineEdit::Password);  // 默认隐藏密码

    // 添加眼睛图标到密码输入框
    QAction *togglePasswordAction = loginPasswdEdit->addAction(
        QIcon(":/resource/image/closeEye.svg"),
        QLineEdit::TrailingPosition
        );
    togglePasswordAction->setCheckable(true);

    // 连接眼睛图标切换信号
    connect(togglePasswordAction, &QAction::toggled, this,
            [this, togglePasswordAction](bool checked) {
                if (checked) {
                    loginPasswdEdit->setEchoMode(QLineEdit::Normal);
                    togglePasswordAction->setIcon(QIcon(":/resource/image/openEye.svg"));
                } else {
                    loginPasswdEdit->setEchoMode(QLineEdit::Password);
                    togglePasswordAction->setIcon(QIcon(":/resource/image/closeEye.svg"));
                }
            });

    // -------------------------------------------
    // 4. 创建图形验证码输入框和显示控件
    // -------------------------------------------
    imgVerifyCodeEdit = new QLineEdit(accountLoginPage);
    imgVerifyCodeEdit->setPlaceholderText("右侧验证码");
    imgVerifyCodeEdit->setFixedHeight(40);
    imgVerifyCodeEdit->setObjectName("imgVerifyCodeEdit");

    imgVerifyCode = new QWidget(accountLoginPage);
    imgVerifyCode->setFixedSize(120, 40);

#if TEST_UI
    imgVerifyCode->setStyleSheet("background-color: #cad2d9;");  // 测试用背景色
#endif

    // -------------------------------------------
    // 5. 设置布局
    // -------------------------------------------
    QGridLayout *accountLoginLayout = new QGridLayout(accountLoginPage);
    accountLoginPage->setLayout(accountLoginLayout);
    accountLoginLayout->setSpacing(10);
    accountLoginLayout->setContentsMargins(20,10,20,10);

    accountLoginLayout->addWidget(loginAccountEdti,0,0,1,2);
    accountLoginLayout->addWidget(loginPasswdEdit,1,0,1,2);
    accountLoginLayout->addWidget(imgVerifyCodeEdit,2,0,1,1);
    accountLoginLayout->addWidget(imgVerifyCode,2,1,1,1);

    // -------------------------------------------
    // 6. 添加到 QStackedWidget
    // -------------------------------------------
    stackWidget->addWidget(accountLoginPage);
}


// ================================================================================
// 函数: initPhonePage
// 描述: 初始化手机登录/注册页面（Page 1）
// ================================================================================
void LoginWindow::initPhonePage()
{
    // -------------------------------------------
    // 1. 创建页面容器
    // -------------------------------------------
    phoneLoginPage = new QWidget(stackWidget);

    // -------------------------------------------
    // 2. 设置布局
    // -------------------------------------------
    QGridLayout *phoneLoginLayout = new QGridLayout(phoneLoginPage);
    phoneLoginPage->setLayout(phoneLoginLayout);
    phoneLoginLayout->setSpacing(10);
    phoneLoginLayout->setContentsMargins(20,10,20,10);

    // -------------------------------------------
    // 3. 创建手机号输入框
    // -------------------------------------------
    loginPhoneEdit = new QLineEdit(phoneLoginPage);
    loginPhoneEdit->setFixedHeight(40);
    loginPhoneEdit->setObjectName("loginPhoneEdit");
    loginPhoneEdit->setPlaceholderText("请输入手机号");

    // -------------------------------------------
    // 4. 创建短信验证码输入框
    // -------------------------------------------
    phoneVerifyCodeEdit = new QLineEdit(phoneLoginPage);
    phoneVerifyCodeEdit->setObjectName("phoneVerifyCodeEdit");
    phoneVerifyCodeEdit->setFixedHeight(40);
    phoneVerifyCodeEdit->setPlaceholderText("请输入验证码");

    // -------------------------------------------
    // 5. 创建发送验证码按钮
    // -------------------------------------------
    sendVerifyCodeBtn = new QPushButton(phoneLoginPage);
    sendVerifyCodeBtn->setFixedSize(100,40);
    sendVerifyCodeBtn->setObjectName("sendVerifyCodeBtn");
    sendVerifyCodeBtn->setText("发送验证码");

    // -------------------------------------------
    // 6. 添加到布局
    // -------------------------------------------
    phoneLoginLayout->addWidget(loginPhoneEdit,0,0,1,3);
    phoneLoginLayout->addWidget(phoneVerifyCodeEdit,1,0,1,2);
    phoneLoginLayout->addWidget(sendVerifyCodeBtn,1,2,1,1);

    // -------------------------------------------
    // 7. 添加到 QStackedWidget
    // -------------------------------------------
    stackWidget->addWidget(phoneLoginPage);
}


// ================================================================================
// 函数: initBtnGroup
// 描述: 初始化底部按钮组（提交按钮 + 两个切换按钮）
// ================================================================================
void LoginWindow::initBtnGroup()
{
    // -------------------------------------------
    // 1. 创建按钮组容器
    // -------------------------------------------
    QWidget *btnGroupWidget = new QWidget(mainFrame);
    mainFrameLayout->addWidget(btnGroupWidget);

    // -------------------------------------------
    // 2. 设置网格布局
    // -------------------------------------------
    QGridLayout *btnGroupGLayout = new QGridLayout(btnGroupWidget);
    btnGroupGLayout->setSpacing(10);
    btnGroupGLayout->setContentsMargins(20,0,20,10);
    btnGroupWidget->setLayout(btnGroupGLayout);

    // -------------------------------------------
    // 3. 创建提交按钮（登录/注册）
    // -------------------------------------------
    loginSubmitBtn = new QPushButton(btnGroupWidget);
    loginSubmitBtn->setFixedHeight(40);
    loginSubmitBtn->setObjectName("loginSubmitBtn");
    loginSubmitBtn->setText("登录");

    // -------------------------------------------
    // 4. 创建切换登录/注册按钮
    // -------------------------------------------
    switchRegisterOrLogin = new QPushButton(btnGroupWidget);
    switchRegisterOrLogin->setFixedHeight(30);
    switchRegisterOrLogin->setObjectName("switchRegisterOrLogin");
    switchRegisterOrLogin->setText("切换到注册");

    // -------------------------------------------
    // 5. 创建切换账号/手机按钮
    // -------------------------------------------
    switchPhoneOrAccount = new QPushButton(btnGroupWidget);
    switchPhoneOrAccount->setFixedHeight(30);
    switchPhoneOrAccount->setObjectName("switchPhoneOrAccount");
    switchPhoneOrAccount->setText("切换到手机");

    // -------------------------------------------
    // 6. 添加到布局（提交按钮占满一行，两个切换按钮左右分布）
    // -------------------------------------------
    btnGroupGLayout->addWidget(loginSubmitBtn, 0,0,1,3);  // 第0行，占3列
    btnGroupGLayout->addWidget(switchRegisterOrLogin, 1,0,1,1);  // 第1行第0列

    // 中间添加弹簧
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    btnGroupGLayout->addItem(horizontalSpacer,1,1);

    btnGroupGLayout->addWidget(switchPhoneOrAccount, 1,2,1,1);  // 第1行第2列

#if TEST_UI
    // btnGroupWidget->setStyleSheet("background-color: rgb(135, 69, 243);");
#endif
}


// ================================================================================
// 函数: updateUI
// 描述: 根据状态标志位（isAccountMode、isLogin）更新界面
// ================================================================================
void LoginWindow::updateUI() {
    // -------------------------------------------
    // 1. 构建标题文字
    // -------------------------------------------
    QString modeText = isAccountMode ? "账号" : "手机号";
    QString actionText = isLogin ? "登录" : "注册";
    mainTitleLabel->setText(modeText + actionText);  // "账号登录" / "手机号注册" 等

    // -------------------------------------------
    // 2. 切换页面（账号模式 -> 页面0，手机模式 -> 页面1）
    // -------------------------------------------
    stackWidget->setCurrentIndex(isAccountMode ? 0 : 1);

    // -------------------------------------------
    // 3. 更新切换按钮文字
    // -------------------------------------------
    switchPhoneOrAccount->setText(isAccountMode ? "切换至手机" : "切换至账号");
    switchRegisterOrLogin->setText(isLogin ? "切换到注册" : "切换到登录");

    // -------------------------------------------
    // 4. 更新提交按钮文字
    // -------------------------------------------
    loginSubmitBtn->setText(actionText);
}


// ================================================================================
// 函数: initSignal
// 描述: 初始化信号槽连接（连接两个切换按钮的点击信号）
// ================================================================================
void LoginWindow::initSignal()
{
    // -------------------------------------------
    // 1. 连接切换登录/注册按钮
    // -------------------------------------------
    connect(switchRegisterOrLogin, &QPushButton::clicked, this, [=](){
        isLogin = !isLogin;  // 切换登录/注册状态
        updateUI();          // 更新界面
    });

    // -------------------------------------------
    // 2. 连接切换账号/手机按钮
    // -------------------------------------------
    connect(switchPhoneOrAccount, &QPushButton::clicked, this, [=](){
        isAccountMode = !isAccountMode;  // 切换账号/手机状态
        updateUI();                      // 更新界面
    });

    // -------------------------------------------
    // 3. 连接登录/注册 成功 or 失败
    // -------------------------------------------
    connect(loginSubmitBtn, &QPushButton::clicked, this, [=](){
        Toast::showMessage("测试弹窗");
    });


}

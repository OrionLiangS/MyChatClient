#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QObject>
#include <QWidget>
#include "infowidget.h"
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>


/**
 * @brief The LoginWindow class 登录/注册主窗口
 * @details
 * 继承自 InfoWidget，使用 IS_MAIN 模式（屏幕居中、带标题栏、可拖动）。
 * 界面从上到下依次为：头像/二维码区 -> 表单切换区 -> 底部按钮组。
 *
 * 核心结构：
 * - 头像区: 显示上次登录用户头像，点击可切换二维码登录（预留功能）。
 * - 表单区: QStackedWidget 实现账号登录/手机登录两种模式的无闪烁切换。
 *   - Page 0: 账号登录/注册（账号 + 密码 + 图形验证码）
 *   - Page 1: 手机登录/注册（手机号 + 短信验证码 + 发送按钮）
 * - 按钮组: 提交按钮 + 两个切换按钮（登录/注册、账号/手机）。
 *
 * 状态管理：
 * - isAccountMode: true=账号模式, false=手机模式
 * - isLogin: true=登录模式, false=注册模式
 * - 通过两个 bool 标志位组合出 4 种场景，统一由 updateUI() 更新界面。
 */
class LoginWindow : public InfoWidget
{
    Q_OBJECT
public:
    /**
     * @brief LoginWindow 构造函数
     * @param parent 父窗口指针
     */
    LoginWindow(QWidget *parent = nullptr);

protected:
    /**
     * @brief initLoginWindow 初始化登录窗口
     * @details 依次调用各区域初始化函数，最后调用 updateUI() 设置初始状态
     */
    void initLoginWindow();

    /**
     * @brief initAvatarOrQRcode 初始化头像/二维码区域
     * @details 创建头像按钮，显示上次登录用户头像（预留二维码切换功能）
     */
    void initAvatarOrQRcode();

    /**
     * @brief initStackedWidget 初始化表单切换区域
     * @details 创建 QStackedWidget，调用 initAccountPage() 和 initPhonePage() 构建两个页面
     */
    void initStackedWidget();

    /**
     * @brief initAccountPage 初始化账号登录/注册页面（Page 0）
     * @details 创建账号输入框、密码输入框（带眼睛图标）、图形验证码输入框和显示控件
     */
    void initAccountPage();

    /**
     * @brief initPhonePage 初始化手机登录/注册页面（Page 1）
     * @details 创建手机号输入框、短信验证码输入框、发送验证码按钮
     */
    void initPhonePage();

    /**
     * @brief initBtnGroup 初始化底部按钮组
     * @details 创建提交按钮（占满一行）、切换登录/注册按钮、切换账号/手机按钮（左右分布）
     */
    void initBtnGroup();

    /**
     * @brief updateUI 根据状态标志位更新界面
     * @details 根据 isAccountMode 和 isLogin 的组合，更新标题、按钮文字、页面索引
     */
    void updateUI();

    /**
     * @brief initSignal 初始化信号槽连接
     * @details 连接两个切换按钮的点击信号，切换状态标志位并调用 updateUI()
     */
    void initSignal();

private:
    // ============================================
    // 状态标志位
    // ============================================
    bool isAccountMode = true;  ///< true=账号模式, false=手机模式
    bool isLogin = true;        ///< true=登录模式, false=注册模式

    // ============================================
    // UI 组件声明
    // ============================================

    // --- 头像区 ---
    QPushButton *avatarOrQRcode;  ///< 头像/二维码按钮

    // --- 表单切换区 ---
    QStackedWidget *stackWidget;  ///< 表单切换容器
    QWidget *accountLoginPage;    ///< 账号登录/注册页面（Page 0）
    QWidget *phoneLoginPage;      ///< 手机登录/注册页面（Page 1）

    // --- 账号页面控件 ---
    QLineEdit *loginAccountEdti;   ///< 账号输入框
    QLineEdit *loginPasswdEdit;    ///< 密码输入框（带眼睛图标）
    QLineEdit *imgVerifyCodeEdit;  ///< 图形验证码输入框
    QWidget *imgVerifyCode;        ///< 图形验证码显示控件

    // --- 手机页面控件 ---
    QLineEdit *loginPhoneEdit;        ///< 手机号输入框
    QLineEdit *phoneVerifyCodeEdit;   ///< 短信验证码输入框
    QPushButton *sendVerifyCodeBtn;   ///< 发送验证码按钮

    // --- 底部按钮组 ---
    QPushButton *loginSubmitBtn;         ///< 提交按钮（登录/注册）
    QPushButton *switchRegisterOrLogin;  ///< 切换登录/注册按钮
    QPushButton *switchPhoneOrAccount;   ///< 切换账号/手机按钮
};

#endif // LOGINWINDOW_H

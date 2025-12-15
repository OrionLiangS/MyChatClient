#ifndef SELFINFO_H
#define SELFINFO_H

#include <QDialog>

#include <QObject>

#include <QWidget>

#include <QPushButton>

#include <QLabel>

#include <QLineEdit>

#include <QGraphicsDropShadowEffect>


class SelfInfo : public QDialog
{
    Q_OBJECT
public:
    SelfInfo(QWidget*parent);
    bool event(QEvent *event)override;
    void showEvent(QShowEvent *event) override; // 新增声明
private:
    // 头像
    QPushButton *selfInfoAvatar; ///< 头像

    // 类比微信号
    QLabel *IdTag;              ///< ID的提示框
    QLabel *Id;                 ///< ID号

    // 类比网名
    QLabel *nameShow;           ///< 名字
    QLineEdit *nameEdit;        ///< 修改名字时的行输入框
    QPushButton *nameSubmitBtn; ///< 修改后名字的提交按钮
    QPushButton *nameEditBtn;   ///< 点击修改名字的按钮

    // 个签
    QLabel *signTag;            ///< 个性签名的提示
    QLabel *signShow;           ///< 个性签名展示
    QLineEdit *signEdit;        ///< 修改个性签名时的行输入框
    QPushButton *signEditBtn;   ///< 点击修改个性签名
    QPushButton *signSubmitBtn; ///< 点击提交个性签名

    // 手机号
    QLabel *telTag;             ///< 手机号提示
    QLabel *telShow;            ///< 手机号展示
    QLineEdit *telEdit;         ///< 手机号修改输入框
    QPushButton *telEditBtn;    ///< 手机号编辑按钮
    QPushButton *telSubmitBtn;  ///< 手机号提交按钮
    QLineEdit *telVerifyCode;   ///< 验证码输入框
    QPushButton *getVerifyCode; ///< 获取验证码

    // ======================
    // 设置ObjectName
    // =====================
    void setObjectNameForSelf();
};

#endif // SELFINFO_H

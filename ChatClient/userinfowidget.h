#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include "model/data.h"
#include "infowidget.h"

using model::Message;

class UserInfoWidget : public InfoWidget
{
    Q_OBJECT
public:
    UserInfoWidget(QWidget*parent);

private:
    QPushButton *userInfoAvatar; ///< 头像
    QLabel *userInfoId;
    QLabel *userInfoTel;
    QLabel *userInfoNikeName;

    QPushButton *userInfoApplyFriendBtn;
    QPushButton *userInfoSendMessageBtn;
    QPushButton *userInfoDeleteFriendBtn;

    Message message;
    // ======================
    // 设置ObjectName
    // =====================
    void setObjectNameForSelf();

    // ======================
    // 初始化信号槽
    // =====================
    // void initSignalSlots();


    // 禁用抖动
    void selfInfoSetFocusPolicy();
};

#endif // USERINFOWIDGET_H

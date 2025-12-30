#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QObject>
#include <QWidget>
#include "infowidget.h"
#include "toolswidget.h"
#include "model/data.h"

using model::UserInfo;

class QVBoxLayout;
class QLineEdit;
class QHBoxLayout;
class QStackedWidget;
class QLabel;
class FriendResultCard;

class AddFriendDialog : public InfoWidget
{
    Q_OBJECT
public:
    AddFriendDialog(QWidget *parent = nullptr);

    void doSearch();

    void setSearchKeyword(const QString& key);
private:
    void initMainWidget();
    void initTitle();
    void initSignalSlots();
    void initSearchWidget();
    void initStackedWidget();
    void setSearchInfo(UserInfo *userinfo);
    bool checkIsFriend(const QString &id);



private:
    QVBoxLayout *mainLayout;

    QPushButton *closeAddFriendBtn;
    QWidget *addFriendTitleWidget;

    QWidget *addFriendSearchWidget;
    LineEditFocus *addFriendSearchEdit;
    QPushButton *addFriendSearchBtn;

    QStackedWidget *m_stackedWidget;

    QLabel *errorPage;

    FriendResultCard* resultCard;

protected:
};

enum FriendRelationEnum {
    RELATION_OWN,       // 自己
    RELATION_FRIEND,    // 好友
    RELATION_STRANGER   // 陌生人
};

class FriendResultCard : public QWidget
{
    Q_OBJECT
public:
    FriendResultCard(QWidget *parent = nullptr);
    void setInfo(UserInfo *userInfo, FriendRelationEnum relation);
    void updateUI();

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void initUI();
    void initAvatarUI();
    void initBtnGroupUI();
    void initSignalSlots();
    void addSeparator();

    FriendRelationEnum m_relation;
    UserInfo m_userInfo;
    QVBoxLayout *mainLayout;
    QWidget *container;
    QPushButton *friendResultCardAvatar;
    QLabel* m_nickName;
    QLabel* m_id;
    QPushButton *friendResultCardAddBtn;
    QPushButton *friendResultCardSendBtn;
    QPushButton *friendResultCardDelBtn;
    QVBoxLayout *containerLayout;
};


#endif // ADDFRIENDDIALOG_H

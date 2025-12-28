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

class AddFriendDialog : public InfoWidget
{
    Q_OBJECT
public:
    AddFriendDialog(QWidget *parent = nullptr);


private:
    void initMainWidget();
    void initTitle();
    void initSignalSlots();
    void initSearchWidget();
    void initCardWidget();
private:
    QVBoxLayout *mainLayout;

    QPushButton *closeAddFriendBtn;
    QWidget *addFriendTitleWidget;

    QWidget *addFriendSearchWidget;
    LineEditFocus *addFriendSearchEdit;
    QPushButton *addFriendSearchBtn;

    QHBoxLayout *cardHLayout;

protected:
};



class FriendResultCard : public QWidget
{
    Q_OBJECT
public:
    FriendResultCard(const UserInfo& userInfo, QWidget *parent = nullptr);
protected:
void paintEvent(QPaintEvent *event) override;
private:
    const UserInfo& userInfo;

};


#endif // ADDFRIENDDIALOG_H

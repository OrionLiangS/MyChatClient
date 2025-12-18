#ifndef SESSIONDETAILSPAGE_H
#define SESSIONDETAILSPAGE_H

#include <QObject>

#include <QWidget>

#include <QPushButton>

#include <QLabel>

/**
 * @brief The AvatarItem class
 * @details
 * 会话详情页中的单个标签
 * - 标签包括一个头像与一个Label显示NikeName
 */
class AvatarItem : public QWidget{

    Q_OBJECT
public:
    explicit AvatarItem(QWidget *parent = nullptr, const QString& name = "",const QIcon& avatarIcon = QIcon());
    void setAvatarObjectName(const QString &objectName);
signals:
    void clicked();
protected:

    QPushButton *avatarBtn;
    QLabel *nameLabel;
};



/**
 * @brief The SessionDetailsPage class
 * @details
 * 单聊会话容器 - 需要部署至抽屉sidebar中
 */
class SessionDetailsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SessionDetailsPage(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
signals:
    void signalAddFriendClicked();              // 添加群聊中的好友
    void signalRemoveFriendClicked();           // 删除群聊中的好友
    void signalShowUserDetail(const QString& userId); // 想要看某人详情
};


#endif // SESSIONDETAILSPAGE_H

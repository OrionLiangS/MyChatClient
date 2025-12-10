#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>

#include <QScrollArea>

#include <QPushButton>

#include <QLabel>

#include <QStyleOption>

#include <QStyle>

#include <QPainter>

// 继承自QScrollArea滚动区域
class SessionFriendArea : public QScrollArea
{
    Q_OBJECT
public:
    /**
     * @brief SessionFriendArea
     * @param parent 父对象
     * @details
     * 构造一个SessionFriendArea对象
     */
    explicit SessionFriendArea(QWidget *parent = nullptr);

    void clear();

    void addItem(const QIcon &avatar, const QString &name, const QString &text);


signals:

private:


    /**
     * @brief container 滚动区域内的容器
     * @details
     * 当容器内的高度>滚动区域时将在水平或垂直区域中出现一个滚动条
     * 宽度不实现(已做了分割器进行分割)
     */
    QWidget* container;

    /**
     * @brief AreaSetStyle
     * @details
     * 由于.qss样式设置未生效, 因此暂时采用成员函数调用setStyleSheet的方式进行样式设置
     * @todo
     */
    void AreaSetStyle();

};




/**
 * @brief The SessionFrienItem class
 * @details
 * SessionFriendArea滚动区域中的单个Item
 * 单个Item用于作为单项好友标签或者是会话标签
 * * 当为好友标签时, text部分为好友签名
 * * 当为会话标签时, text部分表示最后一条消息
 */
class SessionFriendItem : public QWidget{
    Q_OBJECT

public:
    /**
     * @brief SessionFrienItem
     * @param owner 该Icon的所属者(SessionFriendArea)
     * @param avatar 头像
     * @param name 好友名
     * @param text 个性签名/最后一条消息
     * @details
     * 用于构造单个SessionFrientItem
     */
    explicit SessionFriendItem(QWidget *owner,const QIcon &avatar, const QString &name, const QString &text);

signals:
protected:
    QPushButton* avatarBtn; ///< 头像部分

    QLabel* nameLabel; ///< 名字标签

    // 当继承为ApplyItem时需要对该控件进行移除
    QLabel* messageLabel; ///< 文本标签 (用于显示签名或最后一条消息)

    // -- 在子类中定义 --
    // QPushButton* accept = nullptr; ///< 同意按钮
    // QPushButton* reject = nullptr; ///< 拒绝按钮
private:
    QWidget *owner; ///< 所属者
    bool selected = false; ///< 是否选中状态 默认为false - 为后期选中样式进行监控
    void paintEvent(QPaintEvent *event);

}; // SessionFriendItem


#endif // SESSIONFRIENDAREA_H

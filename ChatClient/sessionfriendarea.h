#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>

#include <QScrollArea>

#include <QPushButton>

#include <QLabel>

#include <QStyleOption>

#include <QStyle>

#include <QPainter>

#include <QMouseEvent>


/**
 * @brief The ItemType enum
 * @details
 * 用于分类不停的Item (如AddItem时需要指定类型)
 */
enum ItemType{
    SessionItemType,
    FriendItemType,
    ApplyItemType
};




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

    /**
     * @brief clear 清除列表
     * @details
     * 清除当前列表中的所有Item
     */
    void clear();


    /**
     * @brief addItem 增加Item
     * @param itemType Item的类型
     * @param itemId Item的ID(不同的Item其ID含义不同)
     * @param avatar 头像
     * @param name 姓名
     * @param text 文本内容
     * @details
     * 向列表项中添加一个Item
     * 所添加的Item根据类型进行分别, 不同类型的Item, 其id含义不相同
     */
    void addItem(ItemType itemType, const QString &itemId,const QIcon &avatar, const QString &name, const QString &text);


    /**
     * @brief clickItem 点击Item(非鼠标点击)
     * @param index 传入Item的索引(首元素为0)
     * @details
     * 通过索引以代码的方式实现对某个Item的点击
     */
    void clickItem(int index);

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



    void select();

signals:
protected:
    QPushButton* avatarBtn;             ///< 头像部分

    QLabel* nameLabel;                  ///< 名字标签

    // 当继承为ApplyItem时需要对该控件进行移除
    QLabel* messageLabel;               ///< 文本标签 (用于显示签名或最后一条消息)



    // -- 在子类中定义 --
    // 父类中不能定义子类属性
    // QPushButton* accept = nullptr;   ///< 同意按钮
    // QPushButton* reject = nullptr;   ///< 拒绝按钮


protected:
// =======================================
// 事件重写
// =======================================

    void mousePressEvent(QMouseEvent *event) override;


private:
    QWidget *owner;                     ///< 所属者
    bool selected = false;              ///< 是否选中状态 默认为false - 为后期选中样式进行监控
    void paintEvent(QPaintEvent *event) override;




}; // SessionFriendItem


/**
 * @brief The SessionItem class
 * @details
 * SessionFriendItem的子类, 会话Item
 */
class SessionItem:public SessionFriendItem{
    Q_OBJECT
public:
    SessionItem(QWidget *owner, const QString &chatSessionId, const QIcon &avatar, const QString &name, const QString &message);
private:
    QString chatSessionId; ///< 消息列表ID
}; // SessionItem



/**
 * @brief The FriendItem class
 * @details
 * SessionFriendItem的子类, 好友Item
 */
class FriendItem:public SessionFriendItem{
    Q_OBJECT
public:
    FriendItem(QWidget *owner, const QString &friendId, const QIcon &avatar, const QString &name, const QString &signature);
private:
    QString friendId; ///< 好友列表ID
}; // FriendItem


/**
 * @brief The ApplyItem class
 * @details
 * SessionFriendItem的子类, 好友申请Item
 * * 该Item并不存在Text部分, 但存在对应的申请按钮部分, 即一个Accept与一个Reject
 */
class ApplyItem:public SessionFriendItem{
    Q_OBJECT
public:
    ApplyItem(QWidget *owner, const QString &applyId, const QIcon &avatar, const QString &name);
private:
    QString applyId; ///< 好友申请条目ID
    QPushButton *acceptBtn; ///< 接受按钮
    QPushButton *rejectBtn; ///< 拒绝按钮

}; // ApplyItem


#endif // SESSIONFRIENDAREA_H

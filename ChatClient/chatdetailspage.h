#ifndef CHATDETAILSPAGE_H
#define CHATDETAILSPAGE_H

#include <QObject>

#include <QWidget>

#include <QPushButton>

#include <QLabel>

#include "floatingscrollarea.h"

#include <QLineEdit>

#include <QVBoxLayout>

/**
 * @brief The ChatDetailType enum
 * @details
 * - 用于判断当前聊天为单聊还是群聊
 */
enum ChatDetailType {
    CHAT_DETAIL_PRIVATE, // 私聊/单聊
    CHAT_DETAIL_GROUP    // 群聊
};


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
 * @brief The ChatDetailsPage class
 */
class ChatDetailsPage : public QWidget{
    Q_OBJECT
public:

    /**
     * @brief createChatDetailsPage ChatDetailsPage的工厂函数
     * @param type 会话类型
     * @param parent 会话详情的父对象
     * @return 返回指向子类对象的父类指针
     */
    static ChatDetailsPage* createChatDetailsPage(ChatDetailType type, QWidget *parent=nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    ChatDetailsPage(QWidget *parent = nullptr);


private:
};


/**
 * @class GroupChatDetailsPage
 * @brief 群聊详情页 UI 类
 * @details
 * 该类负责展示群聊的详细信息，包括成员列表、群公告、群名称以及管理功能。
 * 界面采用垂直布局，顶部为搜索框，下方为可滚动的详情区域。
 * 滚动区域内部分为：头像网格 -> 更多按钮 -> 功能菜单 -> 底部操作。
 *
 * @inherits ChatDetailsPage
 */
class GroupChatDetailsPage : public ChatDetailsPage
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针 (默认为 nullptr)
     */
    explicit GroupChatDetailsPage(QWidget *parent = nullptr);

protected:
    // 如果后续需要自定义绘制背景，可重写此函数
    // void paintEvent(QPaintEvent *event) override;

private:
    // --- 纯净重构：只拆分，不改逻辑 ---
    void initBaseLayout();      // 初始化最外层 + 搜索框 + 滚动区
    void initAvatarArea();      // 初始化头像网格 + 查看更多
    void initGroupInfo();       // 初始化群公告、群名称
    void initFooter();          // 初始化底部按钮

    // 辅助函数：只用来加那条横线
    void addSeparator();


private:
    /**
     * @brief 初始化信号槽连接
     * @details 连接内部 UI 控件的信号到外部业务逻辑，或处理界面交互逻辑。
     */
    void initSignalSlots();

    // ============================================================
    //  UI 成员变量 - 基础框架
    // ============================================================

    /**
     * @brief 界面主布局管理器
     * @details 垂直布局，管理顶部的搜索框和下方的滚动区域。
     */
    QVBoxLayout* groupChatVlayout;

    /**
     * @brief 顶部群成员搜索框
     * @details 固定在顶部，不随下方内容滚动。用于过滤群成员列表。
     */
    QLineEdit *groupChatSearch;

    /**
     * @brief 自定义浮动滚动区域
     * @details 占据主界面的剩余空间，用于承载过长的详情内容。
     */
    FloatingScrollArea *groupChatScroll;

    // ============================================================
    //  UI 成员变量 - 滚动容器
    // ============================================================

    /**
     * @brief 滚动内容的实际容器 Widget
     * @details 这是 ScrollArea 的 setWidget() 对象，所有可滚动内容都添加在这个 Widget 上。
     */
    QWidget *groupChatDetailsScrollContainer;

    /**
     * @brief 滚动容器的垂直布局
     * @details 负责从上到下排列：头像区域、功能菜单、退出按钮等。
     */
    QVBoxLayout *groupDetailsScrollContainerVlayout;

    // ============================================================
    //  UI 成员变量 - 群成员模块
    // ============================================================

    /**
     * @brief 头像网格的容器 Widget
     * @details 包裹 Grid Layout，用于统一管理头像区域的边距或背景。
     */
    QWidget* groupContainerAvatarItemWidget;

    /**
     * @brief 群成员头像网格布局
     * @details 以 4列 x N行 的方式展示群成员头像。
     */
    QGridLayout* groupContainerAvatarItemLayout;

    /**
     * @brief “查看更多群成员”按钮
     * @details 点击后可展开全部成员或跳转至独立的成员列表窗口。
     */
    QPushButton *groupContainerMoreAvatarItemBtn;

    // ============================================================
    //  UI 成员变量 - 功能菜单模块
    // ============================================================

    /** @brief “群公告”左侧标题标签 */
    QLabel *groupAnnouncementTag;

    /** @brief “群公告”右侧点击区域/内容显示按钮 */
    QPushButton* groupAnnouncement;

    /** @brief “群名称”左侧标题标签 */
    QLabel* groupNameTag;

    /** @brief “群名称”右侧点击区域/内容显示按钮 */
    QPushButton* groupName;

    /**
     * @brief 清空聊天记录按钮
     * @details 通常显示为红色文字，用于删除本地存储的消息记录。
     */
    QPushButton *clearChatHistory;

    // ============================================================
    //  UI 成员变量 - 底部操作
    // ============================================================

    /**
     * @brief 退出/解散群聊按钮
     * @details 位于页面最底部，执行退出群聊或解散群聊（如果是群主）的操作。
     */
    QPushButton* exitGroupChatBtn;

signals:
    /**
     * @brief 点击了“添加成员”按钮 [+]
     * @details 触发选人窗口以邀请新成员。
     */
    void signalAddFriendClicked();

    /**
     * @brief 点击了“移除成员”按钮 [-]
     * @details 触发踢人模式或选人窗口以移除成员。
     */
    void signalRemoveFriendClicked();

    /**
     * @brief 请求查看用户详情
     * @param userId 被点击用户的唯一标识 ID
     * @details 当点击群成员头像时触发。
     */
    void signalShowUserDetail(const QString& userId);
};



/**
 * @brief The SessionDetailsPage class
 * @details
 * 单聊会话容器 - 需要部署至抽屉sidebar中
 */
class PrivateChatDetailsPage : public ChatDetailsPage
{
    Q_OBJECT
public:
    explicit PrivateChatDetailsPage(QWidget *parent = nullptr);

protected:

private:
    void initSignalSlots();     ///< 初始化信号槽

signals:
    void signalAddFriendClicked();              // 添加群聊中的好友
    void signalRemoveFriendClicked();           // 删除群聊中的好友
    void signalShowUserDetail(const QString& userId); // 想要看某人详情
};


#endif // CHATDETAILSPAGE_H

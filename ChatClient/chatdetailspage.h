#ifndef CHATDETAILSPAGE_H
#define CHATDETAILSPAGE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "floatingscrollarea.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include "toolswidget.h"
#include "infowidget.h"


/**
 * @enum ChatDetailType
 * @brief 会话类型枚举
 * @details 用于工厂模式中区分创建单聊还是群聊的详情页。
 */
enum ChatDetailType {
    CHAT_DETAIL_PRIVATE, ///< 私聊/单聊模式
    CHAT_DETAIL_GROUP    ///< 群聊模式
};


/**
 * @class AvatarItem
 * @brief 会话详情页中的单个头像组件
 * @details
 * 该组件由一个圆形头像按钮 (QPushButton) 和下方的昵称标签 (QLabel) 组成。
 * - 支持设置特定 ObjectName 以应用不同的 QSS 样式（如添加/移除按钮）。
 * - 昵称过长时会自动进行省略处理。
 */
class AvatarItem : public QWidget{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     * @param name 显示的昵称
     * @param avatarIcon 头像图标资源
     */
    explicit AvatarItem(QWidget *parent = nullptr, const QString& name = "",const QIcon& avatarIcon = QIcon());

    /**
     * @brief 设置内部头像按钮的 ObjectName
     * @param objectName QSS 样式 ID
     * @details 主要用于区分普通成员头像与特殊的 [添加]/[移除] 按钮，以便在 QSS 中加载不同的背景图。
     */
    void setAvatarObjectName(const QString &objectName);

signals:
    /** @brief 当点击头像区域（按钮）时触发此信号 */
    void clicked();

protected:
    QPushButton *avatarBtn; ///< 头像按钮
    QLabel *nameLabel;      ///< 昵称标签
};


/**
 * @class ChatDetailsPage
 * @brief 会话详情页基类 (抽象工厂)
 * @details 定义了详情页的基本接口，并提供工厂方法用于创建具体页面。
 */
class ChatDetailsPage : public QWidget{
    Q_OBJECT
public:
    /**
     * @brief 工厂函数: 创建会话详情页
     * @param type 会话类型 (群聊/私聊)
     * @param parent 父对象
     * @return ChatDetailsPage* 指向具体子类对象的指针
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
 * 负责展示群聊的详细信息和管理功能。
 *
 * **布局结构：**
 * - **顶部**：群成员搜索框。
 * - **中间 (滚动区)**：
 * - 头像网格 (Grid Layout, 4列)。
 * - 群信息栏 (群名称、群公告)。
 * - **底部**：功能按钮 (清空记录、退出群聊)。
 *
 * @inherits ChatDetailsPage
 */
class GroupChatDetailsPage : public ChatDetailsPage
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    explicit GroupChatDetailsPage(QWidget *parent = nullptr);

    /**
     * @brief 向网格中追加一个群成员
     * @param userId 用户ID
     * @param name 显示名称
     * @param avatar 头像图标
     * @details 内部自动根据当前成员数量计算 Grid 的行列坐标。
     */
    void addMemberItem(const QString& userId, const QString& name, const QIcon& avatar);

protected:
    // void paintEvent(QPaintEvent *event) override;

private:
    // --- 界面初始化模块 ---
    void initBaseLayout();      ///< 初始化整体垂直布局、搜索框及滚动区容器
    void initAvatarArea();      ///< 初始化头像网格、预置加减号按钮
    void initGroupInfo();       ///< 初始化群公告、群名称显示区域
    void initFooter();          ///< 初始化底部功能按钮 (清空/退出)
    void addSeparator();        ///< 辅助函数：添加灰色横向分割线
    void initConfirmModifygroupName(); ///< 初始化修改群名确认弹窗

    /**
     * @brief 初始化信号槽连接
     */
    void initSignalSlots();

    // ============================================================
    //  UI 成员变量 - 基础框架
    // ============================================================
    QVBoxLayout* groupChatVlayout;              ///< 主布局
    QLineEdit *groupChatSearch;                 ///< 顶部搜索框
    FloatingScrollArea *groupChatScroll;        ///< 自定义滚动区域

    // ============================================================
    //  UI 成员变量 - 滚动容器内部
    // ============================================================
    QWidget *groupChatDetailsScrollContainer;       ///< 滚动内容的实体 Widget
    QVBoxLayout *groupDetailsScrollContainerVlayout;///< 滚动内容的垂直布局

    // ============================================================
    //  UI 成员变量 - 群成员模块
    // ============================================================
    QWidget* groupContainerAvatarItemWidget;        ///< 头像网格的容器
    QGridLayout* groupContainerAvatarItemLayout;    ///< 4列网格布局管理器
    QPushButton *groupContainerMoreAvatarItemBtn;   ///< (已废弃) 查看更多按钮

    // ============================================================
    //  UI 成员变量 - 群信息模块
    // ============================================================
    QLabel *groupAnnouncementTag;   ///< "群公告" 标签
    QPushButton* groupAnnouncement; ///< 群公告内容按钮
    QLabel* groupNameTag;           ///< "群名称" 标签
    QPushButton* groupName;         ///< 群名称显示按钮
    LineEditFocus* groupNameEdit;   ///< 群名称编辑框 (Focus 增强版)
    QLabel* groupNameLabel;         ///< 显示群名称的 Label (位于 Button 内部)

    // --- 修改名称相关 ---
    InfoWidget* confirmModifyGroupName; ///< 修改名称确认弹窗
    QPushButton *acceptModifyNameBtn;   ///< 弹窗确认按钮
    QPushButton *cancelModifyNameBtn;   ///< 弹窗取消按钮

    // ============================================================
    //  UI 成员变量 - 底部操作
    // ============================================================
    QPushButton *clearChatHistory;  ///< 清空聊天记录按钮
    QPushButton* exitGroupChatBtn;  ///< 退出群聊按钮

signals:
    /** @brief 点击了“添加成员”按钮 [+] */
    void signalAddFriendClicked();

    /** @brief 点击了“移除成员”按钮 [-] */
    void signalRemoveFriendClicked();

    /**
     * @brief 请求查看用户详情
     * @param userId 被点击用户的ID
     */
    void signalShowUserDetail(const QString& userId);
};


/**
 * @class PrivateChatDetailsPage
 * @brief 单聊详情页 UI 类
 * @details
 * 展示单聊对象的详细信息及操作。
 *
 * **布局结构：**
 * - **头部**：横向展示对方头像及添加按钮。
 * - **中间**：功能入口 (查找聊天记录)。
 * - **底部**：管理操作 (清空记录、删除好友)。
 *
 * @inherits ChatDetailsPage
 */
class PrivateChatDetailsPage : public ChatDetailsPage
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    explicit PrivateChatDetailsPage(QWidget *parent = nullptr);

private:
    /** @brief 初始化信号槽连接 */
    void initSignalSlots();

    // --- 界面初始化模块 ---
    void initBaseLayout();      ///< 初始化最外层垂直布局
    void initAvatarArea();      ///< 初始化顶部头像区域 (横向布局)
    void initFindHistoryBtn();  ///< 初始化“查找聊天记录”按钮
    void initFooter();          ///< 初始化底部按钮 (清空/删除)
    void addSeparator();        ///< 辅助函数：添加灰色横向分割线

private:
    // ============================================================
    //  UI 成员变量
    // ============================================================
    QVBoxLayout *privateChatVlayout;        ///< 页面主垂直布局

    // --- 头像区域 ---
    QWidget *privateChatAvatarWidget;       ///< 头像区域容器
    QHBoxLayout* privateChatAvatarHlayout;  ///< 头像区域横向布局

    // --- 功能按钮 ---
    QPushButton *privateChatFindHistory;    ///< [查找聊天记录] 按钮
    QPushButton *clearChatHistory;          ///< [清空聊天记录] 按钮
    QPushButton *privateDeleteFriend;       ///< [删除好友] 按钮

signals:
    /** @brief 点击了 [添加] 按钮，触发选人窗口 */
    void signalAddFriendClicked();

    /** @brief 点击了 [删除好友] 按钮 (预留信号) */
    void signalRemoveFriendClicked();

    /** @brief 请求查看用户详情 */
    void signalShowUserDetail(const QString& userId);
};

#endif // CHATDETAILSPAGE_H

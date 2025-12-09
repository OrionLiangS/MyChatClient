#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include <QPushButton>

#include <QLineEdit>

#include "model/data.h"

#include "sessionfriendarea.h"

/**
 * @brief The MainWidget class
 * @details
 * 设计为单例类(懒汉加载模式)
 */
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    ~MainWidget();

public:
    static MainWidget* getInstance();

private:
    // 单例实例
    static MainWidget* instance;

    // 单例需要将构造私有化
    MainWidget(QWidget *parent = nullptr);

    // ============================================
    // 设置UI布局 (左导航栏 中列表 右对话框)
    // ============================================

    /**
     * @brief leftWindow 窗口左侧部分(导航栏)
     */
    QWidget* leftWindow;

    /**
     * @brief midWindow 窗口中间部分(好友列表/会话列表/申请列表)
     */
    QWidget* midWindow;

    /**
     * @brief rightWindow 窗口右侧部分(聊天框)
     */
    QWidget* rightWindow;


    // ============================================
    // 左侧窗口元素
    // ============================================

    /**
     * @brief userAvatar 用户头像
     * @details
     * 头像采用按钮的目的是方便实现其他点击细节(点击头像弹出详细信息)
     */
    QPushButton* userAvatar;
    QPushButton* sessionTabButton;  ///< 会话标签页
    QPushButton* friendTabButton;  ///< 好友标签页
    QPushButton* applyTabButton;    ///< 好友申请标签页

    // ============================================
    // 窗口初始化
    // ============================================

    void initMainWindow();   ///< 初始化主窗口
    void initLeftWindow();   ///< 初始化左窗口
    void initMidWindow();    ///< 初始化中间窗口
    void initRightWindow();  ///< 初始化右窗口


    // ============================================
    // 枚举当前所激活标签页
    // ============================================
    enum ActiveTab{
        SESSION_LIST,   ///< 会话标签页
        FRIEND_LIST,    ///< 好友标签页
        APPLY_LIST      ///< 好友申请标签页
    };

    //当前标签页(默认为SESSION_LIST)
    ActiveTab activeTab = SESSION_LIST;

    // ============================================
    // 初始化信号槽
    // ============================================

    void initSignalSlots();     ///< 初始化信号槽

    // 切换标签页
    void switchTabToSession();  ///< 切换至会话标签页
    void switchTabToFriend();   ///< 切换至好友标签页
    void switchTabToApply();    ///< 切换至好友申请标签页


    // ============================================
    // midWindow 所需控件元素
    // ============================================
    QWidget *searchWidget;          ///< 搜索 Widget 限定高度66
    QLineEdit *searchEdit;          ///< 搜索框
    QPushButton *addFriendBtn;      ///< 搜索按钮
    SessionFriendArea *sessionFriendArea; ///< 好友列表部分
    // ...


    // ============================================
    // 加载列表
    // ============================================
    void loadSessionList();     ///< 加载会话列表
    void loadFriendList();      ///< 加载好友列表
    void loadApplyList();       ///< 加载好友申请列表






};
#endif // MAINWIDGET_H

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>


#include <QPushButton>

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

    ///////////////////////////////////////////////
    /// 设置UI布局 (左导航栏 中列表 右对话框)
    ///////////////////////////////////////////////

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


    ////////////////////////////////////////////////
    /// 左侧窗口元素
    ////////////////////////////////////////////////

    /**
     * @brief userAvatar 用户头像
     * @details
     * 头像采用按钮的目的是方便实现其他点击细节(点击头像弹出详细信息)
     */
    QPushButton* userAvatar;
    QPushButton* sessionTabButton;  ///< 会话标签页
    QPushButton* friendsTabButton;  ///< 好友标签页
    QPushButton* applyTabButton;    ///< 好友申请标签页

    ///////////////////////////////////////////////
    /// 窗口初始化
    ///////////////////////////////////////////////

    void initMainWindow();   ///< 初始化主窗口
    void initLeftWindow();   ///< 初始化左窗口
    void initMidWindow();    ///< 初始化中间窗口
    void initRightWindow();  ///< 初始化右窗口
};
#endif // MAINWIDGET_H

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QFrame>

#include <QGraphicsDropShadowEffect>

#include <QVBoxLayout>
#include <QLabel>

#include <QPushButton>
#include <QPoint>


/**
 * @brief WindowModal 枚举：窗口模态类型
 * @details
 * 定义 InfoWidget 的三种显示模式：
 * - IS_MODAL: 模态窗口，阻塞父窗口交互，居中于父窗口显示
 * - NON_MODAL: 非模态窗口，鼠标位置弹出，失焦自动关闭
 * - IS_MAIN: 主窗口模式，屏幕居中，带标题栏和拖动功能
 */
enum class WindowModal{
    IS_MODAL,   ///< 模态窗口
    NON_MODAL,  ///< 非模态窗口
    IS_MAIN     ///< 主窗口
};


/**
 * @brief The InfoWidget class 通用弹窗基类
 * @details
 * 继承自 QDialog，作为所有弹窗的基类。
 * 提供无边框、圆角、阴影的统一视觉风格，支持三种显示模式。
 *
 * 核心特性：
 * - 无边框 + 透明背景 + 圆角白色容器 (mainFrame)
 * - 模态模式 (IS_MODAL): 居中于父窗口，阻塞父窗口交互
 * - 非模态模式 (NON_MODAL): 鼠标位置弹出，失焦自动关闭
 * - 主窗口模式 (IS_MAIN): 屏幕居中，带标题栏，支持拖动
 *
 * 子类继承：
 * - SelfInfoWidget / UserInfoWidget: 非模态模式
 * - AddFriendDialog / ChooseFriendWidget / HistoryMessageWidget: 模态模式
 * - LoginWidget (待实现): 主窗口模式
 */
class InfoWidget : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief InfoWidget 构造函数
     * @param parent 父窗口指针
     * @param modal 窗口模态类型，默认为 NON_MODAL
     */
    InfoWidget(QWidget *parent, WindowModal modal = WindowModal::NON_MODAL);

    /**
     * @brief event 事件过滤器
     * @param event 事件对象
     * @return 是否继续传递事件
     * @details 非模态模式下监听 ActivationChange 事件，失焦时自动关闭窗口
     */
    bool event(QEvent *event)override;

    /**
     * @brief showEvent 窗口显示事件
     * @param event 显示事件对象
     * @details 根据模态类型计算窗口位置：
     *          - IS_MODAL: 居中于父窗口
     *          - IS_MAIN: 居中于屏幕
     *          - NON_MODAL: 构造时已设置位置，此处不处理
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief getFrame 获取内部容器
     * @return mainFrame 指针
     * @details 子类通过此接口获取 mainFrame，向其中添加自定义内容
     */
    QFrame* getFrame();

    /**
     * @brief setTitleText 设置标题文字
     * @param titleName 标题文本
     * @details 仅在 IS_MAIN 模式下有效，设置标题栏的文字内容
     */
    void setTitleText(const QString& titleName);

protected:
    /**
     * @brief mousePressEvent 鼠标按下事件
     * @param event 鼠标事件对象
     * @details 设置窗口焦点，确保窗口激活
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief eventFilter 事件过滤器
     * @param watched 被监听的对象
     * @param event 事件对象
     * @return 是否拦截事件
     * @details IS_MAIN 模式下监听标题栏的鼠标事件，实现窗口拖动功能
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief initModalMain 初始化主窗口模式
     * @details 创建标题栏（标题文字 + 关闭按钮）、设置布局、安装事件过滤器
     */
    void initModalMain();

    QFrame* mainFrame;              ///< 内部白色圆角容器
    QPoint m_dragPos;               ///< 拖动时鼠标相对窗口左上角的偏移量
    WindowModal windowModal;        ///< 当前窗口模态类型

    // --- IS_MAIN 模式专用控件 ---
    QVBoxLayout *mainFrameLayout = nullptr;     ///< mainFrame 的主布局（仅 IS_MAIN 模式）
    QWidget *mainTitleWidget = nullptr;         ///< 标题栏容器（仅 IS_MAIN 模式）
    QLabel *mainTitleLabel = nullptr;           ///< 标题文字（仅 IS_MAIN 模式）
    QPushButton *mainTitleCloseBtn = nullptr;   ///< 关闭按钮（仅 IS_MAIN 模式）
};

#endif // INFOWIDGET_H

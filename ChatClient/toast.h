#ifndef TOAST_H
#define TOAST_H

#include <QDialog>

class QTimer;

/**
 * @brief The Toast class 全局通知提示窗口
 * @details
 * 继承自 QDialog，用于在屏幕底部居中显示临时通知消息。
 * 
 * 核心特性：
 * - 无边框、置顶、半透明背景的独立窗口
 * - 固定尺寸 800x150，屏幕底部居中（距底部 100px）
 * - 白色圆角容器 + 阴影效果（QGraphicsDropShadowEffect）
 * - 默认 1.5 秒后自动关闭
 * - 鼠标悬浮时暂停倒计时，移开后重新计时
 * - 使用 Qt::WA_DeleteOnClose 自动释放内存
 * 
 * 使用方式：
 * @code
 * Toast::showMessage("登录成功");
 * Toast::showMessage("网络连接失败，请重试");
 * @endcode
 */
class Toast : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Toast 构造函数
     * @param text 要显示的提示文本
     * @details 
     * 创建无边框置顶窗口，设置固定尺寸和位置，
     * 添加阴影效果，启动自动关闭定时器。
     */
    Toast(const QString& text);

    /**
     * @brief showMessage 静态方法，显示全局通知
     * @param text 要显示的提示文本
     * @details 
     * 创建 Toast 实例并显示，窗口会在 1.5 秒后自动关闭并释放内存。
     * 这是推荐的使用方式，无需手动管理 Toast 对象生命周期。
     */
    static void showMessage(const QString& text);

protected:
    /**
     * @brief enterEvent 鼠标进入事件
     * @param event 事件对象
     * @details 鼠标悬浮在 Toast 上时，暂停自动关闭倒计时，
     *          让用户有足够时间阅读较长的提示信息。
     */
    void enterEvent(QEnterEvent* event) override;

    /**
     * @brief leaveEvent 鼠标离开事件
     * @param event 事件对象
     * @details 鼠标移开后，重新启动 1.5 秒倒计时，
     *          窗口将在倒计时结束后自动关闭。
     */
    void leaveEvent(QEvent* event) override;

private:
    QTimer* m_closeTimer;  ///< 自动关闭定时器（单次触发，1.5 秒）
};

#endif // TOAST_H

#include "toast.h"
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QFrame>
#include <QGraphicsDropShadowEffect>


// ################################################################################
// 类: Toast
// 描述: 全局通知提示窗口，用于在屏幕底部显示临时消息。
//       窗口结构：透明背景 -> 白色圆角容器（带阴影）-> 居中文字标签。
//       支持鼠标悬浮暂停倒计时，移开后继续倒计时自动关闭。
// ################################################################################


// ================================================================================
// 函数: Toast (构造函数)
// 描述: 初始化通知窗口的所有 UI 元素和行为逻辑
// ================================================================================
Toast::Toast(const QString &text)
{
    // -------------------------------------------
    // 1. 基础窗口设置（参考 InfoWidget）
    // -------------------------------------------
    // FramelessWindowHint: 无边框窗口
    // Tool: 独立工具窗口，不在任务栏显示
    // WindowStaysOnTopHint: 始终置顶
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);  // 透明背景，用于显示阴影
    this->setAttribute(Qt::WA_DeleteOnClose);          // 关闭时自动释放内存
    this->setFixedSize(800, 150);

    // -------------------------------------------
    // 2. 创建内部容器（白色圆角）
    // -------------------------------------------
    QFrame* mainFrame = new QFrame(this);
    mainFrame->setObjectName("toastMainFrame");
    mainFrame->setStyleSheet(
        "#toastMainFrame {"
        "   background-color: #FFFFFF;"
        "   border-radius: 10px;"
        "}"
        );

    // -------------------------------------------
    // 3. 添加阴影效果
    // -------------------------------------------
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);                    // 四周均匀阴影
    shadow->setColor(QColor(0, 0, 0, 80));      // 半透明黑色
    shadow->setBlurRadius(20);                  // 模糊半径
    mainFrame->setGraphicsEffect(shadow);

    // -------------------------------------------
    // 4. 全局布局（20px 边距用于阴影显示）
    // -------------------------------------------
    QVBoxLayout* globalLayout = new QVBoxLayout(this);
    globalLayout->setContentsMargins(20, 20, 20, 20);
    globalLayout->addWidget(mainFrame);

    // -------------------------------------------
    // 5. 内部布局（文字标签容器）
    // -------------------------------------------
    QVBoxLayout* layout = new QVBoxLayout(mainFrame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // -------------------------------------------
    // 6. 创建文字标签
    // -------------------------------------------
    QLabel* label = new QLabel(text, mainFrame);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 20px; color: #757575; background: transparent;");
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(label);

    // -------------------------------------------
    // 7. 计算位置（屏幕底部居中）
    // -------------------------------------------
    QScreen* screen = QApplication::primaryScreen();
    int x = (screen->size().width() - this->width()) / 2;
    int y = screen->size().height() - this->height() - 100;  // 距底部 100px
    this->move(x, y);

    // -------------------------------------------
    // 8. 设置自动关闭定时器（1.5 秒）
    // -------------------------------------------
    m_closeTimer = new QTimer(this);
    m_closeTimer->setSingleShot(true);   // 单次触发
    m_closeTimer->setInterval(1500);     // 1.5 秒
    connect(m_closeTimer, &QTimer::timeout, this, &Toast::close);
    m_closeTimer->start();
}


// ================================================================================
// 函数: showMessage (静态方法)
// 描述: 创建并显示全局通知窗口
// ================================================================================
void Toast::showMessage(const QString &text)
{
    Toast* toast = new Toast(text);
    toast->show();
    // 注意：无需手动 delete，Qt::WA_DeleteOnClose 会在窗口关闭时自动释放
}


// ================================================================================
// 函数: enterEvent
// 描述: 鼠标进入事件 — 暂停自动关闭倒计时
// ================================================================================
void Toast::enterEvent(QEnterEvent *event)
{
    QDialog::enterEvent(event);
    m_closeTimer->stop();  // 暂停倒计时，让用户有时间阅读消息
}


// ================================================================================
// 函数: leaveEvent
// 描述: 鼠标离开事件 — 重新启动倒计时
// ================================================================================
void Toast::leaveEvent(QEvent *event)
{
    QDialog::leaveEvent(event);
    m_closeTimer->start();  // 重新开始 1.5 秒倒计时
}

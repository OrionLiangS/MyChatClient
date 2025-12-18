#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QObject>

#include <QWidget>

#include <QVBoxLayout>

#include <QPropertyAnimation> ///< 用于绘制动画


/**
 * @brief The Sidebar class
 * @details
 * 抽屉容器 - 暂时用于作右上角按钮的会话详情页
 */
class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(QWidget *parent = nullptr, int m_startY=0);

    /**
     * @brief setContent 为抽屉容器塞进内容
     * @param content
     */
    void setContent(QWidget* content);

    /**
     * @brief showSidebar
     * @details
     * 展示抽屉
     */
    void showSidebar();

    /**
     * @brief hideSidebar
     * @details
     * 隐藏抽屉
     */
    void hideSidebar();

    /**
     * @brief toggle
     * @details
     * 自动切换抽屉状态
     */
    void toggle();

protected:
    /**
     * @brief eventFilter
     * @param watched
     * @param event
     * @return
     * @details
     * 拦截父窗口的事件, 来判断当前是需要隐藏
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief paintEvent
     * @param event
     * @details
     * 绘制窗口 - 当状态为show时需要进行展示
     */
    void paintEvent(QPaintEvent *event)override;


private:
    // 默认情况下传入RightWindow
    QWidget *m_parent;              ///< 记录父窗口
    QWidget *m_content;             ///< 内部内容
    QVBoxLayout *m_layout;          ///< 内部布局
    QPropertyAnimation *m_anim;     ///< 动画对象
    bool m_isShow;                  ///< 当前状态
    int m_width =255;               ///< 侧边栏宽度

    /**
     * @brief updatePosition
     * @details
     * 根据父窗口的大小自适应大小
     */
    void updatePosition();


    const int m_startY;     ///< 增添起始Y值, 记录Y值位置

signals:
};

#endif // SIDEBAR_H

#ifndef FLOATINGSCROLLAREA_H
#define FLOATINGSCROLLAREA_H

#include <QScrollArea>

#include <QPropertyAnimation>


class QScrollBar;
class QResizeEvent;
class QEvent; // 新增

class FloatingScrollArea : public QScrollArea
{
    Q_OBJECT
    Q_PROPERTY(int scrollValue READ verticalScrollValue WRITE setVerticalScrollValue)
public:
    explicit FloatingScrollArea(QWidget *parent = nullptr);

    // [新增] 供动画系统调用的 Getter/Setter
    int verticalScrollValue() const;
    void setVerticalScrollValue(int value);

protected:

    void wheelEvent(QWheelEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    // 鼠标进入事件
    void enterEvent(QEnterEvent *event) override; // Qt6用QEnterEvent, Qt5用QEvent
    // 鼠标离开事件
    void leaveEvent(QEvent *event) override;

private:
    void initScrollBar();
    // 统一控制显示逻辑的函数
    void updateScrollBarVisible();

    QScrollBar *m_vScrollBar;
    bool m_isHover; // 记录当前鼠标是否在区域内
    bool m_isSyncing = false; // 防止滚动条信号乒乓


    // [新增] 动画对象
    QPropertyAnimation *m_scrollAnimation;
};

#endif // FLOATINGSCROLLAREA_H

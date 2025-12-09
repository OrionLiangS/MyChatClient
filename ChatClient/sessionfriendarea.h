#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>

#include <QScrollArea>

// 继承自QScrollArea滚动区域
class SessionFriendArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit SessionFriendArea(QWidget *parent = nullptr);

    void AreaSetStyle();
signals:

private:


    /**
     * @brief container 滚动区域内的容器
     * @details
     * 当容器内的高度>滚动区域时将在水平或垂直区域中出现一个滚动条
     * 宽度不实现(已做了分割器进行分割)
     */
    QWidget* container;

};

#endif // SESSIONFRIENDAREA_H

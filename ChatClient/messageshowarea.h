#ifndef MESSAGESHOWAREA_H
#define MESSAGESHOWAREA_H

#include <QScrollArea>

#include <QWidget>

#include "model/data.h"

#include <QPushButton>


// 通常情况下 在头文件中尽量避免完全展开命名空间 而是采用局部展开
using model::Message;

/**
 * @brief The MessageShowArea class
 * @details
 * 消息展示区类
 */
class MessageShowArea : public QScrollArea
{
    Q_OBJECT
public:
    MessageShowArea();

private:
    /**
     * @brief container
     * @details
     * 消息展示区的容器
     */
    QWidget *container;
    void AreaSetStyle();

}; // MessageShowArea


/**
 * @brief The MessageItem class
 * @details
 * 单条消息的Item, 即单条消息元素
 * * 通常情况下需要同时支持 文本/语音/文件/图片
 * * 当前只考虑文本消息 - from 2025.12.12
 */
class MessageItem : public QWidget{
    Q_OBJECT
public:
    MessageItem(bool isLeft);

    static MessageItem* makeMessageItem(bool isLeft, const Message &message);

private:
    bool isLeft; ///< 判断是否为左侧消息

}; // MessageItem

#endif // MESSAGESHOWAREA_H

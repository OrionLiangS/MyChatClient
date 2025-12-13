#ifndef MESSAGESHOWAREA_H
#define MESSAGESHOWAREA_H

#include <QScrollArea>

#include <QWidget>

#include "model/data.h"

#include <QPushButton>

#include <QLabel>

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

    /**
     * @brief makeMessageItem 创建消息Item
     * @param isLeft 判断是否为左侧消息
     * @param message 消息数据内容
     * @return 返回一个MessageItem指针
     */
    static MessageItem* makeMessageItem(bool isLeft, const Message &message);

protected:

    // ==============================
    // 为不同的消息类型创建不同的消息体 (暂未完成)
    // ==============================

    static MessageItem*  makeTextMessageItem();     ///< @todo

    static MessageItem* makeImageMessageItem();     ///< @todo

    static MessageItem* makeFileMessageItem();     ///< @todo

    static MessageItem* makeSpeechMessageItem();     ///< @todo


private:

    /**
     * @brief MessageItem 构造函数
     * @param isLeft 判断是否为左侧消息Item
     * @details
     * 采用工厂模式创建对象 因此设置为构造函数私有化 防止构造函数被调用
     */
    MessageItem(bool isLeft);

    bool isLeft; ///< 判断是否为左侧消息

}; // MessageItem


/**
 * @brief The MessageContentLabel class
 * @details
 * - 该类用于创建表示文本消息的正文部分
 * - 即消息中的气泡框
 * - 需要采用绘图API进行创建
 * - 由于该项目/客户端需要采用一定的主题色
 * - 因此文本框若是为右侧消息(本用户所发送)选择使用与项目类似的蓝色色系 暂定 #269cdb
 */
class MessageContentLabel : public QWidget{
    Q_OBJECT
public:
    /**
     * @brief MessageContentLabel
     * @param text - 文本中所需要显示的内容
     * @param isLeft - 判断当前消息是否为左侧消息
     */
    MessageContentLabel(const QString &text, bool isLeft);
private:
    QLabel *messageContentLabel;     ///< 用于容纳消息的内容部分
    bool isLeft;                     ///< 用于判断当前消息为左侧消息还是右侧消息
};



#endif // MESSAGESHOWAREA_H

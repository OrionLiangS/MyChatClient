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

    /**
     * @brief addMessage - 从列表的最下部进行插入
     * @param isLeft - 判断为左侧消息还是右侧消息
     * @param message - 消息体
     */
    void addMessage(bool isLeft, const Message& message);

    /**
     * @brief addFrontMessage
     */
    void addFrontMessage(bool isLeft, const Message& message);

    /**
     * @brief clearMessage
     */
    void clearMessage();
private:
    QWidget *container;
    void AreaSetStyle();

}; // MessageShowArea


/**
 * @brief The MessageItem class
 * @details
 * 单条消息的Item
 */
class MessageItem : public QWidget{
    Q_OBJECT
public:

    /**
     * @brief makeMessageItem 创建消息Item
     */
    static MessageItem* makeMessageItem(bool isLeft, const Message &message);

protected:
    // ==============================
    // 工厂方法
    // ==============================
    static QWidget* makeTextMessageItem(bool isLeft, const QString& message);
    static QWidget* makeImageMessageItem();
    static QWidget* makeFileMessageItem();
    static QWidget* makeSpeechMessageItem();

    // 【关键修复】新增：声明 resizeEvent，否则 cpp 里没法实现！
    void resizeEvent(QResizeEvent *event) override;

private:
    /**
     * @brief MessageItem 构造函数
     */
    MessageItem(bool isLeft);

    bool isLeft;

    // 【关键修复】新增：持有中间内容的指针，否则 cpp 里找不到这个变量！
    QWidget* contentWidget = nullptr;

}; // MessageItem


/**
 * @brief The MessageContentLabel class
 * @details
 * 文本消息气泡
 */
class MessageContentLabel : public QWidget{
    Q_OBJECT
public:
    /**
     * @brief MessageContentLabel
     */
    MessageContentLabel(const QString &text, bool isLeft);

    // 【关键修复】新增：手动计算并设置大小的函数
    void updateContentSize(int parentWidth);

    /**
     * @brief paintEvent - 重写事件
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *messageContentLabel;     ///< 用于容纳消息的内容部分
    bool isLeft;                     ///< 用于判断当前消息为左侧消息还是右侧消息

    // 【关键修复】新增：保存计算后的宽高
    int m_contentWidth = 0;
    int m_contentHeight = 0;
};

#endif // MESSAGESHOWAREA_H

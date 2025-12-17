#ifndef MESSAGEEDITAREA_H
#define MESSAGEEDITAREA_H

#include <QWidget>

#include <QTextEdit>

#include <QPushButton>


/**
 * @brief The MessageEditArea class - 消息编辑区
 */
class MessageEditArea : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief MessageEditArea - 消息编辑区的构造函数
     * @param parent - 父控件
     * @details
     * - 用于初始化消息编辑区
     */
    explicit MessageEditArea(QWidget *parent = nullptr);
private:
    /**
     * @brief paintEvent
     * @param event
     * @details
     * - 重写绘画函数
     * - 一个自定义的继承于QWidget的类必须重写绘画事件才能被进行渲染
     */
    void paintEvent(QPaintEvent *event) override;

    QPushButton *sendMessageBtn; ///< 消息发送按钮 (发送(&S))

    QPushButton *sendImageBtn; ///< 发送图片按钮

    QPushButton *sendFileBtn; ///< 发送文件按钮

    QPushButton *sendSpeechBtn; ///< 发送语音按钮

    QPushButton *historyMessageBtn; ///< 历史消息按钮

    QTextEdit *messageTextEditArea; ///< 消息编辑区(本质就是一个QTextEdit)

    QWidget *messageEditToolWidget; ///< 消息编辑区的工具栏(放置发送图片/文件/语音 与 历史消息按钮)
    QWidget *messageEditSendBtnWidget; ///< 消息编辑区发送按钮所在Widget

    /**
     * @brief enableSendMessageBtn - slotFunc 信号函数
     * @details
     * - 信号函数, 用于判断发送按钮是否可用
     * - 使用规则:
     * 1) 当TextEdit中不存在有效内容时为不可用
     * 2) 当TextEdit中存在有效内容时为可用
     */
    void enableSendMessageBtn();

signals:
};

#endif // MESSAGEEDITAREA_H

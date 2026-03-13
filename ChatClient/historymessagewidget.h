#ifndef HISTORYMESSAGEWIDGET_H
#define HISTORYMESSAGEWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "infowidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "toolswidget.h"
#include <QDateEdit>
#include <QRadioButton>
#include <QStackedWidget>
#include "floatingscrollarea.h"
#include "model/data.h"

using model::Message;


/**
 * @brief The HistoryItem class 历史消息列表项
 * @details
 * 单条历史消息的容器控件。
 * 内部使用 QGridLayout 布局：头像、昵称、时间、消息内容。
 * 通过工厂方法 makeHistoryItem() 根据消息类型构造对应的内容控件。
 */
class HistoryItem : public QWidget {
    Q_OBJECT
public:
    HistoryItem();

    /**
     * @brief makeHistoryItem 工厂方法，根据消息数据构造历史消息项
     * @param msg 消息数据实体
     * @return 构造完成的 HistoryItem 指针
     * @details 根据 msg.messageType 创建对应的内容控件（目前仅实现文本类型），
     *          并使用 QGridLayout 将头像、昵称、时间、内容组装为一个完整的消息条目。
     */
    static HistoryItem *makeHistoryItem(const Message &msg);
};


/**
 * @brief The HistoryMessageWidget class 历史聊天记录主窗口
 * @details
 * 继承自 InfoWidget，作为模态对话框使用。
 * 界面从上到下依次为：标题栏、搜索/日期切换区、消息滚动列表。
 *
 * 核心结构：
 * - 标题栏: 居中标题 + 右侧关闭按钮。
 * - 搜索区: QStackedWidget 实现搜索模式/日期模式的无闪烁切换，
 *           底部 RadioButton 控制切换。
 * - 滚动区: FloatingScrollArea 承载历史消息列表，
 *           通过 addHistoryMsg() 逐条添加 HistoryItem。
 */
class HistoryMessageWidget : public InfoWidget
{
    Q_OBJECT
public:
    /**
     * @brief HistoryMessageWidget 构造函数
     * @param parent 父窗口指针
     */
    HistoryMessageWidget(QWidget* parent);

    /**
     * @brief addHistoryMsg 添加一条历史消息到滚动列表
     * @param msg 消息数据实体
     * @details 调用 HistoryItem::makeHistoryItem() 构造消息项，追加到滚动区域布局末尾。
     */
    void addHistoryMsg(const Message &msg);

    /**
     * @brief clear 清空所有历史消息
     * @details 从布局末尾向前遍历，逐个移除并销毁 HistoryItem。
     */
    void clear();

protected:
    void initHistoryMsgWidget();  ///< 初始化窗口外框（尺寸、背景、主布局）
    void initTitle();             ///< 初始化标题栏（标题文字 + 关闭按钮）
    void initSearch();            ///< 初始化搜索区（QStackedWidget + RadioButton）
    void initScrollarea();        ///< 初始化滚动区域（FloatingScrollArea + 内部容器）

private:
    // ============================================
    // UI 组件声明
    // ============================================

    QVBoxLayout *mainLayout;                ///< 主布局（垂直）

    // --- 标题栏 ---
    QWidget *historyMsgWidgetTitle;         ///< 标题栏容器
    QPushButton *closeHistoryMsgBtn;        ///< 关闭按钮

    // --- 搜索区 ---
    QWidget *searchWidget;                  ///< 搜索区外层容器
    QStackedWidget *searchModeStack;        ///< 搜索/日期模式切换栈

    LineEditFocus *searchEdit;              ///< 搜索输入框（Page 0）

    QDateEdit *bgnSel;                      ///< 起始日期选择器（Page 1）
    QDateEdit *endSel;                      ///< 结束日期选择器（Page 1）
    QPushButton *dateConfirmBtn;            ///< 日期确认按钮（Page 1）

    QRadioButton *searchSelBtn;             ///< 搜索模式单选按钮
    QRadioButton *dateSelBtn;               ///< 日期模式单选按钮

    // --- 滚动区 ---
    FloatingScrollArea *historyMsgScrollArea;   ///< 历史消息滚动区域
    QWidget *historyMsgScrollWidget;            ///< 滚动区域内部容器
    QVBoxLayout *historyMsgScrollLayout;        ///< 滚动区域内部垂直布局
};

#endif // HISTORYMESSAGEWIDGET_H

#include "messageeditarea.h"

#include <QStyleOption>

#include <QPainter>

#include <QStyle>

#include <QPaintEvent>

#include <QVBoxLayout>

#include <QHBoxLayout>

#include <QSpacerItem>

#include "debug.h"

#include "historymessagewidget.h"

// #####################################################
// 构造函数(初始化MessagEditArea输入区域)
// #####################################################
MessageEditArea::MessageEditArea(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumHeight(147);
    this->setMaximumHeight(350);
    this->setObjectName("messageEditArea");



    // =======================================
    // 构建布局
    // =======================================

    // 设置toolWidget中每个Btn的大小
    QSize size(35,35);
    QSize iconSize(25, 25);

    // 创建QWidget 控件
    messageEditToolWidget = new QWidget(this);
    messageEditToolWidget->setObjectName("messageEditToolWidget");
    messageEditToolWidget->setFixedHeight(50);

    messageEditSendBtnWidget = new QWidget(this);
    messageEditSendBtnWidget->setObjectName("messageEditSendBtnWidget");
    messageEditSendBtnWidget->setFixedHeight(60);

    // =======================================
    // 初始化按钮
    // =======================================
    sendMessageBtn = new QPushButton(messageEditSendBtnWidget);
    sendMessageBtn->setObjectName("sendMessageBtn");
    sendMessageBtn->setFixedSize(100, 35);
    sendMessageBtn->setEnabled(false);
    sendMessageBtn->setText("发送(&S)");

    sendImageBtn = new QPushButton(messageEditToolWidget);
    sendImageBtn->setObjectName("sendImageBtn");
    sendImageBtn->setFixedSize(size);
    sendImageBtn->setIconSize(iconSize);
    sendImageBtn->setIcon(QIcon(":/resource/image/sendImageBtn.png"));

    sendFileBtn = new QPushButton(messageEditToolWidget);
    sendFileBtn->setObjectName("sendFileBtn");
    sendFileBtn->setFixedSize(size);
    sendFileBtn->setIconSize(iconSize);
    sendFileBtn->setIcon(QIcon(":/resource/image/sendFileBtn.png"));

    sendSpeechBtn = new QPushButton(messageEditToolWidget);
    sendSpeechBtn->setObjectName("sendSpeechBtn");
    sendSpeechBtn->setFixedSize(size);
    sendSpeechBtn->setIconSize(iconSize);
    sendSpeechBtn->setIcon(QIcon(":/resource/image/sendSpeechBtn.png"));


    historyMessageBtn = new QPushButton(messageEditToolWidget);
    historyMessageBtn->setObjectName("historyMessageBtn");
    historyMessageBtn->setFixedSize(size);
    historyMessageBtn->setIconSize(iconSize);
    historyMessageBtn->setIcon(QIcon(":/resource/image/historyMessageBtn.png"));


    messageTextEditArea = new QTextEdit(this);
    messageTextEditArea->setObjectName("messageTextEditArea");
    messageTextEditArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    // =======================================
    // 创建布局管理器
    // =======================================
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(20,0,20,0);
    vlayout->setSpacing(0);
    this->setLayout(vlayout);

    // 将基本控件塞进vlayout中
    vlayout->addWidget(messageEditToolWidget,0);
    vlayout->addWidget(messageTextEditArea,1);
    vlayout->addWidget(messageEditSendBtnWidget,0);


    // ==============================================================================
    // 在messageEditToolWidget中设置布局(水平布局HBoxLayout)
    // ==============================================================================
    QHBoxLayout* editToolLayout = new QHBoxLayout(messageEditToolWidget);
    editToolLayout->setSpacing(10);
    messageEditToolWidget->setLayout(editToolLayout);
    editToolLayout->setContentsMargins(0,0,0,0);
    editToolLayout->addWidget(sendImageBtn);
    editToolLayout->addWidget(sendFileBtn);
    editToolLayout->addWidget(sendSpeechBtn);
    editToolLayout->addWidget(historyMessageBtn);
    editToolLayout->addStretch();
    messageEditToolWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);



    // ==============================================================================
    // 在messageEditSendBtnWidget中设置布局(水平布局HBoxLayout)
    // ==============================================================================
    QHBoxLayout *sendBtnWidgetLayout = new QHBoxLayout(messageEditSendBtnWidget);
    messageEditSendBtnWidget->setLayout(sendBtnWidgetLayout);
    sendBtnWidgetLayout->addStretch();
    sendBtnWidgetLayout->addWidget(sendMessageBtn);
    sendBtnWidgetLayout->setContentsMargins(0,10,0,20);
#if TEST_UI
    // messageEditSendBtnWidget->setStyleSheet("background-color: red;");
#endif


    // ==============================================================================
    // messageTextEditArea绑定信号判断是否启用sendMessageBtn
    // ==============================================================================
    connect(messageTextEditArea, &QTextEdit::textChanged,this, &MessageEditArea::enableSendMessageBtn);

    // ==============================================================================
    // 信号槽绑定
    // ==============================================================================
    // - HistoryBtn绑定
    connect(historyMessageBtn, &QPushButton::clicked, this, [=](){
        HistoryMessageWidget* historyMsgWidget = new HistoryMessageWidget(this);
        historyMsgWidget->setObjectName("historyMsgWidget");
        historyMsgWidget->show();
    });
}


// #####################################################
// 重写绘画事件
// #####################################################
void MessageEditArea::paintEvent(QPaintEvent *event)
{
    // 1. 初始化样式选项
    QStyleOption opt;
    opt.initFrom(this);

    // 2. 创建画家
    QPainter p(this);

    // 3. 核心代码：使用 Qt 的样式引擎绘制这个控件
    // PE_Widget 表示绘制一个通用的 Widget 背景
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}



// #####################################################
// 信号函数, 用于判断当前sendMessageBtn是否可用
// #####################################################
void MessageEditArea::enableSendMessageBtn()
{
    /// 判断textEdit中是否为空来启用sendMessageBtn
    if(this->messageTextEditArea->toPlainText().trimmed().isEmpty()){
        this ->sendMessageBtn->setEnabled(false);
    }
    else{
        this ->sendMessageBtn->setEnabled(true);
    }
}

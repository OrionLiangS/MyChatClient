#include <QWidget>
#include <QEvent>
#include <QVBoxLayout>
#include "infowidget.h"
#include <QMouseEvent>
#include "model/data.h"


// ################################################################################
// 类: InfoWidget
// 描述: 通用弹窗基类，提供三种显示模式（模态/非模态/主窗口）。
//       所有弹窗继承此类，复用无边框、圆角、阴影的统一视觉风格。
// ################################################################################


// ================================================================================
// 函数: InfoWidget (构造函数)
// 描述: 根据 modal 参数初始化不同模式的窗口属性，创建内部容器和阴影效果
// ================================================================================
InfoWidget::InfoWidget(QWidget*parent, WindowModal modal):
    QDialog(parent),
    windowModal(modal)
{
    this->setObjectName("selfInfoWidget");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // =========================================
    // 0. 基础窗口属性设置
    // =========================================
    if(windowModal == WindowModal::IS_MODAL){
        // 模态模式：阻塞父窗口交互，固定尺寸 667x533
        this->setWindowModality(Qt::ApplicationModal);
        this->setFixedSize(667,533);
    }
    else if(windowModal == WindowModal::NON_MODAL){
        // 非模态模式：鼠标位置弹出，固定尺寸 320x360
        this->setFixedSize(320, 360);
        this->move(QCursor::pos());
    }
    else if(windowModal == WindowModal::IS_MAIN){
        // 主窗口模式：屏幕居中，固定尺寸 315x430
        this->setFixedSize(315, 430);
    }
    else{
        LOG()<<"错误的模态属性";
    }

    this->setAttribute(Qt::WA_DeleteOnClose);

    // =========================================
    // 1. 创建内部容器与特效
    // =========================================
    mainFrame = new QFrame(this);
    mainFrame->setObjectName("mainFrame");
    mainFrame->setStyleSheet(
        "#mainFrame {"
        "   background-color: #FFFFFF;"
        "   border-radius: 10px;"
        "}"
        );

    // 添加阴影效果
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setBlurRadius(20);
    mainFrame->setGraphicsEffect(shadow);

    // 全局布局：20px 边距，容纳 mainFrame
    QVBoxLayout* globalLayout = new QVBoxLayout(this);
    globalLayout->setContentsMargins(20, 20, 20, 20);
    globalLayout->addWidget(mainFrame);

    // 主窗口模式：初始化标题栏
    if(windowModal == WindowModal::IS_MAIN){
        initModalMain();
    }
}


// ================================================================================
// 函数: event
// 描述: 事件过滤器，非模态模式下监听窗口激活状态变化
// ================================================================================
bool InfoWidget::event(QEvent *event)
{
    // 非模态模式：失焦自动关闭
    if(windowModal == WindowModal::NON_MODAL)
    {
        // 监听窗口激活状态改变
        // 当点击窗口外部时，当前窗口变成"非激活"状态 (isActiveWindow() 为 false)
        if (event->type() == QEvent::ActivationChange)
        {
            if (!this->isActiveWindow())
            {
                this->close(); // 失去焦点，自动关闭
            }
        }
    }
    return QDialog::event(event);
}


// ================================================================================
// 函数: showEvent
// 描述: 窗口显示事件，根据模态类型计算并设置窗口位置
// ================================================================================
void InfoWidget::showEvent(QShowEvent *event)
{
    // 1. 模态模式：居中于父窗口
    if (windowModal == WindowModal::IS_MODAL && this->parentWidget()) {
        // 获取最顶层的父窗口（通常是 MainWidget）
        QWidget* topLevel = this->parentWidget()->window();

        if (topLevel) {
            // 获取父窗口在屏幕上的绝对位置
            QPoint parentGlobalPos = topLevel->mapToGlobal(QPoint(0, 0));

            // 计算居中坐标：父窗口位置 + (父宽-子宽)/2
            int xOffset = (topLevel->width() - this->width()) / 2;
            int yOffset = (topLevel->height() - this->height()) / 2;

            this->move(parentGlobalPos.x() + xOffset, parentGlobalPos.y() + yOffset);
        }
    }
    // 2. 主窗口模式：居中于屏幕
    else if(windowModal == WindowModal::IS_MAIN){
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect rect = screen->availableGeometry();
        this->move((rect.width() - this->width())/2,(rect.height() - this->height())/2);
    }

    // 3. 必须调用父类的 showEvent，否则窗口可能无法正常显示
    QDialog::showEvent(event);

    // 4. 激活窗口并获得焦点
    this->activateWindow();
    this->setFocus();
}


// ================================================================================
// 函数: getFrame
// 描述: 获取内部容器 mainFrame，供子类添加自定义内容
// ================================================================================
QFrame *InfoWidget::getFrame()
{
    return mainFrame;
}


// ================================================================================
// 函数: initModalMain
// 描述: 初始化主窗口模式（IS_MAIN）的标题栏和布局
// ================================================================================
void InfoWidget::initModalMain()
{
    // -------------------------------------------
    // 1. 初始化 mainFrame 的主布局
    // -------------------------------------------
    mainFrameLayout = new QVBoxLayout(mainFrame);
    mainFrame->setLayout(mainFrameLayout);
    mainFrameLayout->setContentsMargins(0,0,0,0);
    mainFrameLayout->setSpacing(0);
    mainFrameLayout->setAlignment(Qt::AlignTop);

    // -------------------------------------------
    // 2. 初始化标题栏容器
    // -------------------------------------------
    mainTitleWidget = new QWidget(mainFrame);
    mainFrameLayout->addWidget(mainTitleWidget);
    mainTitleWidget->setFixedHeight(31);

    // -------------------------------------------
    // 3. 初始化标题栏布局
    // -------------------------------------------
    QHBoxLayout *titleLayout = new QHBoxLayout(mainTitleWidget);
    mainTitleWidget->setLayout(titleLayout);
    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->setSpacing(0);

    // -------------------------------------------
    // 4. 初始化标题文字
    // -------------------------------------------
    mainTitleLabel = new QLabel(mainTitleWidget);
    mainTitleLabel->setFixedHeight(31);
    mainTitleLabel->setObjectName("mainTitleLabel");
    mainTitleLabel->setStyleSheet("color: #9e9e9e; font-size: 15px; font-family:\"思源黑体\"; font-weight: bold;");

    // -------------------------------------------
    // 5. 初始化关闭按钮
    // -------------------------------------------
    mainTitleCloseBtn = new QPushButton(mainTitleWidget);
    mainTitleCloseBtn->setFixedSize(50,31);
    mainTitleCloseBtn->setObjectName("mainTitleCloseBtn");
    mainTitleCloseBtn->setText(QChar(0x00D7));

    // -------------------------------------------
    // 6. 组装标题栏布局
    // -------------------------------------------
    titleLayout->addSpacing(50);
    titleLayout->addStretch();
    titleLayout->addWidget(mainTitleLabel,0, Qt::AlignHCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(mainTitleCloseBtn,0, Qt::AlignRight);

    // -------------------------------------------
    // 7. 连接关闭按钮信号
    // -------------------------------------------
    connect(mainTitleCloseBtn, &QPushButton::clicked, this, &InfoWidget::close);

    // -------------------------------------------
    // 8. 安装事件过滤器，监听标题栏拖动
    // -------------------------------------------
    mainTitleWidget->installEventFilter(this);
}


// ================================================================================
// 函数: setTitleText
// 描述: 设置标题文字（仅 IS_MAIN 模式有效）
// ================================================================================
void InfoWidget::setTitleText(const QString &titleName)
{
    if(windowModal == WindowModal::IS_MAIN && mainTitleLabel)
        mainTitleLabel->setText(titleName);
}


// ================================================================================
// 函数: mousePressEvent
// 描述: 鼠标按下事件，设置窗口焦点
// ================================================================================
void InfoWidget::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
    QDialog::mousePressEvent(event);
}


// ================================================================================
// 函数: eventFilter
// 描述: 事件过滤器，IS_MAIN 模式下监听标题栏的鼠标事件实现窗口拖动
// ================================================================================
bool InfoWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == mainTitleWidget && windowModal == WindowModal::IS_MAIN){
        if(event->type() == QEvent::MouseButtonPress){
            // 鼠标按下：记录鼠标相对窗口左上角的偏移量
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                m_dragPos = mouseEvent->globalPosition().toPoint() - this->frameGeometry().topLeft();
            }
        }
        else if (event->type() == QEvent::MouseMove) {
            // 鼠标移动：根据偏移量移动窗口
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->buttons() & Qt::LeftButton) {
                this->move(mouseEvent->globalPosition().toPoint() - m_dragPos);
            }
        }
    }
    return QDialog::eventFilter(watched, event);
}

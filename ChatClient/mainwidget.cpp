#include "mainwidget.h"

#include <QIcon>

#include <QFile>

#include <QHBoxLayout>

#include <QSplitter>

/**
 * @brief MainWidget::instance 单例实例
 */
MainWidget* MainWidget::instance = nullptr;


/**
 * @brief MainWidget::getInstance 获取单例实例
 * @details
 * 若是单例不存在则创建一个单例
 * 若是单例存在则获取已有的单例
 * @return
 * 返回一个单例指针
 */
MainWidget *MainWidget::getInstance()
{
    if(instance == nullptr){
        // 不传入参数, 默认为nullptr - 表示以桌面作为父窗口
        instance = new MainWidget();
    }
    return instance;
}

/**
 * @brief MainWidget::MainWidget 构造函数
 * @param parent
 */
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("聊天室");
    qDebug() << "Icon exists?" << QFile::exists(":/resource/image/logo.png");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));


    // ===========================
    //  初始化窗口
    // ===========================
    initMainWindow();   // 初始化主窗口
    initLeftWindow();   // 初始化左窗口
    initMidWindow();    // 初始化中间窗口
    initRightWindow();  // 初始化右窗口

}

/**
 * @brief MainWidget::initMainWindow 初始化全局窗口
 */
void MainWidget::initMainWindow()
{
    // 1. 窗口基础设置
    this->resize(1000, 600);
    this->setMinimumSize(627, 570);


    // A. 创建外层的大布局 (水平)
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // B. 创建左侧窗口 (独立于 Splitter 之外)
    leftWindow = new QWidget(this);
    leftWindow->setFixedWidth(70); // 彻底固定
    leftWindow->setStyleSheet("background-color: #ECECEC; border: none; border-right: 1px solid #D5D5D5;");

    // C. 创建右侧的 Splitter (只用来管理 中间 和 右侧)
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1);
    splitter->setStyleSheet("QSplitter::handle { background-color: transparent; }"); // 透明把手

    // D. 创建中间和右侧窗口
    midWindow = new QWidget(this);
    rightWindow = new QWidget(this);

    // 样式设置
    midWindow->setStyleSheet("background-color: #F7F7F7; border: none; border-right: 1px solid #D5D5D5;");
    rightWindow->setStyleSheet("background-color: #EDEDED; border: none;");

    // ============================================================
    // ⚙️ 组装核心
    // ============================================================

    // 1. 把 中、右 塞进 Splitter
    splitter->addWidget(midWindow);
    splitter->addWidget(rightWindow);

    // 2. 设置 Splitter 行为 (防止中间窗口消失)
    splitter->setCollapsible(0, false); // 中间窗口不能折叠
    splitter->setCollapsible(1, false); // 右侧窗口不能折叠

    midWindow->setMinimumWidth(200);    // 中间最小宽
    midWindow->setMaximumWidth(400);    // 中间最大宽
    rightWindow->setMinimumWidth(500);  // 右侧最小宽

    // 拉伸策略：拖动窗口时，增量全给右侧
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    // 3. 把 左窗口 和 Splitter 塞进主布局
    // 结构： [ Fixed Left ] + [ Splitter (Mid | Right) ]
    mainLayout->addWidget(leftWindow);
    mainLayout->addWidget(splitter);
}

/**
 * @brief MainWidget::initLeftWindow 初始化左窗口
 */
void MainWidget::initLeftWindow()
{
    /**
     * @todo
     */
}

/**
 * @brief MainWidget::initMidWindow 初始化中间窗口
 */
void MainWidget::initMidWindow()
{
    /**
     * @todo
     */
}

/**
 * @brief MainWidget::initRightWindow 初始化右侧窗口
 */
void MainWidget::initRightWindow()
{
    /**
     * @todo
     */
}

MainWidget::~MainWidget() {}

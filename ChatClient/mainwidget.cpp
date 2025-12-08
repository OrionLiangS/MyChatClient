#include "mainwidget.h"

#include <QIcon>

#include <QFile>

#include <QHBoxLayout>

#include <QVBoxLayout>

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
    // qDebug() << "Icon exists?" << QFile::exists(":/resource/image/logo.png");
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
    // 隐藏标题栏 - 暂不考虑实现
    // this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    // this->setAttribute(Qt::WA_TranslucentBackground);
    this->setObjectName("MainWidget");
    this->setStyleSheet("MainWidget");



    // A. 创建外层的大布局 (水平)
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); //设置Layout内控件四周的空白
    mainLayout->setSpacing(0); // 设置布局中各个控件紧密布局

    // B. 创建左侧窗口 (独立于 Splitter 之外)
    leftWindow = new QWidget(this);
    leftWindow->setFixedWidth(70); // 彻底固定
    leftWindow->setObjectName("leftWindow");
    leftWindow->setStyleSheet("leftWindow");

    // C. 创建右侧的 Splitter (只用来管理 中间 和 右侧)
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1);
    splitter->setObjectName("mainSplitter");
    splitter->setStyleSheet("mainSplitter::handle"); // 透明把手

    // D. 创建中间和右侧窗口
    midWindow = new QWidget(this);
    midWindow->setObjectName("midWindow");
    rightWindow = new QWidget(this);
    rightWindow->setObjectName("rightWindow");

    // 样式设置
    midWindow->setStyleSheet("midWindow");
    rightWindow->setStyleSheet("rightWindow");

    // ============================================================
    //  组装核心
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
    // 设置布局管理器
    QVBoxLayout *vlayout = new QVBoxLayout(leftWindow);
    leftWindow->setLayout(vlayout);
    vlayout->setSpacing(10);
    vlayout->setContentsMargins(0,30,0,0);

    // 标签大小
    QSize icon_size(27,27);

    // 头像大小
    QSize default_size(45,45);

    // ============================
    // 1) 创建头像
    // ============================

    userAvatar = new QPushButton(leftWindow);
    userAvatar->setFixedSize(default_size);
    userAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    userAvatar->setIconSize(default_size);
    // 设置ObjectName
    userAvatar->setObjectName("userAvatar");
    // 设置样式
    userAvatar->setStyleSheet("userAvatar");



    // ============================
    // 2) 创建标签按钮
    // ============================

    // lamdba 表达式 - 创建QPushButton对象
    auto createBtn = [&](const QString& objName, const QString& iconPath = "")->QPushButton*{
        QPushButton* btn = new QPushButton(leftWindow);
        btn->setFixedSize(default_size);
        btn->setIconSize(icon_size);

        btn->setObjectName(objName);
        btn->setIcon(QIcon(iconPath));
        btn->setStyleSheet(objName);
        return btn;
    };


    sessionTabButton = createBtn("sessionTab", ":/resource/image/sessionTab.png");
    friendsTabButton = createBtn("friendTab", ":/resource/image/friendTab.png");
    applyTabButton = createBtn("applyTab", ":/resource/image/applyTab.png");


    // ============================
    // 3) 添加到布局
    // ============================
    vlayout->addWidget(userAvatar, 0, Qt::AlignHCenter);
    vlayout->addWidget(sessionTabButton, 0, Qt::AlignHCenter);
    vlayout->addWidget(friendsTabButton, 0, Qt::AlignHCenter);
    vlayout->addWidget(applyTabButton, 0, Qt::AlignHCenter);

    // 添加弹簧
    vlayout->addStretch();

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

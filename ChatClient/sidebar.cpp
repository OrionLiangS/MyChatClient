#include "sidebar.h"

#include <QEvent>

#include <QMouseEvent>

#include <QStyleOption>

#include <QPainter>

#include <QStyle>


Sidebar::Sidebar(QWidget *parent, int m_startY)
    :QWidget(parent), m_parent(parent), m_content(nullptr), m_isShow(false), m_startY(m_startY)
{
    this->setObjectName("Sidebar");

    // ============================
    // 1) 安装事件监听器
    // ============================
    // 安装事件监听器需要保证父对象必须存在 否则无法安装时间监听器
    if(m_parent) {
        this->setParent(parent);
        // 安装事件监听器 用于监听父类的事件
        m_parent->installEventFilter(this);
    }

    // ===========================
    // 初始化抽屉
    // ===========================
    // 设置大小
    this->setFixedSize(m_width, m_parent->height()); //
    this->hide();

    // 初始化布局
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);

    // 初始化动画
    m_anim = new QPropertyAnimation(this, "pos");
    // 持续事件
    m_anim->setDuration(250);
    // 设置丝滑曲线 todo
    m_anim->setEasingCurve(QEasingCurve::OutCubic);

}



void Sidebar::setContent(QWidget *content)
{
    // 判断 m_content 是否存在内容, 若是存在则清理原有的内容, 将
    if(m_content){
        m_layout->removeWidget(m_content);
        m_content->deleteLater();
    }
    m_content = content;
    // 将新内容加进Layout布局中
    m_layout->addWidget(m_content);
}

void Sidebar::showSidebar()
{
    // 断开上次连接
    disconnect(m_anim, &QPropertyAnimation::finished, this, &Sidebar::hide);

    // 显示前先更新大小
    updatePosition();
    this->show();
    this->raise(); // 顶层显式

    // 设置动画进入
    int parentW = m_parent->width();
    m_anim->stop();
    m_anim->setStartValue(QPoint(parentW, m_startY));
    m_anim->setEndValue(QPoint(parentW-m_width, m_startY));
    m_anim->start();

    m_isShow = true;
}

void Sidebar::hideSidebar()
{
    int parentW = m_parent->width(); // 获取父类长度
    m_anim->stop();
    m_anim->setStartValue(this->pos());
    m_anim->setEndValue(QPoint(parentW, m_startY));

    connect(m_anim, &QPropertyAnimation::finished, this, &Sidebar::hide, Qt::UniqueConnection);

    m_anim->start();
    m_isShow = false;
}

void Sidebar::toggle()
{
    if(m_isShow) hideSidebar();
    else showSidebar();
}

bool Sidebar::eventFilter(QObject *watched, QEvent *event)
{
    // 监听 ResizeEvent 事件
    if(watched == m_parent && event->type() == QEvent::Resize){
        // 父窗口大小发生变化, update更新大小
        updatePosition();
    }
    if(m_isShow && event->type() == QEvent::MouseButtonPress){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        // mapToGlobal 确保坐标系一致
        QPoint globalPos = mouseEvent->globalPos();

        // 如果点击点 不在 Sidebar 的矩形范围内
        // 且 Sidebar 是显示的
        if (!this->geometry().contains(this->mapFromGlobal(globalPos))) {
            hideSidebar();
            return true; // 拦截事件，不让父窗口响应这次点击 (可选)
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Sidebar::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Sidebar::updatePosition()
{
    if(!m_parent) return; // 无父亲
    this->setFixedSize(m_width, m_parent->height());

    if(!m_isShow){
        this->move(m_parent->width(), m_startY); // 移到界外
    }
    else this->move(m_parent->width()-m_width, m_startY); // 吸附右侧
}



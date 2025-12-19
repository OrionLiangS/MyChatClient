#include "floatingscrollarea.h"
#include <QScrollBar>
#include <QResizeEvent>
#include <QEvent>

FloatingScrollArea::FloatingScrollArea(QWidget *parent)
    : QScrollArea(parent), m_vScrollBar(nullptr), m_isHover(false) // 1. 初始化 hover 状态
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setWidgetResizable(true);

    this->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    initScrollBar();
}

void FloatingScrollArea::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);
    int barWidth = 8;
    if(m_vScrollBar) {
        m_vScrollBar->setGeometry(this->width() - barWidth, 0, barWidth, this->height());
        m_vScrollBar->raise();
    }
}

//  2. 鼠标进入 -> 标记为悬停 -> 更新显示
void FloatingScrollArea::enterEvent(QEnterEvent *event)
{
    QScrollArea::enterEvent(event);
    m_isHover = true;
    updateScrollBarVisible();
}

//  3. 鼠标离开 -> 标记为离开 -> 更新显示
void FloatingScrollArea::leaveEvent(QEvent *event)
{
    QScrollArea::leaveEvent(event);
    m_isHover = false;
    updateScrollBarVisible();
}

//  4. 核心逻辑：只有当 (鼠标悬停 + 需要滚动) 同时满足时，才显示
void FloatingScrollArea::updateScrollBarVisible()
{
    if(!m_vScrollBar) return;

    // 获取原生滚动条的范围
    int min = this->verticalScrollBar()->minimum();
    int max = this->verticalScrollBar()->maximum();

    // 判断是否真的需要滚动 (内容高度 > 容器高度)
    bool needScroll = (max > min);

    // 最终决定：既要 Hover，又要 NeedScroll
    // (如果你希望拖动时鼠标出去了也不消失，逻辑会更复杂一点，但这个基本够用了)
    m_vScrollBar->setVisible(m_isHover && needScroll);
}

void FloatingScrollArea::initScrollBar()
{
    m_vScrollBar = new QScrollBar(Qt::Vertical, this);

    // 默认先隐藏
    m_vScrollBar->hide();

    m_vScrollBar->setStyleSheet(R"(
        QScrollBar:vertical {
            width: 10px;
            background: transparent;
            margin: 0px;
        }
        /* 滑块平时颜色 */
        QScrollBar::handle:vertical {
            background: rgba(180, 180, 180, 150);
            border-radius: 4px;
            min-height: 80px;
        }
        /* 滑块 Hover 颜色 (加深一点) */
        QScrollBar::handle:vertical:hover {
            background: rgba(150, 150, 150, 200);
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: transparent; }
    )");

    connect(this->verticalScrollBar(), &QScrollBar::valueChanged,
            m_vScrollBar, &QScrollBar::setValue);

    connect(m_vScrollBar, &QScrollBar::valueChanged,
            this->verticalScrollBar(), &QScrollBar::setValue);

    //  5. 范围改变时，不再直接 setVisible，而是通过 updateScrollBarVisible 判断
    connect(this->verticalScrollBar(), &QScrollBar::rangeChanged,
            this, [=](int min, int max){
                m_vScrollBar->setRange(min, max);
                m_vScrollBar->setPageStep(this->verticalScrollBar()->pageStep());
                updateScrollBarVisible(); // 重新检查可见性
            });
}

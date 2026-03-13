#include "floatingscrollarea.h"
#include <QScrollBar>
#include <QResizeEvent>
#include <QEvent>
#include <QScroller>

FloatingScrollArea::FloatingScrollArea(QWidget *parent)
    : QScrollArea(parent), m_vScrollBar(nullptr), m_isHover(false)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setWidgetResizable(true);
    this->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    // [新增] 初始化动画对象
    // "scrollValue" 对应头文件中定义的 Q_PROPERTY
    m_scrollAnimation = new QPropertyAnimation(this, "scrollValue", this);
    // 缓动曲线：OutQuint (五次曲线) 是一种非常接近物理惯性的减速效果
    m_scrollAnimation->setEasingCurve(QEasingCurve::OutQuint);
    // 动画时长：400毫秒，这个时间越长，惯性感觉越重
    m_scrollAnimation->setDuration(700);

    initScrollBar();
}

// [新增] 属性 Setter: 动画实际上是在改变这个值
void FloatingScrollArea::setVerticalScrollValue(int value)
{
    if (this->verticalScrollBar()) {
        this->verticalScrollBar()->setValue(value);
    }
}

// [新增] 属性 Getter
int FloatingScrollArea::verticalScrollValue() const
{
    if (this->verticalScrollBar()) {
        return this->verticalScrollBar()->value();
    }
    return 0;
}

// [核心修改] 滚轮事件：计算目标 + 启动动画
void FloatingScrollArea::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() == 0) return;

    QScrollBar *vBar = this->verticalScrollBar();
    if (!vBar) return;

    // 1. 停止当前正在跑的动画（如果用户连续快速滚动，要打断上一次，接续下一次）
    if (m_scrollAnimation->state() == QAbstractAnimation::Running) {
        m_scrollAnimation->stop();
    }

    // 2. 计算滚动的距离
    // 这里设置步长，比如每次滚动 80 像素，配合动画会很舒服
    int scrollStep = 80;
    int delta = event->angleDelta().y();
    int direction = (delta > 0) ? -1 : 1; // >0 向上滚(值变小), <0 向下滚(值变大)

    // 3. 计算目标值
    // 起点应该是当前的"视觉位置" (vBar->value())
    int startValue = vBar->value();
    int targetValue = startValue + (direction * scrollStep);

    // 4. 边界限制 (防止目标值超出范围)
    targetValue = qBound(vBar->minimum(), targetValue, vBar->maximum());

    // 5. 如果已经在边界了，就不做动画了
    if (startValue == targetValue) return;

    // 6. 配置并启动动画
    m_scrollAnimation->setStartValue(startValue);
    m_scrollAnimation->setEndValue(targetValue);
    m_scrollAnimation->start();

    // 接受事件
    event->accept();
}


void FloatingScrollArea::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);

    if(!m_vScrollBar) return;

    // 1. 定义你想要的尺寸
    int barWidth = 10;      // 滚动条宽度
    int bottomMargin = 0;  //  核心参数：底部留白距离
    // 如果觉得还是越界，把这个数字改大（比如 30 或 40）

    // 2. 计算几何位置
    // X = 靠最右
    int x = this->width() - barWidth;

    // Y = 从顶端开始 (或者也可以设置 topMargin)
    int y = 0;

    // W = 固定宽度
    int w = barWidth;

    // H = 总高度 - 底部留白
    // 这就是解决"底部越界"的唯一真理：必须减去底部的 margin
    int h = this->height() - bottomMargin;

    // 安全检查：防止窗口极小时崩溃
    if (h < 0) h = 0;

    // 3. 应用设置
    m_vScrollBar->setGeometry(x, y, w, h);
    m_vScrollBar->raise();
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
            this, [=](int value){
                if (m_isSyncing) return;
                m_isSyncing = true;
                m_vScrollBar->setValue(value);
                m_isSyncing = false;
            });

    connect(m_vScrollBar, &QScrollBar::valueChanged,
            this, [=](int value){
                if (m_isSyncing) return;
                m_isSyncing = true;
                this->verticalScrollBar()->setValue(value);
                m_isSyncing = false;
            });

    //  5. 范围改变时，不再直接 setVisible，而是通过 updateScrollBarVisible 判断
    connect(this->verticalScrollBar(), &QScrollBar::rangeChanged,
            this, [=](int min, int max){
                m_vScrollBar->setRange(min, max);
                m_vScrollBar->setPageStep(this->verticalScrollBar()->pageStep());
                updateScrollBarVisible(); // 重新检查可见性
            });
}

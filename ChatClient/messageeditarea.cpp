#include "messageeditarea.h"

#include <QStyleOption>

#include <QPainter>

#include <QStyle>

#include <QPaintEvent>


MessageEditArea::MessageEditArea(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumHeight(147);
    this->setMaximumHeight(350);
    this->setObjectName("messageEidtArea");
}

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

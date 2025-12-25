#include "toolswidget.h"
#include <QLineEdit>

LineEditFocus::LineEditFocus(QWidget *parent)
{

}

LineEditFocus::~LineEditFocus()
{

}


void LineEditFocus::focusOutEvent(QFocusEvent *event)
{
    emit signalLoseFocus();
    QLineEdit::focusOutEvent(event);
    m_focus = false;
}

void LineEditFocus::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    m_focus = true;
}

#ifndef TOOLSWIDGET_H
#define TOOLSWIDGET_H

#include <QLineEdit>

class LineEditFocus : public QLineEdit {
    Q_OBJECT
public:
    explicit LineEditFocus(QWidget *parent = nullptr) ;
    ~LineEditFocus() override;

signals:
    void signalLoseFocus();


protected:
    // 焦点离开
    void focusOutEvent(QFocusEvent *event) override;

    // 焦点进入
    void focusInEvent(QFocusEvent *event) override;

    bool m_focus;

};

#endif // TOOLSWIDGET_H

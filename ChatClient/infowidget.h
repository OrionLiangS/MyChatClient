#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QFrame>

#include <QGraphicsDropShadowEffect>


class InfoWidget : public QDialog
{
    Q_OBJECT
public:
    InfoWidget(QWidget *parent, bool m_isModal = false);
    bool event(QEvent *event)override;
    void showEvent(QShowEvent *event) override; // 新增声明

protected:
    QFrame* mainFrame;
    bool m_isModal;
};

#endif // INFOWIDGET_H

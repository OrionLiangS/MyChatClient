#ifndef MESSAGEEDITAREA_H
#define MESSAGEEDITAREA_H

#include <QWidget>

class MessageEditArea : public QWidget
{
    Q_OBJECT
public:
    explicit MessageEditArea(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // MESSAGEEDITAREA_H

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

/**
 * @brief The MainWidget class
 * @details
 * 设计为单例类(懒汉加载模式)
 */
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    ~MainWidget();

public:
    static MainWidget* getInstance();

private:
    // 单例实例
    static MainWidget* instance;

    // 单例需要将构造私有化
    MainWidget(QWidget *parent = nullptr);

};
#endif // MAINWIDGET_H

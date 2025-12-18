#ifndef CHOOSEFRIENDWIDGET_H
#define CHOOSEFRIENDWIDGET_H

#include <QObject>

#include <QWidget>

#include "infowidget.h"

#include <QHBoxLayout>

#include <QLineEdit>

class ChooseFriendWidget : public InfoWidget
{
    Q_OBJECT
public:
    ChooseFriendWidget(QWidget* parent = nullptr);

private:
    QWidget* totalContainer;    ///< 左侧容器

    QLineEdit *totalContSearchEdit; ///< 左侧容器顶端搜索框

    // ----------------------------------

    QWidget* selectedContainer; ///< 右侧容器

    /**
     * @brief initTotalContainer - 初始化TotalContainer
     * @param mainLayout - 传入最外层的Layout
     * @details
     * 初始化TotalContainer(左侧container)
     */
    void initTotalContainer(QHBoxLayout* mainLayout);

    /**
     * @brief initSelectedContainer
     * @param mainLayout - 传入最外层的Layout
     * @details
     * 初始化SelectedContaine(右侧container)
     */
    void initSelectedContainer(QHBoxLayout* mainLayout);
};

#endif // CHOOSEFRIENDWIDGET_H

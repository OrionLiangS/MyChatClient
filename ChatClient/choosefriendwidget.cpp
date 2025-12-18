#include "choosefriendwidget.h"

#include <QWidget>

#include <QHBoxLayout>

#include <QVBoxLayout>

#include <QGridLayout>

#include "debug.h"

ChooseFriendWidget::ChooseFriendWidget(QWidget* parent)
    :InfoWidget(parent, true)
{
    // ==================
    // 创建最外层布局管理器
    // ==================
    QHBoxLayout *mainLayout = new QHBoxLayout(mainFrame);
    mainLayout->setContentsMargins(0,10,0,10);
    mainLayout->setSpacing(0);

    // =============================
    // 初始化左右侧container
    // =============================
    initTotalContainer(mainLayout);
    initSelectedContainer(mainLayout);

#if TEST_UI
    totalContainer->setStyleSheet("background-color: #afd473;");
    selectedContainer->setStyleSheet("background-color: #aa83f1;");
#endif

}

void ChooseFriendWidget::initTotalContainer(QHBoxLayout *mainLayout)
{
    // =============================
    // 初始化布局
    // =============================
    totalContainer = new QWidget(mainFrame);
    mainLayout->addWidget(totalContainer);

    QVBoxLayout *vlayout = new QVBoxLayout(totalContainer);
    totalContainer->setLayout(vlayout);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(10);
    vlayout->setAlignment(Qt::AlignTop);

    // =============================
    // 创建顶端布局
    // =============================
    QWidget* totalContSearchWidget = new QWidget(totalContainer);
    // 固定大小
    totalContSearchWidget->setFixedHeight(60);

    // 创建搜索框的Layout布局
    QVBoxLayout* searchLayout = new QVBoxLayout(totalContSearchWidget);
    totalContSearchWidget->setLayout(searchLayout);
    searchLayout->setContentsMargins(20,0,20,0);
    searchLayout->setSpacing(0);
    searchLayout->setAlignment(Qt::AlignTop);


    // 初始化搜索框  - 此处搜索框的搜索是通过LineEdit的textChanged进行搜索(TODO - 暂不实现)
    totalContSearchEdit = new QLineEdit(totalContainer);
    totalContSearchEdit->setFixedHeight(35);
    searchLayout->addWidget(totalContSearchEdit);

    vlayout->addWidget(totalContSearchWidget);
}

void ChooseFriendWidget::initSelectedContainer(QHBoxLayout *mainLayout)
{
    selectedContainer = new QWidget(mainFrame);
    mainLayout->addWidget(selectedContainer);

}

#ifndef CHOOSEFRIENDWIDGET_H
#define CHOOSEFRIENDWIDGET_H

#include <QObject>
#include <QWidget>
#include "infowidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QScrollBar>
#include "floatingscrollarea.h"
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

/**
 * @brief The ChooseFriendWidget class 选择好友窗口
 * @details
 * 继承自 InfoWidget，通常作为模态对话框使用 (isModal=true)。
 * 界面主要分为左右两个区域：
 * - 左侧 (totalContainer): 展示所有待选好友列表，包含搜索框和自定义悬浮滚动区域。
 * - 右侧 (selectedContainer): 展示当前已选中的好友头像列表。
 */
class ChooseFriendWidget : public InfoWidget
{
    Q_OBJECT
public:
    /**
     * @brief ChooseFriendWidget 构造函数
     * @param parent 父窗口指针 (默认为 nullptr)
     */
    ChooseFriendWidget(QWidget* parent = nullptr);

private:
    // ============================================
    // 左侧区域控件 (Total Container)
    // ============================================

    /**
     * @brief totalContainer 左侧主容器
     * @details 包含搜索框和好友列表滚动区
     */
    QWidget* totalContainer;

    /**
     * @brief totalContSearchEdit 左侧顶部的搜索输入框
     */
    QLineEdit *totalContSearchEdit;

    /**
     * @brief totalScrollArea 左侧好友列表的滚动区域
     * @details 使用自定义的 FloatingScrollArea 实现悬浮滚动条效果
     */
    FloatingScrollArea *totalScrollArea;

    /**
     * @brief totalScrollWidget 滚动区域内部的实体容器
     * @details 所有的 ChooseFriendItem 都将添加到这个 Widget 的布局中
     */
    QWidget *totalScrollWidget;

    /**
     * @brief totalScrollVlayout 滚动内容容器的垂直布局管理器
     */
    QVBoxLayout *totalScrollVlayout;

    // ============================================
    // 右侧区域控件 (Selected Container)
    // ============================================

    /**
     * @brief selectedContainer 右侧容器
     * @details 用于展示已勾选的好友预览 包含标题, 好友列表与按钮组
     */
    QWidget* selectedContainer;

    FloatingScrollArea *selectedScrollArea;

    QWidget *selectedScrollWidget;

    QWidget *selectedBottomGropWidget;

    QPushButton *selectedFinishBtn;

    QPushButton *selectedCancelBtn;

    QLabel *selectedContTitleTag;

    QLabel *selectedContNumLabel;


    // ============================================
    // 初始化私有方法
    // ============================================

    /**
     * @brief initTotalContainer 初始化左侧容器布局
     * @param mainLayout 主窗口的水平布局管理器 (父布局)
     * @details 创建并组装左侧的搜索框、滚动区及列表容器
     */
    void initTotalContainer(QHBoxLayout* mainLayout);

    /**
     * @brief initSelectedContainer 初始化右侧容器布局
     * @param mainLayout 主窗口的水平布局管理器 (父布局)
     * @details 创建并组装右侧的已选好友展示区
     */
    void initSelectedContainer(QHBoxLayout* mainLayout);



    /**
     * @brief addItem 添加好友项
     * @param avatar 头像Icon
     * @param name 名字QString
     * @param isChecked 判断所传进的项是否为选中状态
     */
    void addItemFroTotalContainer(const QIcon &avatar, const QString &name, bool isChecked);

    void initSignalSlots();
};


/**
 * @brief The ChooseFriendItem class 好友选择列表中的单个条目
 * @details
 * 继承自 QPushButton，目的是为了让整个条目区域都能响应点击事件。
 * 包含三个主要部分：
 * 1. 复选框 (QCheckBox) - 显示选中状态
 * 2. 头像 (QPushButton) - 显示好友头像
 * 3. 昵称 (QLabel) - 显示好友名称
 */
class ChooseFriendItem : public QPushButton{
    Q_OBJECT
public:
    /**
     * @brief ChooseFriendItem 构造函数
     * @param avatar 好友头像图标
     * @param name 好友昵称
     * @param isChecked 初始选中状态
     * @param parent 父窗口指针
     */
    ChooseFriendItem(const QIcon &avatar, const QString &name, bool isChecked ,QWidget* parent = nullptr);

protected:
    /**
     * @brief clickHandler 点击事件处理函数
     * @details 响应整个 Item 被点击时的逻辑，通常用于反转 CheckBox 的选中状态
     */
    void clickHandler();

private:
    /**
     * @brief initObjectNames 初始化子控件对象名
     * @details 用于 QSS 样式控制
     */
    void initObjectNames();

    /**
     * @brief initSignalSlot 初始化信号槽连接
     * @details 连接点击信号到 clickHandler
     */
    void initSignalSlot();

    QCheckBox *chooseFriendCheckBox;   ///< 左侧复选框
    QPushButton *chooseFriendAvatar;   ///< 中间头像(作为按钮显示)
    QLabel *chooseFriendNikeName;      ///< 右侧昵称文本

};

#endif // CHOOSEFRIENDWIDGET_H

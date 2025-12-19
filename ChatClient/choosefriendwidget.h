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
     * @brief totalScrollWidget 左侧滚动区域内部的实体容器
     * @details 所有的 ChooseFriendItem 都将添加到这个 Widget 的布局中
     */
    QWidget *totalScrollWidget;

    /**
     * @brief totalScrollVlayout 左侧滚动内容容器的垂直布局管理器
     */
    QVBoxLayout *totalScrollVlayout;

    // ============================================
    // 右侧区域控件 (Selected Container)
    // ============================================

    /**
     * @brief selectedContainer 右侧容器
     * @details 用于展示已勾选的好友预览，包含标题、好友列表与按钮组
     */
    QWidget* selectedContainer;

    /**
     * @brief selectedScrollArea 右侧已选好友列表的滚动区域
     */
    FloatingScrollArea *selectedScrollArea;

    /**
     * @brief selectedScrollWidget 右侧滚动区域内部的实体容器
     * @details 所有的 SelectedFriendItem 都将添加到这个 Widget 的布局中
     */
    QWidget *selectedScrollWidget;

    /**
     * @brief selectedBottomGropWidget 右侧底部的按钮容器
     * @details 包含“完成”和“取消”按钮
     */
    QWidget *selectedBottomGropWidget;

    /**
     * @brief selectedFinishBtn “完成”按钮
     * @details 点击后确认选择并创建群聊
     */
    QPushButton *selectedFinishBtn;

    /**
     * @brief selectedCancelBtn “取消”按钮
     * @details 点击后关闭窗口
     */
    QPushButton *selectedCancelBtn;

    /**
     * @brief selectedContTitleTag 右侧标题文本 ("发起群聊")
     */
    QLabel *selectedContTitleTag;

    /**
     * @brief selectedContNumLabel 右侧已选人数统计文本 ("已选择人数...")
     */
    QLabel *selectedContNumLabel;

    /**
     * @brief selectedScrollVlayout 右侧滚动内容容器的垂直布局管理器
     */
    QVBoxLayout *selectedScrollVlayout;

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
     * @details 创建并组装右侧的已选好友展示区，包括标题栏、列表区和底部按钮
     */
    void initSelectedContainer(QHBoxLayout* mainLayout);

    // ============================================
    // 辅助方法
    // ============================================

    /**
     * @brief addItemFroTotalContainer 向左侧列表添加好友项
     * @param avatar 头像Icon
     * @param name 名字QString
     * @param isChecked 初始选中状态
     */
    void addItemFroTotalContainer(const QIcon &avatar, const QString &name, bool isChecked);

    /**
     * @brief addItemForSelectedContainer 向右侧列表添加已选好友项
     * @param avatar 头像Icon
     * @param name 名字QString
     */
    void addItemForSelectedContainer(const QIcon &avatar, const QString &name);

    /**
     * @brief initSignalSlots 初始化信号槽连接
     */
    void initSignalSlots();
};


/**
 * @brief The ChooseFriendItem class 好友选择列表中的单个条目 (左侧使用)
 * @details
 * 继承自 QPushButton，使得整个条目区域都能响应点击事件。
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
     * @details 响应整个 Item 被点击时的逻辑，用于反转 CheckBox 的选中状态
     */
    void clickHandler();

protected:
    /**
     * @brief initObjectNames 初始化子控件对象名
     * @details 用于 QSS 样式控制 (如 #chooseFriendAvatar, #chooseFriendCheckBox)
     */
    void initObjectNames();

    /**
     * @brief initSignalSlot 初始化信号槽连接
     * @details 连接 clicked 信号到 clickHandler
     */
    void initSignalSlot();

    /**
     * @brief ChooseFriendItemHLayout 水平布局管理器
     * @details 供子类访问以修改布局（如添加删除按钮）
     */
    QHBoxLayout *ChooseFriendItemHLayout;

    QCheckBox *chooseFriendCheckBox;   ///< 左侧复选框
    QPushButton *chooseFriendAvatar;   ///< 中间头像(作为按钮显示)
    QLabel *chooseFriendNikeName;      ///< 右侧昵称文本

};


/**
 * @brief The SelectedFriendItem class 已选好友列表中的单个条目 (右侧使用)
 * @details
 * 继承自 ChooseFriendItem，复用了头像和昵称的显示逻辑。
 * 差异点：
 * 1. 隐藏了 CheckBox。
 * 2. 增加了删除按钮 (SelectedFriendItemDelBtn)。
 * 3. 高度较小 (35px)。
 */
class SelectedFriendItem : public ChooseFriendItem{
    Q_OBJECT
public:
    /**
     * @brief SelectedFriendItem 构造函数
     * @param avatar 头像
     * @param name 昵称
     * @param parent 父窗口
     */
    SelectedFriendItem(const QIcon &avatar, const QString &name,  QWidget *parent);

protected:
    /**
     * @brief initSignalSlots 初始化信号槽
     * @details 连接删除按钮的点击事件
     */
    void initSignalSlots();

private:
    /**
     * @brief SelectedFriendItemDelBtn 删除按钮
     * @details 显示在条目最右侧，点击后将该好友从已选列表中移除
     */
    QPushButton *SelectedFriendItemDelBtn;
};

#endif // CHOOSEFRIENDWIDGET_H

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
#include <QMap>

class ChooseFriendItem;
class SelectedFriendItem;

/**
 * @brief The ChooseFriendWidget class 选择好友窗口
 * @details
 * 继承自 InfoWidget，作为模态对话框使用。
 * 实现左右联动选择逻辑：
 * - 左侧 (TotalContainer): 展示好友列表，Item 包含 CheckBox。
 * - 右侧 (SelectedContainer): 展示已选头像，Item 包含删除按钮。
 *
 * 核心逻辑：
 * 1. 左侧点击 -> 发出信号(带Item指针) -> Widget 根据指针创建/销毁右侧 Item。
 * 2. 右侧点击删除 -> 通过持有的左侧指针直接调用 setChecked(false) -> 触发上述第1步逻辑。
 */
class ChooseFriendWidget : public InfoWidget
{
    Q_OBJECT
public:
    /**
     * @brief ChooseFriendWidget 构造函数
     * @param parent 父窗口指针
     */
    explicit ChooseFriendWidget(QWidget* parent = nullptr);

    /**
     * @brief getSelectedUserIds 获取当前选中的所有用户ID
     * @return 用户ID列表
     * @details 遍历内部映射表，收集所有已选左侧 Item 的 ID。
     */
    QList<QString> getSelectedUserIds() const;

private:
    // ============================================
    // 核心联动数据
    // ============================================

    /**
     * @brief m_selectedMap 左右 Item 映射表
     * @details Key: 左侧 Item 指针 (本体); Value: 右侧 Item 指针 (影子)。
     * 用于在左侧取消选中时，能 O(1) 找到右侧对应的条目进行删除。
     */
    QMap<ChooseFriendItem*, SelectedFriendItem*> m_selectedMap;

    // ============================================
    // UI 组件声明
    // ============================================

    // 左侧容器组件
    QWidget* totalContainer;
    QLineEdit *totalContSearchEdit;
    FloatingScrollArea *totalScrollArea;
    QWidget *totalScrollWidget;
    QVBoxLayout *totalScrollVlayout;

    // 右侧容器组件
    QWidget* selectedContainer;
    FloatingScrollArea *selectedScrollArea;
    QWidget *selectedScrollWidget;
    QVBoxLayout *selectedScrollVlayout;

    // 底部与标题组件
    QWidget *selectedBottomGropWidget;
    QPushButton *selectedFinishBtn;
    QPushButton *selectedCancelBtn;
    QLabel *selectedContTitleTag;
    QLabel *selectedContNumLabel;

    // ============================================
    // 初始化方法
    // ============================================

    void initTotalContainer(QHBoxLayout* mainLayout);
    void initSelectedContainer(QHBoxLayout* mainLayout);

    /**
     * @brief addItemFroTotalContainer 向左侧列表添加好友
     * @param userId 用户唯一ID
     * @param avatar 头像
     * @param name 昵称
     */
    void addItemFroTotalContainer(const QString& userId, const QIcon &avatar, const QString &name);

    void initSignalSlots();

private slots:
    /**
     * @brief onItemStatusChanged 处理左侧 Item 状态变化的槽函数
     * @param item 发出信号的左侧 Item 指针
     * @details
     * - 若 item 被选中: new SelectedFriendItem(item), 存入 Map, 加到右侧布局。
     * - 若 item 被取消: 从 Map 取出对应的 SelectedFriendItem, delete 掉。
     */
    void onItemStatusChanged(ChooseFriendItem* item);

signals:
    void signalFinished(const QList<QString>& ids);
};


/**
 * @brief The ChooseFriendItem class 左侧好友列表项
 * @details
 * 包含复选框、头像、昵称，并存储 UserID。
 * 点击时改变 CheckBox 状态，并发射信号通知 Widget。
 */
class ChooseFriendItem : public QPushButton{
    Q_OBJECT
public:
    /**
     * @brief ChooseFriendItem 构造函数
     * @param userId 用户ID
     * @param avatar 头像
     * @param name 昵称
     * @param isChecked 初始选中状态
     * @param parent 父窗口
     */
    ChooseFriendItem(const QString& userId, const QIcon &avatar, const QString &name, bool isChecked ,QWidget* parent = nullptr);

    /**
     * @brief getIcon 获取头像 (供子类复用)
     */
    QIcon getIcon() const;

    /**
     * @brief getName 获取昵称 (供子类复用)
     */
    QString getName() const;

    /**
     * @brief getUserId 获取用户ID (用于业务逻辑)
     */
    QString getUserId() const;

    /**
     * @brief setChecked 设置选中状态 (公开接口)
     * @param checked 是否选中
     * @details 供右侧 SelectedFriendItem 调用。调用此函数会修改 CheckBox 状态，
     * 进而触发信号，形成删除闭环。
     */
    void setChecked(bool checked);

    /**
     * @brief isChecked 获取当前 CheckBox 的选中状态
     */
    bool isChecked() const;

signals:
    /**
     * @brief signalStatusChanged 状态改变信号
     * @param item 指向自身的指针 (this)
     * @details 当 CheckBox 状态发生变化时(无论是点击还是代码修改)触发。
     */
    void signalStatusChanged(ChooseFriendItem* item);

protected:
    /**
     * @brief clickHandler 内部点击处理
     * @details 响应 QPushButton 的 clicked，去切换内部 CheckBox 的状态。
     */
    void clickHandler();

    void initObjectNames();
    void initSignalSlot();

    QHBoxLayout *ChooseFriendItemHLayout;
    QCheckBox *chooseFriendCheckBox;
    QPushButton *chooseFriendAvatar;
    QLabel *chooseFriendNikeName;

    QString m_userId; ///< 存储用户ID
};


/**
 * @brief The SelectedFriendItem class 右侧已选列表项
 * @details
 * 继承自 ChooseFriendItem，本质是左侧 Item 的“关联副本”。
 * 构造时接收左侧 Item 的指针，点击删除按钮时通过该指针修改左侧状态。
 */
class SelectedFriendItem : public ChooseFriendItem{
    Q_OBJECT
public:
    /**
     * @brief SelectedFriendItem 构造函数
     * @param item 左侧本体的指针 (必须有效)
     * @param parent 父窗口
     * @details 直接从 item 获取头像、名字进行初始化，并保存 item 指针。
     */
    SelectedFriendItem(ChooseFriendItem* item, QWidget* parent);

private:
    /**
     * @brief initSignalSlots 初始化删除逻辑
     */
    void initSignalSlots();

    /**
     * @brief m_refItem 左侧本体指针
     * @details "遥控器"：用于在右侧点击删除时，操作左侧 Item 取消勾选。
     */
    ChooseFriendItem* m_refItem;

    QPushButton *SelectedFriendItemDelBtn;
};

#endif // CHOOSEFRIENDWIDGET_H

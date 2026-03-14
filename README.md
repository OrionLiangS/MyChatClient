# MyChatSystemClient

#### 介绍
微服务即时通讯系统 - 客户端

#### 软件架构

基于 Qt6 / C++ 的即时通讯客户端，采用单例主窗口 + 三栏布局（左导航栏、中列表区、右聊天区）。UI 层使用 QSS 全局样式表 + 部分硬编码 StyleSheet。项目使用 CMake 构建。

---

### 文件与类功能速查

#### 入口与全局

| 文件 | 说明 |
|------|------|
| `main.cpp` | 程序入口。加载全局 QSS 样式表，获取 `MainWidget` 单例并显示。 |
| `debug.h` | 调试开关宏（`TEST_UI`、`TEST_GROUP_CHAT`、`TEST_PRIVATE_CHAT`），控制是否加载 Mock 测试数据。 |

#### 数据模型 (`model/data.h`)

整个项目的数据层，全部在 `namespace model` 中。

| 内容 | 说明 |
|------|------|
| `LOG()` 宏 | 带文件名和行号的日志输出工具。 |
| `formatTime()` / `getTime()` | 时间戳格式化与获取的工具函数。 |
| `makeIcon()` | 将 `QByteArray` 二进制数据转为 `QIcon`。 |
| `loadFileToByteArray()` / `writeFileToByteArray()` | 文件读写工具函数。 |
| `MessageType` 枚举 | 消息类型：文本、图片、文件、语音、未知。 |
| `UserInfo` 类 | 用户信息实体（昵称、手机号、userId、签名、头像）。 |
| `Message` 类 | 单条消息实体，包含工厂方法 `makeMessage()` 按类型构造消息。 |
| `ChatSessionInfo` 类 | 会话信息实体（会话ID、最新消息、会话名称、头像）。 |

#### 工具类

| 文件 | 类 | 说明 |
|------|-----|------|
| `toolswidget.h/.cpp` | `LineEditFocus` | 工具控件。继承 `QLineEdit`，增加了焦点进入/离开的信号 `signalLoseFocus`，供其他组件监听输入框失焦事件（如群名称编辑框失焦时触发保存确认）。 |

#### 主窗口

| 文件 | 类 | 说明 |
|------|-----|------|
| `mainwidget.h/.cpp` | `MainWidget` | 单例主窗口。三栏布局：左侧导航栏（头像 + 会话/好友/申请三个 Tab 按钮）、中间列表区（搜索框 + `SessionFriendArea`）、右侧聊天区（标题栏 + 消息展示区 + 消息编辑区 + 侧边抽屉）。 |
| | | `initLeftWindow()` — 初始化左侧导航栏（头像、Tab 按钮）。 |
| | | `initMidWindow()` — 初始化中间搜索框和列表滚动区。 |
| | | `initRightWindow()` — 初始化右侧聊天框、消息展示/编辑区、抽屉侧边栏。 |
| | | `switchTabToSession/Friend/Apply()` — 切换三个标签页，更新图标并加载对应列表。 |
| | | `loadSessionList/FriendList/ApplyList()` — 加载列表数据（目前为 TODO）。 |
| | | `onSearchEditReturnPressed()` — 搜索框回车时打开添加好友弹窗并自动搜索。 |

#### 弹窗基类

| 文件 | 类 | 说明 |
|------|-----|------|
| `infowidget.h/.cpp` | `InfoWidget` | 所有弹窗的基类。继承 `QDialog`，无边框 + 透明背景 + 圆角阴影容器 `mainFrame`。支持模态（居中显示）和非模态（鼠标位置弹出，失焦自动关闭）两种模式。`SelfInfoWidget`、`UserInfoWidget`、`AddFriendDialog`、`ChooseFriendWidget` 均继承自它。 |

#### 个人信息 / 用户信息弹窗

| 文件 | 类 | 说明 |
|------|-----|------|
| `selfinfowidget.h/.cpp` | `SelfInfoWidget` | 当前登录用户的个人信息弹窗（非模态）。点击左侧头像弹出。展示并支持编辑：昵称、ID、个性签名、手机号（含验证码）。每个字段都有"展示/编辑/提交"三态切换逻辑。 |
| `userinfowidget.h/.cpp` | `UserInfoWidget` | 其他用户的信息卡片弹窗（非模态）。点击消息区头像弹出。展示对方昵称、ID、手机号，底部有"申请好友"、"发消息"、"删除好友"三个操作按钮。 |

#### 添加好友弹窗

| 文件 | 类 | 说明 |
|------|-----|------|
| `addfrienddialog.h/.cpp` | `AddFriendDialog` | 添加好友的模态弹窗。包含搜索框和 `QStackedWidget`（空白页/错误页/结果卡片页）。 |
| | | `doSearch()` — 执行搜索（目前为 Mock 数据，TODO 接入网络请求）。 |
| | | `setSearchInfo()` — 根据搜索结果判断关系（自己/好友/陌生人）并刷新 UI。 |
| | | `setSearchKeyword()` — 外部接口，设置搜索框文本（供主窗口搜索框联动调用）。 |
| | `FriendResultCard` | 搜索结果卡片组件。展示头像、昵称、ID，以及根据关系状态显示/隐藏的按钮组（添加好友、删除好友、发消息）。 |

#### 选择好友窗口（建群用）

| 文件 | 类 | 说明 |
|------|-----|------|
| `choosefriendwidget.h/.cpp` | `ChooseFriendWidget` | 选择好友的模态弹窗（用于发起群聊时选人）。左右双栏联动：左侧好友列表带 CheckBox，右侧已选列表带删除按钮。通过 `m_selectedMap` 映射表实现 O(1) 的左右同步增删。 |
| | | `onItemStatusChanged()` — 核心槽函数，处理左侧勾选/取消时同步增删右侧条目。 |
| | | `getSelectedUserIds()` — 获取所有已选用户 ID 列表。 |
| | `ChooseFriendItem` | 左侧好友列表项（CheckBox + 头像 + 昵称）。 |
| | `SelectedFriendItem` | 右侧已选列表项，持有左侧 Item 的指针，点击删除时"遥控"左侧取消勾选。 |

#### 会话/好友/申请列表区

| 文件 | 类 | 说明 |
|------|-----|------|
| `sessionfriendarea.h/.cpp` | `SessionFriendArea` | 中间栏的滚动列表区域（继承 `QScrollArea`）。通过 `addItem()` 按类型添加不同的 Item。 |
| | | `clear()` — 清空所有列表项。 |
| | | `clickItem(index)` — 以代码方式选中指定索引的 Item。 |
| | `SessionFriendItem` | 列表项基类。包含头像、名字、文本标签。实现了点击选中高亮（`select()`）和虚函数 `active()` 供子类重写业务逻辑。 |
| | `SessionItem` | 会话列表项。`active()` 中处理点击会话的逻辑（TODO）。 |
| | `FriendItem` | 好友列表项。`active()` 中处理点击好友的逻辑（TODO）。 |
| | `ApplyItem` | 好友申请列表项。去掉了文本标签，替换为"接受"和"拒绝"按钮。 |

#### 消息展示区

| 文件 | 类 | 说明 |
|------|-----|------|
| `messageshowarea.h/.cpp` | `MessageShowArea` | 消息展示区（继承 `QScrollArea`）。 |
| | | `addMessage()` — 尾部插入一条消息。 |
| | | `addFrontMessage()` — 头部插入（用于加载历史消息）。 |
| | | `clearMessage()` — 清空所有消息。 |
| | `MessageItem` | 单条消息的容器。工厂方法 `makeMessageItem()` 根据消息类型创建对应内容（目前仅实现文本类型）。左右布局根据 `isLeft` 区分自己和对方。点击头像弹出 `UserInfoWidget`。 |
| | `MessageContentLabel` | 文本消息气泡。自绘圆角矩形 + 小尖角，支持自动换行和根据父控件宽度自适应大小。 |

#### 消息编辑区

| 文件 | 类 | 说明 |
|------|-----|------|
| `messageeditarea.h/.cpp` | `MessageEditArea` | 消息输入区域。包含工具栏（发送图片/文件/语音/历史消息按钮）、`QTextEdit` 文本编辑框、发送按钮。 |
| | | `enableSendMessageBtn()` — 监听文本变化，输入框为空时禁用发送按钮，有内容时启用。 |

#### 会话详情页（右侧抽屉内容）

| 文件 | 类 | 说明 |
|------|-----|------|
| `chatdetailspage.h/.cpp` | `ChatDetailsPage` | 会话详情页基类（抽象工厂）。`createChatDetailsPage()` 工厂方法根据类型创建群聊或单聊详情页。 |
| | `AvatarItem` | 详情页中的头像组件（圆形头像 + 昵称标签），昵称过长自动省略。 |
| | `GroupChatDetailsPage` | 群聊详情页。包含：搜索群成员、4列头像网格（带 [+] 添加和 [-] 移除按钮）、群名称（支持点击编辑 + 失焦确认弹窗）、群公告、清空聊天记录、退出群聊。 |
| | `PrivateChatDetailsPage` | 单聊详情页。包含：对方头像 + 添加按钮、查找聊天记录、清空聊天记录、删除好友。 |

#### 侧边抽屉

| 文件 | 类 | 说明 |
|------|-----|------|
| `sidebar.h/.cpp` | `Sidebar` | 右侧滑出式抽屉容器。通过 `QPropertyAnimation` 实现滑入/滑出动画。`toggle()` 切换显示/隐藏。监听父窗口 Resize 事件自适应大小，点击抽屉外部区域自动收起。用于承载 `ChatDetailsPage`。 |

#### 自定义滚动区域

| 文件 | 类 | 说明 |
|------|-----|------|
| `floatingscrollarea.h/.cpp` | `FloatingScrollArea` | 自定义滚动区域（继承 `QScrollArea`）。实现悬浮式滚动条（鼠标悬停时显示，离开时隐藏）和惯性滚动动画（`QPropertyAnimation` + `OutQuint` 缓动曲线）。被 `ChooseFriendWidget`、`GroupChatDetailsPage` 等多处复用。 |


---

### Phase 2 — 待优化项

#### 1. 模态弹窗内存泄漏
`AddFriendDialog` 和 `ChooseFriendWidget` 以 `isModal=true` 构造，模态分支未设置 `WA_DeleteOnClose`，`close()` 只隐藏不销毁。每次打开都会泄漏一个实例。
- 修复：在模态分支构造中加 `setAttribute(Qt::WA_DeleteOnClose)`。

#### 2. 模态对话框系统提示音
Windows 下点击模态弹窗外部会触发系统"咚"提示音，这是操作系统对 `WindowModality` 的默认行为，Qt 无法关闭。
- 方案：将模态弹窗改为"伪模态" — 去掉 `setWindowModality()`，`exec()` 改为 `show()`，在父窗口上覆盖一层半透明遮罩 `QWidget` 挡住交互，视觉效果与模态一致但无系统提示音。
- 注意事项：
  - `exec()` 是阻塞的，`show()` 是非阻塞的。当前代码中 `exec()` 之后均无依赖返回值的逻辑（`ChooseFriendWidget` 通过 `signalFinished` 信号传递结果），因此改动不影响现有功能。
  - `InfoWidget` 继承自 `QDialog`，是独立顶层窗口；遮罩是主窗口的子控件。独立窗口天然在父窗口子控件之上，层级关系成立。
  - 若后续有人在 `show()` 之后写依赖弹窗结果的同步代码，需改为信号槽异步处理。

#### 3. 主窗口 QLineEdit 焦点问题
主窗口中间 `searchEdit` 点击空白区域不会失焦，原因是 `leftWindow`/`midWindow`/`rightWindow` 及其子控件默认 `focusPolicy` 为 `NoFocus`，点击时不会抢占焦点。
- 方案：在 `MainWidget` 构造中为三个窗口设置 `setFocusPolicy(Qt::ClickFocus)`，点击时自动抢占焦点使 `QLineEdit` 失焦。
- `LineEditFocus` 的 `signalLoseFocus` 信号可配合使用，在失焦时执行业务逻辑（如清空搜索框、收起结果等）。

#### 4. AddFriendDialog 中 LineEditFocus 未利用
`addFriendSearchEdit` 类型为 `LineEditFocus`，但未 connect `signalLoseFocus` 信号，失焦通知能力未被使用。后期可补充失焦逻辑。

#### 5. 子窗口滚动卡顿 — QGraphicsDropShadowEffect 性能问题
弹窗（`InfoWidget` 子类）内的 `FloatingScrollArea` 滚动时明显卡顿，主窗口左侧的 `SessionFriendArea` 则不卡。原因并非滚动条实现本身，而是子窗口的 `QGraphicsDropShadowEffect` 阴影效果：
- `QGraphicsDropShadowEffect` 通过离屏渲染实现 — 先将 `mainFrame` 画到临时缓冲区，再做高斯模糊生成阴影，最后合成到屏幕。
- 子窗口内任何控件重绘都可能触发阴影重新计算，滚动时每帧都在做模糊，是最大的性能杀手。
- 主窗口不卡是因为没有 `QGraphicsDropShadowEffect`，且滚动时只有一个区域需要重绘。

优化方案（参考微信 PC 端做法）：
- 微信使用九宫格阴影贴图 — 预先制作一张带阴影的圆角矩形 PNG，切成九宫格拉伸，渲染时只是一次 `drawPixmap`，零计算开销。
- 去掉 `InfoWidget` 中的 `QGraphicsDropShadowEffect`，改为以下任一方案：
  1. QSS `border-image` 九宫格阴影图片（最贴近微信做法，跨平台一致）。
  2. 在 `InfoWidget::paintEvent` 中手动绘制阴影并缓存到 `QPixmap`（`m_shadowCache`），窗口大小不变时只计算一次。弹窗都是 `setFixedSize` 的，所以阴影缓存只会生成一次。
  3. 简化为 `border: 1px solid rgba(0,0,0,30)` 淡边框代替阴影（最简单，几乎零开销，但视觉效果弱一些）。

#### 6. 控件数量对滚动性能的影响
大量子控件（如 `MessageShowArea` 中 30 条消息 × 每条 4-5 个控件 = 120-150 个控件）在滚动时全部参与布局计算和重绘，叠加阴影效果后更加明显。
- 长期方案：虚拟列表（只渲染可见区域控件，滚出视口的回收复用），类似 `QListView` + `QAbstractItemModel` + 自定义 `QStyledItemDelegate`。
- 短期缓解：减少 Item 内部控件层级，将头像+名字+气泡合并为单个自绘控件（`paintEvent` 绘制而非子控件拼接）；消息列表做懒加载，只加载可见范围附近的消息。

---


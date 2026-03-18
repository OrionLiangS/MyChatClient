#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>
#include <QWidget>

#include"data.h"

namespace model{

/**
 * @brief The DataCenter class 数据中心 (单例模式)
 * @details
 * 全局唯一的数据管理中心，负责集中管理客户端运行期间的所有核心业务数据。
 *
 * 设计要点:
 * 1. 单例模式: 通过 getInstance() 获取唯一实例，构造函数私有化。
 * 2. 指针成员: 大部分列表类型成员使用指针 (初始化为 nullptr)，
 *    用 nullptr 表示"服务端尚未返回有效数据"的无效状态，
 *    与"已返回但列表为空"做区分。
 * 3. Hash 成员: recentMessage / memberList / unReadMessageCount
 *    在构造时即实例化，通过 key 是否存在来判断数据有效性。
 *
 * 数据流向:
 *   NetClient (网络层) → DataCenter (数据层) → UI (展示层)
 *   NetClient 将服务端响应写入 DataCenter，UI 从 DataCenter 读取并渲染。
 */
class DataCenter : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getInstance 获取 DataCenter 单例
     * @return 指向唯一 DataCenter 实例的指针
     */
    static DataCenter *getInstance();

    ~DataCenter();

private:
    DataCenter(); ///< 私有构造函数 (单例模式)

    // ============================================
    // 单例实例
    // ============================================
    static DataCenter* instance;                ///< 静态单例指针 (初始化为 nullptr)

    // ============================================
    // 登录会话
    // ============================================
    QString loginSessionId;                     ///< 登录会话ID (服务端返回，后续请求携带做身份校验，可持久化实现免密登录)

    // ============================================
    // 当前用户信息
    // ============================================
    UserInfo *mySelf = nullptr;                 ///< 当前登录用户的基本信息

    // ============================================
    // 好友列表
    // ============================================
    QList<UserInfo> *friendList = nullptr;       ///< 好友信息列表 (nullptr 表示服务端尚未返回)

    // ============================================
    // 会话列表
    // ============================================
    QList<ChatSessionInfo> *chatSessionList = nullptr;  ///< 会话列表 (单聊 + 群聊，注意: 会话列表 ≠ 好友列表)
    QString currentChatSessionId = "";                  ///< 当前选中的会话ID (空串表示无选中会话)

    // ============================================
    // 群聊成员
    // ============================================
    QHash<QString, QList<UserInfo>>* memberList = nullptr;  ///< 群聊成员列表 (Key: chatSessionId, 构造时即实例化)

    // ============================================
    // 好友申请
    // ============================================
    QList<UserInfo>* applyList = nullptr;        ///< 待处理的好友申请列表

    // ============================================
    // 消息相关
    // ============================================
    QHash<QString, QList<Message>>* recentMessage = nullptr;    ///< 各会话的最近消息列表 (Key: chatSessionId, 构造时即实例化)
    QHash<QString, int> *unReadMessageCount = nullptr;          ///< 各会话的未读消息计数 (Key: chatSessionId, 可持久化存储)

    // ============================================
    // 搜索结果
    // ============================================
    QList<UserInfo> *serchUserResult = nullptr;   ///< 好友搜索结果 (服务端模糊搜索返回，客户端做精确匹配二次过滤)
    QList<Message> *searchMessageResult = nullptr;///< 历史消息搜索结果列表

    // ============================================
    // 短信验证码
    // ============================================
    QString currentVerifyCodeId = "";             ///< 验证码凭证ID (第一次请求由服务端返回，第二次请求携带做校验关联)

signals:
};

} // end namespace model

#endif // DATACENTER_H

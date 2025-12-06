#pragma once

#include <QString>

#include <QIcon>

namespace model { // - namespace model

/**
 * @brief 消息类型枚举
 * @details
 * --------------------------------------
 * 1) 意图:
 * 枚举单条消息的具体内容类型。
 * 2) 类型说明:
 * - 基础类型: 文本
 * - 多媒体类型: 图片, 文件, 语音
 * - 错误处理: 未知类型
 * --------------------------------------
 **/
enum MessageType {
    TEXT_TYPE,      ///< 文本类型消息
    IMAGE_TYPE,     ///< 图片类型消息
    FILE_TYPE,      ///< 文件类型消息
    SPEECH_TYPE,    ///< 语音类型消息
    UNKNOWN_TYPE    ///< 未知/错误类型 (防御性默认值)
};



/**
 * @brief 用户信息实体类
 * @details
 * --------------------------------------
 * 1) 意图:
 * 用于在客户端和服务端之间传输用户基本信息
 * 包括：用户昵称, 用户ID, 头像, 电话, 签名
 * 2) QString 的 userId:
 * 采用QString对userId进行类型管理而非整形类型(int/long long)
 * 原因: 兼容分布式系统中的雪花算法或形成UUID形成策略, 若是使用整形类型可兼顾数据库的auto_increment主键自增, 若是使用整形, 数据库中对数据的管理也需要进行分库分表, 增加使用成本
 * 目的: 防止前端/客户端处理大量溢出并确保跨微服务的全局唯一性
 * --------------------------------------
 **/
class UserInfo{
public:

    // ==================================
    // 支持登录方式
    // ==================================
    QString nickname;      ///< 用户昵称
    QString phone;         ///< 用户手机号

    // ==================================
    // 唯一标识
    // ==================================
    QString userId;        ///< 用户唯一id

    // ==================================
    // 其他信息展示
    // ==================================
    QString description;   ///< 用户签名
    QIcon avatar;          ///< 用户头像

}; // UserInfo



/**
 * @brief 消息信息实体类
 * @details
 * --------------------------------------
 * 1) 意图:
 * 用于描述单条消息, 重点存储单条消息的元数据
 * 2) 成员说明:
 * * messageId: 单条消息id - 单条消息的唯一标识符
 * * chatSessionId: 会话id - 一条消息应该是具体属于某个会话的, 因此需要关联其对应的会话
 * * time: 消息时间 - 一条消息需要展示其所发送的时间, 通过格式化的方式进行对时间的展示
 * * messageType: 单条消息的类型 - 一条消息的类型有文本类型, 图片类型, 文件类型以及语音类型 @see MessageType
 * * sender: 消息的发送者 - 一条消息发送至本端时需要获取发送者的元数据
 * * content: 消息具体内容 - 消息内容采用QByteArray的方式存储, 主要是因为其类型之多, 需要兼容纯文本以及各种文件;
 * - 若是采用QString进行存储, 那么无法存储二进制的内容(Qt对字节与字符有严格判定, 当涉及到字节则使用QByteArray, 字符则使用QString)
 * * fileId: 文件的身份标识 - 当消息类型为文件/图片/语音的时候有效, 当消息类型为文本时则为""空串
 *  - 这种方式主要解决大量文件类型消息时的带宽减负, 当识别到对应的消息类型时, 将会通过对应的消息类型构造对应消息类型的占位
 *  - 当用户点击该内容时, 再次向服务器发起请求该文件的具体内容
 * * fileName: 文件名 - 只有当文件类型为FILE_TYPE (@see MessageType) 时才有效, 图片与语音同样为文件消息, 但并不需要展示文件名
 * --------------------------------------
 **/
class Message{
public:

    // ==================================
    // 核心标识
    // ==================================
    QString messageId;          ///< 消息id(消息唯一标识符)
    QString chatSessionId;      ///< 会话id

    // ==================================
    // 消息元数据
    // ==================================
    QString time;               ///< 消息时间
    MessageType messageType;    ///< 消息类型
    UserInfo sender;            ///< 发送者的信息

    // ==================================
    // 消息内容
    // ==================================
    QByteArray content;         ///< 消息具体内容

    // ==================================
    // 文件消息类型
    // ==================================
    QString fileId;             ///< 文件标识(文件/图片/语音)
    QString fileName;           ///< 文件名称(纯文件类型)


    /**
     * @brief 消息工厂类
     * @details
     * --------------------------------------
     * 1) 意图:
     * 通过工厂模式来构造一个对应的消息类对象
     * 2) 参数:
     * @param messageType - 消息类型
     * @param chatSessionId - 会话id
     * @param sender - 发送者
     * @param content - 消息内容
     * @param extraInfo - 当消息为文件消息时作为文件名补充
     * 3) 返回值:
     * @return 返回一个构造好的 Message 对象
     **/
    static Message makeMessage(MessageType messageType, const QString& chatSessionId,
                               const UserInfo& sender, const QByteArray& content,
                                const QString& extraInfo ){
        switch(messageType){
        case TEXT_TYPE:
            return makeTextMessage(chatSessionId, sender, content);
        case IMAGE_TYPE:
            return makeImageMessage(chatSessionId, sender, content);
        case FILE_TYPE:
            return makeFileMessage(chatSessionId, sender, content, extraInfo);
        case SPEECH_TYPE:
            return makeSpeechMessage(chatSessionId, sender, content);
        }
    }

private:
    // -------------------------------------------------------------------------
    // 内部构造辅助函数 (Private Helpers)
    // -------------------------------------------------------------------------

    // 构造messageId生成器
    /**
     * @brief makeId
     * @return 返回QString类型的messageId
     */
    static QString makeId(){
        /**
         * @todo
         */
        return "";
    }

    // 构造常规消息
    static Message makeTextMessage(const QString&chatSessionId, const UserInfo&sender, const QByteArray&content){
        Message message;
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.fileId="";
        message.messageType = TEXT_TYPE;

        /**
         * @todo
         */
        message.time = ;
        message.messageId = makeId();

    }

    // 构造图片消息
    static Message makeImageMessage(const QString&chatSessionId, const UserInfo&sender, const QByteArray&content){
        /**
         * @todo
         */
    }

    // 构造文件消息
    static Message makeFileMessage(const QString&chatSessionId, const UserInfo&sender, const QByteArray&content, const QString&extraInfo){
        /**
         * @todo
         */
    }

    // 构造语音消息
    static Message makeSpeechMessage(const QString&chatSessionId, const UserInfo&sender, const QByteArray&content){
        /**
         * @todo
         */
    }

}; // Message


/**
 * @brief 会话信息实体类
 * @details
 * --------------------------------------
 * 1) 意图:
 * 存储会话的元数据与摘要信息
 * - 主要渲染左侧会话列表的内容
 * - 同时为会话聊天框提供标题, 图标等基础信息, 并不包含聊天记录
 * 2) 成员说明:
 * * chatSessionId: 会话ID - 标识单个会话以确保会话唯一性
 * * lastMessage: 当前会话的最新消息 - 该条消息将会展示在会话列表中对应会话的条目中以能够对最新的一条消息进行可视化
 * * chatSessionName: 会话昵称 - 用于标识会话列表中对该回话的标识以及会话框中会话的标题, 单聊为对方昵称, 群聊为群聊名称
 * * avatar: 会话头像 - 如果为单聊, 头像为对方的头像, 如果是群聊, 头像则是群聊的头像
 * * userId: 用户id(关联ID) - 如果是单聊会话, userId表示为对方用户的id, 若是群聊则设为"", 后续通过其他方式将完整的用户id列拿到
 * --------------------------------------
 **/
class ChatSessionInfo{
public:
    // ==================================
    // 唯一标识
    // ==================================
    QString chatSessionId;      ///< 标识会话的唯一性

    // ==================================
    // 其他元数据
    // ==================================
    Message lastMessage;        ///< 当前会话中最新的消息
    QString chatSessionName;    ///< 会话昵称(单聊/群聊)
    QIcon avatar;               ///< 会话头像
    QString userId;             ///< 用户id

}; // ChatSessionInfo


} // - end namespace model

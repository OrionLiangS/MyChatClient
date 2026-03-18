#pragma once

#include <QString>

#include <QIcon>

#include <QDebug>

#include <QUuid>

#include <QDateTime>

#include <QIcon>

#include <QPixmap>

#include <QFile>

#include <QFileInfo>


#include "base.qpb.h"  // Protobuf 自动生成的头文件 (由 qt_add_protobuf 根据 base.proto 生成)

namespace model { // - namespace model


////////////////////////////////////////////////////////////////////
/// 工具类宏 - 日志相关
////////////////////////////////////////////////////////////////////

/**
 * @brief getFileName
 * @param path 传入一个 __FILE__ 宏作为对应的路径
 * @return 返回一个 QString 的 fileName 文件名
 * @details
 * 为 Log() 宏通过QFileInfo获取对应的文件名
 * 不能直接在getFileName中使用__FILE__的原因是
 * 若是在该函数中直接使用__FILE__, 那么获取到的path永远是data.h
 */
inline static QString getFileName(const QString& path){
    return QFileInfo(path).fileName();
}

// 为LOG()获取TAG标签
#define TAG QString("[%1: %2]").arg(model::getFileName(__FILE__), QString::number(__LINE__))

// 日志 - noquote() 为QDebug打印时不使用引号
#define LOG() qDebug().noquote() << TAG


////////////////////////////////////////////////////////////////////
/// 工具类函数
////////////////////////////////////////////////////////////////////

/**
 * @brief formatTime
 * @details
 * 工具类函数 - 用于生成格式化的QString的时间
 * 采用inline或static避免在链接阶段出现函数重定义问题
 * @return 格式化的时间
 */
static inline QString formatTime(int64_t timestamp){
    // 将时间戳转化为时间
    QDateTime datetime = QDateTime::fromSecsSinceEpoch(timestamp);
    // 格式化显示时间
    return datetime.toString("MM-dd HH:mm:ss");
}

/**
 * @brief getTime
 * @details
 * 工具类函数 - 获取一个秒级的时间戳(为formatTime()所准备)
 * @return 返回一个秒级的时间戳
 */
static inline int64_t getTime(){
    return QDateTime::currentSecsSinceEpoch();
}

/**
 * @brief makeIcon
 * @details
 * 将二进制数据转化为Icon图标
 * @param byteArray 传入一个 QByteArray 二进制数据对象
 * @return 返回一个QIcon 对象
 */
static inline QIcon makeIcon(const QByteArray& byteArray){
    // 通过QPixmap读取QByteArray数据
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);
    // 通过QPixmap来构造一个QIcon对象并返回
    return QIcon(pixmap);
}

/**
 * @brief loadFileToByteArray
 * @details
 * 工具类函数 读文件操作 - 将某个文件中的内容以二进制的方式进行读取并返回
 * @param path 传入一个需要读取的文件路径
 * @return 返回一个QByteArray对象
 */
static inline QByteArray loadFileToByteArray(const QString &path){
    QFile file(path);
    // 以只读的方式进行打开
    bool ok  = file.open(QFile::ReadOnly);
    // 判断打开失败
    if(!ok){
        LOG()<<"file open failed";
        return QByteArray();
    }
    // 读取文件内容保存为QByteArray
    QByteArray content = file.readAll();
    // 关闭文件
    file.close();
    return content;
}



/**
 * @brief writeFileToByteArray
 * @details
 * 工具类函数 写文件操作 - 将对应的内容写到对应路径上的文件
 * @param path 传入一个需要写入文件的路径
 * @param content 需要写入的内容
 */
static inline void writeFileToByteArray(const QString &path, const QByteArray& content){
    QFile file(path);
    // 以只写方式打开
    bool ok = file.open(QFile::WriteOnly);
    // 判断打开失败
    if(!ok){
        LOG()<<"file open failed";
        return;
    }
    // 将内容写至对应的文件中
    file.write(content);
    // 刷盘操作(刷新缓冲区)
    file.flush();
    // 关闭文件
    file.close();
}




////////////////////////////////////////////////////////////////////
/// 消息类型枚举
////////////////////////////////////////////////////////////////////


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



////////////////////////////////////////////////////////////////////
/// 用户信息实体类
////////////////////////////////////////////////////////////////////


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
    QString nickname = "";      ///< 用户昵称
    QString phone = "";         ///< 用户手机号

    // ==================================
    // 唯一标识
    // ==================================
    QString userId = "";        ///< 用户唯一id

    // ==================================
    // 其他信息展示
    // ==================================
    QString description = "";   ///< 用户签名
    QIcon avatar;          ///< 用户头像

    /**
     * @brief load 从 Protobuf 的 UserInfo 对象加载数据到当前实例
     * @param userInfo Protobuf 生成的 my_im::UserInfo 对象
     * @details
     * 将服务端返回的 Protobuf 用户信息逐字段映射到客户端的 model::UserInfo。
     * 头像字段: 若服务端返回的 avatar 为空 (未设置头像)，则使用默认头像；
     *          否则通过 makeIcon() 将二进制数据转为 QIcon。
     */
    void load(const my_im::UserInfo& userInfo){
        this->userId = userInfo.userId();
        this->nickname = userInfo.nickname();
        this->phone = userInfo.phone();
        this->description = userInfo.description();

        if(userInfo.avatar().isEmpty()){
            this->avatar = QIcon(":/resource/image/defaultAvatar.png");
        }else{
            this->avatar = makeIcon(userInfo.avatar());
        }
    }

}; // UserInfo




////////////////////////////////////////////////////////////////////
/// 消息信息实体类
////////////////////////////////////////////////////////////////////


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
    QString messageId = "";          ///< 消息id(消息唯一标识符)
    QString chatSessionId = "";      ///< 会话id

    // ==================================
    // 消息元数据
    // ==================================
    QString time;               ///< 消息时间
    MessageType messageType = TEXT_TYPE;    ///< 消息类型
    UserInfo sender;            ///< 发送者的信息

    // ==================================
    // 消息内容
    // ==================================
    QByteArray content;         ///< 消息具体内容

    // ==================================
    // 文件消息类型
    // ==================================
    QString fileId = "";             ///< 文件标识(文件/图片/语音)
    QString fileName = "";           ///< 文件名称(纯文件类型)


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
        case UNKNOWN_TYPE:
            LOG()<<"unknow type message";
            return Message();
        }
        return Message();

    }


    /**
     * @brief load 从 Protobuf 的 MessageInfo 对象加载数据到当前实例
     * @param messageInfo Protobuf 生成的 my_im::MessageInfo 对象
     * @details
     * 将服务端返回的 Protobuf 消息信息映射到客户端的 model::Message。
     * 解析流程:
     *   1. 提取消息元数据 (messageId, chatSessionId, timestamp, sender)
     *   2. 根据 messageType 分支解析对应的消息体:
     *      - STRING: 直接取 content 文本
     *      - IMAGE:  通过 hasFileId/hasImageContent 判断字段是否存在
     *                (实时转发时两者都有，历史拉取时只有 fileId)
     *      - FILE:   同上，额外提取 fileName
     *      - SPEECH: 同上
     *   3. optional 字段必须先用 has*() 判断再取值，避免读到默认空值
     */
    void load(const my_im::MessageInfo &messageInfo){
        this->messageId = messageInfo.messageId();
        this->chatSessionId = messageInfo.chatSessionId();
        this->time = formatTime(messageInfo.timestamp());
        this->sender.load(messageInfo.sender());

        auto type = messageInfo.message().messageType();
        if(my_im::MessageTypeGadget::MessageType::STRING == type){
            this->messageType = TEXT_TYPE;
            this->content = messageInfo.message().stringMessage().content().toUtf8();
        }
        else if(my_im::MessageTypeGadget::MessageType::IMAGE == type){
            this->messageType = IMAGE_TYPE;
            if(messageInfo.message().imageMessage().hasFileId()){
                this->fileId = messageInfo.message().imageMessage().fileId();
            }
            if(messageInfo.message().imageMessage().hasImageContent()){
                this->content = messageInfo.message().imageMessage().imageContent();
            }
        }
        else if(my_im::MessageTypeGadget::MessageType::FILE == type){
            this->messageType = FILE_TYPE;
            if(messageInfo.message().fileMessage().hasFileId()){
                this->fileId = messageInfo.message().fileMessage().fileId();
            }
            if(messageInfo.message().fileMessage().hasFileContents()){
                this->content = messageInfo.message().fileMessage().fileContents();
            }
        }
        else if(my_im::MessageTypeGadget::MessageType::SPEECH == type){
            this->messageType = SPEECH_TYPE;
            if(messageInfo.message().speechMessage().hasFileId()){
                this->fileId = messageInfo.message().speechMessage().fileId();
            }
            if(messageInfo.message().speechMessage().hasFileContents()){
                this->content = messageInfo.message().speechMessage().fileContents();
            }
        }
        else{
            LOG()<<"Unknow messageType for \"Message::load\"";
        }

}

private:
    // -------------------------------------------------------------------------
    // 内部构造辅助函数 (Private Helpers)
    // -------------------------------------------------------------------------

    // 构造messageId生成器
    /**
     * @brief makeId
     * @details
     * 通过QUuid::createUuid创建对应的uuid
     * 同时为了增加可读性 对uuid进行截取并加上'M'表示是一个message的uuid
     * @return 返回QString类型的messageId
     */
    static QString makeId(){
        return 'M'+QUuid::createUuid().toString().sliced(25, 12);
    }

    // 构造常规消息
    static Message makeTextMessage(const QString&chatSessionId, const UserInfo&sender, const QByteArray&content){
        // 创建类
        Message message;
        // 进行赋值
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.messageType = TEXT_TYPE;
        // 获取格式化时间
        message.time = formatTime(getTime());
        // 创建message唯一标识符
        message.messageId = makeId();
        message.fileId="";
        message.fileName = "";
        return message;
    }

    // 构造图片消息
    static Message makeImageMessage(const QString&chatSessionId, const UserInfo&sender, const QByteArray&content){
        // 创建类
        Message message;
        // 进行赋值
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.messageType = IMAGE_TYPE;
        // 获取格式化时间
        message.time = formatTime(getTime());
        // 创建message唯一标识符
        message.messageId = makeId();
        // 先设置为空 后期根据需求再进行设置
        message.fileId="";
        // 非FILE_TYPE
        message.fileName = "";
        return message;
    }

    // 构造文件消息
    static Message makeFileMessage(const QString&chatSessionId, const UserInfo&sender, const QByteArray&content, const QString&extraInfo){
        // 创建类
        Message message;
        // 进行赋值
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.messageType = FILE_TYPE;
        // 获取格式化时间
        message.time = formatTime(getTime());
        // 创建message唯一标识符
        message.messageId = makeId();
        // 先设置为空 后期根据需求再进行设置
        message.fileId="";
        // 设置文件名
        message.fileName = extraInfo;
        return message;
    }

    // 构造语音消息
    static Message makeSpeechMessage(const QString&chatSessionId, const UserInfo&sender, const QByteArray&content){
        // 创建类
        Message message;
        // 进行赋值
        message.chatSessionId = chatSessionId;
        message.sender = sender;
        message.content = content;
        message.messageType = SPEECH_TYPE;
        // 获取格式化时间
        message.time = formatTime(getTime());
        // 创建message唯一标识符
        message.messageId = makeId();
        // 先设置为空 后期根据需求再进行设置
        message.fileId="";
        // 非 FILE_TYPE
        message.fileName = "";
        return message;
    }

}; // Message



////////////////////////////////////////////////////////////////////
/// 会话信息实体类
////////////////////////////////////////////////////////////////////


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
    QString chatSessionId = "";      ///< 标识会话的唯一性

    // ==================================
    // 其他元数据
    // ==================================
    Message lastMessage;        ///< 当前会话中最新的消息
    QString chatSessionName = "";    ///< 会话昵称(单聊/群聊)
    QIcon avatar;               ///< 会话头像
    QString userId = "";             ///< 用户id


    /**
     * @brief load 从 Protobuf 的 ChatSessionInfo 对象加载数据到当前实例
     * @param chatSessionInfo Protobuf 生成的 my_im::ChatSessionInfo 对象
     * @details
     * 将服务端返回的 Protobuf 会话信息映射到客户端的 model::ChatSessionInfo。
     * 解析逻辑:
     *   - singleChatFriendId: optional 字段，单聊时为对方用户ID，群聊时不存在
     *   - avatar: 有头像数据则转换，无则根据是否为单聊/群聊设置不同的默认头像
     *   - prevMessage: optional 字段，新建会话可能没有最新消息，
     *                  存在时递归调用 Message::load() 解析
     */
    void load(const my_im::ChatSessionInfo &chatSessionInfo){
        this->chatSessionId = chatSessionInfo.chatSessionId();
        this->chatSessionName = chatSessionInfo.chatSessionName();
        if(chatSessionInfo.hasSingleChatFriendId()){
            this->userId = chatSessionInfo.singleChatFriendId();
        }

        if(chatSessionInfo.hasAvatar()&&(!chatSessionInfo.avatar().isEmpty())){
            // 有头像数据
            this->avatar = makeIcon(chatSessionInfo.avatar());
        }else{
            // 无头像数据 - 判别为单聊/群聊 - 分别设置单聊和群聊的默认头像
            if(chatSessionInfo.singleChatFriendId() == ""){
                this->avatar = QIcon(":/resource/image/defaultAvatar.png");
            }else this->avatar = QIcon(":/resource/image/groupChatSessionAvatar.svg");
        }
        if(chatSessionInfo.hasPrevMessage()){
            lastMessage.load(chatSessionInfo.prevMessage());
        }
    }

}; // ChatSessionInfo


} // - end namespace model

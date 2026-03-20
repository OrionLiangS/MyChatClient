#include "datacenter.h"



// ################################################################################
// 类: DataCenter
// 描述: 全局数据中心 (单例模式)
//       集中管理客户端运行期间的所有核心业务数据，
//       作为 NetClient (网络层) 与 UI (展示层) 之间的数据中转站。
// ################################################################################

namespace model{

    // 静态成员初始化
    DataCenter *DataCenter::instance = nullptr;


    // ================================================================================
    // 函数: getInstance
    // 描述: 获取 DataCenter 单例 (懒汉式)
    // ================================================================================
    DataCenter *DataCenter::getInstance()
    {
        if(instance == nullptr){
            instance = new DataCenter();
        }
        return instance;
    }


    // ================================================================================
    // 函数: ~DataCenter (析构函数)
    // 描述: 释放所有堆上分配的成员指针，防止内存泄漏。
    //       注: 单例生命周期与进程一致，实际上析构函数不会被调用，
    //       此处编写仅为规范性考虑。
    //       对 nullptr 执行 delete 是 C++ 标准允许的合法操作，无副作用。
    // ================================================================================
    DataCenter::~DataCenter()
    {
        delete mySelf;
        delete friendList;
        delete chatSessionList;
        delete memberList;
        delete applyList;
        delete recentMessage;
        delete unReadMessageCount;
        delete serchUserResult;
        delete searchMessageResult;
    }


    // ================================================================================
    // 函数: DataCenter (构造函数)
    // 描述: 初始化需要立即可用的 Hash 类型成员。
    //       QList 类型成员保持 nullptr，用于区分"未加载"与"已加载但为空"。
    // ================================================================================
    DataCenter::DataCenter()
    {
        // -----------------------------------------------------------
        // 实例化 Hash 类型成员 (通过 key 是否存在判断数据有效性)
        // -----------------------------------------------------------

        recentMessage = new QHash<QString, QList<Message>>();

        memberList = new QHash<QString, QList<UserInfo>>();

        unReadMessageCount = new QHash<QString, int>();

        // -----------------------------------------------------------
        // 从本地文件恢复持久化数据 (loginSessionId, unReadMessageCount)
        // -----------------------------------------------------------
        loadDataFile();
    }

    // ================================================================================
    // 函数: initDataFile
    // 描述: 初始化本地数据文件 (AppData/ChatClient.json)
    //       首次运行或文件丢失时调用，创建目录并写入空 JSON 骨架。
    // ================================================================================
    void DataCenter::initDataFile()
    {
        // -----------------------------------------------------------
        // 1. 构造文件路径 (使用系统 AppData 目录)
        // -----------------------------------------------------------
        QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) ;
        QString filePath = basePath+ "/ChatClient.json";
        LOG()<<"filePath: "<<filePath;

        // -----------------------------------------------------------
        // 2. 确保目录存在 (不存在则递归创建)
        // -----------------------------------------------------------
        QDir dir;
        if(!dir.exists(basePath)){
            dir.mkpath(basePath);
        }

        // -----------------------------------------------------------
        // 3. 创建文件并写入空 JSON 骨架
        // -----------------------------------------------------------
        QFile file(filePath);
        if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
            LOG()<<"文件打开失败"<<file.errorString();
            return;
        }
        QString data = "{\n\n}";
        file.write(data.toUtf8());
        file.close();
    }


    // ================================================================================
    // 函数: saveDataFile
    // 描述: 将内存中需要持久化的数据序列化为 JSON 写入本地文件。
    //       持久化字段: loginSessionId, unReadMessageCount
    // ================================================================================
    void DataCenter::saveDataFile()
    {
        // -----------------------------------------------------------
        // 1. 构建文件路径，文件不存在则初始化
        // -----------------------------------------------------------
        QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+ "/ChatClient.json";

        QDir dir;
        if(!dir.exists(filePath)){
            initDataFile();
        }

        // -----------------------------------------------------------
        // 2. 构造 JSON 对象 (序列化内存数据)
        // -----------------------------------------------------------
        QJsonObject jsonObj;
        jsonObj["loginSessionId"] = loginSessionId;

        // 将 unReadMessageCount (QHash) 转为嵌套 JSON 对象
        QJsonObject unRead;
        for(auto it = unReadMessageCount->begin();it!=unReadMessageCount->end();++it){
            unRead[it.key()] = it.value();
        }
        jsonObj["unReadMessageCount"] = unRead;

        // -----------------------------------------------------------
        // 3. 以文本方式写入本地文件
        // -----------------------------------------------------------
        QFile file(filePath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            LOG()<< "Open \""<<filePath<<"\" faild: "<<file.errorString();
            return;
        }

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObj);
        file.write(jsonDoc.toJson());
        file.close();
    }


    // ================================================================================
    // 函数: loadDataFile
    // 描述: 从本地 JSON 文件加载持久化数据到内存。
    //       在 DataCenter 构造时调用，若文件不存在则先 initDataFile()。
    //       恢复字段: loginSessionId, unReadMessageCount
    // ================================================================================
    void DataCenter::loadDataFile()
    {
        // -----------------------------------------------------------
        // 1. 构建文件路径，文件不存在则初始化
        // -----------------------------------------------------------
        QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+ "/ChatClient.json";

        QDir dir;
        if(!dir.exists(filePath)){
            initDataFile();
        }

        // -----------------------------------------------------------
        // 2. 以只读文本方式打开文件
        // -----------------------------------------------------------
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            LOG()<< "Open \""<<filePath<<"\" faild: "<<file.errorString();
            return;
        }

        // -----------------------------------------------------------
        // 3. 解析 JSON 文档
        //    isNull(): 解析本身失败 (非法 JSON 格式)
        //    isEmpty(): 解析成功但内容为空 (如 "{}")
        // -----------------------------------------------------------
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
        if(jsonDoc.isNull()){
            LOG()<<"Open \""<<filePath<<"\" faild: "<<file.errorString()<<" - "<<"The Json format is wrong";
            file.close();
            return;
        }

        // -----------------------------------------------------------
        // 4. 反序列化: 将 JSON 字段恢复到内存成员
        // -----------------------------------------------------------
        QJsonObject jsonObj = jsonDoc.object();

        // 恢复登录会话ID
        this->loginSessionId = jsonObj["loginSessionId"].toString();

        // 恢复未读消息计数 (嵌套 JSON 对象 → QHash)
        QJsonObject unRead = jsonObj["unReadMessageCount"].toObject();
        unReadMessageCount->clear();
        for(auto beg = unRead.begin();beg!=unRead.end();++beg){
            unReadMessageCount->insert(beg.key(), beg.value().toInt());
        }

        LOG()<<"loginSessionId: "<<this->loginSessionId;

        file.close();
    }
} // end namespace model

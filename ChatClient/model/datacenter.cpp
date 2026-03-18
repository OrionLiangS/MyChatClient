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
    }

} // end namespace model

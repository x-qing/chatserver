#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include<muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include "json.hpp"
#include <mutex>
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"

using json = nlohmann::json; 

using namespace std;

using namespace muduo;
using namespace muduo::net;

// 表示处理消息的事件回调类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn,json &js,Timestamp )>;

// 单例模式设计聊天服务器业务类
class ChatService{
public:
    // 获取单例对象的接口函数
    static ChatService* instance();
    
    // 处理登录业务
    void login(const TcpConnectionPtr &conn,json &js,Timestamp time);

    // 处理注册业务
    void reg(const TcpConnectionPtr &conn,json &js,Timestamp time);

    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn,json &js,Timestamp time);

    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn,json &js,Timestamp time);

    // 获取消息对应的处理器
    MsgHandler gethandler(int msgid);


    // 群组消息的处理
    void createGroup(const TcpConnectionPtr &conn,json &js,Timestamp time);

    void addGroup(const TcpConnectionPtr &conn,json &js,Timestamp time);

    void groupChat(const TcpConnectionPtr &conn,json &js,Timestamp time);


    void handleRedisSubscribeMessage(int userid, string msg);


    // 处理注销业务
    void loginout(const TcpConnectionPtr &conn,json &js,Timestamp time);

    // 服务器异常，业务重置方法
    void reset();

    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);

private:

    ChatService();

    // 存储消息id和其对应的业务处理方法
    unordered_map<int,MsgHandler> _msgHandlerMap;

    // 存储在线用户的通信连接
    unordered_map<int,TcpConnectionPtr> _userConnMap;

    // 数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;

    GroupModel _groupModel;

    // reids操作对象
    Redis _redis;

    // 定义互斥锁，用来保证_userConnMap 线程安全
    mutex _connMutex;
};

#endif
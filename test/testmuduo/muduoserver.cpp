#if 0

/*
muduo网络库给用户提供了两个主要的类
TcpServer：用于编写服务端程序
TcpClient：用于编写客户端程序

epoll+ 线程池
好处：能够把网络 I/O的代码和业务代码（用户的连接和断开   用户的可读写事件）区分开
*/

#include <muduo/net/TcpServer.h>
#include <functional>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
// 基于muduo网络库开发服务器程序
/*
1.组合tcpserver对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要什么参数，输出chatServer的构造函数
4.在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
5.设置合适的服务端线程数量，muduo库自己会分配IO线程和worker线程
*/
class ChatServer
{
public:
    ChatServer(EventLoop *loop,               // 事件循环
               const InetAddress &listenAddr, //  ip+port,,服务器名字
               const string &nameArg) : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMassage, this, _1, _2, _3));

        // 设置服务器端的线程数量
        _server.setThreadNum(4); // 1个IO线程，3个worker线程
    }
    // 开始事件循环
    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << " status:online" << endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << " status:offline" << endl;
            conn->shutdown();   // close(fd)
            // _loop->quit();
        }
    }
    // 专门处理用户的读写事件的
    void onMassage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,                  // 缓冲区，，接收数据的时间信息
                   Timestamp time)
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << " time:"<<time.toString() << endl;
        conn->send(buf);
    }

    TcpServer _server;
    EventLoop *_loop;
};

int main(){
    EventLoop loop;
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop,addr,"ChatServer");
    server.start();  // listenfd epoll_ctl = epoll
    loop.loop();   // epollwait以阻塞的方式等待新用户的连接，已连接用户的读写事件等操作
    return 0;
}

#endif
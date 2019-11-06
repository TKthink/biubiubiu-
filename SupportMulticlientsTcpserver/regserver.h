#ifndef REGSERVER_H
#define REGSERVER_H
#include <QTcpServer>
#include "database.h"
namespace tcp_server_private {
    class TcpServerPrivate;
}
class QTcpSocket;
/**
 * @brief Tcp多客户端服务器
 */
class regServer : public QTcpServer {
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父QObject
     */
    explicit regServer(QObject *parent = Q_NULLPTR);
    /**
     * @brief 析构函数
     *    非多线程模式行为：关闭所有连接后析构
     *    多线程模式行为：关闭所有连接及线程池后析构
     */
    ~regServer();
//    void verifyUser(qintptr handle,QTcpSocket* client_socket);
//    void userRegister(qintptr handle,QTcpSocket* client_socket);
signals:
    /**
     * @brief 客户端连入
     * @param 连接句柄
     * @param socket指针
     */
    void ClientConnected(qintptr, QTcpSocket*);//发送新用户连接信息
    /**
     * @brief socket已断开连接
     *   若需要在socket后析构后进行操作的可连接此信号
     * @param 连接句柄
     */
    void ClientDisconnected(qintptr);
    /**
     * @brief 主动断开连接信号
     *   若服务端想要主动断开与客户端连接将会发出此信号
     *   此信号发出这表明进行断开操作不表明断开成功，成功以SocketDisconnected信号为准
     * @param 连接句柄
     */
    void InitiativeDisConnectClient(qintptr);
    void newUser();
protected slots:
    /**
     * @brief 客户端已断开槽
     *   此槽与客户端的已断开信号连接
     * @param handle
     */
    void ClientDisconnectedSlot(qintptr handle);
    /**
     * @brief 验证用户密码
     *   通过之后保持连接
     * @param handle
     */
public slots:
    void userRegister(qintptr handle,QTcpSocket* client_socket);
protected:
    /**
     * @brief 重写-有连接到来
     *    连接到来不一定连接，会根据maxPendingConnections决定是否连接
     * @param handle 连接句柄
     */
    virtual void incomingConnection(qintptr handle);
private:
    tcp_server_private::TcpServerPrivate *private_;
};
#endif // TCP_SERVER_H

#include "regserver.h"
#include "tcp_server_private.h"
//构造函数
regServer::regServer(QObject *parent)
    : QTcpServer(parent),
      private_(new tcp_server_private::TcpServerPrivate) {
}
//析构函数
regServer::~regServer() {
    for(tcp_server_private::TcpSocket *client : private_->clients.values()) {
        client->disconnectFromHost();
        auto handle = client->socketDescriptor();
        client->deleteLater();
        //告知其他调用者 当前socket断开，避免有需要在socket后执行的方法
        emit ClientDisconnected(handle);
    }
    if(private_)
        delete private_;
    this->close();
}
//重写-有连接到来
void regServer::incomingConnection(qintptr handle) {
    //超出最大连接数量关闭连接
    if (private_->clients.size() > maxPendingConnections()) {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(handle);
        tcp.disconnectFromHost();
        return;
    }
    auto client_socket = new tcp_server_private::TcpSocket(handle);
    Q_ASSERT(client_socket->socketDescriptor() == handle);
    //socket断开连接的信号与server槽连接
    connect(client_socket,
            &tcp_server_private::TcpSocket::ClientDisconnected,
            this,
            &regServer::ClientDisconnectedSlot);
    //主动断开连接的操作
    connect(this,
            &regServer::InitiativeDisConnectClient,
            client_socket,
            &tcp_server_private::TcpSocket::DisconnectSocket);
    //map记录
    private_->clients.insert(handle, client_socket);
    connect(client_socket, &QTcpSocket::readyRead,
            [=]() {
        regServer::userRegister(handle,client_socket);
    });
}
//客户端已断开槽
void regServer::ClientDisconnectedSlot(qintptr handle) {
    //map中移除
    private_->clients.remove(handle);
    qDebug()<<handle<<"disconnected";
    //发出信号
    emit ClientDisconnected(handle);
}


 void regServer::userRegister(qintptr handle,QTcpSocket* client_socket){
     qDebug()<<"regin";
     QString str = client_socket->read(client_socket->bytesAvailable());
     QString password = str.mid(str.indexOf(" ")+1);
     QString ID = str.mid(0,str.indexOf(" "));
     qDebug()<<ID<<"regID";
     qDebug()<<password<<"regPassword";
     database database;
     int flag=database.OpenAndRegister(ID,password);
     if(flag==1){
         client_socket->write("1");//注册成功
         qDebug()<<"register success";
         emit newUser();
     }
     else if(flag==3){//注册失败
         client_socket->write("3");
         qDebug()<<"register failed";
     }
     else{
         client_socket->write("2");//账号已存在
         qDebug()<<"register ID occupied";
     }
     emit InitiativeDisConnectClient(handle);
 }

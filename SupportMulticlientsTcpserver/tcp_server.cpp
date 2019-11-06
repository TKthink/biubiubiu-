#include "tcp_server.h"
QMap<int, QString> connectedIfo;
//构造函数
TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent),
      private_(new tcp_server_private::TcpServerPrivate) {
}
//析构函数
TcpServer::~TcpServer() {
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
void TcpServer::incomingConnection(qintptr handle) {
    //超出最大连接数量关闭连接
    if (private_->clients.size() > maxPendingConnections()) {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(handle);
        tcp.disconnectFromHost();
        return;
    }
    this->handle=handle;
    client_socket = new tcp_server_private::TcpSocket(handle);
    Q_ASSERT(client_socket->socketDescriptor() == handle);
    //socket断开连接的信号与server槽连接
    connect(client_socket,
            &tcp_server_private::TcpSocket::ClientDisconnected,
            this,
            &TcpServer::ClientDisconnectedSlot);
    //主动断开连接的操作
    connect(this,
            &TcpServer::InitiativeDisConnectClient,
            client_socket,
            &tcp_server_private::TcpSocket::DisconnectSocket);
    //map记录
    private_->clients.insert(handle, client_socket);
//    connect(client_socket, &QTcpSocket::readyRead,
//            [=]() {
//        TcpServer::verifyUser(handle);
//    });
    connect(client_socket, &QTcpSocket::readyRead,this,
        &TcpServer::verifyUser);
}
//客户端已断开槽
void TcpServer::ClientDisconnectedSlot(qintptr handle) {
    //map中移除
    private_->clients.remove(handle);
    qDebug()<<handle<<"disconnected";
    //发出信号
    emit ClientDisconnected(handle);
}

 void TcpServer::verifyUser(){
     QString str = client_socket->read(client_socket->bytesAvailable());
     QString password = str.mid(str.indexOf(" ")+1);
     QString ID = str.mid(0,str.indexOf(" "));
     qDebug()<<ID<<"loginID";
     qDebug()<<password<<"loginPassword";
     database database;
     int flag=database.OpenAndVerify(ID,password);
     if(flag==2){
         client_socket->write("2");//账号密码正确，成功登陆
         qDebug()<<"connected";
         connectedIfo.insert(handle,ID);
         emit ClientConnected(handle, client_socket);
         disconnect(client_socket, &QTcpSocket::readyRead,this,
                        &TcpServer::verifyUser);
     }
     else if(flag==3){//密码错误
         qDebug()<<"connected failed1";
         client_socket->write("3");
         emit InitiativeDisConnectClient(handle);
     }
     else if(flag==1){
         qDebug()<<"connected failed2";
         qDebug()<<"fatal error";
     }
     else{
         qDebug()<<"connected failed3";
         client_socket->write("1");//账号不存在
         emit InitiativeDisConnectClient(handle);
     }
 }

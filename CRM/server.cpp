#include "server.h"
#include "ui_server.h"
#include <QHostAddress>

class CustomTabStyle:public QProxyStyle
{
public:
    CustomTabStyle(){}


    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const                   //设置Bar上面的内容大小
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 150;                                     // 设置每个tabBar中item的大小
            s.rheight() = 50;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;     //定义一个矩形区域

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));                //蓝色底
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);     //使文字位置居中
                if (tab->state & QStyle::State_Selected) { //设置文字颜色
                    painter->setPen(0xf8fcff);           //白色字
                }
                else {
                    painter->setPen(0x000001);           //黑色字
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }

};

server::server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
    setWindowTitle("服务器");
    tcpServer = NULL;
    tcpSocket = NULL;

    //监听套接字，指定父对象，让其自动回收空间
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);

    if(!tcpServer->listen(QHostAddress::Any,6666)) {
        qDebug()<<tcpServer->errorString();    //错误信息
    }
    connect(this->tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));


    ///文件传送套接字
        this->filesocket = new QTcpSocket(this);
        this->fileserver = new QTcpServer(this);
        this->fileserver->listen(QHostAddress::Any,80100);
        connect(this->fileserver,SIGNAL(newConnection()),this,SLOT(acceptFileConnection()));

}

/*void incomingConnection(int socketDescriptor)
{
     QTcpSocket *tcpclientsocket = new QTcpSocket(this);//只要有新的连接就生成一个新的通信套接字
    //将新创建的通信套接字描述符指定为参数socketdescriptor
    tcpclientsocket->setSocketDescriptor(socketDescriptor);

    //将这个套接字加入客户端套接字列表中
    tcpclientsocketlist.append(tcpclientsocket);


    //接收到tcpclientsocket发送过来的更新界面的信号
    connect(tcpclientsocket, &QTcpSocket::updateserver, this, &server::sliotupdateserver);
    connect(tcpclientsocket, &QTcpSocket::clientdisconnected, this, &server::slotclientdisconnect);

}*/

server::~server()
{
    delete ui;
}

void server::server_New_Connect() {
    //获取客户端连接
    QTcpSocket *socket_temp = tcpServer->nextPendingConnection();//根据当前新连接创建一个QTcpSocket
    m_socket=socket_temp;
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &server::socket_Read_Data);
    //当断开连接时发出的信号
   // QObject::connect(socket_temp, &QTcpSocket::disconnected, this, &server::socket_Disconnected);
    qDebug()<<"new clinet connect in";
}


void server::acceptConnection()
{
    ////返回一个socket连接
    this->tcpSocket = this->tcpServer->nextPendingConnection();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(socket_Read_Data()));

}

void server::socket_Read_Data(){
    ////接收数据
    QString str = this->tcpSocket->readAll();
    QString temp="wltk1234";
    QString correctCode ="#123";
    QString wrongCode ="321#";
    if(str==temp){
        this->tcpSocket->write(correctCode.toLatin1());
    }
    else{
        this->tcpSocket->write(wrongCode.toLatin1());

    }
}

void server::acceptFileConnection()
{
    bytesWritten = 0;
    ///每次发送数据大小为64kb
    perDataSize = 64*1024;
    this->filesocket = this->fileserver->nextPendingConnection();
    ///接受文件
    connect(filesocket,SIGNAL(readyRead()),this,SLOT(updateFileProgress()));
    connect(filesocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(updateFileProgress(qint64)));
    connect(filesocket,SIGNAL(bytesWritten(qint64)),this,SLOT(displayError(QAbstractSocket::SocketError socketError)));
}

void server::updateFileProgress()
{
    QDataStream inFile(this->filesocket);
    inFile.setVersion(QDataStream::Qt_4_8);

    ///如果接收到的数据小于16个字节，保存到来的文件头结构
    if(bytesReceived <= sizeof(qint64)*2)
    {
        if((filesocket->bytesAvailable()>=(sizeof(qint64))*2) && (filenameSize==0))
        {
            inFile>>totalBytes>>filenameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((filesocket->bytesAvailable()>=filenameSize) && (filenameSize != 0))
        {
            inFile>>filename;
            bytesReceived += filenameSize;
            ////接收的文件放在指定目录下
            filename = "ClientData/"+filename;
            localFile = new QFile(filename);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug()<<"Server::open file error!";
                return;
            }
        }
        else
            return;
    }
    /////如果接收的数据小于总数据，则写入文件
    if(bytesReceived < totalBytes)
    {
        bytesReceived += filesocket->bytesAvailable();
        inBlock = filesocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    ////数据接收完成时
    if(bytesReceived == totalBytes)
    {
      // this->ui.browser->append("Receive file successfully!");
        bytesReceived = 0;
        totalBytes = 0;
        filenameSize = 0;
        localFile->close();
        //filesocket->close();
    }
}



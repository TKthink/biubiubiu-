#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QTextOption>
#include <QPainter>
#include <QFile>

namespace Ui {
class server;
}

class server : public QWidget
{
    Q_OBJECT

protected:
    void incomingConnection(int socketDescriptor);

public:
    explicit server(QWidget *parent = nullptr);
    ~server();
    QList<QTcpSocket*> tcpclientsocketlist;

private slots:
    void acceptConnection();
    void socket_Read_Data();
    void updateserver(QString, int);
    void acceptFileConnection();
    void updateFileProgress();
    void server_New_Connect();
private:
    Ui::server *ui;

    QTcpServer *tcpServer; //监听套接字
    QTcpSocket *tcpSocket; //通信套接字
    QTcpSocket *m_socket;
    QTcpServer *fileserver;
    QTcpSocket *filesocket;

    qint64 totalBytes;
        qint64 bytesReceived;
        qint64 bytestoWrite;
        qint64 bytesWritten;
        qint64 filenameSize;
        QString filename;
        ///每次发送数据大小
        qint64 perDataSize;
        QFile *localFile;
        ////本地缓冲区
        QByteArray inBlock;
        QByteArray outBlock;
};

#endif // SERVER_H

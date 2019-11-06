#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QObject>
#include <QMutex>
#include "database.h"

class myThread: public QObject
{
    Q_OBJECT
public:
    myThread(QTcpSocket *socket,QString username);
    ~myThread();
    void stop();
signals:
    void readyLoad(QString username,QString filename);
public slots:
    void ServerRecvedSlot(QTcpSocket *socket);
    void loadData(QString username,QString filename);
private:
    QTcpSocket *socket;
    qint64 totalBytes=0;
    qint64 bytesReceived=0;
    qint64 bytestoWrite=0;
    qint64 bytesWritten=0;
    qint64 filenameSize=0;
    QString filename;
    QString username;
    QFile *localFile;
    QByteArray inBlock;
    QByteArray outBlock;
};

#endif // MYTHREAD_H

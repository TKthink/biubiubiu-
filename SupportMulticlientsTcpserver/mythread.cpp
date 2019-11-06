#include "mythread.h"

myThread::myThread(QTcpSocket *socket,QString username)
{
    this->username=username;
    this->socket=socket;
    //connect(socket, &QTcpSocket::readyRead,this,&myThread::ServerRecvedSlot);
    connect(socket,&QTcpSocket::readyRead,this,
                [=]() {
            ServerRecvedSlot(socket);
        },Qt::BlockingQueuedConnection);
    connect(this,&myThread::readyLoad,this,&myThread::loadData);
}

myThread::~myThread(){

}

void myThread::ServerRecvedSlot(QTcpSocket *socket) {
//    Q_UNUSED(handle);
//    qDebug()<<socket->peerAddress().toString()<<socket->peerPort();
//    QString send_data = QString("%1 %2").
//            arg(socket->peerAddress().toString()).
//            arg(socket->peerPort());
//    model_->insertRow(0,new QStandardItem(send_data));
    //socket->write(send_data.toLatin1());

    qint64 tep=socket->bytesAvailable();
    while(tep!=0){
        QDataStream inFile(socket);
        inFile.setVersion(QDataStream::Qt_5_12);
        ///�������յ�������С��16���ֽڣ����浽�����ļ�ͷ�ṹ
        if(bytesReceived <= sizeof(qint64)*2)
        {
            if((socket->bytesAvailable()>=(sizeof(qint64))*2) && (filenameSize==0))
            {
                inFile>>totalBytes>>filenameSize;
                bytesReceived += sizeof(qint64)*2;
            }
            if((socket->bytesAvailable()>=filenameSize) && (filenameSize != 0))
            {
                inFile>>filename;
                bytesReceived += filenameSize;
                filename = "C:/Users/WANGLIANG04/Desktop/clientdata/"+username+'_'+filename;
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
        /////�������յ�����С�������ݣ���д���ļ�
        else if(bytesReceived < totalBytes)
        {
            //qint64 m_inBlock = socket->bytesAvailable();
            if(totalBytes-bytesReceived<65536){
                //qint64 maxlen=totalBytes-bytesReceived;
                inBlock = socket->read(totalBytes-bytesReceived);
            }else if(totalBytes-bytesReceived>65536){
                inBlock = socket->read(65536);
            }
            bytesReceived += inBlock.size();
            localFile->write(inBlock);
            inBlock.resize(0);
        }
        ////���½�������ʾ
        //this->ui.progressBar_fileProgress->setMaximum(totalBytes);
        //this->ui.progressBar_fileProgress->setValue(bytesReceived);
        ////���ݽ�������ʱ
        if(bytesReceived == totalBytes)
        {
            localFile->close();
            emit readyLoad(username,filename);//数据存数据库信号
            //this->ui.browser->append("Receive file successfully!");
            bytesReceived = 0;
            totalBytes = 0;
            filenameSize = 0;
            //filesocket->close();
//            while (1) {
//                qDebug()<<socket;
//            }
        }
        tep=socket->bytesAvailable();
        //qDebug()<<socket->readAll()<<"message";
    }
}

void myThread::loadData(QString username,QString filename) {
    database a;
    QString temp;
    QFile f(filename);
    filename = filename.right(filename.size()-filename.lastIndexOf('_')-1);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open failed." << endl;
    }else{
        QTextStream textInput(&f);
        QString lineStr;
        temp=a.lastlogTime(username,filename);
        if(temp.length()!=0){
            while(!textInput.atEnd())
            {
                lineStr = textInput.readLine();
                if(lineStr == temp){break;}
                //ui->logContent->append(lineStr);
            }
        }
        while(!textInput.atEnd())
        {
            lineStr = textInput.readLine();
            int pos = lineStr.toStdString().find_first_of(":")+6;
            a.logdataIn(username,filename,lineStr.mid(0,pos),lineStr);
        }
    }
}

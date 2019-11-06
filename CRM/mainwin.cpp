#include "mainwin.h"
#include "ui_mainwin.h"
//#include "client.h"

MainWin::MainWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    payloadSize=64*1024;
    ui->clientProgressBar->setValue(0);
}

MainWin::~MainWin()
{
    delete ui;
}

void MainWin::on_pushButton_2_clicked()
{
    this->filename = QFileDialog::getOpenFileNames(this,"upload file","/var/log");
    for (int i = 0; i < filename.size(); i++){
            QString str_path = filename[i];
            //单个文件路径
            ui->textBrowser->insertPlainText(filename[i]+"\n");
        }
    connect(fileSocket,&QTcpSocket::bytesWritten,this,&MainWin::updateClientProgress);
}

void MainWin::on_pushButton_clicked()
{
    ui->clientProgressBar->setValue(0);
    if(filename.empty()) {return;}
    //i < filename.size(); i++;
    QString str_path = filename[flag];
    //单个文件路径
    this->localFile = new QFile(filename[flag]);
    if(!localFile->open(QFile::ReadOnly))
    {
        ui->error_label->setText("open file error");
        ui->error_label->setStyleSheet("color:red");
    }
    else {
        this->totalBytes = localFile->size();
        QDataStream sendout(&outBlock,QIODevice::WriteOnly);
        sendout.setVersion(QDataStream::Qt_5_12);
        qDebug()<<filename[flag];
        QString currentFileName = filename[flag].right(filename[flag].size()-filename[flag].lastIndexOf('/')-1);
        qDebug()<<currentFileName;
        sendout<<qint64(0)<<qint64(0)<<currentFileName;
        totalBytes += outBlock.size();
        sendout.device()->seek(0);
        sendout<<totalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));
        bytesToWrite = totalBytes;
        //fileSocket->write(outBlock);
        fileBytesWritten+=(int)fileSocket->write(outBlock);
        bytesToWrite-=fileBytesWritten;
        ui->clientProgressBar->setMaximum(totalBytes);
        ui->clientProgressBar->setValue(fileBytesWritten);
        outBlock.resize(0);
    }
}
void MainWin::updateClientProgress(){
    //fileBytesWritten+=(int)numBytes;
    //bytesToWrite-=numBytes;
    if(bytesToWrite>0){
        outBlock=localFile->read(qMin(bytesToWrite,payloadSize));
        //fileSocket->write(outBlock);
        int temp=(int)fileSocket->write(outBlock);
        bytesToWrite-=temp;
        fileBytesWritten+=temp;
        ui->clientProgressBar->setValue(fileBytesWritten);
        outBlock.resize(0);
    }
    else{
        localFile->close();
    }

    if(fileBytesWritten==totalBytes){
        //ui->clientStatusLabel->setText(QString("传送文件 %1 成功").arg(m_fileName));
        //localFile->close();
        //fileSocket->close();
        fileBytesWritten=0;
        totalBytes=0;
        outBlock.resize(0);
        bytesToWrite=0;
        //ui->textBrowser->
        if(flag<filename.size()-1){
            flag++;
            on_pushButton_clicked();
        }else {
            filename.clear();
            flag=0;
        }
    }
}


void MainWin::getSocket(QTcpSocket *fileSocket){
    this->fileSocket=fileSocket;
}

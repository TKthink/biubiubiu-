#include "client.h"
#include "ui_client.h"
#include <QHostAddress>

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
        //分配空间，指定父对象
    tcpSocket = new QTcpSocket(this);
    setWindowTitle("客户端");

}

client::~client()
{
    delete ui;
}


void client::on_pushButton_clicked()
{
    tcpSocket->abort();
    tcpSocket->connectToHost("10.72.245.83",8000,QTcpSocket::ReadWrite);

    if(!tcpSocket->waitForConnected()) {
        QMessageBox::critical(NULL, "failed", "用户创建失败", QMessageBox::Yes |
                 QMessageBox::No, QMessageBox::Yes);
        qDebug()<<"client connect error";
    }
    else {
        QString str = ui->lineEdit->text();
       // this->tcpSocket->write(str.toUtf8().data(),str.length());
        str = str+" "+ui->lineEdit_2->text();
        this->tcpSocket->write(str.toUtf8().data(),str.length());
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(receiveData()));
    }
}

void client::on_pushButton_2_clicked()
{
    RegisterDialog reg;
    reg.setWindowModality(Qt::ApplicationModal);//注册界面
    reg.show();
    reg.exec();
}

void client::receiveData(){
    QString serget = tcpSocket->readAll();  //if id password is correct,then do
    qDebug()<<serget;
    if(serget=="2"){
        w.getSocket(tcpSocket);
        this->close();
        w.show();
    }
    else if(serget=="3"){
        ui->label->setText("password wrong");
        ui->label->setStyleSheet("color:red");
        ui->label->setAlignment(Qt::AlignHCenter);
    }else if(serget=="1"){
        ui->label->setText("userID does not exist");
        ui->label->setStyleSheet("color:red");
        ui->label->setAlignment(Qt::AlignHCenter);
    }
}

#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    setWindowTitle("register");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_pushButton_clicked()
{
    // password confirm and save data
    regTcpsocket = new QTcpSocket(this);
    regTcpsocket->abort();
    regTcpsocket->connectToHost("10.72.245.83",6666,QTcpSocket::ReadWrite);
    if(!regTcpsocket->waitForConnected()) {
        ui->label->setText("连接失败!");
        ui->label->setStyleSheet("color:red");
        ui->label->setAlignment(Qt::AlignHCenter);
    }
    else {
        if(ui->lineEdit->text().length()>0&&ui->lineEdit_2->text().length()>0){
            if(ui->lineEdit_2->text()==ui->lineEdit_3->text()){//两次输入密码一致才执行
                qDebug()<<"reg1";
                QString str = ui->lineEdit->text();
                str = str+" "+ui->lineEdit_2->text();
                this->regTcpsocket->write(str.toUtf8().data(),str.length());
                //this->close();
                connect(regTcpsocket,SIGNAL(readyRead()),this,SLOT(receiveData()));
            }
            else{
                qDebug()<<"reg2";
                QMessageBox::warning(NULL, "failed", "密码不一致，请再次确认输入!", QMessageBox::Yes |
                                      QMessageBox::No, QMessageBox::Yes);
            }
        }else{
            QMessageBox::warning(NULL, "failed", "用户名或密码不能为空!", QMessageBox::Yes |
                                  QMessageBox::No, QMessageBox::Yes);
        }
    }
}

void RegisterDialog::receiveData(){
    QString serget = regTcpsocket->readAll();  //if id password is correct,then do
    qDebug()<<serget;
    if(serget=="1"){
        this->close();

    }else if(serget=="2"){
        QMessageBox::warning(NULL, "failed", "用户名已存在!", QMessageBox::Yes |
                              QMessageBox::No, QMessageBox::Yes);
    }else{
        ui->label->setText("服务器错误!");
        ui->label->setStyleSheet("color:red");
        ui->label->setAlignment(Qt::AlignHCenter);
    }
}

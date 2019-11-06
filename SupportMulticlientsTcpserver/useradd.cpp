#include "useradd.h"
#include "ui_useradd.h"

userAdd::userAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userAdd)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

userAdd::~userAdd()
{
    delete ui;
}

void userAdd::on_OK_clicked()
{
    if(ui->name->text().length()>0&&ui->password->text().length()>0){
        if(ui->password->text()==ui->password_2->text()){//两次输入密码一致才执行
            QString name = ui->name->text();
            QString password = ui->password->text();
            database add;
            int state = add.OpenAndRegister(name,password);
            if(state==1){
                emit newUser();
                QMessageBox::StandardButton dialog = QMessageBox::information
                        (NULL, "success", "用户创建成功", QMessageBox::Yes |
                         QMessageBox::No, QMessageBox::Yes);
                if(dialog){
                    close();
                }
            }else if(state==2){
                QMessageBox::warning(NULL, "failed", "用户已存在", QMessageBox::Yes |
                                     QMessageBox::No, QMessageBox::Yes);
            }else{
                QMessageBox::critical(NULL, "failed", "用户创建失败", QMessageBox::Yes |
                                      QMessageBox::No, QMessageBox::Yes);
            }
        }
        else{
            QMessageBox::warning(NULL, "failed", "密码不一致，请再次确认输入!", QMessageBox::Yes |
                                 QMessageBox::No, QMessageBox::Yes);
        }
    }else{
        QMessageBox::warning(NULL, "failed", "用户名或密码不能为空!", QMessageBox::Yes |
                             QMessageBox::No, QMessageBox::Yes);
    }
}

#include "fliterdialog.h"
#include "ui_fliterdialog.h"

FliterDialog::FliterDialog(QWidget *parent,fliter *citem) :
    QDialog(parent),
    ui(new Ui::FliterDialog)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(true);
    if(citem!=nullptr){
        ui->name->text()=citem->name;
        ui->content->text()=citem->requirements;
        ui->checkBox->setChecked(citem->check);
    }
}

FliterDialog::~FliterDialog()
{
    delete ui;
}

void FliterDialog::on_confirm_clicked()
{
    QFile file("fliter.json");
        if(!file.open(QIODevice::ReadWrite)) {
            qDebug() << "File open failed!";
        } else {
            qDebug() <<"File open successfully!";
        }

        QJsonDocument jdc(QJsonDocument::fromJson(file.readAll()));
        QJsonObject obj = jdc.object();
        QString name = ui->name->text();
        QString content = ui->content->text();
        QJsonObject fliter1;
        obj.remove(name);
        fliter1[QString('requirements')] =  content;
        fliter1[QString('check')] =  ui->checkBox->isChecked();
        obj.insert(name,fliter1);
        jdc.setObject(obj);
        file.seek(0);
        file.write(jdc.toJson());
        file.flush();
        file.close();
}

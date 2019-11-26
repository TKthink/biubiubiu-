#include "fliterdialog.h"
#include "ui_fliterdialog.h"

FliterDialog::FliterDialog(QString name,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FliterDialog)
{
    this->name = name;
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->checkBox->setChecked(true);
    if(name!=nullptr){
        ui->name->setText(name);
        QFile file("fliter.json");
            if(!file.open(QIODevice::ReadWrite)) {
                qDebug() << "File open failed!";
            } else {
                qDebug() <<"File open successfully!";
            }
        QJsonDocument jdc(QJsonDocument::fromJson(file.readAll()));
        QJsonObject obj = jdc.object();
        QJsonObject fliter1 = obj[name].toObject();
        ui->content->setText(fliter1["requirements"].toString());
        ui->checkBox->setChecked(fliter1["check"].toBool());
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
        QString content = ui->content->text();
        QJsonObject fliter1;
        obj.remove(name);
        fliter1[QString("requirements")] =  content;
        fliter1[QString("check")] =  ui->checkBox->isChecked();
        name = ui->name->text();
        obj.insert(name,fliter1);
        jdc.setObject(obj);
        file.resize(0);
        file.seek(0);
        file.write(jdc.toJson());
        file.flush();
        file.close();
        this->close();
}

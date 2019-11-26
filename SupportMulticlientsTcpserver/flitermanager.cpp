#include "flitermanager.h"
#include "ui_flitermanager.h"

fliterManager::fliterManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fliterManager)
{
    ui->setupUi(this);
    ui->tableView->setModel(model_);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //列宽自动调整
    ui->tableView->setSelectionBehavior(QTreeView::SelectRows);   //一次选中整行
    ui->tableView->setEditTriggers(QTreeView::NoEditTriggers);    //不能编辑
    init();
}

fliterManager::~fliterManager()
{
    delete ui;
}

void fliterManager::on_Add_clicked()
{
    FliterDialog add(nullptr);
    add.setWindowModality(Qt::ApplicationModal);
    add.show();
    add.exec();
    init();
}

void fliterManager::on_Properties_clicked()
{
    int row = ui->tableView->currentIndex().row();
    QString name = model_->item(row,0)->text();
    FliterDialog add(name);
    add.setWindowModality(Qt::ApplicationModal);
    add.show();
    add.exec();
    init();
}

void fliterManager::on_Remove_clicked()
{
    int row = ui->tableView->currentIndex().row();
    QString name = model_->item(row,0)->text();
    QMessageBox::StandardButton confirm = QMessageBox::information
            (NULL, "Delete", "确认删除？", QMessageBox::Yes |
             QMessageBox::No, QMessageBox::Yes);
    if(confirm==QMessageBox::Yes)
    {
        DeleteFliter(name);
        model_->removeRow(row);
    }
}

void fliterManager::DeleteFliter(QString name)
{
    QFile file("fliter.json");
        if(!file.open(QIODevice::ReadWrite)) {
            qDebug() << "File open failed!";
        } else {
            qDebug() <<"File open successfully!";
        }
        QJsonDocument jdc(QJsonDocument::fromJson(file.readAll()));
        QJsonObject obj = jdc.object();
        obj.remove(name);
        jdc.setObject(obj);
        file.resize(0);
        file.seek(0);
        file.write(jdc.toJson());
        file.flush();
        file.close();
}

void fliterManager::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_Properties_clicked();
}

void fliterManager::init()
{
    model_->removeRows(0,model_->rowCount());
    QFile file("fliter.json");
        if(!file.open(QIODevice::ReadWrite)) {
            qDebug() << "File open failed!";
        } else {
            qDebug() <<"File open successfully!";
        }

        QJsonDocument jdc(QJsonDocument::fromJson(file.readAll()));
        QJsonObject obj = jdc.object();
        file.close();
        for(int i=0;i<obj.keys().size();i++)
        {
            QList<QStandardItem*> items;//
            QStandardItem* item1 = new QStandardItem(obj.keys()[i]);
            items.append(item1);
            model_->appendRow(items);
        }
}


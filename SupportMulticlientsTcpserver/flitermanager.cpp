#include "flitermanager.h"
#include "ui_flitermanager.h"

fliterManager::fliterManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fliterManager)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //列宽自动调整
    ui->tableView->setSelectionBehavior(QTreeView::SelectRows);   //一次选中整行
    ui->tableView->setEditTriggers(QTreeView::NoEditTriggers);    //不能编辑
}

fliterManager::~fliterManager()
{
    delete ui;
}

void fliterManager::on_Add_clicked()
{
    FliterDialog add;
    add.setWindowModality(Qt::ApplicationModal);
    add.show();
    add.exec();
}

void fliterManager::on_Properties_clicked()
{
    QStandardItemModel *model_ = new QStandardItemModel(this);
    int row = ui->tableView->currentIndex().row();
    QString name = model_->item(row,0)->text();

    FliterDialog add;
    add.setWindowModality(Qt::ApplicationModal);
    add.show();
    add.exec();
}

void fliterManager::on_Remove_clicked()
{
    QStandardItemModel *model_ = new QStandardItemModel(this);
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
        file.seek(0);
        file.write(jdc.toJson());
        file.flush();
        file.close();
}

void fliterManager::on_tableView_doubleClicked(const QModelIndex &index)
{

}

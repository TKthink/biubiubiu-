#include "flitermanager.h"
#include "ui_flitermanager.h"

fliterManager::fliterManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fliterManager)
{
    ui->setupUi(this);
}

fliterManager::~fliterManager()
{
    delete ui;
}

void fliterManager::on_Add_clicked()
{

}

void fliterManager::on_Properties_clicked()
{

}

void fliterManager::on_Remove_clicked()
{

}


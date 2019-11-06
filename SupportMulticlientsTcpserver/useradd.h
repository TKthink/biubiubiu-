#ifndef USERADD_H
#define USERADD_H

#include <QDialog>
#include <QMessageBox>
#include "database.h"
#include "tcp_server.h"

namespace Ui {
class userAdd;
}

class userAdd : public QDialog
{
    Q_OBJECT
signals:
    void newUser();
public:
    explicit userAdd(QWidget *parent = nullptr);
    ~userAdd();

private slots:
    void on_OK_clicked();

private:
    Ui::userAdd *ui;
};

#endif // USERADD_H

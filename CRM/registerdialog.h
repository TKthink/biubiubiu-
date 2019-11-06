#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    ~RegisterDialog();

private slots:
    //void on_buttonBox_accepted();
    void receiveData();
    void on_pushButton_clicked();

private:
    Ui::RegisterDialog *ui;
    QTcpSocket *regTcpsocket;
};

#endif // REGISTERDIALOG_H

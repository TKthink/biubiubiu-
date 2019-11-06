#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <registerdialog.h>
#include <QTcpSocket>
#include<mainwin.h>


namespace Ui {
class client;
}

class client : public QWidget
{
    Q_OBJECT

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void receiveData();
public:
    explicit client(QWidget *parent = nullptr);
    ~client();
    MainWin w;
private:
    Ui::client *ui;
    QTcpSocket *tcpSocket; //通信套接字
};

#endif // CLIENT_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "tcp_server.h"
#include "database.h"
#include <regserver.h>
#include <QFile>
#include "mythread.h"
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <QTreeView>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <database.h>
#include "useradd.h"
#include <QTreeWidgetItem>
#include <flitermanager.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
        void DeleteItem(QString name);
        void InitTable();
        /**
        * @brief 初始化服务端界面
         */
        User* FindItem(QString name);
        void showTbl(QVector<User*> showTbl);
        void saveToDatabase(QString name,QString filename);
private slots:
    void ClientConnected(qintptr handle, QTcpSocket *socket);
    void ClientDisconnected(qintptr handle);
    /**
     * @brief 服务端收到消息的信号
     *    若想要统一管理或做日志处理可连接此信号
     * @param 收到消息的连接句柄
     * @param 收到消息的socket指针
     * @param 收到的消息内容
     */
    void appendUser();
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_Add_clicked();

    void on_Delete_clicked();

    void on_Search_clicked();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_manage_filters_triggered();

    void on_back_clicked();

private:
    Ui::MainWindow *ui;
    TcpServer *tcp_server_;
    regServer *registerTcp_server_;
    QStandardItemModel *model_;
    qint64 totalBytes=0;
    qint64 bytesReceived=0;
    qint64 bytestoWrite=0;
    qint64 bytesWritten=0;
    qint64 filenameSize=0;
    QByteArray inBlock;
    QByteArray outBlock;
    QTcpSocket* mysocket;

    QThread* m_objThread;
    myThread* m_obj;
    qintptr handle;
    QVector<User*> userIfo;

    User *lookingUser;//正在查看的用户的信息
};

#endif // MAINWINDOW_H

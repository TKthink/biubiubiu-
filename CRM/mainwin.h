#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QDateTime>
#include <QDataStream>
#include <QTcpSocket>

#include <QFile>
#include <QFileDialog>

namespace Ui {
class MainWin;
}

class MainWin : public QWidget
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);
    ~MainWin();
    void getSocket(QTcpSocket *fileSocket);

private slots:
    void on_pushButton_2_clicked();
    void updateClientProgress();
    void on_pushButton_clicked();

private:
    Ui::MainWin *ui;
    QStringList filename;
    QFile *localFile;
    qint64 totalBytes;
    QByteArray outBlock;
    qint64 bytesToWrite;
    QTcpSocket *fileSocket;
    qint64 fileBytesWritten;
    qint64 payloadSize;
    int flag = 0;
};

#endif // MAINWIN_H

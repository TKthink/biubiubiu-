#ifndef FLITERDIALOG_H
#define FLITERDIALOG_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

namespace Ui {
class FliterDialog;
}

class FliterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FliterDialog(QString name,QWidget *parent = nullptr);
    ~FliterDialog();

private slots:
    void on_confirm_clicked();

private:
    Ui::FliterDialog *ui;
    QString name;
};

#endif // FLITERDIALOG_H

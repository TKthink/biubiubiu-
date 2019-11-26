#ifndef FLITERDIALOG_H
#define FLITERDIALOG_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

typedef struct _fliter{
    QString name="";
    QString requirements="";
    bool check=true;
}fliter;

namespace Ui {
class FliterDialog;
}

class FliterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FliterDialog(QWidget *parent = nullptr,fliter *citem = nullptr);
    ~FliterDialog();

private slots:
    void on_confirm_clicked();

private:
    Ui::FliterDialog *ui;
};

#endif // FLITERDIALOG_H

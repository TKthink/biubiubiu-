#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>

typedef struct _User{
    QString name;
    QString password;
    QString time;
    int num;
    int state;

}User;
typedef struct _fliter{
    QString name="";
    QString requirements="";
    bool check=true;
}fliter;

class database
{
public:
    database();
    ~database();
    int OpenAndVerify(QString ID,QString password);
    int OpenAndRegister(QString ID,QString password);
    int logdataIn(QString ID,QString logName,QString time,QString content);
    QVector<User*> userget();
    void resetState(QString ID);
    void userDel(QString ID);
    QVector<User*> search(QString ID);
    QString lastlogTime(QString ID,QString logName);
    QVector<QString> logdataOut(QString ID,QString logName,QVector<fliter*>);
private:
    QString mDefaultDB = "logrec.db";
    QVector<User*> mItems;  //所有用户数据
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
};

#endif // DATABASE_H

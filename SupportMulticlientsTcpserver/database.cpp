#include "database.h"

database::database()
{
    db.setDatabaseName(mDefaultDB);
    db.open();
}

database::~database(){
    db.close();
}

int database::OpenAndVerify(QString ID,QString password)
{
    qDebug()<<"Database Opened";
    QSqlQuery sql_query;
    QString create_table = "create table if not exists user (name varchar(20) primary key not null, \
            password varchar(16) NOT NULL,\
            time varchar DEFAULT '0',\
            num integer,\
            state integer DEFAULT '0')";
            QString verifyUser = QString("select password from user where name='%1'").arg(ID);
    qDebug()<<verifyUser;
    sql_query.prepare(create_table);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"table success";
        sql_query.prepare(verifyUser);
        if(!sql_query.exec()){//执行错误 or 用户不存在
            qDebug()<<sql_query.lastError();
            return 1;
        }else{
            while(sql_query.next())
            {
                QString truePassword = sql_query.value(0).toString();
                qDebug()<<"temppassword"<<truePassword<<"   "<<password;
                if(truePassword==password){//账号密码正确，成功登陆
                    verifyUser = QString("update user set state='1' where name='%1'").arg(ID);
                    sql_query.exec(verifyUser);
                    QDateTime time =QDateTime::currentDateTime();
                    QString current_date =time.toString("yyyy.MM.dd hh:mm:ss ddd");
                    verifyUser = QString("update user set time='%1' where name='%2'").arg(current_date).arg(ID);
                    sql_query.exec(verifyUser);
                    return 2;
                }
            }
        }
    }
}


int database::OpenAndRegister(QString ID,QString password)
{
    qDebug()<<"Database Opened";
    QSqlQuery sql_query;
    QString create_table = "create table if not exists user (name varchar(20) primary key not null, \
            password varchar(16) NOT NULL,\
            time varchar DEFAULT '0',\
            num integer ,\
            state integer DEFAULT '0')";
            QString askUser = QString("select name from user where name='%1'").arg(ID);
    qDebug()<<askUser;
    sql_query.prepare(create_table);
    if(sql_query.exec())
    {
        qDebug()<<"table success";
        sql_query.prepare(askUser);
        if(sql_query.exec(askUser)){//成功执行
            QString userIn = QString("insert into user (name,password) values('%1','%2')").arg(ID).arg(password);
            sql_query.prepare(userIn);
            if(sql_query.exec()) {
                qDebug()<<"注册成功";
                return 1;}//注册成功
            else {
                qDebug()<<sql_query.lastError();
                return 2;
            }//用户已存在
        }
        else
        {
            qDebug()<<3;
            return 3;//程序错误
        }
    }
}

QVector<User*> database::userget()
{
    QSqlQuery sql_query;
    QString select_all_sql = "select * from user";
    //查询所有记录
    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            User* p = new User;
            p->name = sql_query.value(0).toString();
            p->password = sql_query.value(1).toString();
            p->time = sql_query.value(2).toString();
            p->num = sql_query.value(3).toInt();
            p->state = sql_query.value(4).toInt();
            mItems.push_back(p);
        }
    }
    return mItems;
}

void database::resetState(QString ID){
    QSqlQuery sql_query;
    QString reset = QString("update user set state='0' where name='%1'").arg(ID);
    //查询所有记录
    sql_query.exec(reset);
}

void database::userDel(QString ID){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(mDefaultDB);
    QSqlQuery sql_query;
    QString reset = QString("delete from user where name='%1'").arg(ID);
    //查询所有记录
    sql_query.exec(reset);
}

QVector<User*> database::search(QString ID){
    QVector<User*> searchTbl;
    User* p = new User;
    QSqlQuery sql_query;
    QString reset = QString("select * from user where name like '%1'").arg('%'+ID+'%');
    //查询所有记录
    sql_query.exec(reset);
    while(sql_query.next())
    {
        p->name = sql_query.value(0).toString();
        p->password = sql_query.value(1).toString();
        p->time = sql_query.value(2).toString();
        p->num = sql_query.value(3).toInt();
        p->state = sql_query.value(4).toInt();
        searchTbl.push_back(p);
    }
    return  searchTbl;
}

int database::logdataIn(QString ID,QString logName,QString time,QString content)
{
    qDebug()<<"logData Opened";
    QSqlQuery sql_query;
    QString create_table = "create table if not exists logData (num integer primary key AUTOINCREMENT,\
            name varchar(20) not null, \
            logName varchar(16) NOT NULL,\
            time varchar DEFAULT '0',\
            content TEXT NOT NULL)";
            sql_query.prepare(create_table);
    if(sql_query.exec())
    {
        qDebug()<<"table success";
        QString insert = QString("insert into logData (name,logName,time,content) values('%1','%2','%3','%4')")
                .arg(ID).arg(logName).arg(time).arg(content);
        qDebug()<<insert;
        sql_query.prepare(insert);
        if(sql_query.exec(insert)){//成功执行
            return 1;
        }
        else
        {
            qDebug()<<2;
            return 2;//程序错误
        }
    }
}

QString database::lastlogTime(QString ID,QString logName){
    QSqlQuery sql_query;
    QString reset = QString("select content from logData where name='%1' and logName='%2' ORDER BY num DESC")
            .arg(ID).arg(logName);
    //查询所有记录
    sql_query.exec(reset);
    if(sql_query.next())
    {
        return sql_query.value(0).toString();
    }else{return "";}
}

QVector<QString> database::logdataOut(QString ID,QString logName,QVector<fliter*> fliterTbl){
    QVector<QString> searchTbl;
    QSqlQuery sql_query;
    QString reset,content;
    reset = QString("select content from logData where name='%1' and logName='%2'").arg(ID).arg(logName);
    for (int i=0;i<fliterTbl.size();i++) {
        fliter* p=fliterTbl[i];
        if(p->check){
            reset = reset+QString(" and content like '%1'").arg('%'+p->requirements+'%');
        }else{
            reset = reset+QString(" and content not like '%3'").arg('%'+p->requirements+'%');
        }
    }

    //查询所有记录ID
    sql_query.exec(reset);
    while(sql_query.next())
    {
        content = sql_query.value(0).toString();
        searchTbl.push_back(content);
    }
    return  searchTbl;
}

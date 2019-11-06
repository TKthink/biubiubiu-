#include "mainwindow.h"
#include "ui_mainwindow.h"


extern QMap<int, QString> connectedIfo;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    //connect(this, &MainWindow::ServerRecved, this, &MainWindow::ServerRecvedSlot);

    InitTable();   //初始化界面

    tcp_server_ = new TcpServer(this);
    registerTcp_server_= new regServer(this);
    if(!tcp_server_->listen(QHostAddress::Any, 8000)) {
        qDebug()<<tcp_server_->errorString();    //错误信息
        return;
    }
    if(!registerTcp_server_->listen(QHostAddress::Any, 6666)) {
        qDebug()<<registerTcp_server_->errorString();    //错误信息
        return;
    }
    qDebug()<<"listening";
    //model_->insertRow(0,new QStandardItem("listening"));
    //登录监听
    connect(tcp_server_,
        &TcpServer::ClientConnected,
        this,
        &MainWindow::ClientConnected);//允许传文件监听
    connect(tcp_server_,
        &TcpServer::ClientDisconnected,
        this,
        &MainWindow::ClientDisconnected);//登录断开监听
    //注册监听
    connect(registerTcp_server_,
        &regServer::newUser,
        this,
        &MainWindow::appendUser);
}

MainWindow::~MainWindow() {
    tcp_server_->close();
    delete ui;
}

void MainWindow::ClientConnected(qintptr handle, QTcpSocket* socket) {
    this->handle=handle;
    //model_->insertRow(0,new QStandardItem(QString::number(handle)+" connected"));
    InitTable();
    //多线程创建
    m_objThread= new QThread();
    m_obj = new myThread(socket,*connectedIfo.find(handle));
    connect(tcp_server_,&TcpServer::ClientDisconnected,m_objThread,&QThread::quit);
    connect(tcp_server_,&TcpServer::ClientDisconnected,m_objThread,&QObject::deleteLater);
    connect(tcp_server_,&TcpServer::ClientDisconnected,m_obj,&QObject::deleteLater);

    m_obj->moveToThread(m_objThread);
    m_objThread->start();
}

void MainWindow::ClientDisconnected(qintptr handle) {
    //model_->insertRow(0,new QStandardItem(QString::number(handle)+" disconnected"));
    database reset;
    reset.resetState(*connectedIfo.find(handle));
    connectedIfo.remove(handle);
    InitTable();
}

void MainWindow::InitTable()
{
    model_ = new QStandardItemModel(this);
    QStringList headers;
    headers<<QStringLiteral("name")<<QStringLiteral("last connection")
          <<QStringLiteral("state");
    model_->setHorizontalHeaderLabels(headers);
    ui->tableView->setModel(model_);
    //ui->tableView->setColumnWidth(0,400);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //列宽自动调整
    ui->tableView->setSelectionBehavior(QTreeView::SelectRows);   //一次选中整行
    ui->tableView->setEditTriggers(QTreeView::NoEditTriggers);    //不能编辑
    database a;
    userIfo=a.userget();
    model_->removeRows(0,model_->rowCount());
    showTbl(userIfo);
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString name = model_->item(index.row(),0)->text();
    lookingUser = FindItem(name);
    QTreeWidgetItemIterator it(ui->treeWidget);
    ui->stackedWidget->setCurrentIndex(1);
    ui->treeWidget->setCurrentItem(*it);
}

User *MainWindow::FindItem(QString name)
{
    for(int i=0;i<userIfo.size();i++)
    {
        if(userIfo[i]->name == name)
        {
            return userIfo[i];
        }
    }
    return NULL;
}

void MainWindow::on_Add_clicked()
{
    userAdd dlg;
    connect(&dlg,&userAdd::newUser,this,&MainWindow::appendUser);
    dlg.exec();
}

void MainWindow::on_Delete_clicked()
{
    int row = ui->tableView->currentIndex().row();
    QString name = model_->item(row,0)->text();
    QMessageBox::StandardButton confirm = QMessageBox::information
            (NULL, "Delete", "确认删除用户？", QMessageBox::Yes |
             QMessageBox::No, QMessageBox::Yes);
    if(confirm==QMessageBox::Yes)
    {
        DeleteItem(name);
        model_->removeRow(row);
    }
}

void MainWindow::on_Search_clicked()
{
    database search;
    QString text=ui->searchLine->text();
    QVector<User*> searchTbl = search.search(text);
    model_->removeRows(0,model_->rowCount());
    showTbl(searchTbl);
}

void MainWindow::DeleteItem(QString name)
{
    database del;
    emit tcp_server_->InitiativeDisConnectClient(handle);
    connectedIfo.remove(handle);
    del.userDel(name);
}

void MainWindow::appendUser(){
    database a;
    userIfo=a.userget();
    User* p = userIfo[userIfo.size()-1];
    QList<QStandardItem*> items;//
    QStandardItem* item1 = new QStandardItem(p->name);
    QStandardItem* item2 = new QStandardItem(p->time);
    QStandardItem* item3 = new QStandardItem(QString::number(p->state));
    items.append(item1);
    items.append(item2);
    items.append(item3);
    model_->appendRow(items);
}

void MainWindow::showTbl(QVector<User*> showTbl){
    model_->removeRows(0,model_->rowCount());
    for(int i=0;i<showTbl.size();i++)
    {
        User* p = showTbl[i];

        //关键字过滤
        QList<QStandardItem*> items;//
        QStandardItem* item1 = new QStandardItem(p->name);
        QStandardItem* item2 = new QStandardItem(p->time);
        QStandardItem* item3 = new QStandardItem(QString::number(p->state));
        items.append(item1);
        items.append(item2);
        items.append(item3);
        model_->appendRow(items);
    }
}

void MainWindow::saveToDatabase(QString name,QString filename){//存进数据库
    database a;
    QString temp;
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open failed." << endl;
    }else{
        QTextStream textInput(&f);
        QString lineStr;
        temp=a.lastlogTime(name,filename);
        if(temp.length()!=0){
            while(!textInput.atEnd())
            {
                lineStr = textInput.readLine();
                if(lineStr.contains(temp)){break;}
                //ui->logContent->append(lineStr);
            }
        }
        while(!textInput.atEnd())
        {
            lineStr = textInput.readLine();
            int pos = lineStr.toStdString().find_first_of(":")+6;
            a.logdataIn(name,filename,lineStr.mid(0,pos),lineStr);
        }
    }
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)//当前选择的日志类型
{
    ui->logContent->clear();
    database a;
    QVector<QString> tbl;
    int flag=0;
    if(!current->parent()){
        QString s = current->text(0);
        qDebug()<<s;
        QVector<fliter*> fliterTbl;
        tbl = a.logdataOut(lookingUser->name,s,fliterTbl);
    }
    while (flag!=tbl.size()) {
        ui->logContent->append(tbl[flag]);
        flag++;
    }
}

void MainWindow::on_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_manage_filters_triggered()
{
}

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent, QString username)
    : QWidget(parent)
  , ui(new Ui::Widget)
{
    //配置
     ui->setupUi(this);
     this->userName = username;
     udpSocket = new QUdpSocket(this);
     port = 8898;
     udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);//??

     //connect
     connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::processPendingDatagrams);//读取完毕则..
     sendMsg(UserEnter);//广播消息
}

Widget::~Widget()
{
    delete ui;
}

void Widget::usrEnter(QString username, QString IP)
{
    bool isEmpty = ui->usrTableWidget->findItems(username,Qt::MatchExactly).isEmpty();

    if(isEmpty)
    {
        QTableWidgetItem *usr = new QTableWidgetItem(username);
        QTableWidgetItem *ip = new QTableWidgetItem(IP);

        ui->usrTableWidget->insertRow(0);
        ui->usrTableWidget->setItem(0,0,usr);
        ui->usrTableWidget->setItem(0,1,ip);

        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->msgBrowser->append(tr("%1在线!").arg(username));
        ui->label->setText(tr("在线人数:%1").arg(ui->usrTableWidget->rowCount()));

        sendMsg(UserEnter);//向其他端点告知上线
    }
}

void Widget::userLeft(QString username, QString time)
{
    int rowNum = ui->usrTableWidget->findItems(username,Qt::MatchExactly).first()->row();//第一列的行数

    ui->usrTableWidget->removeRow(rowNum);
    ui->msgBrowser->setTextColor(Qt::gray);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->msgBrowser->append(tr("%1于%2离开").arg(username).arg(time));
    ui->label->setText(tr("在线人数:%1").arg(ui->usrTableWidget->rowCount()));
}

void Widget::sendMsg(MsgType type, QString addr)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);

    QString address = getIp();
    out<<type<<getUser();//消息类型 + 用户名

    switch (type)
    {
    case Msg:
    {
        if(ui->msgTextEdit->toPlainText()==""){
            QMessageBox::warning(0,"warning","发送内容为空!!");
            return;
        }
        out<<address<<getMsg();//写入本机IP  +  消息
        ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
        break;
    }
    case UserLeft:
        out<<address;//写入IP
        break;
    case FileName:
        break;
    case Refuse:
        break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);//广播 DataStream
}

QString Widget::getIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
        if(addr.protocol()==QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0;
}

QString Widget::getUser()
{
    return userName;
}

QString Widget::getMsg()
{
    QString msg = ui->msgTextEdit->toHtml();
    ui->msgTextEdit->clear();
    ui->msgTextEdit->setFocus();
    return msg;
}

void Widget::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())//有可读取内容
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());//设置缓冲区大小

        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int msgType;
        in>>msgType;
        QString username,ip,msg;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-ddhh::mm:ss");

        switch (msgType) {
        case Msg:
        {
            in>>username>>ip>>msg;
            ui->msgBrowser->setTextColor(Qt::blue);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->msgBrowser->append("["+username+"]"+time);
            ui->msgBrowser->append(msg);
        }
            break;
        case UserEnter:
        {
            in>>username>>ip;
            usrEnter(username,ip);
            break;
        }
        case UserLeft:
        {
            in>>username;
            userLeft(username,ip);
            break;
        }
        case FileName:
        {
            break;
        }
        case Refuse:
        {
            break;
        }
        }

    }
}


void Widget::on_sendBtn_clicked()
{
    sendMsg(Msg);
}




































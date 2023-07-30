#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

enum MsgType{Msg,UserEnter,UserLeft,FileName,Refuse};//消息

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent , QString username);
    ~Widget();
protected:
    void usrEnter(QString username,QString ip);
    void userLeft(QString username,QString time);
    void sendMsg(MsgType type,QString addr = "");//广播UDP消息
    QString getIp();
    QString getUser();
    QString getMsg();
private:
    QUdpSocket *udpSocket;
    qint16 port;
    QString userName;
    Ui::Widget *ui;
private slots:
    void processPendingDatagrams();//接受UDP消息
    void on_sendBtn_clicked();
};
#endif // WIDGET_H

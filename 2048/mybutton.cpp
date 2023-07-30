#include "mybutton.h"

myButton::myButton(QWidget *parent) : QPushButton(parent)
{
    //setMouseTracking(true);
}

myButton::myButton(const QString &text, QWidget *parent)
{
    QPushButton(text,parent);
}

void myButton::enterEvent(QEnterEvent *ev)
{
    qDebug()<<"enter";
}

void myButton::leaveEvent(QEvent *event)
{
    qDebug()<<"leave";
}

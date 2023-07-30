#include "mypushbutton.h"


MyPushButton::MyPushButton(const QString &text, QWidget *parent):
    QPushButton(text,parent)
{

}

void MyPushButton::enterEvent(QEnterEvent *ev)
{
    setCursor(Qt::PointingHandCursor);
//    Qt::ArrowCursor
//    Qt::PointingHandCursor
}

void MyPushButton::leaveEvent(QEvent *ev)
{
    setCursor(Qt::ArrowCursor);
}

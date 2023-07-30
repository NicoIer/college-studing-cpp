#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <qdebug.h>
class MyPushButton : public QPushButton
{
    Q_OBJECT//自定义控件必须要使用Q_OBJECT宏
public:
    MyPushButton(const QString&text,QWidget*parent=nullptr);

    void enterEvent(QEnterEvent*ev);
    void leaveEvent(QEvent*ev);
};

#endif // MYPUSHBUTTON_H

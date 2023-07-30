#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <qdebug.h>
#include <QPushButton>
class myButton : public QPushButton
{
    Q_OBJECT
public:
    explicit myButton(QWidget *parent = nullptr);
    myButton(const QString&text,QWidget*parent);
public:
    void enterEvent(QEnterEvent*ev)override;
    void leaveEvent(QEvent*event)override;
signals:

};

#endif // MYBUTTON_H

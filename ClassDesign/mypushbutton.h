#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QObject>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QWidget*parent=nullptr);
private:
    void iniPushButton();
};

#endif // MYPUSHBUTTON_H

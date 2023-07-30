#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget*parent):QPushButton(parent)
{
    iniPushButton();
}

void MyPushButton::iniPushButton()
{
    QString style ="QPushButton{"
                    "border: 1px solid #dcdfe6;"
                    "border-radius:5px;"
                    "background-color:rgb(143,122,102);"
                    "color:white;"
                    "}"
                    "QPushButton:hover{"
                    "background-color:#ecf5ff;"
                    "color:#409eff;"
                    "}";
    this->setStyleSheet(style);
}

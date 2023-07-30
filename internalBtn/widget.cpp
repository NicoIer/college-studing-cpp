#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QPushButton*btn = new QPushButton(this);

    btn->resize(100,100);
    btn->move(100,100);
    btn->show();
}

Widget::~Widget()
{
    delete ui;
}


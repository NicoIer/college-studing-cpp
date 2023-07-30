#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    qDebug()<<text;
    QFont font = ui->fontComboBox->currentFont();
    font.setBold(true);
    font.setPixelSize(48);
    ui->showLabel->setText(text);
    ui->showLabel->setFont(font);
}


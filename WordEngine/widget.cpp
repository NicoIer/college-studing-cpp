#include "widget.h"
#include "ui_widget.h"
#include "qdebug.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::OpenWord(QString &filename)
{
    word = new QWordEngine(filename);
}

void Widget::CloseOffice()
{

}

void Widget::on_pushButton_clicked()
{
    path = QFileDialog::getOpenFileName(this,"选择doc文档","E://","word(*.doc)");
    OpenWord(path);
}

void Widget::on_pushButton_3_clicked()
{
    word->addText(ui->lineEdit->text(),TITLE_ONE);
    word->insertEnter();
}

void Widget::on_pushButton_4_clicked()
{
    word->save();
}

void Widget::on_pushButton_2_clicked()
{
    if(!word->open())
        QMessageBox::warning(this,"打开失败","检查文件路径\n 是否安装word引擎");
}


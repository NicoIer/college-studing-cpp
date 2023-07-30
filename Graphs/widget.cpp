#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    iniMember();
    //消息响应
    connect(logo,&LogoWidget::start,this,[=](){
        logo->hide();
        this->show();
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *ev)
{

}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{

    return QWidget::eventFilter(watched,event);
}

void Widget::iniMember()
{
    this->setWindowIcon(QIcon(":/ChenJ.png"));
    logo = new LogoWidget(this);
    //载入窗口的实现
    logo->show();
    this->hide();



}

void Widget::on_iniBtn_clicked()
{
    ui->dfs->iniDevice();
    ui->pirm->iniDevice();
    ui->dij->iniDevice();
}


void Widget::on_startBtn_clicked()
{
    QString beg = ui->lineEdit->text();
    if(beg.isEmpty())
        return;
    ui->dfs->dfsPaint(beg);
    ui->pirm->primPaint(beg);
    ui->dij->dijPaint(beg);
}


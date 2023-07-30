#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    iniWidget();

    connect(ui->StoneGameBtn,&QPushButton::clicked,this,&Widget::StoneGame);
    connect(ui->ListBtn,&QPushButton::clicked,this,&Widget::List);
    connect(ui->BTreeBtn,&QPushButton::clicked,this,&Widget::Bst);
    connect(ui->SearchBtn,&QPushButton::clicked,this,&Widget::Search);

    connect(stoneGameWidget,&StoneGameWidget::back,this,&Widget::back);
    connect(bstWidget,&BstWidget::back,this,&Widget::back);
    connect(listWidget,&ListWidget::back,this,&Widget::back);
    connect(searchWidget,&SearchWidget::back,this,&Widget::back);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::StoneGame()
{
    //StoneGame Widget
    this->hide();
    stoneGameWidget->show();
}

void Widget::List()
{

    listWidget->show();
}

void Widget::Bst()
{
    this->hide();
    bstWidget->show();
}

void Widget::Search()
{
    this->hide();
    searchWidget->show();
}

void Widget::back()
{
    this->show();
}

void Widget::iniWidget()
{
    //ini Widget Style
    this->setWindowTitle("数据结构课设");
    this->setWindowIcon(QIcon(":/image/ChenJ.png"));

    //ini member
    stoneGameWidget = new StoneGameWidget(this);
    stoneGameWidget->setWindowFlag(Qt::Window);
    bstWidget = new BstWidget(this);
    bstWidget->setWindowFlag(Qt::Window);
    listWidget = new ListWidget(this);
    listWidget->setWindowFlag(Qt::Window);
    searchWidget = new SearchWidget(this);
    searchWidget->setWindowFlag(Qt::Window);
}


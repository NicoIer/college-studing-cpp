#include "bstwidget.h"
#include "ui_bstwidget.h"
#include <QMessageBox>
BstWidget::BstWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BstWidget)
{
    ui->setupUi(this);
}

BstWidget::~BstWidget()
{
    delete ui;
}

void BstWidget::on_pushButton_clicked()
{
    QString mid = ui->lineEdit->text();
    QString prev = ui->lineEdit_2->text();
    auto midlist = mid.split(",");
    auto prevlist = prev.split(",");
    vector<int>m;
    vector<int>p;
    for(auto&i:midlist)
        m.push_back(i.toInt());
    for(auto&i:prevlist)
        p.push_back(i.toInt());
    root = createTree2(m,p);
    QMessageBox::information(this,"WIN","二叉树构建完毕");
}


void BstWidget::on_pushButton_2_clicked()
{
    auto o = midOrder(root);
    bool flag = judge(o);
    if(flag)
        QMessageBox::information(this,"WIN","是二叉搜索树");
    else
        QMessageBox::information(this,"WIN","不是二叉搜索树");
}

void BstWidget::on_pushButton_3_clicked()
{
    auto head = transform(root);
    QMessageBox::information(this,"WIN","转化完毕");
    QString str;
    for (head;head!=nullptr ;head=head->right )
    {
        str+=QString::number(head->val)+"->";
    }
    str+="null";
    ui->showLabel->setText(str);
}


void BstWidget::on_pushButton_4_clicked()
{
    this->hide();
    emit back();
}


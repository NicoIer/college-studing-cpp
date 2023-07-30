#include "listwidget.h"
#include "ui_listwidget.h"

ListWidget::ListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListWidget)
{
    ui->setupUi(this);
}

ListWidget::~ListWidget()
{
    delete ui;
}

void ListWidget::on_pushButton_clicked()
{
    this->hide();
    emit back();
}


void ListWidget::on_createBtn_clicked()
{
    QString str = ui->lineEdit->text();
    if(str.isEmpty())
        return;
    else
    {
        vector<int>nodes;
        auto numList = str.split("#");
        bool flag = true;
        for(auto&num:numList)
        {
            nodes.push_back(num.toInt(&flag));
            if(!flag)
            {
                QMessageBox::warning(this,"error","请正确输入");
                return;
            }
        }
        //
        List l(nodes);
        this->head = l.head;//得到单链表头
        QMessageBox::information(this,"WIN","创建成功");
    }
}


void ListWidget::on_pushButton_5_clicked()
{//展示
    if(!isTwo)
    {//单头
        if(head)
        {//不为空
            QString str;
            for(ListNode*tmp =head;tmp;tmp=tmp->next)
            {//遍历
                str+=QString::number(tmp->val)+" -> ";
            }
            str+="null";
            ui->showLabel->setText(str);
        }
        else
        {
            QMessageBox::warning(this,"Error","链表为空");
        }
    }
    else
    {//双头
        if(head&&head2)
        {
            QString list1,list2;
            auto p1p2 = location(head);
            beg = p1p2.first;
            end = p1p2.second;
            qDebug()<<beg->val;
            qDebug()<<end->val;
            for(ListNode*tmp=head;tmp!=end;tmp=tmp->next)
            {
                list1+=QString::number(tmp->val)+"->";
            }
            list1+=QString::number(end->val);
            for(ListNode*tmp=head2;tmp!=end;tmp=tmp->next)
            {
                list2+=QString::number(tmp->val)+"->";
            }
            list2+=QString::number(end->val);
            ui->showLabel->setText(list1+"\n"+list2+"\n"+"从"+QString::number(beg->val)+"到"+QString::number(end->val));

        }
        else
        {
            QMessageBox::warning(this,"Error","双表头有误!!");
        }
    }
}


void ListWidget::on_finishBtn_clicked()
{
    if(!isTwo)
    {
        if(head)
        {//链表已经建立
            QString str;
            str = ui->lineEdit_2->text();
            auto nums = str.split("#");
            if(nums.size()!=2)
            {
                QMessageBox::warning(this,"Error","节点过多!!!");
                return;
            }
            int a = nums[0].toInt();
            int b = nums[1].toInt();
            auto tmp = this->head;
            isTwo = Connect(head,a,b,head2);
            if(!isTwo)
            {
                QMessageBox::warning(this,"Error","创建双头失败 检查输入");
                return;
            }
            else
            {
                QMessageBox::information(this,"WIN","创建成功");
            }
        }
        else
        {
            QMessageBox::warning(this,"Error","先建立链表");
        }
    }
    else
    {
        QMessageBox::warning(this,"Error","已经是双头带环了");
    }
}


void ListWidget::on_remakeBtn_clicked()
{
    if(isTwo)
    {
        head = remake(head,head2);
        isTwo =false;
        QMessageBox::information(this,"WIN","复原成功");
    }
    else
    {
        QMessageBox::warning(this,"ERROR","不带环！！！！");
    }
}


void ListWidget::on_judgeBtn_clicked()
{
    if(isTwo)
    {
        QMessageBox::information(this,"WIN","在节点值为"+QString::number(beg->val)+"处相交");
    }
    else
    {
        QMessageBox::warning(this,"ERROR","");
    }
}


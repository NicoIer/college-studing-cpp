#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , p (new PrintWidget)
{
    ui->setupUi(this);
    //member ini
    tree = new BstTree<QChar>();//无根的空树
    //配置窗口
    this->setWindowTitle("二叉树树的生成 绘制");
    ui->paintBtn->setEnabled(false);//未构建树之前无法绘图
    //不想再实现的功能
    ui->showBtn->setEnabled(false);
    ui->btnIniPen->setEnabled(false);
    //槽函数关联
    connect(ui->creatBtn,&QPushButton::clicked,this,&Widget::CreatTree);
    connect(ui->paintBtn,&QPushButton::clicked,this,[=](){
        p->SetRoot(tree);//设置root
        p->show();//显示窗口
    });
    //调试时使用
    connect(ui->showBtn,&QPushButton::clicked,this,[=](){
        //更多功能
    });

    connect(ui->btnIniPen,&QPushButton::clicked,this,[=](){
        //配置画笔
    });

}

Widget::~Widget()
{
    delete ui;
}
//字符串中是否有重复元素
bool Widget::unique(QString Inorder)
{
    for(auto i:Inorder)
        if(Inorder.indexOf(i)!=Inorder.lastIndexOf(i))
            return false;
    return true;
}

// 树的生成
void Widget::CreatTree()
{
    //树指针为空呢？ 没有办法判断isEmpty
    if(tree!=nullptr)//树指针不为空
    {
        if(!(tree->isEmpty()))//树非空 生成一颗新树 则delete原树
        {
            delete tree;
            tree = nullptr;
        }
    }
    Solution<QChar>deal;// = new Solution<QChar>();
    //保存对应序列输入
    auto prev=ui->prevEidt->text();
    auto post=ui->postEdit->text();
    auto in=ui->inEidt->text();
    //序列转数组 Qstring到QChar*
    QChar* prevOrder = prev.data();
    QChar* postOrder = post.data();
    QChar* inOrder = in.data();

    auto len = in.size();//计算中序大小

    //判断序列合法  之后再做
    //元素重名判断
    if(!unique(in)||!unique(prev)||!unique(post))
    {
        QMessageBox::warning(this,"异常捕获","序列有错！！！");
        ui->paintBtn->setEnabled(false);
        return;
    }
        if(!in.isEmpty()||!(prev.isEmpty()&&post.isEmpty()))//中序不为空 且 前序/后序不同时为空
        {

        //根据CheckBtn判断是何种情况的树的生成
            if(ui->prev->isChecked())//前序
            {
                try {
                    auto root = deal.bulidTreePrev(prevOrder,inOrder,len);
                    tree = new BstTree<QChar>(root);
                    QMessageBox::information(this,"树的生成","成功");
                }
                catch (...) {
                    QMessageBox::warning(this,"异常捕获","序列有错！！！");
                    ui->paintBtn->setEnabled(false);
                    return;
                }

            }
            else if(ui->post->isChecked())//后序
            {
                try {
                    auto root = deal.buildTreePost(postOrder,inOrder,len);
                    tree = new BstTree<QChar>(root);
                    QMessageBox::information(this,"树的生成","成功");
                }
                catch (...) {
                    QMessageBox::warning(this,"异常捕获","序列有错！！！");
                    ui->paintBtn->setEnabled(false);
                    return;
                }

            }
            else
            {
                QMessageBox::warning(this,"Waning","请勾选序列");
                return;
            }
        }
        else//空序列无法构造
        {
            QMessageBox::warning(this,"Waning","空树");
            return;
        }



    //树已经构造完毕
        ui->paintBtn->setEnabled(true);
}



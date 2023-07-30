#include "printwidget.h"
#include "ui_printwidget.h"

PrintWidget::PrintWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintWidget)
{
    ui->setupUi(this);
    //窗口配置
    this->setWindowTitle("PRINT");
}

PrintWidget::~PrintWidget()
{
    delete ui;
}

void PrintWidget::SetRoot(BstTree<QChar> *r)
{
    tree = r;
    iniVar();//计算参数
}

void PrintWidget::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, *pixmap);//绘制pixmap
}


void PrintWidget::DrawTree(QPainter * painter,BstNode<QChar> *root, QPoint begin, QPoint end)
{

    if(root!=nullptr)//树高为0不会画图
    {
        if(flag)//分布画图？
        {
            //停一下
            QTime dieTime = QTime::currentTime().addMSecs(500);
            while( QTime::currentTime() < dieTime )
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            update();//画一次刷新一次
        }

        painter->setPen(eilPen);//拿起画圈的笔
        painter->drawEllipse((begin+end)/2,20,20);//圈圈绘制好了 可以打包成一个函数？

        //圈圈绘好了 把元素值也打印出来吧
        painter->setPen(textPen);//拿起写字的笔
        painter->drawText(QRect((begin+end)/2-QPoint(15,60),end),root->el);//在圈圈的圆心上面画图

        //拿起画线的笔
        painter->setPen(linePen);
        //希望把线画细一点
        if(root->left!=nullptr)
            painter->drawLine((begin+end)/2,QPoint((0.75*begin.x()+0.25*end.x()),end.y()+treeHeight*0.5));//四等分点
        if(root->right!=nullptr)
            painter->drawLine((begin+end)/2,QPoint((0.25*begin.x()+0.75* end.x()),end.y()+treeHeight*0.5));//等分点公式导致出错


        begin.setY(begin.y()+treeHeight);//更新坐标
        end.setY(end.y()+treeHeight);

        auto mid = (end.x()+begin.x())/2;//求中线 ******
        DrawTree(painter,root->left,begin,QPoint(mid,end.y()));
        DrawTree(painter,root->right,QPoint(mid,begin.y()),end);
    }
    else
        return;
}

void PrintWidget::iniVar()
{
    treeHeight = tree->height(tree->Root());//初始化树的高度
    if(treeHeight==0)//无符号数 防止被0除
        treeHeight = -1;
    treeHeight = this->height()/treeHeight;//计算层高

    //初始化画布pixmap
    pixmap = new QPixmap(this->width(),this->height());//构建画布
    //构建画家
    painterOnPixmap = new QPainter(pixmap);
    pixmap->fill(Qt::white);
    //为写字做好准备
    painterOnPixmap->setFont(QFont("Arial Black",20,20));
}

void PrintWidget::iniPen(QColor color, int width, Qt::PenStyle style)
{
    //设置画笔
    linePen.setColor(color);//设置画笔颜色
    linePen.setWidth(width);//笔宽
    linePen.setStyle(style);//设置笔的风格

    textPen.setColor(QColor(255,165,0));
    textPen.setWidth(20);
    textPen.setStyle(style);

    eilPen.setColor(color);
    eilPen.setWidth(width);
    eilPen.setStyle(style);

    painterOnPixmap->setPen(linePen);//画家拿笔
}

//分布打印
void PrintWidget::on_btnStep_clicked()
{
    //重置画布 画家并更新
    iniVar();
    update();
    iniPen();

    flag = true;
    ui->btnStep->setEnabled(false);//禁用按钮
    ui->btnAll->setEnabled(false);

    DrawTree(painterOnPixmap,tree->Root(),QPoint(0,0),QPoint(this->width(),treeHeight));
    update();
    ui->btnStep->setEnabled(true);//启用按钮
    ui->btnAll->setEnabled(true);
}

//一次打印
void PrintWidget::on_btnAll_clicked()
{
    //重置画布并更新
    iniVar();
    update();
    iniPen();

    flag = false;
    ui->btnStep->setEnabled(false);//禁用按钮
    ui->btnAll->setEnabled(false);
    DrawTree(painterOnPixmap,tree->Root(),QPoint(0,0),QPoint(this->width(),treeHeight));
    update();
    ui->btnStep->setEnabled(true);//启用按钮
    ui->btnAll->setEnabled(true);
}


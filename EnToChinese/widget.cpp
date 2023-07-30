#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //配置窗口
    ui->setupUi(this);
    this->setWindowTitle("Search");
    this->setWindowIcon(QIcon(":/image/ChenJ.png"));


    //配置定时器
    timer = new QTimer(this);
    timer->start(300);//200ms检测一次

    //读取csv
    data.read_csv(":/csv/EnWords.csv");


    //为编辑框配置搜索Action
    auto searBtn = new QAction(ui->lineEdit);
    searBtn->setIcon(QIcon(":/image/searchIcon.png"));


    //为Completer配置视图模型
    stringListModel = new QStringListModel(this);


    //为编辑框配置Competer
    competer = new QCompleter(ui->lineEdit);
    competer->setMaxVisibleItems(10);//最大可见匹配
            //competer->setModelSorting()//搜索模式 基于视图模型的设置  若视图模型为树 则可以二分搜索 quick
    competer->setModel(stringListModel);//补全模型设置
    competer->setCaseSensitivity(Qt::CaseInsensitive);//大小写不敏感
            //competer->setFilterMode(Qt::MatchRecursive);//匹配规则  MatchRecursive与默认相同 这里不设置


    //配置编辑框 的搜索Action
    ui->lineEdit->setPlaceholderText("Enter to Search");
    ui->lineEdit->addAction(searBtn,QLineEdit::TrailingPosition);


    //配置编辑框的competer
    ui->lineEdit->setCompleter(competer);




    //消息响应
    connect(searBtn,&QAction::triggered,this,[=](){
        //对应Chinese的搜索在这里实现
        QString English = ui->lineEdit->text();//获取text
        if(data.map.find(English) != data.map.end())
        {//找到 pair
            qDebug()<<data.map[English];
            qDebug()<<"list查找次数"<<data.list.indexOf({English,data.map[English]});
            ui->anwser->setText(data.map[English]);
        }
    });


    //消息响应  基于时间检测的函数不太能够符合功能
    connect(timer,&QTimer::timeout,this,[=](){
        //查找并更新complete的操作在这里实现
//******************************************************************************//
        /*//基于list很容易实现 往后数几个下标就可以了  缺点时模糊查出来的并不一定是你想要的  如果是基于二叉搜索树配有权重就能够达成理想的结果
//        auto i = data.englishList.indexOf(ui->lineEdit->text());
//        //qDebug()<<data.map[ui->lineEdit->text()];
//        //qDebug()<<i;
//        indicator.clear();
//        if(i != -1)
//        {
//            ++i;//不显示自己
//            for(int j =0;j!=10;++j)
//            {
//                //qDebug()<<data.list[i+j].first;
//                indicator<<data.list[i+j].first;
//            }
//            stringListModel->setStringList(indicator);
//        }*/
//******************************************************************************//

        //基于Hash存储的更新complete
        /*static QString preText = "" ;
        auto prefix = ui->lineEdit->text();
        if(prefix==preText)
            return;
        preText = prefix;
        if(prefix.isEmpty())
            return;
        indicator.clear();
        int count = 10;//只统计10个符合要求的
        if(data.hash.find(prefix)!=data.hash.end())
        {
            --count;
            indicator<<prefix;
        }
        for(auto i = data.hash.begin();i!=data.hash.end()&&count!=0;++i)
        {
            //qDebug()<<i.key();
            if(i.key().mid(0,prefix.size())==prefix)
            {//前缀匹配
                indicator<<i.key();
                --count;
            }
        }
        stringListModel->setStringList(indicator);
        //qDebug()<<"time out";
        */

    });

    //消息响应  对于cpu来说常数时间复杂度的运算(hash遍历)不算什么   如果实际考虑到功能的话     使用线程后台计算会很好
    connect(ui->lineEdit,&QLineEdit::textEdited,this,[=](){
        //正在输入时
        ui->anwser->clear();
        //基于Hash存储的更新complete
        static QString preText = "" ;
        auto prefix = ui->lineEdit->text();
        if(prefix==preText)
            return;
        preText = prefix;
        if(prefix.isEmpty())
            return;
        indicator.clear();
        int count = 10;//只统计10个符合要求的
//        bool flag = false;
//        if(data.hash.find(prefix)!=data.hash.end())
//        {
//            --count;
//            indicator<<prefix;
//            flag  = true;
//        }
        for(auto i = data.hash.begin();i!=data.hash.end()&&count!=0;++i)
        {
            //qDebug()<<i.key();
            if(i.key().mid(0,prefix.size())==prefix)
            {//前缀匹配
//                if(flag)
//                {
//                    flag=false;
//                    continue;
//                }
                indicator<<i.key();
                --count;
            }
        }
        indicator.sort();
        stringListModel->setStringList(indicator);
        //qDebug()<<"time out";
    });

    //消息响应
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,[=](){
        searBtn->triggered(false);
    });

}

Widget::~Widget()
{
    delete ui;
}


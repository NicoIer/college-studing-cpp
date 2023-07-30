#include "searchwidget.h"
#include "ui_searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Search");

    timer = new QTimer(this);
    timer->start(300);

    //测试内容

    //..........................................
    auto searchBtn = new QAction(ui->lineEdit);
    searchBtn->setIcon(QIcon(":/image/searchIcon.png"));

    stringListModel = new QStringListModel(this);

    competer = new QCompleter(ui->lineEdit);
    competer->setMaxVisibleItems(10);
    competer->setModel(stringListModel);
    competer->setCaseSensitivity(Qt::CaseInsensitive);//大小写不敏感

    ui->lineEdit->setPlaceholderText("Enter to Search");
    ui->lineEdit->addAction(searchBtn,QLineEdit::TrailingPosition);

    ui->lineEdit->setCompleter(competer);

    connect(searchBtn,&QAction::triggered,this,[=](){
        //在这里显示找到的结果
        QString key = ui->lineEdit->text();
        auto ans = data.find(key);
        if(!ans.isEmpty())
        {//当找到的结果不为空  有很多条呢!!! 我们怎么展示呢
            QString text = "关键字出现文件(已排序)\n";
            for(auto&i:ans)
            {
                text+="文件路径:";
                text+=i.second;
                text+="出现次数:";
                text+=QString::number(i.first);
                text+="\n";
            }
            ui->label->setText(text);
        }
        //当点击搜索按钮时
        });

    connect(timer,&QTimer::timeout,this,[=](){
        //基于时间检测
    });

    connect(ui->lineEdit,&QLineEdit::textEdited,this,[=](){
        //当正在被编辑时
        ui->label->clear();
        static QString preText ="l";
        auto prefix = ui->lineEdit->text();
        if(prefix==preText)
            return;
        preText = prefix;
        if(prefix.isEmpty())
            return;
        indicator.clear();
        int count =10;

        for(auto i = data.keys.begin();i!=data.keys.end()&&count!=0;++i)
        {//这里是找关键词
            if(i.key().mid(0,prefix.size())==prefix)
            {
                indicator<<i.key();//添加关键词到补全器
                --count;
            }
        }
        indicator.sort();
        stringListModel->setStringList(indicator);//设置Edit补全器
    });
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,[=](){
        searchBtn->triggered(false);//按下Enter
    });
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::on_pushButton_2_clicked()
{
    this->hide();
    emit back();
}


void SearchWidget::on_pushButton_clicked()
{
    data.createIndex();//建立索引
    data.saveKeys();//保存索引到文件
    //data.readKeys();//从文件读取索引  建立的时候就已经保存了..
    QMessageBox::information(this,"Index","finished");
}


void SearchWidget::on_pushButton_3_clicked()
{//输入文件路径 查找出现位置 上下文
    int count = 24;//最多8-key+8个上下文片段
    auto road = ui->lineEdit_2->text();
    auto key = ui->lineEdit->text();
    QFile file;
    file.setFileName(road);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file);;
        QString text = in.readAll();
        QStringList ans;
        file.close();

        auto beg = 0;
        while(beg!=text.lastIndexOf(key))
        {
            qDebug()<<"在"<<road<<"中查找"<<key;
            beg = text.indexOf(key,beg+key.size());
            size_t x1 = beg-count<0?0:beg-count;
            qDebug()<<"从"<<x1<<"截取";
            ans.push_back(text.mid(x1,2*count));
            //怎么把对应位置的颜色标红？
            qDebug()<<beg;
        }
        qDebug()<<ans;
        QString str;
        int index=0;
        for(auto&i:ans)
        {
            str+="找到的第"+QString::number(++index)+"条上下文";
            str+=i;
            str+="\n";
        }
        str+="全文如下: \n\n\n"+text;
        QMessageBox::information(this,"在"+road+"中查找"+key,str);


    }
    else
    {
        QMessageBox::information(this,"ERROR","无法打开文件!!");
        return;
    }

}


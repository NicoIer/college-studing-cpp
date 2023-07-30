#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , highSocre(0)
{
    //ini
    ui->setupUi(this);
    ini2048();

    //消息响应
    //游戏窗口发送ScoreInc
    connect(gameWidget,&GameWidget::ScoreInc,this,&Widget::onScoreInc);
    connect(gameWidget,&GameWidget::GameOver,this,&Widget::onGameOver);
    connect(gameWidget,&GameWidget::Win,this,&Widget::onWin);

    //新游戏
    connect(restartBtn,&QPushButton::clicked,this,&Widget::restart);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::onScoreInc(unsigned int score)
{
    //add socre
    QString labelText = QString("<font style = 'font-size:20px; color:rgb(238,228,218);'>SCORE</font>"
                                "<br/>"
                                "<font style = 'font-size:25px; color:rgb(255,255,255); font-weight:bold'>%1</font>").arg(score);
    ScoreLabel->setText(labelText);

    if(score>highSocre)
    {//超越最高分
        //更新最高分
        highSocre = score;
        bestScorelabel->setText(labelText);
        //save into file
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream out(&file);
            out<<highSocre;
            file.close();
        }

    }
}

void Widget::onGameOver()
{
    //暂时草率的MessageBox一下
    QMessageBox::information(this,"GameOver","You LOST");
}

void Widget::onWin()
{
    //暂时草率的MessageBox一下
    QMessageBox::information(this,"Congratulation","You WIN");
}

void Widget::restart()
{
    qDebug()<<"重新开始";
    gameWidget->resart();
}

void Widget::resizeEvent(QResizeEvent *ev)
{
    //不允许缩放!! 哈哈哈 一步到位
}

void Widget::ini2048()
{
    //ini Widget
    this->setWindowTitle("2048");
    this->setWindowIcon(QIcon(":/image/ChenJ.png"));

    //ini gameWidget
    gameWidget = new GameWidget(this);
    gameWidget->setGeometry(2,160,400,400);
    //ini history high score
    file.setFileName("./save.json");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))//必须打开才能读取
    {
        QTextStream in(&file);
        in>>highSocre;
        qDebug()<<highSocre;
        file.close();
    }

    //ini font
    font.setFamily("Consolas");//my favor
    font.setBold(true);
    font.setPixelSize(25);//像素大小

    //ini Btn
    restartBtn = new MyPushButton("New Game",this);
    restartBtn->setGeometry(200,90,200,50);
    restartBtn->setFont(font);
    QString btnStyleSheet="QPushButton{"
                          "color:white;"
                          "border-radius: 4px;"
                          "background-color:rgb(143,122,102);"
                          "}"
                          "QPushButton:hover{"
                          "}";
    restartBtn->setStyleSheet(btnStyleSheet);//My beautiful button
    //ini Label Best
        //富文本
    QString labelText = QString("<font style = 'font-size:20px; color:rgb(238,228,218);'>BEST</font>"
                                "<br/>"
                                "<font style = 'font-size:25px; color:rgb(255,255,255); font-weight:bold'>%1</font>").arg(highSocre);
    bestScorelabel = new QLabel(labelText,this);
    bestScorelabel->setGeometry(280,20,120,60);
    bestScorelabel->setFont(font);
    bestScorelabel->setAlignment(Qt::AlignCenter);//中心布局
    QString labelStyleSheet="QLabel{"
                            "background-color:rgb(187,173,160);"
                            "border-radius: 4px;"
                            "}";
    bestScorelabel->setStyleSheet(labelStyleSheet);

    //ini Label Score
    labelText = QString("<font style = 'font-size:20px; color:rgb(238,228,218);'>SCORE</font>"
                        "<br/>"
                        "<font style = 'font-size:25px; color:rgb(255,255,255); font-weight:bold'>%1</font>").arg(0);
    ScoreLabel = new QLabel(labelText,this);
    ScoreLabel->setGeometry(150,20,120,60);
    ScoreLabel->setFont(font);
    ScoreLabel->setAlignment(Qt::AlignCenter);
    ScoreLabel->setStyleSheet(labelStyleSheet);
    //ini Widget
    this->setStyleSheet("QWidget{"
                        "background-color:rgb(250,248,239);}");
    //setEnable
    this->setFixedSize(404,562);

}


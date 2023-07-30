#include "logowidget.h"

LogoWidget::LogoWidget(QWidget *parent) : QWidget(parent)
{
    //ini
    this->setMouseTracking(true);//任何情况下都捕获鼠标  否则只有按下时才会tracking
    iniWidget();
    //消息
    connect(beginButton,&QPushButton::clicked,this,[=](){emit start();});
}

void LogoWidget::paintEvent(QPaintEvent *ev)
{

}

void LogoWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons()&Qt::LeftButton)
    {
        QPointF y = ev->globalPosition();
        QPointF x = y-this->z;
        qDebug()<<"X = "<<x.x()<<"Y = "<<x.y();
        this->move(x.x(),x.y());
    }

    QWidget::mouseMoveEvent(ev);
}

void LogoWidget::mousePressEvent(QMouseEvent *ev)
{
    QPointF y = ev->globalPosition();//鼠标相对于0,0的位置
    QPointF x = this->geometry().topLeft();//获取窗口左上角坐标
    this->z = y - x;//鼠标相对位移
    if(ev->buttons()&Qt::LeftButton)
    {
    qDebug()<<"鼠标左键按下";
    }

    QWidget::mousePressEvent(ev);
}

void LogoWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug()<<"鼠标释放";
    this->z=QPointF();
    QWidget::mouseReleaseEvent(ev);
}

void LogoWidget::iniWidget()
{
    //属性配置
    this->setWindowFlag(Qt::CustomizeWindowHint);//无标题
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框
    this->setWindowFlag(Qt::Dialog);
    this->setAttribute(Qt::WA_TranslucentBackground,true);//变得透明
    this->resize(320,200);
    //属性成员初始化
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(25);
    //控件成员初始化
    beginButton = new QPushButton("START",this);
    logo = new QLabel(this);

    //成员属性配置
        //Button
    beginButton->setGeometry(60,125,200,50);
    beginButton->setFont(font);
    QString StyleSheet={"QPushButton{"
                        "color:white;"
                        "border-radius:4px;"
                        "background-color:rgb(143,122,102);}"};
    beginButton->setStyleSheet(StyleSheet);
        //Label
    QString labelText = QString("<font style = 'font-size:20px; color:rgb(238,228,218);'>Programed By ChenJ</font>");
    logo->setFont(font);
    logo->setGeometry(10,55,300,55);
    logo->setAlignment(Qt::AlignCenter);
    logo->setText(labelText);
    QString labelStyleSheet="QLabel{"
                            "background-color:rgb(187,173,160);"
                            "border-radius: 4px;"
                            "}";
    logo->setStyleSheet(labelStyleSheet);










}

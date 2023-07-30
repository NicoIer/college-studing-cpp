#include "stonegamewidget.h"
#include "ui_stonegamewidget.h"

StoneGameWidget::StoneGameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StoneGameWidget),
    pix(new QPixmap(700,240))
{
    ui->setupUi(this);
    Nico.loadModel();
    game = GamePlay(10);

    iniPix();
}

StoneGameWidget::~StoneGameWidget()
{
    delete ui;
}

void StoneGameWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.drawPixmap(10,120,*pix);
}

void StoneGameWidget::iniPix()
{
    //装填节点以及计算打印坐标
    for(auto&u:game.gameGraph.graphs)//对所有节点
    {
        QString pointName = QString::number(int(u.first.player))+"p"+QString::number(u.first.stone);
        QPoint point(30+u.first.stone*60,80*(1+int(u.first.player)));
        points.insert(pointName,point);
    }

    QPainter painter(pix);
    pix->fill(Qt::white);
    for(auto&point:points)
    {
        painter.drawEllipse(point,24,24);
        qDebug()<<point;
    }
    update();
}

void StoneGameWidget::on_pushButton_2_clicked()
{
    this->hide();
    emit back();
}


void StoneGameWidget::on_pushButton_clicked()
{
    iniPix();
    QPainter whitePainter(pix);
    whitePainter.setPen(Qt::green);
    QPainter blackPainter(pix);
    Player p = game.now.player;
    QMessageBox::information(this,"Start","玩家"+QString::number(int(p)+1)+"先手"+"\n您是玩家2");
    while(!game.over())
    {
        ui->label_2->setText("玩家"+QString::number(1+int(game.now.player))+"回合");
        QString pointName = QString::number(int(game.now.player))+"p"+QString::number(game.now.stone);
        //当前点的坐标
        whitePainter.drawEllipse(points[pointName],24,24);
        update();
        int select =0;
        if(game.now.player==Nico.now.player)
        {
            Nico.now = game.now;
            select = Nico.out();
            QMessageBox::information(this,"Nico回合","Nico选择拿"+QString::number(select)+"个石头");
        }
        else
        {
            select = QInputDialog::getInt(this,"提示","请选择取的石头",1,1,2,1);
        }
        update();
        if (select == 1)
            if (!++(game))
                QMessageBox::information(this,"ERROR","不能拿"+QString::number(select)+"个石头");
        if (select == 2)
            if (!(game)++)
                QMessageBox::information(this,"Nico","不能拿"+QString::number(select)+"个石头");

        //到这里证明取成功了  我们来修改图片
        blackPainter.drawEllipse(points[pointName],24,24);
        update();
    }

    switch (game.now.player)
    {
    case Player::PLAYER1: QMessageBox::information(this,"WIN","您赢了");; break;
    case Player::PLAYER2:QMessageBox::information(this,"WIN","Nico赢了"); break;
    }
    game.reStart();
}


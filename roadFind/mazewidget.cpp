#include "mazewidget.h"
#include "ui_mazewidget.h"

MazeWidget::MazeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MazeWidget)
{
    ui->setupUi(this);
    iniMaze();//默认初始化迷宫



}

MazeWidget::~MazeWidget()
{
    delete ui;
}

void MazeWidget::paintEvent(QPaintEvent *pe)
{//绘图函数
    QPainter painter(this);
    //绘制装有迷宫样式的Pixmap
}

bool MazeWidget::seekPath(int xBegin, int yBegin)
{
    if(maze.isEmpty())
        return false;//迷宫不存在

    int rx,ry;
    Direction dir;

    for(int i =0 ;i!=8;++i)
    {
        rx = xBegin+Move(Direction(i)).first;
        ry = yBegin+Move(Direction(i)).second;
        dir = Direction(i);

        if(maze[rx][ry]==true&&mark[rx][ry]==true)//路可走 且未被锁定
        {
            mark[rx][ry]=false;//锁定已走过的路
            if(seekPath(rx,ry))
            {
                qDebug()<<"向("<<rx<<","<<ry<<")"<<"移动";
                return true;
            }
        }
    }
    if(xBegin==enter.first&&yBegin==enter.second)
        qDebug()<<"no Path";
    return false;
}

void MazeWidget::iniPainter()
{
    //初始画家

}


bool MazeWidget::iniMaze(QVector<QVector<bool> > Maze)
{
    if(!Maze.isEmpty())
    {
        maze = Maze;
        QVector<bool>temp(Maze[0].size(),1);
        mark = QVector<QVector<bool>>(Maze.size(),temp);
        //出口没有传进来 后面可以改
    }
    else
    {
        //初始化迷宫 0代表墙壁 1代表路
        maze={	{0,0,0,0,0,1,0,0,0},
                {1,1,0,0,0,0,1,0,0} ,
                {0,1,1,0,0,0,0,1,0} ,
                {0,0,1,1,0,0,0,1,0} ,
                {0,0,0,1,0,0,1,0,0} ,
                {0,0,0,1,1,1,0,0,1} ,
                {0,0,0,0,0,0,1,1,0} ,
                {0,0,0,0,0,0,0,0,0} ,
                {0,0,0,0,0,0,0,0,0} };
        //初始化标记
        mark={	{1,1,1,1,1,1,1,1,1},
                {1,1,1,1,1,1,1,1,1} ,
                {1,1,1,1,1,1,1,1,1} ,
                {1,1,1,1,1,1,1,1,1} ,
                {1,1,1,1,1,1,1,1,1} ,
                {1,1,1,1,1,1,1,1,1} ,
                {1,1,1,1,1,1,1,1,1} ,
                {1,1,1,1,1,1,1,1,1} ,
                {1,1,1,1,1,1,1,1,1} };

        outer={5,8};
        enter={1,1};
    }

}

QPair<int, int> MazeWidget::Move(Direction dir)
{
    switch (dir)
    {
    case N:
        return {-1,0};
        break;
    case NE:
        return {-1,1};
        break;
    case NW:
        return {-1,-1};
        break;
    case W:
        return {0,-1};
        break;
    case E:
        return {0,1};
        break;
    case S:
        return {1,0};
        break;
    case SE:
        return {1,1};
        break;
    case SW:
        return {1,-1};
        break;
    }
    throw std::runtime_error("未知方位");
}

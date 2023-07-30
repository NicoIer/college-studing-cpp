#include "gamewidget.h"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent),
    board({{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}})
{
    //connect

    iniColors();
    iniVar();
    iniBoard();
    this->grabKeyboard();
    connect(this,&GameWidget::GestureMove,this,&GameWidget::onGestureMove);
    connect(timer,&QTimer::timeout,this,[=](){update();});


}

void GameWidget::paintEvent(QPaintEvent *ev)
{
    //painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿绘图

    //背景绘制

    QBrush brush(QColor(187,173,160));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(this->rect(),4,4);

    //画格子 画字
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40);
    painter.setFont(font);

    //正在执行动画
    if(isAnimating)
    {
        if(drawAnimation(painter))
        {
            isAnimating = false;
            timer->stop();
            //清除动画
            animationList.clear();

            if(check())
                emit GameOver();
            if(win())
                emit win();
        }
        return;
    }


    for(int i = 0;i!=4;++i)
        for(int j =0;j!=4;++j)
        {
            if(board[i][j])//不为0  即有数字
            {
                qDebug()<<"画有颜色的数字";
                brush.setColor(backColors[getBitCount(board[i][j])]);
                painter.setBrush(brush);
                painter.drawRoundedRect(QRect(QPoint(8+j*w,8+i*h),QPoint(j*w+w,i*h+h)),4,4);
                if(board[i][j]==2||board[i][j]==4)
                {
                    painter.setPen(numColors[0]);
                    painter.drawText(QRectF(QPoint(8+j*w,8+i*h),QPoint(j*w+w,i*h+h)),Qt::AlignCenter,QString::number(board[i][j]));
                    painter.setPen(Qt::NoPen);
                }
                else
                {
                    painter.setPen(numColors[1]);
                    painter.drawText(QRectF(QPoint(8+j*w,8+i*h),QPoint(j*w+w,i*h+h)),Qt::AlignCenter,QString::number(board[i][j]));
                    painter.setPen(Qt::NoPen);
                }
            }
            else
            {
                brush.setColor(QColor(205,193,180));//空背景色
                painter.setBrush(brush);
                painter.drawRoundedRect(QRect(QPoint(8+j*w,8+i*h),QPoint(j*w+w,i*h+h)),4,4);
            }
        }

}

void GameWidget::enterEvent(QEnterEvent *ev)
{
    qDebug()<<"enter";
}

void GameWidget::leaveEvent(QEvent *ev)
{
    qDebug()<<"leave";
}

void GameWidget::keyPressEvent(QKeyEvent *ev)
{
    if(isAnimating)
        return;
    switch(ev->key())
    {
    case Qt::Key_Down:
    {
        emit GestureMove(down);
        break;
    }
    case Qt::Key_Right:
    {
        emit GestureMove(right);
        break;
    }
    case Qt::Key_Up:
    {
        emit GestureMove(up);
        break;
    }
    case Qt::Key_Left:
    {
        emit GestureMove(left);
        break;
    }
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug()<<"按下按钮啦";
}

bool GameWidget::drawAnimation(QPainter &painter)
{
    QBrush brush(QColor(205,193,180));
    painter.setBrush(brush);

    //画每个
    for(int i = 0;i!=4;++i)
        for(int j =0;j!=4;++j)
            painter.drawRoundedRect(QRect(QPoint(8+j*w,8+i*h),QPoint(j*w+w,i*h+h)),4,4);

    //为每个方格播放动画
    for(auto& i:animationList)
        if(!playAnimation(i,painter))
            return false;

    return true;
}

bool GameWidget::playAnimation(Animation &animation, QPainter &painter)
{
    //qDebug()<<"Beg"<<animation.beginPos<<"End"<<animation.endPos;
    bool ok = false;
    QBrush brush(Qt::SolidPattern);
    //move
    if(animation.type==AnimationType::move)
    {
        switch (animation.direct)
        {
        case left:
        {
            if(animation.beginPos.x()>animation.endPos.x())
                animation.beginPos += dPos[left];
            else
            {
                animation.beginPos = animation.endPos;
                ok = true;
            }
            break;
        }
        case right:
        {
            if (animation.beginPos.x() < animation.endPos.x())
                animation.beginPos += dPos[right];
            else
            {
                animation.beginPos = animation.endPos;
                ok = true;
            }
            break;
        }
        case up:
        {
            if (animation.beginPos.y() > animation.endPos.y())
                animation.beginPos += dPos[up];
            else
            {
                animation.beginPos = animation.endPos;
                ok = true;
            }
            break;
        }
        case down:
        {
            if (animation.beginPos.y() < animation.endPos.y())
                animation.beginPos += dPos[down];
            else
            {
                animation.beginPos = animation.endPos;
                ok = true;
            }
            break;
        }
        }
        //方格已经到终点
        if(!ok)
        {
            brush.setColor(backColors[getBitCount(animation.digit)]);
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(animation.beginPos.x(),animation.beginPos.y(),w-8,h-8),4,4);
            if(animation.digit==2||animation.digit==4)
                painter.setPen(numColors[0]);
            else
                painter.setPen(numColors[1]);
            painter.drawText(QRectF(animation.beginPos.x(),animation.beginPos.y(),w-8,h-8),Qt::AlignCenter,QString::number(animation.digit));
        }
        else
        {
            brush.setColor(backColors[getBitCount(animation.digit2)]);
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(animation.beginPos.x(),animation.beginPos.y(),w-8,h-8),4,4);
            if(animation.digit2==2||animation.digit2==4)
                painter.setPen(numColors[0]);
            else
                painter.setPen(numColors[1]);
            painter.drawText(QRectF(animation.beginPos.x(),animation.beginPos.y(),w-8,h-8),Qt::AlignCenter,QString::number(animation.digit2));

        }
        painter.setPen(Qt::NoPen);
    }

    //appear
    if(animation.type==AnimationType::appear)
    {

        if(animation.beginPos.x()>animation.endPos.x())
            animation.beginPos += dPos[4];

        else
        {
            animation.beginPos = animation.endPos;
            ok = true;
        }

        brush.setColor(backColors[getBitCount(animation.digit)]);
        painter.setBrush(brush);
        painter.drawRoundedRect(QRectF(animation.beginPos.x(), animation.beginPos.y(),
                                w -8- 2 * (animation.beginPos.x() - animation.endPos.x()),
                                h -8- 2 * (animation.beginPos.y() - animation.endPos.y())), 4, 4);
        if(animation.digit==2||animation.digit==4)
            painter.setPen(numColors[0]);
        else
            painter.setPen(numColors[1]);
        painter.drawText(QRectF(animation.endPos.x(),animation.endPos.y(),w-8,h-8),
                         Qt::AlignCenter,QString::number(animation.digit));
        painter.setPen(Qt::NoPen);
    }

    return ok;
}

bool GameWidget::check()
{
    if(digitCount==16)
    {
        // 循环检测是否含有相邻的相同数码
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                if (j != 3 && board[i][j] == board[i][j + 1])
                    return false;
                if (i != 3 && board[i][j] == board[i + 1][j])
                    return false;
            }
        return true;
    }

    return false;

}

bool GameWidget::win()
{
    //2048?
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (board[i][j] == 2048)
                return true;
    return false;
}

void GameWidget::onGestureMove(GestureDirect direct)
{
    //遍历相关
    int i,j,k;
    //动画相关
    Animation a;
    bool move = false;
    bool merge = false;
    //mark
    bool isMerge[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

    switch (direct)
    {
    case left:
    {
        for(i=0;i!=4;++i)
        {//left 行 变化
            //[j][k]表示要换到的位置
            j = k = 0;
            while (true)
            {
                while (j<4&&board[i][j]==0) ++j;//找到第一个非0的列
                if(j==4)//该列是边缘 搜索完毕
                    break;
                //交换    代表往0的位置动
                qSwap(board[i][k],board[i][j]);
                if(j!=k)
                    move = true;
                //记录动画
                a.type = AnimationType::move;
                //QRect(QPoint(8+j*w,8+i*h),QPoint(j*w+w,i*h+h)));
                a.beginPos = QPointF(8+j*w, 8+i*h);
                a.endPos = QPointF(8+k*w, 8+i*h);
                a.digit = a.digit2 = board[i][k];

                a.direct = left;

                //合并判断
                if(k!=0&&board[i][k] == board[i][k-1]&&!isMerge[i][k-1])
                {
                    board[i][k-1]<<=1;//*2
                    isMerge[i][k-1]=true;
                    board[i][k] = 0;//这一列数字置0
                    //有方块合并
                    merge = true;
                    a.digit2 = board[i][k - 1];
                    a.endPos = QPointF(8+w*(k-1),8+h*i);
                    // 增加分数
                    score += board[i][k - 1];
                    // 发射增加分数的信号
                    emit ScoreInc(score);
                    // 数码个数-1
                    digitCount--;
                }
                else
                    ++k;
                ++j;

                animationList.append(a);//追加到动画列表
            }
        }
        break;
    }
    case right:{
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3;
            while (true)
            {
                while (j > -1 && board[i][j] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[i][k], board[i][j]);
                if (j != k)
                    move = true;
                a.type = AnimationType::move;
                a.beginPos = QPointF(8+w*j,8+h*i);
                a.endPos = QPointF(8+w*k,8+h*i);
                a.digit = a.digit2 = board[i][k];
                a.direct = right;
                //存在相同数字
                if (k < 3 && board[i][k] == board[i][k + 1] && !isMerge[i][k + 1])
                {
                    board[i][k + 1] <<= 1;
                    isMerge[i][k + 1] = true;
                    board[i][k] = 0;
                    merge = true;
                    a.digit2 = board[i][k + 1];
                    a.endPos = QPointF(8+w*(k+1),8+h*i);
                    score += board[i][k + 1];
                    emit ScoreInc(score);
                    digitCount--;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
    }
    case up:
    {
        for (i = 0; i < 4; i++)
        {
            j = 0, k = 0;
            while (true)
            {
                while (j < 4 && board[j][i] == 0)
                    j++;
                if (j > 3)
                    break;
                qSwap(board[k][i], board[j][i]);
                if (j != k)
                    move = true;
                a.type = AnimationType::move;
                a.beginPos = QPointF(8+w*i,8+h*j);
                a.endPos = QPointF(8+w*i,8+h*k);
                a.digit = a.digit2 = board[k][i];
                a.direct = up;
                if (k > 0 && board[k][i] == board[k - 1][i] && !isMerge[k - 1][i])
                {
                    board[k - 1][i] <<= 1;
                    isMerge[k - 1][i] = true;
                    board[k][i] = 0;
                    merge = true;
                    a.digit2 = board[k - 1][i];
                    a.endPos = QPointF(8+w*i,8+h*(k-1));
                    score += board[k - 1][i];
                    emit ScoreInc(score);
                    digitCount--;
                }
                else
                    k++;
                j++;
                animationList.append(a);
            }
        }
        break;
    }
    case down:
    {
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3;
            while (true)
            {
                while (j > -1 && board[j][i] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[k][i], board[j][i]);
                if (j != k)
                    move = true;

                a.type = AnimationType::move;
                a.beginPos = QPointF(8+w*i,8+h*j);
                a.endPos = QPointF(8+w*i,8+h*k);
                a.digit = a.digit2 = board[k][i];
                a.direct = down;

                if (k < 3 && board[k][i] == board[k + 1][i] && !isMerge[k + 1][i])
                {
                    board[k + 1][i] <<= 1;
                    isMerge[k + 1][i] = true;
                    board[k][i] = 0;
                    merge = true;
                    a.digit2 = board[k + 1][i];
                    a.endPos = QPointF(8+w*i,8+h*(k+1));
                    score += board[k + 1][i];
                    emit ScoreInc(score);
                    digitCount--;
                }
                else
                    k--;

                j--;
                animationList.append(a);
            }
        }
    }
    }

    //可以在这里延时

    if((move||merge)&&digitCount !=16)
    {
        i = rand()%4;
        j = rand()%4;
        while(board[i][j]!=0)
        {
            i = rand()%4;
            j = rand()%4;
        }
        //生成2或者4
        board[i][j]=(rand()%2+1)*2;

        a.type = appear;
        a.beginPos = a.endPos = QPointF(8+w*j,8+h*i);
        a.beginPos +=QPointF(w/2,h/2);
        a.digit = board[i][j];
        animationList.append(a);
        ++digitCount;
    }

    isAnimating = true;//开始绘制动画
    timer->start(10);//timer start
}

void GameWidget::resart()
{
    score = 0;
    digitCount = 2;
    board = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    iniBoard();
    emit ScoreInc(score);
    update();//执行 paintEvent
}

void GameWidget::iniVar()
{
    timer = new QTimer(this);

    dPos={QPointF(-24.5,0),
          QPointF(24.5,0),
          QPointF(0,-24.5),
          QPointF(0,24.5),
          QPointF(-6.125,-6.125)};
}

void GameWidget::iniBoard()
{
    board[rand() % 4][rand() % 4] = 2;
    int i = rand() % 4, j = rand() % 4;
    while (board[i][j] != 0)
        i = rand() % 4, j = rand() % 4;
    board[i][j] = 2;
    update();
}

unsigned int GameWidget::getBitCount(unsigned int n)
{
    int c =0;
    while(n>>=1)
        ++c;
    return c-1;
}

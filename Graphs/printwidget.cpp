#include "printwidget.h"
PrintWidget::PrintWidget(QWidget *parent) : QWidget(parent)
{
    iniMember();
    iniDevice();//便于调试

    //线程相关

}

void PrintWidget::leaveEvent(QEvent *ev)
{
    //qDebug()<<"鼠标离开";
    setCursor(Qt::ArrowCursor);

}

void PrintWidget::enterEvent(QEnterEvent *ev)
{
    //qDebug()<<"鼠标进入";
    setCursor(Qt::PointingHandCursor);

}

void PrintWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,*pixmap);
}

void PrintWidget::resizeEvent(QResizeEvent *)
{
    //resize之后要重新指定画布
    iniPainter();
    drawBeginGraph();//重新绘制原图

}

void PrintWidget::iniDevice()
{
    flag = true;
    iniPainter();
    drawBeginGraph();
}


void PrintWidget::primPaint(const QString&beg)
{
    //给笔换色

    //Graph<QString> minSpanningTree;//最小生成树
    //minSpanningTree.addVertex(beg);//添加节点到已访问集
    //把添加的节点换成已访问色
    pointPen.setColor(colors["textOfEndPOint"]);
    linePen.setColor(colors["lineOfEnd"]);
    pixmapPainter->setPen(pointPen);
    pixmapPainter->drawEllipse(points[beg],20,20);
    update();
    //优先级队列 (元素类型 存储容器 比较函数)
        //队列按权值 int 对 pair<T,T>排序
            //pair<T,T>的first是u(存在于生成树中的顶点)  第二个是v(未加入生成树的节点)
    priority_queue<pair<int, pair<QString, QString>>, vector<pair<int, pair<QString, QString>>>, greater<pair<int, pair<QString, QString>>>> q;

    //已访问节点集合
    set<QString> visited;
    pointPen.setColor(colors["pointOfSearch"]);//正在试探色
    for(auto neighor:graph[beg])
    {//beg的邻居加入 访问队列
        q.push({ neighor.weight,{beg,neighor.vertex} });
        //换色吧 把邻居换成试探色
        delay(500);
        qDebug()<<"给"<<neighor.vertex<<"换色";
        pixmapPainter->drawEllipse(points[neighor.vertex],20,20);
        update();
    }

    while(!q.empty())
    {
        auto front = q.top();
        q.pop();

        QString u = front.second.first;

        QString v = front.second.second;//邻居中最小的

        if(visited.find(v)!=visited.end())
            continue;
        else
            visited.insert(v);//邻居中最小的 且 未被访问的

        pointPen.setColor(colors["textOfEndPOint"]);
        pixmapPainter->setPen(pointPen);
        pixmapPainter->drawEllipse(points[v],20,20);
        update();

        delay(500);
        pixmapPainter->setPen(linePen);//换成画线的笔

        pixmapPainter->drawLine(points[v],points[u]);//在u,v中重新绘线
        update();

        //给自己也要闪一次
        pointPen.setColor(colors["pointOfSearch"]);//正在试探色 试探邻居
        pixmapPainter->setPen(pointPen);
        //pixmapPainter->drawEllipse(points[v],20,20);
        for (auto neighbor : graph[v])
            if (visited.find(neighbor.vertex) == visited.end())
            {
                q.push({ neighbor.weight,{v,neighbor.vertex} });
                pixmapPainter->drawEllipse(points[neighbor.vertex],20,20);
                delay(600);
                update();
            }
    }


}

void PrintWidget::dijPaint(const QString&beg)
{
    vector<QColor> c{
        {Qt::white},
        {Qt::red},
        {Qt::blue},
        {Qt::yellow},
        {Qt::green},
        {Qt::black}
    };
    //用dis存放start到任意一个vertex的最短距离int
    map<QString, int> dis;
    //和pirm不同 这里的pair只用存放距离和点即可 因为起始点是start是唯一的
    priority_queue<pair<int, QString>, vector<pair<int, QString>>, greater<pair<int, QString>>> q;

    pixmapPainter->setFont(font);
    pointPen.setWidth(4);
    textPen.setWidth(4);
    textPen.setColor(c[0]);
    pixmapPainter->setPen(textPen);
    for(QString vertex:graph.getVertexs())
    {//初始化到其他点的距离
        if(vertex == beg)
        {
            dis[beg] = 0;//把直接在脸上写字
            pixmapPainter->drawText(points[vertex],QString::number(dis[vertex]));
            delay(300);
            update();
        }
        else
        {
            dis[vertex] = INT_MAX;
            pixmapPainter->drawText(points[vertex],"MAX");
            delay(300);
            update();
        }
    }
    set<QString> visited;

    q.push({0,beg});//初始点入栈
    QPoint dev(10,0);
    auto temp = beg;
    size_t count = 1;
    while (!q.empty())
    {
        if(count==c.size())
            count =0;
        auto front = q.top();
        q.pop();

        //获取当前正在访问节点
        QString u = front.second;
        pointPen.setColor(Qt::gray);
        pixmapPainter->setPen(pointPen);
        pixmapPainter->drawEllipse(points[u],20,20);//访问你就把你换个色

        pixmapPainter->drawLine(points[beg],points[u]);
        temp = u;
        delay(400);
        update();

        if (visited.find(u) != visited.end())
            continue;
        else
            visited.insert(u);
        //获取到达u的最短路径    因为优先队列已经排过序了
        int shortest_distance_to_u = front.first;
        dis[u] = shortest_distance_to_u;

        //更新距离
        textPen.setColor(c[count]);
        ++count;
        pixmapPainter->setPen(textPen);
        for(auto v:graph[u])//对u的所有邻居
            if (visited.find(v.vertex) == visited.end())//若没有被访问过
            {
                int distance_to_v = v.weight;//u到u的距离
                //start到v的距离 加入结果
                q.push({ shortest_distance_to_u + distance_to_v,v.vertex });
                pixmapPainter->drawText((points[v.vertex]+dev),QString::number(shortest_distance_to_u + distance_to_v));
                delay(600);
                update();
            }
        dev+=QPoint(25,0);

    }

}

void PrintWidget::dfsPaint(const QString&u)
{//深度优先遍历绘图
    //给笔换色
    textPen.setColor(colors["textOfEndPOint"]);
    linePen.setColor(colors["lineOfEnd"]);
    pointPen.setColor(colors["textOfWeight"]);
    //开始吧
    set<QString> visited;
    if(graph.isContain(u))
    {
        //qDebug()<<u<<"在Graph中";
        dfsHelper(u,visited);
    }
}

void PrintWidget::dfsHelper(const QString &u, set<QString> &visited)
{
    visited.insert(u);//标记已经访问
    //qDebug()<<"当前访问节点"<<u;
    //对标记节点绘图
    delay(500);
    pixmapPainter->setPen(pointPen);//换笔
    pixmapPainter->drawEllipse(points[u],20,20);
    update();
    delay(500);
    for(Edge<QString>edge:graph[u])
        if(visited.find(edge.vertex)==visited.end())
        {
            //画图吧
            pixmapPainter->setPen(linePen);
            pixmapPainter->drawLine(points[u],points[edge.vertex]);
            pixmapPainter->setPen(textPen);
            pixmapPainter->drawText((points[u]+points[edge.vertex])/2,QString::number(edge.weight));
            update();
            dfsHelper(edge.vertex,visited);//访问这条边 u - vertex
        }
}

void PrintWidget::drawBeginGraph()
{
    if(!flag)
        return;
    //这里要画线 画圆 画字
    pixmapPainter->setFont(font);

    for(auto point = points.begin();point!=points.end();++point)
    {//把点画出来
        pixmapPainter->setPen(pointPen);
        pixmapPainter->drawEllipse(point.value(),20,20);//画圆
    }
    //现在要给点连线
    pixmapPainter->setPen(linePen);
    for(auto beg = graph.begin();beg!=graph.end();++beg)
    {
        auto vertex = (*beg).first;//点名(QString)
        auto edges = (*beg).second;//其对应边集合(set<Edge<T>  QString:int)
        for(auto edge = edges.begin();edge!=edges.end();++edge)
        {//C++库的迭代器要解引用
            if(graph.isAdjacent(vertex,edge->vertex))
            {//若两点相邻
                pixmapPainter->setPen(linePen);
                pixmapPainter->drawLine(points[vertex],points[edge->vertex]);
                pixmapPainter->setPen(textPen);
                pixmapPainter->drawText((points[vertex]+points[edge->vertex])/2,QString::number(edge->weight));
            }
        }
    }
    //后写字避免 覆盖了

    for(auto point = points.begin();point!=points.end();++point)
    {//把字写出来
        pixmapPainter->setPen(pointPen);
        pixmapPainter->drawEllipse(point.value(),20,20);//画圆
        pixmapPainter->setPen(textPen);
        pixmapPainter->drawText(point.value()+QPoint(-6,5),point.key());
    }
    update();
}

void PrintWidget::delay(unsigned int msecs)
{
    QTime dieTime = QTime::currentTime().addMSecs(msecs);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

}

void PrintWidget::iniMember()
{
    iniGraph();
    iniFont();
    iniColors();
    iniPainter();
}

void PrintWidget::iniPen()
{
    //pen of point
    pointPen.setColor(colors["backOfPoint"]);
    pointPen.setWidth(5);
    pointPen.setStyle(Qt::PenStyle::SolidLine);
    //text pen
    textPen.setColor(colors["textOfBeginPoint"]);
    textPen.setStyle(Qt::PenStyle::SolidLine);
    textPen.setWidth(6);
    //pen of line
    linePen.setColor(colors["lineOfBegin"]);
    linePen.setWidth(4);
    linePen.setStyle(Qt::PenStyle::SolidLine);
    //pen of background
    backPen.setBrush(colors["background"]);
    backPen.setStyle(Qt::PenStyle::SolidLine);
}

void PrintWidget::iniFont()
{
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(18);
}

void PrintWidget::iniColors()
{
    colors=
    {
        {"backOfPoint",QColor(238,206,112)},
        {"textOfBeginPoint",Qt::white},//未被访问节点
        {"textOfEndPOint",QColor(236,200,200)},//已被访问节点
        {"background",QColor(187,165,163)},//Pixmap背景色
        {"lineOfBegin",QColor(242,176,121)},//未被访问边
        {"lineOfEnd",QColor(34,245,10)},//访问过的边
        {"lineOfSearching",QColor(239,197,63)},//正在试探的边
        {"textOfWeight",QColor(245,124,96)},
        {"pointOfSearch",Qt::green}//尝试节点
    };

}

void PrintWidget::iniGraph()
{//对图初始化
    //Text IO
    file.setFileName("./Graphs.json");//编译的时候 影子构建会影响文件路径!!
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&file);
        //准备读取文件中的Graph
        while (!in.atEnd())
        {
            QString data = in.readLine();//获取一行

            QString name = getName(data);
            QPoint point = transToPoint(data);

            points.insert(name,point);//点 - 坐标 集    便于绘图!!!!

            set<Edge<QString>> edgesOf = transToEdges(data);

            graph.addVertex(name);//先添加一个孤儿节点

//            qDebug()<<"当前节点"<<name;
//            qDebug()<<"节点坐标"<<point;
            for(auto &i:edgesOf)
            {
                graph.addVertex(i.vertex);//再加一个orphan node
                graph.addEdge(name,i.vertex,i.weight);//be orphans
//                qDebug()<<i.vertex<<"是"<<name<<"的相邻节点";
            }
//            qDebug()<<"展示一下 看图对不对";
//            graph.show();
            //very good !!
        }

        file.close();
    }
    else
        qDebug()<<"open Graph.json failed";
//        qDebug()<<QDir::currentPath();

    //图读取OK

}

void PrintWidget::iniPainter()
{
    iniPen();
    pixmap = new QPixmap(this->size());
    pixmapPainter = new QPainter(pixmap);
    pixmap->fill(colors["background"]);
}


























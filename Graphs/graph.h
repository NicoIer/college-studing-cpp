#ifndef GRAPH_H
#define GRAPH_H

#define QTDEBUG

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <vector>
#include <queue>
#include <limits>//+无穷


#ifdef QTDEBUG
#include <qdebug.h>
#include <QPoint>
#include <QString>
#include <QStringList>
#endif
#ifndef QTDEBUG
#include <iostream>
#endif



using namespace std;
//边
template <typename T>
class Edge {
public://数据成员
    T vertex;//相邻顶点
    int weight;

public://create
    Edge(T neighborVertex);
    Edge(T neighborVertex, int weight);
public://overload  使其能够存储于map
    bool operator<(const Edge& other)const
    {
        return this->weight < other.weight;
    }

    bool operator>(const Edge& other)const
    {
        return this->vertex > other.vertex;
    }
    //具有相同的端点则== 而不是weight.equal
    bool operator==(const Edge& other)const
    {
        return this->vertex == other.vertex;
    }
};

//加权无向图的定义
template <typename T>
class Graph
{
public://数据成员
    //邻接表储存图 v1:e1-v2
    unordered_map<T, set<Edge<T>>> adjacentGraph;
public://create
    Graph();
public://easy function
    auto begin()
    {
        return adjacentGraph.begin();
    }
    auto end()
    {
        return adjacentGraph.end();
    }
    auto operator[](const T&key)
    {
        return adjacentGraph[key];
    }
public://functiom
    bool isContain(const T& u);//u is in graph?
    bool isAdjacent(const T& u, const T& v);//u is adjacent to v ?

    void addVertex(const T& u);//add u into graph
    void addEdge(const T& u, const T& v, int weight);//add u - edge - v

    void changeWeight(const T& u, const T& v, int weight);

    void removeWeight(const T& u, const T& v);
    void removeVertex(const T& u);
    void removeEdge(const T& u, const T& v);

    int getWeight(const T& u, const T& v);

    int degree(const T& u);
    int largestDrgree();

    int vertexsCount();
    int edgesCount();

    vector<T> getVertexs();
    unordered_map<T, int> getEdgesOf(const T& u);

    void show();

public://深度优先遍历
    void dfsHelper(const T& u, set<T>& visited, vector<T>& result);
    vector<T> dfsByRec(const T& u);
    vector<T> dfsByItr(const T& u);
public://广度优先
    vector<T> bfs(const T& u);

public://Pirm最小生成树
    Graph<T> pirm(T v);
public://Dijkstra最短路径
    map<T, int> dijkstra(T start);
};
#ifndef QTDEBUG
template class Graph<string>;
#endif

#ifdef QTDEBUG
template class Graph<QString>;
QPoint transToPoint(QString vertex);
set<Edge<QString>> transToEdges(QString vertex);
QString getName(QString vertex);
#endif

#endif

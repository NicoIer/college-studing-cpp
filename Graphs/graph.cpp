#include "graph.h"

#ifdef QTDEBUG
QPoint transToPoint(QString vertex)
{
    //Vertex(name:x:y) Edges(name:weight;name:weight;name:weight)
    //Graph<QString> 节点是QString 对应边集是set<Edge<QString>> Edge是vertex + weight组成的
    //我们这样构建QString的内容 name:x:y 利用:就可以分割出来了
    //json中如何组织每一行的字符？
    //A:100:200|B@10:C@5
    QStringList data = vertex.split("|");//得到{A:100:200}{B@10:C@5}
    QStringList Vertex = data[0].split(":");//对"A:100:200"进行分割
    return QPoint(Vertex[1].toInt(),Vertex[2].toInt());
}


set<Edge<QString> > transToEdges(QString vertex)
{
    QStringList data = vertex.split("|");//得到{A:100:200}{B@10;C@5}
    QStringList Edges = data[1].split(":");//对"B@10;C@5"进行分割

    set<Edge<QString>> edges;
    for(auto edge:Edges)
    {//对每一条边进行遍历
        QStringList a = edge.split("@");//获取边
        edges.insert(Edge<QString>(a[0],a[1].toInt()));
    }

    return edges;
}
QString getName(QString vertex)
{
    QStringList data = vertex.split("|");//得到{A:100:200}{B@10:C@5}
    QStringList Vertex = data[0].split(":");//对"A:100:200"进行分割
    return Vertex[0];
}
#endif

template<typename T>
Graph<T>::Graph()
{

}

template<typename T>
bool Graph<T>::isContain(const T& u)
{
    return adjacentGraph.find(u) != adjacentGraph.end();
}

template<typename T>
bool Graph<T>::isAdjacent(const T& u, const T& v)
{
    if (u != v && isContain(u) && isContain(v))
    {
        for (const auto& edge : adjacentGraph[u])
        {//对节点u的所有边遍历
            if (edge.vertex == v)
                return true;
        }
    }
    return false;
}

template<typename T>
void Graph<T>::addVertex(const T& u)
{
    if (!isContain(u))
    {
        set<Edge<T>>edges;
        adjacentGraph[u] = edges;//添加孤儿节点
    }
}

template<typename T>
void Graph<T>::addEdge(const T& u, const T& v, int weight)
{
    if (!isAdjacent(u, v))//u v不相邻
    {//互为邻居
        adjacentGraph[u].insert(Edge<T>(v, weight));
        adjacentGraph[v].insert(Edge<T>(u, weight));
    }
}

template<typename T>
void Graph<T>::changeWeight(const T& u, const T& v, int weight)
{
    if (isContain(u) && isContain(v))
    {//u  v is in graph
        if (adjacentGraph[u].find(Edge<T>(v)) != adjacentGraph[u].end())
        {//如果u的相邻节点集合 含有v

        }
        if (adjacentGraph[v].find(Edge<T>(u)) != adjacentGraph[v].end())
        {//如果v的相邻节点集合 含有u
            adjacentGraph[u].erase(Edge<T>(u));
            adjacentGraph[u].insert(Edge<T>(u, weight));
        }
    }
}

template<typename T>
void Graph<T>::removeWeight(const T& u, const T& v)
{//使对应边的weight = 0
    changeWeight(u, v, 0);
}

template<typename T>
void Graph<T>::removeVertex(const T& u)
{
    if (isContain(u))
    {
        for (auto& vertex : adjacentGraph)//把u变成孤儿
            if (vertex.second.find(Edge<T>(u)) != vertex.second.end())
                vertex.second.erase(Edge<T>(u));
        adjacentGraph.erase(u);//删除孤儿
    }
}

template<typename T>
void Graph<T>::removeEdge(const T& u, const T& v)
{
    if (u == v || !isContain(u) || !isContain(v))
        return;

    if (adjacentGraph[u].find(Edge<T>(v)) != adjacentGraph[u].end())
    {
        adjacentGraph[u].erase(Edge<T>(v));
        adjacentGraph[v].erase(Edge<T>(u));
    }
}

template<typename T>
int Graph<T>::getWeight(const T& u, const T& v)
{
    if (isContain(u) && isContain(v))
    {
        for (Edge<T> edge : adjacentGraph[u])//对u的边集遍历
            if (edge.vertex == v)//如果存在 u 的邻居 v
                return edge.weight;//返回这条边的weight
    }

    return -1;
}

template<typename T>
int Graph<T>::degree(const T& u)
{
    if (isContain(u))
        return adjacentGraph[u].size();

    return -1;//no in it
}

template<typename T>
int Graph<T>::largestDrgree()
{
    size_t maxDrgree = 0;
    for (auto vertex : adjacentGraph)//每个节点的边集大小就是其度
        maxDrgree = max(maxDrgree, vertex.second.size());
    return maxDrgree;
}

template<typename T>
int Graph<T>::vertexsCount()
{
    return adjacentGraph.size();
}

template<typename T>
int Graph<T>::edgesCount()
{//有待商榷
    int count = 0;
    set<Edge<T>> vertex_set;
    //确保一条边只计算一次即可
    for (auto vertex : adjacentGraph)
    {//vertext是当前节点和它的边集组成的pair
        vertex_set.insert(Edge<T>(vertex.first, 0));//连接到当前节点的边
        for (auto edge : vertex.second)
        {//对当前节点的边集中的边
            if (vertex_set.find(edge) != vertex_set.end())
                continue;//若这条边没有出现过
            ++count;//若出现过
        }
    }

    return count;
}

template<typename T>
vector<T> Graph<T>::getVertexs()
{
    vector<T> vertices;
    for (auto vertex : adjacentGraph)
        vertices.push_back(vertex.first);
    return vertices;
}

template<typename T>
unordered_map<T, int> Graph<T>::getEdgesOf(const T& u)
{
    unordered_map<T, int>neighbors;
    if (isContain(u))
    {
        for (Edge<T>edge : adjacentGraph[u])
            neighbors[edge.vertex] = edge.weight;
    }

    return neighbors;
}

template<typename T>
void Graph<T>::show()
{//调试区别
    for (const auto& u : adjacentGraph)//u是pair<点,其边集>
        for (const auto& v : adjacentGraph[u.first])//v是u的边集合中的边
            qDebug() << "顶点" << u.first << ": " << "(相邻顶点: " << v.vertex << ", 边的权重: " << v.weight << ") ";
}


template<typename T>
Edge<T>::Edge(T neighborVertex)
{
    this->vertex = neighborVertex;
    this->weight = 0;
}

template<typename T>
Edge<T>::Edge(T neighborVertex, int weight)
{
    this->vertex = neighborVertex;
    this->weight = weight;
}

template<typename T>
void Graph<T>::dfsHelper(const T& u, set<T>& visited, vector<T>& result)
{
    result.push_back(u);//把点加入加过集合
    visited.insert(u);//标记该点 表示已经访问过了

    for (Edge<T> edge : adjacentGraph[u])//对u的边集
        if (visited.find(edge.vertex) == visited.end())//如果这条边的另一个端点还没被访问
            dfsHelper(edge.vertex, visited, result);//访问它
}

template<typename T>
vector<T> Graph<T>::dfsByRec(const T& u)
{
    vector<T> result;
    set<T> visited;
    if (isContain(u))
        dfsHelper(u, visited, result);
    return result;

}

template<typename T>
vector<T> Graph<T>::dfsByItr(const T& u)//迭代
{
    vector<T> result;
    set<T> visited;
    stack<T> s;

    s.push(u);//初始节点
    while (!s.empty())
    {
        T v = s.top();
        s.pop();//弹出

        if (visited.find(v) != visited.end()) {
            continue;//若v已经访问过
        }
        //v没有被访问
        visited.insert(v);//标记
        result.push_back(v);//放如结果集合

        for (auto w : adjacentGraph[v])//遍历v的边集
        {
            if (visited.find(w.vertex) == visited.end())
            {//该边的另一端点未被访问
                s.push(w.vertex);//加入访问序列
            }
        }
    }
    return  result;
}

template<typename T>
vector<T> Graph<T>::bfs(const T& u)
{
    vector<T>result;
    set<T>visited;
    queue<T>q;

    q.push(u);
    while (!q.empty())
    {
        T v = q.front();
        q.pop();

        if (visited.find(v) != visited.end())
            continue;
        visited.insert(v);
        result.push_back(v);

        for (Edge<T>edge : adjacentGraph[v])
            if (visited.find(edge.vertex) == visited.end())
                q.push(edge.vertex);//未被访问则加入访问序列
    }
    return result;
}

template<typename T>
Graph<T> Graph<T>::pirm(T beg)
{
    //最小生成树
    Graph<T> minSpanningTree;
    //初始化
    minSpanningTree.addVertex(beg);//添加源节点
    //优先级队列 (元素类型 存储容器 比较函数)
        //队列按权值 int 对 pair<T,T>排序
            //pair<T,T>的first是u(存在于生成树中的顶点)  第二个是v(未加入生成树的节点)
    priority_queue<pair<int, pair<T, T>>, vector<pair<int, pair<T, T>>>, greater<pair<int, pair<T, T>>>> q;

    //已访问节点集合
    set<T> visited;

    //初始化   把v的所有邻居加入 待访问集合
    for (auto neighor : adjacentGraph[beg])
    {//对节点v的所有邻居(由边集找到另一端点)
        //neighbor.weight表示该边的权重  {v,neighbor.vertex}表示(v,u)这条边
        q.push({ neighor.weight,{beg,neighor.vertex} });
    }

    while (!q.empty())
    {
        //队首出队  权重最小的一条边
        auto front = q.top();
        q.pop();

        //获取已在最小生成树中的顶点u
        T u = front.second.first;

        //获取未被访问的节点
        T v = front.second.second;
        //若 v已经被访问
        if (visited.find(v) != visited.end())
            continue;//下一位选手
        else
            visited.insert(v);//marked

        //最小生成树中添加 Vertex v 和Edge(v,u)
        minSpanningTree.addVertex(v);
        minSpanningTree.addEdge(v, u, front.first);

        //把刚加入最小生成树的节点v的邻居 加入待访问集合

        for (auto neighbor : adjacentGraph[v])
            if (visited.find(neighbor.vertex) == visited.end())
                q.push({ neighbor.weight,{v,neighbor.vertex} });
    }
    //返回生成树 (计算MST直接把边集的权重一加就好了)
    return minSpanningTree;
}

//这算法简直就是智慧的结晶 !!!!!
template<typename T>
map<T, int> Graph<T>::dijkstra(T start)
{
    //用dis存放start到任意一个vertex的最短距离int
    map<T, int> dis;
    //和pirm不同 这里的pair只用存放距离和点即可 因为起始点是start是唯一的
    priority_queue<pair<int, T>, vector<pair<int, T>>, greater<pair<int, T>>> q;

    //初始化   为start初始化到其他节点的距离
    for (T vertex : getVertexs())//对节点集
        if (vertex == start)
            dis[start] = 0;
        else
            dis[vertex] = INT32_MAX;//到其他顶点的距离设置为 MAX

    //已访问节点集合
    set<T> visited;

    //初始化
    q.push({ 0,start });

    while (!q.empty())
    {
        auto front = q.top();
        q.pop();

        //获取当前正在访问节点
        T u = front.second;

        if (visited.find(u) != visited.end())
            continue;
        else
            visited.insert(u);
        //获取到达u的最短路径    因为优先队列已经排过序了
        int shortest_distance_to_u = front.first;
        dis[u] = shortest_distance_to_u;

        //更新距离
        for(auto v:adjacentGraph[u])//对u的所有邻居
            if (visited.find(v.vertex) == visited.end())//若没有被访问过
            {
                int distance_to_v = v.weight;//u到u的距离
                //start到v的距离 加入结果
                q.push({ shortest_distance_to_u + distance_to_v,v.vertex });
            }

    }
    return dis;
}

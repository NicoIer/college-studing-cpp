#include "StoneGame.h"

StoneGameGraph::StoneGameGraph(int numOfStones)
{
    iniGraph(numOfStones);
}

bool StoneGameGraph::iniGraph(int numOfStones)
{//初始化邻接表 节点 - {节点+权重}集合
    if (numOfStones <= 3)
        return false;
    //有2个开局
    //有2 * numOfStones + 2 个节点
    vector<Staus>starting;
    for (int i = numOfStones - 1; i != -2; --i)
    {
        starting.push_back(Staus(Player::PLAYER1, i + 1));
        starting.push_back(Staus(Player::PLAYER2, i + 1));
    }
    //节点都构造完毕了  现在把节点联系起来 
    //除了 1 1  0 0 节点外 都有两个出度

    //开局节点
    //进行节点
    int i = 0;
    for (i = 0; i != starting.size() - 4; i += 2)
    {
        graphs.insert({ starting[i],vector<pair<int,Staus>>{ {0,starting[i + 3]},{0,starting[i + 5]}} });
        graphs.insert({ starting[i + 1],vector<pair<int,Staus>>{ {0,starting[i + 2]},{0,starting[i + 4]}} });
    }//不特殊的节点处理完毕
    //胜利节点和 1 1节点  从i开始往后+4个即为剩余的节点 i = 18
    graphs.insert({ starting[i],vector<pair<int,Staus>>{ {0,starting[i + 3]}} });
    graphs.insert({ starting[i+1],vector<pair<int,Staus>>{ {0,starting[i + 2]}} });

    graphs.insert({ starting[2 * numOfStones],vector<pair<int,Staus>>{} });
    graphs.insert({ starting[2 * numOfStones + 1],vector<pair<int,Staus>>{} });
    return true;
}

void StoneGameGraph::print()
{//打印调试
    //key: Staus  value: vector<pair<int,Staus>>>
    for (auto& u : graphs)//对每个节点
        for (auto& v : graphs[u.first])//对这个节点的边
        {//cout or qDebug()
            cout << "玩家:(player" << int(u.first.player) << "    stones:" << u.first.stone << ")";
            cout << "指向 玩家:(player" << int(v.second.player) << "   stones:" << v.second.stone << ")" << "权重:" << v.first << endl;
        }

}

bool StoneGameGraph::hasloop()
{
    //初始化入度
    map<Staus, int>in;
    for (auto& u : graphs)
        in.insert({ u.first,0 });

    //记录每个节点的入度
    for (auto& u : graphs)//key :Staus  value: vector<pair<int,Staue>>
        for (auto& v : graphs[u.first])//  weight  :  Staus
            ++in[v.second];

    //设置queue 
    queue<Staus>q;
    //存放入度为0的Node
    for (auto& u : in)
        if (u.second == 0)
            q.push(u.first);

    int count = 0;//统计

    while (!q.empty())
    {
        auto u = q.front();
        q.pop();

        for (auto v : graphs[u])//key weight, value Staus
        {//对u的后继节点v
            --in[v.second];//入度减一
            if (in[v.second] == 0)//若... 则入队
                q.push(v.second);
        }
        ++count;
    }
    return !(count == in.size());
}

void Staus::reset(Player player, int stone)
{
    this->player = player;
    this->stone = stone;
}

bool Staus::operator==(const Staus& other)const
{
    if ((this->player == other.player) && (this->stone == other.stone))
        return true;
    else
        return false;
}

bool Staus::operator<(const Staus& other)const
{
    if (this->stone == other.stone)
    {
        return this->player < other.player;
    }
    else
    {
        return this->stone > other.stone;
    }
}

bool Staus::operator>(const Staus& other)const
{
    return !(*this < other);
}

Staus& Staus::operator=(const Staus& other)
{

    this->player = other.player;
    this->stone = other.stone;
    return *this;
}


bool GamePlay::operator++(int)
{//game++ 取2石头
    for (auto& v : gameGraph[now])//对now的后继
        if (v.second.stone == now.stone - 2)
        {//如果不是11 00 节点就应该进入if
            now.reset(v.second.player, v.second.stone);
            return true; 
        }
    //进行到这里证明now是1 1 0 0节点
    return false;
}

bool GamePlay::operator++()
{//++game 取1个石头
    for (auto& v : gameGraph[now])//对now的后继
        if (v.second.stone == now.stone - 1)
        {//如果不是11 00 节点就应该进入if
            now.reset(v.second.player, v.second.stone);
            return true;
        }
    //进行到这里证明now是1 1 0 0节点
    return false;
}

void GamePlay::reStart()
{
    now = iniStaus(int(gameGraph.graphs.size()) / 2 - 1);
}

bool GamePlay::over()
{
    return now.stone == 0;//剩余石头为0
}

void GamePlay::aiToai()
{
    //初始化两个AI

}

void GamePlay::HumanToAi()
{
    //初始化一个AI AI默认PLAYER2
    ChenJun cj(10);
    cj.loadModel();//载入模型

    //初始化一个玩家 玩家默认PLAYER1
    Staus player(Player::PLAYER1, 10);

    Player p = now.player;//先手的玩家
    cout << "您是 玩家" << int(now.player) << endl;
    cout << "玩家" << int(now.player) << "先手" << endl << endl;;
    while (!over())
    {
        cout << "当前是玩家" << int(now.player) << "回合" << endl;
        cout << "请输入1/2决定你要取几颗石头" << endl;
        int select = 0;

        //判断回合 更新状态
        if (now.player == cj.now.player)
        {
            cj.now = now;
            select = cj.out();
            cout << select << endl;
            cout << "AI" << "决定拿" << select << "个石头" << endl;

        }
        else
        {
            player = now;
            select = cj.out();
            cin >> select;
            cout << "玩家" << "决定拿" << select << "个石头" << endl;
        }
        if (select == 1)
            if (!++(*this))
                cout << "您不能拿1个" << endl;
        if (select == 2)
            if (!(*this)++)
                cout << "您不能拿2个" << endl;
    }
    switch (now.player)
    {
    case Player::PLAYER1:cout << "您 赢得游戏" << endl; break;
    case Player::PLAYER2:cout << "Ai 赢得游戏" << endl; break;
    }
    cj.saveModel();
}
void GamePlay::Start()
{
    cout << "玩家:" << int(now.player) << "先手" << endl;

    while (!over())
    {
        cout << "当前是玩家" << int(now.player) << "的回合" << endl;
        int tmp = 0;
        cout << "玩家" << int(now.player) << "  输入1/2决定你要取几颗石头";
        cin >> tmp;
        if (tmp == 1)
            if (!++(*this))
                cout << "您不能拿1个" << endl;
        if (tmp == 2)
            if(!(*this)++)
                cout << "您不能拿2个" << endl;

    }

    switch (now.player)
    {
    case Player::PLAYER1:cout << "PLAYER1 赢得游戏" << endl; break;
    case Player::PLAYER2:cout << "PLAYER2 赢得游戏" << endl; break;
    }

}

void GamePlay::HumanToHuman()
{
    Start();
}

Staus GamePlay::iniStaus(int numOfStones)
{    
    bool start = throwCoin();
    if (start)
        return Staus(Player::PLAYER1, numOfStones);
    else
        return Staus(Player::PLAYER2, numOfStones);
}

bool GamePlay::throwCoin()
{
    return rand() % 2;
}

int ChenJun::randOut() 
{
    return rand() % 2 + 1;//随机返回1 或者 2
}

//未完成 暂时的
int ChenJun::out()
{
    auto dis = Dij(now);//Dij算法结果获取
    auto &road = dis.first;//获取最优ROAD

    queue<Staus>q;
    q.push(Staus(Player::PLAYER2, 0));//将最终胜利节点加入队列
    Staus next(now);
    while (!q.empty())
    {
        auto u = q.front();
        q.pop();
        if (road.find(u) == road.end())
            continue;
        //如果u有前继 且 是now
        if (road[u] == now)
        {
            next = u;//我们找到了最优胜利ROAD上 当前节点now的后继next
            return now.stone - next.stone;
        }
        else
            q.push(road[u]);//
    }
    //到这里证明 没有路可以走 投降就行了 完事了 投降吧
    return randOut();//随便输出一个呗 就
}
pair<ROAD, map<Staus, int>> ChenJun::Dij(Staus begin)
{
    map<Staus, int>distance;//存放终结果
    map<Staus, Staus>parent;//最短ROAD上节点对应的前继
    set<Staus>visited;//存放已经访问的节点
    priority_queue < pair<int, Staus>, vector<pair<int, Staus>>, less<pair<int, Staus>>> q;//大根堆 用于求最优节点
    //初始化
    //起始节点是begin
    for (auto& u : gameGraph.graphs)
    {
        if (u.first == begin)//到自己是最优的
            distance[u.first] = INT_MAX;
        else                 //到其他节点都是最差的
            distance[u.first] = INT_MIN;
    }
    //初始化
    q.push({ distance[begin],begin });

    //最优ROAD
    //q空代表所有节点都已经访问过
    while (!q.empty())
    {
        //获取当前最优节点
        auto tmp = q.top();
        q.pop();
        int weight = tmp.first;
        Staus u = tmp.second;
        //若这个节点被访问过
        if (visited.find(u) != visited.end())
            continue;
        //若未被访问则标记为已访问
        visited.insert(u);

        //从u的后继寻找最优节点
        for (auto& v : gameGraph[u])
        {
            //若后继已经被访问则访问下一个
            if (visited.find(v.second) != visited.end())
                continue;
            //后继未被访问时 判断{到v的最优ROAD是否 比 到u的最优ROAD+(u,v)的权重  差}
            if (distance[v.second] < distance[u] + v.first)
            {
                distance[v.second] = distance[u] + v.first;
                parent.insert({ v.second,u });//只有在最短ROAD中的节点才会被加入ROAD映射
                q.push({ distance[v.second],v.second });//将这个节点加入待访问节点序列
            }
        }
    }
    return { parent,distance };
}

bool ChenJun::inAndDecrease(Player winner)
{
    //找到p的所有操作  p是胜利者
    for (auto& r : road)
    {//遍历操作集合 r.first出发  r.second是结束
        if (r.first.player == winner)
        {//胜利者的操作  r.first - > r.second
            for (auto& u : gameGraph[r.first])
            {//在游戏状态图中找到胜利者的后继  找到对应操作
                if (u.second == r.second)
                {//找到 胜利者到达的下一状态
                    u.first+=1;//增加权重
                }
            }
        }
        else
        {//失败者的操作
            for (auto& u : gameGraph[r.first])
                if (u.second == r.second)//找到状态图中的对应操作
                    u.first-=1;
        }
    }

    return true;
}

void ChenJun::randTrain(unsigned int times)
{
    GamePlay game(numOfStones);
    int count = 0;
    int select = 0;
    int p1win = 0;
    int p2win = 0;
    while (times--)
    {
        while (!game.over())
        {
            now = game.now;//更新存储状态
            select = randOut();
            if (select == 1)
                if (!++game)//拿取失败重新拿
                    continue;
            if (select == 2)//拿取失败重新拿
                if (!game++)
                    continue;
            //拿取成功添加ROAD
            road.insert({ now, game.now });
        }
        if (game.now.player == Player::PLAYER1)
        {//PLAYER2胜利
            inAndDecrease(Player::PLAYER2);
            ++p2win;
        }
        else
        {//PLAYER1胜利
            inAndDecrease(Player::PLAYER1);
            ++p1win;
        }
        road.clear();//清空ROAD
        game.reStart();
        ++count;
    }
    cout << "PLAYER1胜利" << p1win << "次" << endl;
    cout << "PLAYER2胜利" << p2win << "次" << endl;

    saveModel(PATH);
}

bool ChenJun::loadModel(const string& path)
{
    fstream filestream;//文件输出流
    filestream.open(path, ios::in);

    if (!filestream.is_open())
        return false;

    //开始读取
    //格式  @123@123
    //因为存储和读取的顺序是一样的  所以只用读取权值
    for (auto& u : gameGraph.graphs)
    {//对每个节点
        string line;
        filestream >> line;//读取一行的信息
        auto result = spilt(line, "*");
        for (int i = 0; i != u.second.size(); ++i)
            u.second[i].first = stoi(result[i]);
    }
    filestream.close();
    return true;

}
bool ChenJun::saveModel(const string& path)
{
    //存储格式 (0,10):{0,(1,9)},{0,(1,8)}
    // 0代表PLAYER1 10代表剩余10个石头 : 0代表权值 1代表PLAYER2  9代表剩余石头...
    //分号右边是节点  坐标是其后继节点及边的权值 
    fstream filestream;//文件输出流
    filestream.open(path, ios::out);
    
    if (!filestream.is_open())
        return false;
    
    //开始存储
    for (auto& u : gameGraph.graphs)//对每个节点 key:Staus  value:vector<pair<int,Staus>>
    {
        stringstream line;
        for (auto& v : gameGraph[u.first])//对这个节点的后继  first  int   second  Staus
        {//string真真的没QString好用!!!  stringstream还挺好用
            line << "*" << v.first;
        }
        filestream << line.str() << "\n";
    }
    filestream.close();
    return true;
}

vector<string> spilt(string str, const string& pattern)
{//实现类似QString的字符串分割函数
    string::size_type pos;
    vector<string>result;
    str += pattern;//给一个结束标志
    for (size_t i = 0; i != str.size(); ++i)
    {
        pos = str.find(pattern, i);//从locate i 开始 查找pattern
        if (pos != string::npos)
        {//找到
            auto tmp = str.substr(i, pos - i);
            if (!tmp.empty())
                result.push_back(tmp);//对应范围生成字符串
            i = pos + pattern.size() - 1;//移动i到这个pattern的末尾
        }
    }
    return result;
}

#ifndef STONEGAME_H
#define STONEGAME_H
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <time.h>
#include <sstream>
using namespace std;


/***************************************************/
class Staus;
class StoneGameGraph;
class GamePlay;
/***************************************************/



/***************************************************/
class ChenJun;
#define CJJ ChenJun;
#define PATH "./model/ChenJun.json"
#define ROAD map<Staus,Staus>
/***************************************************/



/***************************************************/
enum class Player {PLAYER1,PLAYER2};
/***************************************************/



/***************************************************/
vector<string>spilt(string str, const string& pattern);
/***************************************************/
class Staus
{//节点 状态
public:
    Staus(Player player = Player::PLAYER1, int stone = 10) :player(player), stone(stone) {}
public:
	void reset(Player player, int stone);
	bool operator==(const Staus& other)const;//用于map查询
	bool operator<(const Staus& other)const;
	bool operator>(const Staus& other)const;
	Staus& operator=(const Staus& other);
public:
	Player player;//谁的回合
	int stone;//剩余多少石头
};

class StoneGameGraph
{//游戏空间状态图
public:
	StoneGameGraph(int numOfStones);//参数是石头数目
	auto& operator[](const Staus& u) { return graphs[u]; }//便于操作  这个&把我害遭了
private:
	bool iniGraph(int numOfStones);//初始化游戏状态图
public:
	void print();
	bool hasloop();
public:
	map<Staus, vector<pair<int,Staus>>>graphs;//节点 + 节点的下一个节点的集合与对应权重
};

class GamePlay
{//游戏
	friend class ChenJun;
	friend class HongShuang;
public:
	StoneGameGraph gameGraph;//游戏状态图
	Staus now;//当前游戏进度
    Player palyer1 = Player::PLAYER1;
    Player player2 = Player::PLAYER2;
public:
	GamePlay(int numOfStones = 10) : gameGraph(numOfStones), now(iniStaus(numOfStones)) {}
	void reStart();
public:
	void HumanToHuman();
	void aiToai();//待定义
	void HumanToAi();//待定义
public:
	bool operator++(int);//后缀  用于取两个石头
	bool operator++();   //前缀  用于取1个石头
private:
    void Start();
public:
	bool over();//判断游戏是否结束  now.player 输了游戏
private:
	Staus iniStaus(int numOfStones);
	bool throwCoin();
};
//先载入模型 再训练 这样每一次训练都可以强化AI 
class ChenJun
{
public:
    ChenJun(int numOfStones = 10) :gameGraph(numOfStones), now(Player::PLAYER1, numOfStones),numOfStones(numOfStones) {};
public:
	int out();//正式对弈下的输出函数
	void randTrain(unsigned int times);//随机训练
	void print() { gameGraph.print(); }
	pair<ROAD,map<Staus, int>> Dij(Staus begin);
private:

	int randOut();//训练模型用 随机输出结果
	bool inAndDecrease(Player p);
public:
	bool loadModel(const string& path = PATH);//读取模型
	bool saveModel(const string& path = PATH);//储存模型
public:
	Staus now;//保存当前游戏状态
private:
	map<Staus, Staus>road;//游戏进行路线 用于最后更新权值  如果1赢 则++ player 减少player2 反之亦然
	StoneGameGraph gameGraph;//存储游戏状态图 这个状态图应该包含权值 是训练过后的成果
private:
	int numOfStones;
};

#endif // !STONEGAME_H

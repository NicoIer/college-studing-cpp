#ifndef FORUM_H
#define FORUM_H
/******************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>
#include <set>
#include <io.h>
#include <map>
/******************************************************/

/******************************************************/
using namespace std;
/******************************************************/

/******************************************************/
#define USER_PATH  "./data/user.txt"
#define CONTENT_PATH "./data/content"
#define HISTORY_PATH "./data/history.txt"
/******************************************************/

/******************************************************/
class content;
class article;
class user;
class forumSystem;
/******************************************************/

/******************************************************/
#define StringList vector<string>

/******************************************************/

/******************************************************/
vector<string> spilt(string str, const string& pattern);
void getFiles(string path, vector<string>& files);
user toUser(const string& str);
content toContent(const string& str);
string getTime();
bool hasKey(const string& str, const string& key);
/******************************************************/


//用户信息
class user
{
public:
	user(const string& name="", const string& password="", const string& phone="", const string& mail="") :name(name), password(password), phone(phone), mail(mail) {}
public:
	string name;
	string password;
	string phone;
	string mail;
public:
	//set<string>visited;//该用户已经浏览过的帖子
	bool operator<(const user& other)const
	{
		return other.name < name;
	}
	bool operator>(const user& other)const
	{
		return other.name < name;
	}
	bool operator==(const user& other)const
	{
		return other.name == name;
	}
};

//楼层内容
class content
{
public:
	content(unsigned int floor = 0, user publisher = user(), string time = "", string text = "") :floor(floor), publisher(publisher), time(time), text(text) {}
public:
	unsigned int floor;//楼层
	user publisher;    //内容发布者
	string time;       //内容发布时间
	string text;       //具体内容
};



//帖子类 存储帖子内容
class article
{
public:
	string title;              //帖子标题
	user publisher;            //帖子发布者
	vector<content> contents;  //帖子内容
	string time;               //帖子发布时间
public:
	bool Append(user u, const string& text);//回帖接口函数
	bool operator<(const article& other)const
	{
		return other.title < title;
	}
	bool operator>(const article& other)const
	{
		return other.title < title;
	}
	bool operator==(const article& other)const
	{
		return other.title == title;
	}
};

//论坛类 存储帖子 用户信息
class forumSystem
{
public:
	user me;          //当前登录用户
public:
	map<string, set<string>> history;//存储用户看过的帖子标题
	set<user>users;//存储用户信息
	vector<article>articles;//存储帖子信息
public:
	void mainWindow(size_t o = 1);//论坛主界面显示
	void articleShow(const int& index);
	void publish();
	void showByCode();//展示论坛  展示所有帖子(显示发布者 发布时间)
	void showByTime();
	void showByNum();
	void showByTitle();
	void find();
public:
	vector<article> findByUser(user u);//由用户查询帖子
	vector<article> findByTitle(string title);//由帖子信息查询帖子

public:
	bool read(); //读取用户/帖子信息
	bool save(); //存储用户/帖子信息
	bool exit(); //退出论坛系统
	bool login();//登录论坛
private:
	bool readHistory();//读取用户历史访问信息
	bool regedit();
	size_t index(const article& a);
};
#endif // !FORUM_H


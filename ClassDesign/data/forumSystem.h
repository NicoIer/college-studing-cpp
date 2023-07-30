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


//�û���Ϣ
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
	//set<string>visited;//���û��Ѿ������������
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

//¥������
class content
{
public:
	content(unsigned int floor = 0, user publisher = user(), string time = "", string text = "") :floor(floor), publisher(publisher), time(time), text(text) {}
public:
	unsigned int floor;//¥��
	user publisher;    //���ݷ�����
	string time;       //���ݷ���ʱ��
	string text;       //��������
};



//������ �洢��������
class article
{
public:
	string title;              //���ӱ���
	user publisher;            //���ӷ�����
	vector<content> contents;  //��������
	string time;               //���ӷ���ʱ��
public:
	bool Append(user u, const string& text);//�����ӿں���
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

//��̳�� �洢���� �û���Ϣ
class forumSystem
{
public:
	user me;          //��ǰ��¼�û�
public:
	map<string, set<string>> history;//�洢�û����������ӱ���
	set<user>users;//�洢�û���Ϣ
	vector<article>articles;//�洢������Ϣ
public:
	void mainWindow(size_t o = 1);//��̳��������ʾ
	void articleShow(const int& index);
	void publish();
	void showByCode();//չʾ��̳  չʾ��������(��ʾ������ ����ʱ��)
	void showByTime();
	void showByNum();
	void showByTitle();
	void find();
public:
	vector<article> findByUser(user u);//���û���ѯ����
	vector<article> findByTitle(string title);//��������Ϣ��ѯ����

public:
	bool read(); //��ȡ�û�/������Ϣ
	bool save(); //�洢�û�/������Ϣ
	bool exit(); //�˳���̳ϵͳ
	bool login();//��¼��̳
private:
	bool readHistory();//��ȡ�û���ʷ������Ϣ
	bool regedit();
	size_t index(const article& a);
};
#endif // !FORUM_H


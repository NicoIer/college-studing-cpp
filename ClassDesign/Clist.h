#ifndef CLIST_H
#define CLIST_H
#include <vector>
#include <stack>
#include <time.h>
#include <iostream>
using namespace std;
//实际上只需要简单的定义单链表 
//然后实现对应的函数即可
//甚至不需要定义链表



/***********************************************************/
struct ListNode;
class List;
/************************************************************/


/************************************************************/
bool Connect(ListNode* head, int a, int b,ListNode* &head2);
ListNode* remake(ListNode* head1, ListNode* head2);
ListNode* judge1(ListNode* head1, ListNode* head2);
ListNode* judge2(ListNode* head1, ListNode* head2);
pair<ListNode*, ListNode*> location(ListNode* head);
/************************************************************/


struct ListNode {
	int val;
	ListNode* next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode* next) : val(x), next(next) {}
	
};

class List
{
public:
	List() :head(nullptr) {}
	List(vector<int>nodes);
    ~List();
	bool empty() { return head == nullptr; }
public:
	ListNode* head;
};
#endif


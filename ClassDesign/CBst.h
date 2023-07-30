#ifndef CBST_H
#define CBST_H
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <stack>
using namespace std;

struct Node;
class CTree;
//#define node shared_ptr<Node>//减少代码量
vector<int> midOrder(shared_ptr<Node> root);
shared_ptr<Node> transform(shared_ptr<Node> root);
void transformHelper(shared_ptr<Node> cur, shared_ptr<Node>& tail);
bool judge(vector<int>mid, bool increase = true);
shared_ptr<Node> createTree1(vector<int>&order, vector<int>&prev);//中序 + 前序 构建二叉树
shared_ptr<Node> createTree2(vector<int>& order, vector<int>& post);//中序 + 后序 构建二叉树
shared_ptr<Node> createHelper1(map<int, int>& save, vector<int>& prev, int begin, int end, int inStart);
shared_ptr<Node> createHelper2(map<int, int>& save, vector<int>& post, int begin, int end, int inStart);
//为了方便 使用智能指针
struct Node
{
	Node(int value = INT32_MIN, Node* left = nullptr, Node* right = nullptr) :val(value), left(left), right(right) {}
    shared_ptr<Node> left;
    shared_ptr<Node> right;
	int val;
};
class CTree
{
public:
	bool empty() { return root == nullptr; }
public:
	CTree(Node* r = nullptr) :root(r) {}
	shared_ptr<Node>root;
private:

};
#endif


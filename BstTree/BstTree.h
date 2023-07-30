#pragma once
#include <queue>
#include <stack>
#include <iostream>
#include <string>
//#include <unordered_map>
#include <map>
using namespace std;
//支持通用二叉树的三叉树
template<typename T>
class BstNode {
public:
	BstNode() {
		left = right = nullptr;
		root = nullptr;
	}
	//接受父节点的构造函数
	BstNode(const T& element,  BstNode<T>* parent = nullptr,BstNode<T>* leftNode = nullptr, BstNode<T>* rightNode = nullptr):
		el(element), left(leftNode), right(rightNode), root(parent)
	{}

public:
	T el;
	BstNode<T>* left;
	BstNode<T>* right;
	BstNode<T>* root;
};

template<typename T>
class BstTree {
public:
    BstTree(BstNode<T>*r=nullptr):root(r){}
	//BstTree(const BstTree& s);//暂且未定义的复制构造函数

    virtual ~BstTree() {
		clear();
	}

	void clear() {
		clear(root);
		root = nullptr;
	}

	bool isEmpty() {
		return root == nullptr;
	}

	void preorder() {
		preorder(root);
	}

	void inorder()
	{
		inorder(root);
	}
	
	void postorder()
	{
		postorder(root);
	}

	T* search(const T& el){
		return search(root, el);
	}

	void breadthFirst();//广度优先遍历 上到下 左到右

	//插入 树的构造都基于insert实现
	void insert(const T&el);
    /*本次作业不需要实现的功能已经注释掉了

    非递归实现前中后序遍历  没要求 不想写
    void iterativePreorder();
    void iterativeInorder();
    void iterativePostorder();

    中序遍历的Morris实现
    void MorrisInorder();

    合并删除
    void deleteByMerging(BstNode<T>*& node);

    void findAndDeleteByMerging(const T&);
    void deleteByCoping(BstNode<T>*&);
    void banlancce(T*, int, int);*/
    BstNode<T>* Root()const{
        return root;
    }
protected:
	BstNode<T>* root;

protected:
	//LRV删除
	void clear(BstNode<T>*r) {
        if(r == nullptr)
            return;
		if (r->left == nullptr && r->right == nullptr)//叶节点 删除
		{
			delete r;
			return;
		}
		if (r != nullptr)
		{
			clear(r->left);
            r->left=nullptr;
			clear(r->right);
            r->right=nullptr;
        }
	}
	T* search(BstNode<T>*, const T&);

	//前中后序遍历  基于递归是很简单实现的
	void preorder(BstNode<T>*);//VLR
	void inorder(BstNode<T>*);//LVR
	void postorder(BstNode<T>*);//LRV
	//返回值
	virtual const T& visit(BstNode<T>* p) {
		//cout << p->el << '  ';
		return p->el;
	}
public:
    //计算节点数目  树的高度
    unsigned int size(BstNode<T>* treeRoot) {
        if (treeRoot == nullptr)
			return 0;
		else
            return 1 + size(treeRoot->left) + size(treeRoot->right);
	}

    unsigned int height(BstNode<T>* treeRoot)const {
        if (treeRoot == nullptr)
            return 0;
        else
        {
            unsigned int i = height(treeRoot->left);
            unsigned int j = height(treeRoot->right);
            return i < j ? j + 1 : i + 1;//看谁深
        }
    }

};

template<typename T>
inline T* BstTree<T>::search(BstNode<T>* p, const T& el)
{
	while (p != nullptr)//存在节点
	{
		if (el == p->el)//当前节点元素是否为查找的元素
			return &(p->el);
		else if (el < p->el)
			p = p->left;
		else
			p = p->right;
	}
	//未找到
	return nullptr;
}

template<typename T>
inline void BstTree<T>::preorder(BstNode<T>*p)
{
	if (p != nullptr)
	{	
		visit(p);
		inorder(p->left);
		inorder(p->right);
	}
}

template<typename T>
inline void BstTree<T>::inorder(BstNode<T>*p)
{
	if (p != nullptr)
	{
		inorder(p->left);
		visit(p);
		inorder(p->right);
	}
}

template<typename T>
inline void BstTree<T>::postorder(BstNode<T>*p)
{
	if (p != nullptr)
	{
		inorder(p->left);
		inorder(p->right);
		visit(p);
    }
}

template<typename T>
inline void BstTree<T>::breadthFirst()
{
	//广度优先遍历
	queue<BstNode<T>*>que;//基于队列实现广度
	BstNode<T>* p = root;//temp = 根节点
	if (p != nullptr) {//节点存在
		que.push(p);//节点入队列
		//原理是按顺序排队 然后顺序出队 就可以实现广度优先
		while (!que.empty())//队列非空 则一直访问 
		{
			p = que.front();//保存队头节点
			que.pop();//出队列

			visit(p);//访问节点

			if (p->left != nullptr)//先左
				que.push(p->left);
			if (p->right != nullptr)//后右
				que.push(p->right);
		}
	}
}

template<typename T>
inline void BstTree<T>::insert(const T& el)
{
	BstNode<T>* p = root;
	BstNode<T>* prev = nullptr;//节点前继

	while (p!=nullptr)//寻找插入地点
	{
		prev = p;//保存节点的根
		if (el < p->el)
			p = p->left;
		else
			p = p->right;
	}

	if (root == nullptr)//树空
		root = new BstNode<T>(el, root);//根节点的根是自己
	else if (el < prev->el)
		prev->left = new BstNode<T>(el, prev);//三叉树构造函数
	else
		prev->right = new BstNode<T>(el, prev);
}

	//基于中序 + 前序/后序 字符串 实现树的构建

template<typename T>
class Solution {
public:
    size_t CreatMap(T* inorder, size_t len);//创建中序索引map
    //前序+中序生成树
    BstNode<T>* bulidTreePrev(T* preorder, T* inorder,size_t len)
    {
        int index = CreatMap(inorder, len);//这一步不会出错
        return buildFromPrev(preorder, 0, index - 1, 0,nullptr);

    }
    //后序+中序生成树
    BstNode<T>* buildTreePost(T* postorder, T* inorder, size_t len)//后+中
	{
        size_t index = CreatMap(inorder, len);//这一步不会出错
		return bulidFromPost(postorder, 0, len, 0, nullptr);
	}

private:
    BstNode<T>* bulidFromPost(T* postorder, int postStart, int postEnd, int inStart, BstNode<T>* parent);
    BstNode<T>* buildFromPrev(T* preorder,int preStart,int preEnd,int inStart,BstNode<T>*parent);//前+中
private:
    map<T, int> map;//保存中序 用于计算
};

template<typename T>
inline size_t Solution<T>::CreatMap(T* inorder, size_t len)
{
	int index = 0;

	for (auto* i = inorder; index != len; ++i, ++index)
		map.insert({ *i, index });
	return index;
}

template<typename T>
inline BstNode<T>* Solution<T>::buildFromPrev(T* preorder, int preStart, int preEnd, int inStart, BstNode<T>* parent)
{
        //递归结束条件 前序已经遍历完毕
        if (preEnd < preStart)
            return nullptr;
        //前序第一位是根节点
        BstNode<T>* root = new BstNode<T>(preorder[preStart],parent);//按照要求构建三叉树
        //找到中序序列中的根节点
        int rootIndex = 0;

        if(map.count(root->el))
        {
            rootIndex = map[root->el];//返回键对应的值 即查找索引
        }
        else//找不到根节点
            throw 520;

        //找不到根节点就错了
        //len 代表左子树的节点个数
        int len = rootIndex - inStart;
        //递归调用  preStart是前序中的开始下标 End是结束下标 inStart是中序的起始下标
        root->left = buildFromPrev(preorder, preStart + 1, preStart + len, inStart,root);

        root->right = buildFromPrev(preorder, preStart + len + 1, preEnd, rootIndex + 1,root);
        return root;
}

template<typename T>
inline BstNode<T>* Solution<T>::bulidFromPost(T* postorder, int postStart,int postEnd,int inStart,BstNode<T>*parent)
{
    //递归结束条件 后序遍历完毕
    if (postEnd <= postStart)
        return nullptr;
    //后序的最后一位是根节点
    BstNode<T>* root = new BstNode<T>(postorder[postEnd - 1], parent);
    //找到中序中的根节点下标
    int rootIndex = 0;

    if(map.count(root->el))
    {
        rootIndex = map[root->el];//返回键对应的值 即查找索引
    }
    else//找不到根节点
        throw 520;
    //左子树的节点个数
    int len = rootIndex - inStart;
    //递归调用 更新后序序列即可
    root->left = bulidFromPost(postorder, postStart, postStart + len, inStart, root);
    root->right = bulidFromPost(postorder, postStart + len, postEnd - 1, rootIndex + 1, root);
    return root;
	
}

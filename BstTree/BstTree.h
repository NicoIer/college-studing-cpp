#pragma once
#include <queue>
#include <stack>
#include <iostream>
#include <string>
//#include <unordered_map>
#include <map>
using namespace std;
//֧��ͨ�ö�������������
template<typename T>
class BstNode {
public:
	BstNode() {
		left = right = nullptr;
		root = nullptr;
	}
	//���ܸ��ڵ�Ĺ��캯��
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
	//BstTree(const BstTree& s);//����δ����ĸ��ƹ��캯��

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

	void breadthFirst();//������ȱ��� �ϵ��� ����

	//���� ���Ĺ��춼����insertʵ��
	void insert(const T&el);
    /*������ҵ����Ҫʵ�ֵĹ����Ѿ�ע�͵���

    �ǵݹ�ʵ��ǰ�к������  ûҪ�� ����д
    void iterativePreorder();
    void iterativeInorder();
    void iterativePostorder();

    ���������Morrisʵ��
    void MorrisInorder();

    �ϲ�ɾ��
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
	//LRVɾ��
	void clear(BstNode<T>*r) {
        if(r == nullptr)
            return;
		if (r->left == nullptr && r->right == nullptr)//Ҷ�ڵ� ɾ��
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

	//ǰ�к������  ���ڵݹ��Ǻܼ�ʵ�ֵ�
	void preorder(BstNode<T>*);//VLR
	void inorder(BstNode<T>*);//LVR
	void postorder(BstNode<T>*);//LRV
	//����ֵ
	virtual const T& visit(BstNode<T>* p) {
		//cout << p->el << '  ';
		return p->el;
	}
public:
    //����ڵ���Ŀ  ���ĸ߶�
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
            return i < j ? j + 1 : i + 1;//��˭��
        }
    }

};

template<typename T>
inline T* BstTree<T>::search(BstNode<T>* p, const T& el)
{
	while (p != nullptr)//���ڽڵ�
	{
		if (el == p->el)//��ǰ�ڵ�Ԫ���Ƿ�Ϊ���ҵ�Ԫ��
			return &(p->el);
		else if (el < p->el)
			p = p->left;
		else
			p = p->right;
	}
	//δ�ҵ�
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
	//������ȱ���
	queue<BstNode<T>*>que;//���ڶ���ʵ�ֹ��
	BstNode<T>* p = root;//temp = ���ڵ�
	if (p != nullptr) {//�ڵ����
		que.push(p);//�ڵ������
		//ԭ���ǰ�˳���Ŷ� Ȼ��˳����� �Ϳ���ʵ�ֹ������
		while (!que.empty())//���зǿ� ��һֱ���� 
		{
			p = que.front();//�����ͷ�ڵ�
			que.pop();//������

			visit(p);//���ʽڵ�

			if (p->left != nullptr)//����
				que.push(p->left);
			if (p->right != nullptr)//����
				que.push(p->right);
		}
	}
}

template<typename T>
inline void BstTree<T>::insert(const T& el)
{
	BstNode<T>* p = root;
	BstNode<T>* prev = nullptr;//�ڵ�ǰ��

	while (p!=nullptr)//Ѱ�Ҳ���ص�
	{
		prev = p;//����ڵ�ĸ�
		if (el < p->el)
			p = p->left;
		else
			p = p->right;
	}

	if (root == nullptr)//����
		root = new BstNode<T>(el, root);//���ڵ�ĸ����Լ�
	else if (el < prev->el)
		prev->left = new BstNode<T>(el, prev);//���������캯��
	else
		prev->right = new BstNode<T>(el, prev);
}

	//�������� + ǰ��/���� �ַ��� ʵ�����Ĺ���

template<typename T>
class Solution {
public:
    size_t CreatMap(T* inorder, size_t len);//������������map
    //ǰ��+����������
    BstNode<T>* bulidTreePrev(T* preorder, T* inorder,size_t len)
    {
        int index = CreatMap(inorder, len);//��һ���������
        return buildFromPrev(preorder, 0, index - 1, 0,nullptr);

    }
    //����+����������
    BstNode<T>* buildTreePost(T* postorder, T* inorder, size_t len)//��+��
	{
        size_t index = CreatMap(inorder, len);//��һ���������
		return bulidFromPost(postorder, 0, len, 0, nullptr);
	}

private:
    BstNode<T>* bulidFromPost(T* postorder, int postStart, int postEnd, int inStart, BstNode<T>* parent);
    BstNode<T>* buildFromPrev(T* preorder,int preStart,int preEnd,int inStart,BstNode<T>*parent);//ǰ+��
private:
    map<T, int> map;//�������� ���ڼ���
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
        //�ݹ�������� ǰ���Ѿ��������
        if (preEnd < preStart)
            return nullptr;
        //ǰ���һλ�Ǹ��ڵ�
        BstNode<T>* root = new BstNode<T>(preorder[preStart],parent);//����Ҫ�󹹽�������
        //�ҵ����������еĸ��ڵ�
        int rootIndex = 0;

        if(map.count(root->el))
        {
            rootIndex = map[root->el];//���ؼ���Ӧ��ֵ ����������
        }
        else//�Ҳ������ڵ�
            throw 520;

        //�Ҳ������ڵ�ʹ���
        //len �����������Ľڵ����
        int len = rootIndex - inStart;
        //�ݹ����  preStart��ǰ���еĿ�ʼ�±� End�ǽ����±� inStart���������ʼ�±�
        root->left = buildFromPrev(preorder, preStart + 1, preStart + len, inStart,root);

        root->right = buildFromPrev(preorder, preStart + len + 1, preEnd, rootIndex + 1,root);
        return root;
}

template<typename T>
inline BstNode<T>* Solution<T>::bulidFromPost(T* postorder, int postStart,int postEnd,int inStart,BstNode<T>*parent)
{
    //�ݹ�������� ����������
    if (postEnd <= postStart)
        return nullptr;
    //��������һλ�Ǹ��ڵ�
    BstNode<T>* root = new BstNode<T>(postorder[postEnd - 1], parent);
    //�ҵ������еĸ��ڵ��±�
    int rootIndex = 0;

    if(map.count(root->el))
    {
        rootIndex = map[root->el];//���ؼ���Ӧ��ֵ ����������
    }
    else//�Ҳ������ڵ�
        throw 520;
    //�������Ľڵ����
    int len = rootIndex - inStart;
    //�ݹ���� ���º������м���
    root->left = bulidFromPost(postorder, postStart, postStart + len, inStart, root);
    root->right = bulidFromPost(postorder, postStart + len, postEnd - 1, rootIndex + 1, root);
    return root;
	
}

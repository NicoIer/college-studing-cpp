#include "CBst.h"

//中序 + 后序 构建二叉树 返回根节点指针
shared_ptr<Node> createTree1(vector<int> &order, vector<int> &prev)
{
    //order存放中序元素及其索引
    map<int, int>save;//元素值 - 对应索引
    int index = 0;
    for (auto& i : order)
    {
        save.insert({ i,index });
        ++index;
    }
    return createHelper1(save, prev, 0, index - 1, 0);//从中序序列0 - size-1中开始构建
}
shared_ptr<Node> createHelper1(map<int,int>&save,vector<int>& prev, int begin, int end, int inStart)
{//VLR + LVR
    //begin  end 指当前子树元素值 应该在中序何索引范围中
    if (end < begin)
        return nullptr;
    shared_ptr<Node> root(new Node(prev[begin]));//node是智能指针 当前根节点是前序的第一个元素
    //中序序列的根节点
    int rootIndex = save[root->val];//获取对应索引
    //该根的左子树节点数目
    int len = rootIndex - inStart;//左子树的长度
    //递归 左子树
    root->left = createHelper1(save, prev, begin + 1, begin + len, inStart);//取下一个根节点 中序的索引起点是inStart
    //递归 右子树
    root->right = createHelper1(save, prev, begin + 1 + len, end, rootIndex + 1);//右子树 范围是begin + 1 + len 到end
    return root;
}
shared_ptr<Node> createTree2(vector<int>& order, vector<int>& post)
{
    map<int, int>save;//元素值 - 对应索引
    int index = 0;
    for (auto& i : order)
    {
        save.insert({ i,index });
        ++index;
    }
    return createHelper2(save, post, 0, index, 0);
}
shared_ptr<Node> createHelper2(map<int, int>& save, vector<int>& post, int begin, int end, int inStart)
{//begin 到 end 范围中找（post序列）
    if (end < begin + 1)
        return nullptr;
    shared_ptr<Node> root(new Node(post[end-1]));

    int rootIndex = save[root->val];

    int len = rootIndex - inStart;
    //LVR  已经找到V的位置 L的长度  因此 V的left应当在V的前面  V的right在V的后面
    //LRV  
    root->left = createHelper2(save, post, begin, begin + len, inStart);//L
    root->right = createHelper2(save, post, begin + len, end-1, rootIndex + 1);//R
    return root;
}

vector<int> midOrder(shared_ptr<Node> root)
{
    if (root == nullptr)
        return vector<int>();
    vector<int>o;
    stack<shared_ptr<Node>>s;
    while (root != nullptr || !s.empty())
    {
        while (root!=nullptr)
        {
            s.push(root);
            root = root->left;
        }
        if (!s.empty())
        {
            root = s.top();
            s.pop();
            o.push_back(root->val);
            root = root->right;
        }
    }
    return o;
}

shared_ptr<Node> transform(shared_ptr<Node> root)
{
    if (root == nullptr)
        return nullptr;
    //链表tail
    shared_ptr<Node> tail = nullptr;
    transformHelper(root, tail);//调整二叉树
    //从tail开始找head
    while (tail->left != nullptr)
        tail = tail->left;
    //此时tail的值是head
    return tail;
}

void transformHelper(shared_ptr<Node> root, shared_ptr<Node>& tail)
{
    if (root == nullptr)
        return;
    //遍历左子树
    if (root->left != nullptr)//递归 直到 当前节点为最左的节点 (最小val)
        transformHelper(root->left, tail);
    //此时已经递归到min val节点 因为root->left == nullptr

    //建立双向连接
    root->left = tail;//min连接到tail 此时当前节点have min val
    if (tail != nullptr)//若当前不是LVR的第一个点 即tail不为空
        tail->right = root;//互指
    //tail随着递归过程而逐渐向末尾移动
    tail = root;
    //遍历右子树
    if (root->right != nullptr)
        transformHelper(root->right, tail);//递归处理root的右子树

}

bool judge(vector<int> mid, bool increase)
{//判断升序序列 降序序列
    if (increase)
        for (int i = 1; i != mid.size(); ++i) {
            if (mid[i - 1] >= mid[i])
                return false;
        }
    else
    {
        for (int i = 1; i != mid.size(); ++i)
        {
            if (mid[i - 1] <= mid[i])
                return false;
        }
    }
    return true;
}

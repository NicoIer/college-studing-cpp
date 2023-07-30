#include "Clist.h"

//与Qt connect冲突 故大写
bool Connect(ListNode* head, int a, int b,ListNode* &head2)//我们将让head2指向新表头
{//我们必须保证 a , b 一定会出现在链表中
	//因为输入不保证 a b一定出现在链表中 且a b 各至少出现一次
	//从head开始找到a,b中先出现/后出现的位置(指针)
	//对应指针
	ListNode* p1 = nullptr;
	ListNode* p2 = nullptr;
	//用于遍历单链表
	auto tmp = head;
	while (tmp->val != a && tmp->val != b)
	{
		tmp = tmp->next;
		if (tmp == nullptr)
			return false;
	}
	//先找到的是a
	if (tmp->val == a)
	{	
		p1 = tmp;
		tmp = tmp->next;
		while (tmp->val != b)
		{
			tmp = tmp->next;
			if (tmp == nullptr)
				return false;
		}
		p2 = tmp;
	}//先找到的是b
	else if (tmp->val == b)
	{
		p1 = tmp;
		tmp = tmp->next;
		while (tmp->val != a)
		{
			tmp = tmp->next;
			if (tmp == nullptr)
				return false;
		}
		p2 = tmp;
	}

	//现在 p1是先出现的  p2是后出现的
	//且 a,b 都是能够出现在链表中的元素 不保证a,b的位置一定不同

	//现在使得a b成环
	//即b指向a  b的后继末尾指向a  b的后继是新的表头 head2
	if (p2->next != nullptr)
	{//b的后继不为空
		head2 = p2->next;
		p2->next = p1;//掉头

		//转移
		auto t = head2;
		while (t->next != nullptr)
			t = t->next;
		t->next = p1;
		
	}
	else
	{//b的后继为空
		head2 = p2->next;
		p2->next = p1;//掉头
	}

	return true;
}
ListNode* remake(ListNode* head1, ListNode* head2)
{//利用head1 head2恢复成单链表
	//**********************这个问题解决了 其余的问题 very easy  (doge)
	//很容易想到快慢指针的方法 当然也是leetcode上学到的
	//head2不变
	//从head1处使用快慢指针  这个方法也可以用来判断是否有环
	auto fast = head1;
	auto slow = head1;
	ListNode* boom = nullptr;//碰撞点
	while (true)
	{
		fast = fast->next->next;
		slow = slow->next;
		if (fast == slow || fast == nullptr)//快慢指针碰撞  还是做一下异常处理 万一没环呢
		{
			boom = slow;//保存碰撞点
			break;
		}
	}
	//找connect的点
	fast = head1;//废物利用
	slow = boom;
	while (true)
	{//简单的数学推导可以得到
		/*碰撞点p到连接点的距离=头指针到连接点的距离*/
		fast = fast->next;
		slow = slow->next;
		if (fast == slow)
		{
			break;
		}
	}
	//现在fast slow 都是连接点a(b->next = a)
	//从fast开始 第一次找到slow之前的位置就是b
	boom = fast;//依旧是废物利用 做fast的前继指针 节约点内存 扣扣嗖嗖的
	fast = fast->next;
	while (true)
	{
		if (fast == slow)
		{
			break;
		}
		fast = fast->next;
		boom = boom->next;
	}
	//此时boom就是b  slow就是a
	//还需要从head2开始找到a 然后改变指向
	fast = head2->next;//依旧是废物利用
	auto last = head2;//没办法啊 必须找到a的另一个前继
	while (true)
	{
		if (fast == slow)
			break;
		fast = fast->next;
		last = last->next;
	}
	//此时last是a的另一个前继

	//掉头吧
	last->next = nullptr;
	boom->next = head2;
	//写完之后发现了优化空间 把while的循环条件换成if的条件可以小小的 优化一点点
	return head1;
}
ListNode* judge1(ListNode* head1, ListNode* head2)
{	
	auto start = clock();
	for (auto head11=head1; head11 != nullptr; head11 = head11->next)
	{
		for (auto tmp = head2; tmp != nullptr; tmp = tmp->next)
		{
			if (tmp == head1)
				return tmp;
		}
	}

	auto end = clock();
	cout << end - start << endl;
	return nullptr;
}
ListNode* judge2(ListNode* head1, ListNode* head2)
{
	auto start = clock();
	ListNode* tmp = nullptr;
	//入栈
	stack<ListNode*>stack1;
	auto stack2 = stack1;

	for (; head1 != nullptr; head1 = head1->next)
		stack1.push(head1);
	for (; head2 != nullptr; head2 = head2->next)
		stack2.push(head2);

	while (!stack1.empty()&&!stack2.empty())
	{
		tmp = stack1.top();
		stack1.pop();
		stack2.pop();
		if (stack1.top() != stack2.top())
			break;
	}
	auto end = clock();
	cout << end - start << endl;
	return tmp;
}
pair<ListNode*, ListNode*> location(ListNode* head1)
{
	//**********************这个问题解决了 其余的问题 very easy  (doge)
//很容易想到快慢指针的方法 当然也是leetcode上学到的
//head2不变
//从head1处使用快慢指针  这个方法也可以用来判断是否有环
	auto fast = head1;
	auto slow = head1;
	ListNode* boom = nullptr;//碰撞点
	while (true)
	{
		fast = fast->next->next;
		slow = slow->next;
		if (fast == slow || fast == nullptr)//快慢指针碰撞  还是做一下异常处理 万一没环呢
		{
			boom = slow;//保存碰撞点
			break;
		}
	}
	//找connect的点
	fast = head1;//废物利用
	slow = boom;
	while (true)
	{//简单的数学推导可以得到
		/*碰撞点p到连接点的距离=头指针到连接点的距离*/
		fast = fast->next;
		slow = slow->next;
		if (fast == slow)
		{
			break;
		}
	}
	//现在fast slow 都是连接点a(b->next = a)
	//从fast开始 第一次找到slow之前的位置就是b
	boom = fast;//依旧是废物利用 做fast的前继指针 节约点内存 扣扣嗖嗖的
	fast = fast->next;
	while (true)
	{
		if (fast == slow)
			break;
		fast = fast->next;
		boom = boom->next;
	}
	//此时boom就是b  slow就是a
	return { slow,boom };
}
List::List(vector<int> nodes)
{
	if (nodes.empty())
	{
		head = nullptr;
		return;
	}
	head = new ListNode(nodes[0],nullptr);//创建头节点
	auto tmp = head;
	for (int i = 1; i != nodes.size(); ++i, tmp = tmp->next)
	{
		auto p = new ListNode(nodes[i]);
		tmp->next = p;
	}
}
List::~List()
{
}

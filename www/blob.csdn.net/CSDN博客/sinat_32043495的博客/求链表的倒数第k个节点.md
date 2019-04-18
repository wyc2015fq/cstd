# 求链表的倒数第k个节点 - sinat_32043495的博客 - CSDN博客





2017年11月28日 22:55:39[LZXandTM](https://me.csdn.net/sinat_32043495)阅读数：46标签：[C++剑指offer](https://so.csdn.net/so/search/s.do?q=C++剑指offer&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/sinat_32043495/article/category/7307292)









                
#include<iostream>

using namespace std;



struct ListNode

{
int m_value;
ListNode *m_pNext;
ListNode(int k) :m_value(k), m_pNext(NULL)
{
};

};



/*

（1）第一个指针从链表的头指针开始遍历向前走k-1，第二个指针保持不动；

（2）从第k步开始，第二个指针也开始从链表的头指针开始遍历；

（3）由于两个指针的距离保持在k-1，当第一个（走在前面的）指针到达链表的尾结点时，第二个指针（走在后面的）指针正好是倒数第k个结点。

*/

/*

（1）输入的head为空指针。由于代码会试图访问空指针指向的内存，程序崩溃。

     解决：在处理前增加判断空指针的代码

（2）输入的以head为头结点的链表的结点总数少于k。由于在for循环中会在链表上向前走k-1步，仍然会由于空指针造成程序崩溃。

     解决：在for循环中增加判断下一个节点是否是空指针的代码

（3）输入的参数k为0。由于k是一个无符号整数，那么在for循环中k-1得到的将不是-1，而是4294967295（无符号的0xFFFFFFFF）。

    因此for循环执行的次数远远超出我们的预计，同样也会造成程序崩溃。

    解决：同（1），在处理前的判断中也判断参数k是否为0。

*/

ListNode*FindKthFromtail(ListNode*pHead, int k)

{
if (pHead == NULL || k == 0)
{
return NULL;
}
ListNode*Ahead = pHead;
ListNode*Behind = NULL;


for (int i = 0; i < k - 1; i++)
{
if (Ahead->m_pNext == NULL)
return NULL;
Ahead = Ahead->m_pNext;
}

Behind = pHead;
while (Ahead->m_pNext != NULL)
{
Ahead = Ahead->m_pNext;
Behind = Behind->m_pNext;
}

return Behind;

}



//单元测试

int main()

{
ListNode* head = new ListNode(1);
ListNode* Node1 = new ListNode(2);
ListNode* Node2 = new ListNode(3);
ListNode* Node3 = new ListNode(4);
ListNode* Node4 = new ListNode(5);
ListNode* Node5 = new ListNode(6);
ListNode* Node6 = new ListNode(7);

head->m_pNext = Node1;
Node1->m_pNext = Node2;
Node2->m_pNext = Node3;
Node3->m_pNext = Node4;
Node4->m_pNext = Node5;
Node5->m_pNext = Node6;
Node6->m_pNext = NULL;

ListNode* p = FindKthFromtail(head, 3);
cout << p->m_value << endl;

getchar();
return 0;

}
            



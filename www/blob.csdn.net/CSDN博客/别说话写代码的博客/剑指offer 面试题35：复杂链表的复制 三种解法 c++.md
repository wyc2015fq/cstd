# 剑指offer 面试题35：复杂链表的复制 三种解法 c++ - 别说话写代码的博客 - CSDN博客





2018年11月23日 13:02:22[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：141
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：请实现函数ComplexListNode* Clone(ComplexListNode* pHead)，复制一个复杂链表。在复杂链表中，每个结点除了有一个pNext指针指向下一个结点之外，还有一个pSibling指向链表中的任意结点或者NULL。结点的定义如下：

```
struct ComplexListNode{
    int label;
    ComplexListNode* pNext;
    ComplexListNode* pSibling;
};
```

![](https://img-blog.csdnimg.cn/2018112311203148.png)

思路：

（1）复制原链表每个节点并用pNext 连起来，接下来设置每个节点的pSibling。可以通过每次遍历查找每个元素的pSibling，这样时间复杂度是O(n^2)

（2）可以通过空间换取时间，将原始链表和复制链表的结点通过哈希表对应起来，这样查找的时间就从O(N)变为O(1)。具体为：复制原始链表上的每个结点N创建N'，然后把这些创建出来的结点用pNext连接起来。同时把<N,N'>的配对信息方法一个哈希表中；然后设置复制链表中的每个结点的pSibling指针，如果原始链表中结点N的pSibling指向结点S，那么在复制链表中，对应的N'应该指向S'。

```cpp
/*
struct RandomListNode {
    int label;
    struct RandomListNode *next, *random;
    RandomListNode(int x) :
            label(x), next(NULL), random(NULL) {
    }
};
*/
class Solution {
public:
    typedef std::map<RandomListNode*,RandomListNode*> MAP;
    RandomListNode* clonenode(RandomListNode* pHead,MAP &hashnode)  //复制一个链表
    {
        RandomListNode *pcopy = new RandomListNode(0); //建立一个头结点
        RandomListNode *p=pcopy,*tmp;
        while(pHead)
        {
            tmp=new RandomListNode(pHead->label);
            p->next = tmp;
            hashnode[pHead] = tmp;
            p=p->next;
            pHead = pHead->next;
        }return pcopy->next;
    }
    void linkRandomNode(RandomListNode* pHead,RandomListNode* cnode,MAP &hashnode)
    {
        while(cnode)
        {
            cnode->random = hashnode[pHead->random];   //将对应节点映射过来
            cnode=cnode->next;
            pHead=pHead->next;
        }
    }
    RandomListNode* Clone(RandomListNode* pHead)
    {
        if(pHead == nullptr) return nullptr;
        MAP hashnode;
        RandomListNode* cnode = clonenode(pHead,hashnode);
        linkRandomNode(pHead,cnode,hashnode);
        return cnode;
    }
};
```

（3）在不使用辅助空间的情况下实现O(N)的时间效率。第一步：根据原始链表的每个结点N创建对应的N'，然后将N‘通过pNext接到N的后面；第二步：设置复制出来的结点的pSibling。假设原始链表上的N的pSibling指向结点S，那么其对应复制出来的N'是N->pNext指向的结点，同样S'也是结点S->pNext指向的结点。第三步：把长链表拆分成两个链表，把奇数位置的结点用pNext连接起来的就是原始链表，把偶数位置的结点通过pNext连接起来的就是复制链表。

![](https://img-blog.csdnimg.cn/20181123112049569.png)

![](https://img-blog.csdnimg.cn/20181123112101949.png)

![](https://img-blog.csdnimg.cn/20181123112130892.png)

```cpp
class Solution {  //链接两个相邻表的方法
public:
    void clonenode(RandomListNode *pHead)  //复制在一个链表并将其连起来
    {
        RandomListNode *pclone;
        while(pHead)
        {
            pclone = new RandomListNode(pHead->label);
            pclone->next = pHead->next;
            pHead->next = pclone;
            pHead = pclone->next;
        }
    }
    void clonerandom(RandomListNode *pHead)  //复制random指针
    {
        RandomListNode *pclone;
        while(pHead)
        {
            pclone = pHead->next;
            if(pHead->random)
                pclone->random = pHead->random->next;
            pHead = pclone->next;
        }
    }
    RandomListNode* divorcelist(RandomListNode *pHead)
    {
        RandomListNode* pcHead,*pcnode,*pnode = pHead; //复制链表头结点，复制链表遍历节点，遍历节点
        if(pnode)   //解决两个链表的头结点，后面节点可以统一操作
        {
            pcHead = pcnode = pnode->next; //复制链表的头结点和遍历节点指向复制链表第一个节点
            pnode->next = pcnode->next;  
            pnode = pnode->next;   
        }
        while(pnode)   
        {
            pcnode->next = pnode->next;//交叉连接
            pcnode = pcnode->next;
            pnode->next = pcnode->next;
            pnode = pnode->next;
        }return pcHead;
    }
    RandomListNode* Clone(RandomListNode* pHead)
    {
        if(pHead == nullptr) return nullptr;
        clonenode(pHead);
        clonerandom(pHead);
        return divorcelist(pHead);
    }
};
```






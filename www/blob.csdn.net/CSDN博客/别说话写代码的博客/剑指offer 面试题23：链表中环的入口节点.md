# 剑指offer 面试题23：链表中环的入口节点 - 别说话写代码的博客 - CSDN博客





2018年11月15日 20:22:30[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：39
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：一个链表中包含环，请找出该链表的环的入口结点。

解法：

1.使用哈希表，遍历整个链表，并将链表结点存入哈希表中（这里我们使用容器set），如果遍历到某个链表结点已经在set中，那么该点即为环的入口结点

2.如果链表存在环，我们无需计算环的长度n，只需在相遇时，让一个指针在相遇点出发，另一个指针在链表首部出发，然后两个指针一次走一步，当它们相遇时，就是环的入口处。

法一：

```cpp
class Solution {
public:
    ListNode* EntryNodeOfLoop(ListNode* pHead)
    {
        if(pHead == nullptr  || pHead->next==nullptr) return nullptr;
        set<ListNode*> listset;
        while(pHead != nullptr)
        {
            if(listset.find(pHead)  == listset.end())  //找到最后一个元素都没找到
            {
                listset.insert(pHead);
                pHead=pHead->next;
            }else return pHead;
        }return nullptr;
    }
}
```

法二：

```cpp
/*
struct ListNode {
    int val;
    struct ListNode *next;
    ListNode(int x) :
        val(x), next(NULL) {
    }
};
*/
 
class Solution {
public:
    ListNode* EntryNodeOfLoop(ListNode* pHead)
    {
        if(pHead==NULL || pHead->next==NULL)
            return NULL;
 
        ListNode* pSlow=pHead;
        ListNode* pFast=pHead;
        // detect if the linklist is a circle
        while(pFast!=NULL && pFast->next!=NULL){
            pSlow=pSlow->next;
            pFast=pFast->next->next;
            if(pSlow==pFast)
                break;
        }
        // if it is a circle
        if(pFast!=NULL){
            pSlow=pHead;
            while(pSlow!=pFast){
                pSlow=pSlow->next;;
                pFast=pFast->next;
            }
        }
         
        return pFast;
    }
};
```






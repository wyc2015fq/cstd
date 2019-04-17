# 剑指offer 面试题18：删除链表中的重复节点 c++ - 别说话写代码的博客 - CSDN博客





2018年11月19日 16:29:07[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：38
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









本题有两种

第一题：在一个排序的链表中，存在重复的结点，请删除该链表中重复的结点，重复的结点不保留，返回链表头指针。 例如，链表1->2->3->3->4->4->5 处理后为 1->2->3->4->5   （ 只是去掉了重复节点）

思路：两个指针 一前一后，若前后相等，删掉后面的

```cpp
class Solution {
public:
    ListNode* deleteDuplication(ListNode* pHead)
    {
        if(pHead==nullptr) return nullptr;
        ListNode* pre=pHead;
        ListNode* p=pre->next;
        while(p!=nullptr)
        {
            if(pre->val == p->val)
            {
                pre->next = p->next;
                free(p);
                p=pre->next;
                continue;
            }
            pre=pre->next;
            p=p->next;
        }return pHead;
    }
};
```

第二题：在一个排序的链表中，存在重复的结点，请删除该链表中重复的结点，重复的结点不保留，返回链表头指针。 例如，链表1->2->3->3->4->4->5 处理后为 1->2->5   （只要节点重复了删除所有值一样的节点）（注意和上面题目区别）

思路：先处理如果要删除的是第一个节点的情况，先建一个头结点给原来链表，这样操作就统一了。三个指针，pre是当前指针前一个，p是当前指针，pnext是下一个，如果一直是p->val==pnext，那就删掉，直到两个 不相等

```cpp
class Solution {
public:
    ListNode* deleteDuplication(ListNode* pHead)
    {
        if(pHead==nullptr ) return nullptr;
        ListNode* newhead = new ListNode(-1);
        newhead->next=pHead; 
        ListNode* pre=newhead;
        ListNode* p=pHead,*pnext =p->next;
        while(p!=nullptr &&p->next!=nullptr)
        {
            if(p->val == pnext->val)
            {
                while(pnext!=nullptr && pnext->val==p->val)
                {
                    ListNode* temp = pnext;
                    pnext=pnext->next;
                    free(temp);
                    temp=nullptr;
                }
                pre->next=pnext;
                p=pnext;
            }else{
                pre=p;
                p=p->next;
            }
            pnext=p->next;
        }return newhead->next;
    }
};
```






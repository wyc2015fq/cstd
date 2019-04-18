# 剑指offer - 合并两个排序的链表 - curryche的博客 - CSDN博客





2018年08月15日 20:27:50[curryche](https://me.csdn.net/whwan11)阅读数：36








## 题目

合并两个排序的链表 

时间限制：1秒 空间限制：32768K 热度指数：297782 

本题知识点： 链表
题目描述 

输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。

## 解法

代码

```
/*
struct ListNode {
    int val;
    struct ListNode *next;
    ListNode(int x) :
            val(x), next(NULL) {
    }
};*/
class Solution {
public:
    ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
    {
        if(pHead1==NULL)
            return pHead2;
        if(pHead2==NULL)
            return pHead1;

        ListNode* newhead=NULL;
        ListNode* id1=NULL;
        ListNode* id2=NULL;
        ListNode* new_last=NULL;

        if(pHead1->val<=pHead2->val)
        {
            newhead=pHead1;
            id1=pHead1->next;
            id2=pHead2;
        }
        else
        {
            newhead=pHead2;
            id1=pHead1;
            id2=pHead2->next;
        }

        new_last=newhead;
        new_last->next=NULL;

        while(id1!=NULL&&id2!=NULL)
        {
            if(id1->val<=id2->val)
            {
                new_last->next=id1;
                id1=id1->next;
                new_last=new_last->next;
                new_last->next=NULL;
            }
            else
            {
                new_last->next=id2;
                id2=id2->next;
                new_last=new_last->next;
                new_last->next=NULL;
            }
        }

        if(id1!=NULL)
            new_last->next=id1;
        if(id2!=NULL)
            new_last->next=id2;

        return newhead;




    }
};
```





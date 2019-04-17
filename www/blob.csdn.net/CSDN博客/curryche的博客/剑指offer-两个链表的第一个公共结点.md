# 剑指offer - 两个链表的第一个公共结点 - curryche的博客 - CSDN博客





2018年09月04日 21:27:06[curryche](https://me.csdn.net/whwan11)阅读数：30标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

两个链表的第一个公共结点 

时间限制：1秒 空间限制：32768K 热度指数：141214 

本题知识点： 链表
题目描述 

输入两个链表，找出它们的第一个公共结点。

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
    ListNode* FindFirstCommonNode( ListNode* pHead1, ListNode* pHead2) {
        if(pHead1==NULL||pHead2==NULL)
            return (ListNode*)NULL;

        int L1,L2,i;
        L1=0;
        L2=0;
        ListNode* iter1=pHead1;
        ListNode* iter2=pHead2;

        while(iter1!=NULL)
        {
            L1++;
            iter1=iter1->next;
        }

        while(iter2!=NULL)
        {
            L2++;
            iter2=iter2->next;
        }
        iter1=pHead1;
        iter2=pHead2;
        if(L1>L2)
        {
            i=0;
            while(i<L1-L2&&iter1!=NULL)
            {
                iter1=iter1->next;
                i++;
            }

        }
        if(L2>L1)
        {
            i=0;
            while(i<L2-L1&&iter2!=NULL)
            {
                iter2=iter2->next;
                i++;
            }

        }
        while(iter1!=iter2&&iter1!=NULL&&iter2!=NULL)
        {
            iter1=iter1->next;
            iter2=iter2->next;
        }
        return iter1;
    }
};
```






# 剑指offer - 链表中倒数第k个结点 - curryche的博客 - CSDN博客





2018年08月13日 22:39:30[curryche](https://me.csdn.net/whwan11)阅读数：21标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

链表中倒数第k个结点

时间限制：1秒 空间限制：32768K 热度指数：432979

算法知识视频讲解 

题目描述 

输入一个链表，输出该链表中倒数第k个结点。
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
    ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) 
    {
        ListNode* first=NULL;
        ListNode* second=NULL;

        if(pListHead==NULL)
            return second;
        int count=0;

        first=pListHead;
        second=pListHead;
        while(first)
        {
            first=first->next;
            if(count>=k)
                second=second->next;
            count++;

        }
        if(k>count)
            second=NULL;
        return second;
    }
};
```






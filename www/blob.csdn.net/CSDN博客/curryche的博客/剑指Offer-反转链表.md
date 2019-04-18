# 剑指Offer - 反转链表 - curryche的博客 - CSDN博客





2018年08月15日 20:14:46[curryche](https://me.csdn.net/whwan11)阅读数：34标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

反转链表 

时间限制：1秒 空间限制：32768K 热度指数：314538 

本题知识点： 链表
题目描述 

输入一个链表，反转链表后，输出新链表的表头。

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
    ListNode* ReverseList(ListNode* pHead) {
        if(pHead==NULL)
            return (ListNode*)NULL;

        ListNode* newhead=pHead;
        ListNode* current=pHead->next;
        ListNode* temp;

        if(current==NULL)
            return pHead;

        newhead->next=NULL;
        while(current!=NULL)
        {
            temp=current->next;
            current->next=newhead;
            newhead=current;
            current=temp;
        }
        return newhead;
    }
};
```






# 剑指Offer - 链表中环的入口结点 - curryche的博客 - CSDN博客





2018年08月15日 20:02:35[curryche](https://me.csdn.net/whwan11)阅读数：45








## 题目

链表中环的入口结点 

时间限制：1秒 空间限制：32768K 热度指数：109368 

本题知识点： 链表
题目描述 

给一个链表，若其中包含环，请找出该链表的环的入口结点，否则，输出null。

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
};
*/
class Solution {
public:
    ListNode* EntryNodeOfLoop(ListNode* pHead)
    {
        if(pHead==NULL)
            return (ListNode*)NULL;

        ListNode* fast=NULL;
        ListNode* slow=NULL;
        bool loop;
        int count,i;

        slow=pHead->next;
        if(slow==NULL)
            return (ListNode*)NULL;

        fast=slow;

        loop=false;
        while(slow!=NULL&&fast!=NULL)
        {
            slow=slow->next;
            fast=fast->next;
            if(fast==NULL)
                break;
            fast=fast->next;
            if(slow==fast)
            {
                loop=true;
                break;
            }
        }
        if(!loop)
            return (ListNode*)NULL;

        count=1;
        slow=slow->next;
        fast=fast->next->next;

        while(slow!=fast)
        {
            slow=slow->next;
            fast=fast->next->next;
            count++;
        }
        fast=pHead;
        slow=pHead;

        i=0;
        while(i<count)
        {
            fast=fast->next;
            i++;
        }
        while(slow!=fast)
        {
            slow=slow->next;
            fast=fast->next;
        }
        return slow;
    }
};
```





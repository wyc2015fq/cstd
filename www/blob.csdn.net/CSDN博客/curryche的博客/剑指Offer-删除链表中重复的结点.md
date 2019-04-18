# 剑指Offer - 删除链表中重复的结点 - curryche的博客 - CSDN博客





2018年08月10日 21:25:19[curryche](https://me.csdn.net/whwan11)阅读数：22标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

删除链表中重复的结点 

时间限制：1秒 空间限制：32768K 热度指数：187116 

本题知识点： 链表
题目描述 

在一个排序的链表中，存在重复的结点，请删除该链表中重复的结点，重复的结点不保留，返回链表头指针。 例如，链表1->2->3->3->4->4->5 处理后为 1->2->5

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
    ListNode* deleteDuplication(ListNode* pHead)
{
    ListNode* newhead = NULL;
    ListNode* current = pHead;
    ListNode* new_last = NULL;
    int former_sim;

    if (pHead == NULL)
        return newhead;

    if (pHead->next == NULL)
    {
        return pHead;
    }

    former_sim = pHead->next->val;

    while (current != NULL&&current->next != NULL)
    {

        if (current->val == former_sim)
        {
            current = current->next;
        }
        else if (current->val == current->next->val)
        {
            former_sim = current->val;
            current = current->next->next;
        }
        else
        {
            if (newhead == NULL)
            {
                newhead = current;
                new_last = newhead;
            }
            else
            {
                new_last->next = current;
                new_last = current;
            }
            former_sim = current->val;
            current = current->next;

            new_last->next = NULL;
        }
    }

    if (current!=NULL)
    {

        if (current->val != former_sim)
        {
            if (newhead == NULL)
            {
                newhead = current;
            }
            else
            {
                new_last->next = current;
            }
        }
    }


    return newhead;
}
};
```






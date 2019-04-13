
# 206. Reverse Linked List - OraYang的博客 - CSDN博客

2017年07月30日 16:49:00[OraYang](https://me.csdn.net/u010665216)阅读数：186标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Reverse a singly linked list.
思路：
递归的方法
代码：

```python
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if(head==NULL||head->next==NULL)
            return head;
        ListNode* p = head->next;
        ListNode* n = reverseList(p);
        head->next = NULL;
        p->next = head;
        return n;
        
    }
};
```



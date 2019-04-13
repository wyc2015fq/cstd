
# 83. Remove Duplicates from Sorted List - OraYang的博客 - CSDN博客

2017年07月15日 11:43:12[OraYang](https://me.csdn.net/u010665216)阅读数：116标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a sorted linked list, delete all duplicates such that each element appear onlyonce.
For example,
Given`1->1->2`, return`1->2`.
Given`1->1->2->3->3`, return`1->2->3`.
思路：这道题目很简单的，就是链表重复值的剔除，由于已经排序，则只需要后面的值与前面的值比较即可。
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
    ListNode* deleteDuplicates(ListNode* head) {
        if(head == NULL)  
            return head;  
        ListNode *pre = head;  
        ListNode *cur = head->next;  
        while(cur!=NULL)  
        {  
            if(cur->val == pre->val)  
                pre->next = cur->next;  
            else      
                pre = cur;  
            cur = cur->next;  
        }  
        return head;
    }
};
```



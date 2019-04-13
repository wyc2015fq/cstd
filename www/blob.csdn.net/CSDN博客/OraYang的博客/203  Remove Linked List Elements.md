
# 203. Remove Linked List Elements - OraYang的博客 - CSDN博客

2017年07月30日 15:14:23[OraYang](https://me.csdn.net/u010665216)阅读数：168所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Remove all elements from a linked list of integers that have valueval.
Example
Given:1 --> 2 --> 6 --> 3 --> 4 --> 5 --> 6,val= 6
Return:1 --> 2 --> 3 --> 4 --> 5
思路：
本题比较简单，主要考察的是链表的操作
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
    ListNode* removeElements(ListNode* head, int val) {
        if(head==NULL)
            return head;
        ListNode* slow = head;
        ListNode* fast = head->next;
        while(fast)
        {
            if(fast->val==val)
            {
                slow->next = fast->next;
                fast = fast->next;
            }
            else
            {
              slow = slow->next;
              fast = fast->next;  
            }
            
        }
        if(head->val==val)
            head = head->next;
        return head;
        
    }
};
```



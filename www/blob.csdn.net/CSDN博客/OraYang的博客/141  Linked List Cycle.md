
# 141. Linked List Cycle - OraYang的博客 - CSDN博客

2017年07月25日 17:17:59[OraYang](https://me.csdn.net/u010665216)阅读数：172所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a linked list, determine if it has a cycle in it.
Follow up:
Can you solve it without using extra space?
思路：这道题目纯属数学题，判断是否圆环，使用两个指针，一个指针每次前进1，一个指针每次前进2，假设存在环的情况下后者必定会追上前者，如果不存在圆环，则判断后者指针是否为NULL(**此处注意指针判断需要两次**)
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
    bool hasCycle(ListNode *head) {
        ListNode* fast = head;
        while(1)
        {
            
            if(fast==NULL)
                return false;
            fast = fast->next;
            if(fast==NULL)
                return false;
            fast = fast->next;
            head = head->next;
            if(fast==head)
                return true;
        }
      
    }
};
```



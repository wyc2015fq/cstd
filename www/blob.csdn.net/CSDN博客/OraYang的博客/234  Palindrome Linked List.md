
# 234. Palindrome Linked List - OraYang的博客 - CSDN博客

2017年08月02日 08:56:50[OraYang](https://me.csdn.net/u010665216)阅读数：358标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a singly linked list, determine if it is a palindrome.
思路：
遍历一次链表维护一个数组并存储对应value,再从头遍历链表的同时从尾部遍历数组，并进行比较
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
    bool isPalindrome(ListNode* head) {
        ListNode* temp = head;
        vector<int> nums;
        while(temp)
        {
            nums.push_back(temp->val);
            temp = temp->next;
        }
        vector<int>::iterator itr= nums.end();
        itr--;
        while(head)
        {
            if(head->val==*itr)
            {
                itr--;
                head= head->next;
            }
            else
                return false;
        }
        return true;
    }
};
```



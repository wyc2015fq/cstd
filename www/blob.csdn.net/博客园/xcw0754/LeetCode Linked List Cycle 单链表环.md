# LeetCode Linked List Cycle  单链表环 - xcw0754 - 博客园
# [LeetCode Linked List Cycle  单链表环](https://www.cnblogs.com/xcw0754/p/4681854.html)
题意：给一个单链表，判断其是否出现环！
思路：搞两个指针，每次，一个走两步，另一个走一步。若有环，他们会相遇，若无环，走两步的指针必定会先遇到NULL。
```
1 /**
 2  * Definition for singly-linked list.
 3  * struct ListNode {
 4  *     int val;
 5  *     ListNode *next;
 6  *     ListNode(int x) : val(x), next(NULL) {}
 7  * };
 8  */
 9 class Solution {
10 public:
11     bool hasCycle(ListNode *head) {
12         if(!head)   return false;
13         ListNode * one=head, *two=head->next;
14         while(two && two->next && one!=two)
15         {
16             one=one->next;
17             two=two->next->next;
18         }
19         if(two&&two->next)    return true;
20         else       return false;
21     }
22 };
AC代码
```


# LeetCode Remove Linked List Elements 删除链表元素 - xcw0754 - 博客园
# [LeetCode Remove Linked List Elements 删除链表元素](https://www.cnblogs.com/xcw0754/p/4456808.html)
![](https://images0.cnblogs.com/blog2015/641737/201504/252205330464786.png)
题意：移除链表中元素值为val的全部元素。
思路：算法复杂度肯定是O(n)，那么就在追求更少代码和更少额外操作。我做不出来。
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
11     ListNode* removeElements(ListNode* head, int val) {
12         while(head&&head->val==val)    head=head->next;
13         if(!head)    return 0;
14         ListNode *tmp=head;
15         while(head&&head->next)
16         {
17             if(head->next->val==val)
18                 head->next=head->next->next;
19             else
20                 head=head->next;
21         }
22         return tmp;
23     }
24 };
Remove Linked List Elements
```


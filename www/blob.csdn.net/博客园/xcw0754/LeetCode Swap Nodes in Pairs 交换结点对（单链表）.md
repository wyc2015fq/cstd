# LeetCode Swap Nodes in Pairs  交换结点对（单链表） - xcw0754 - 博客园
# [LeetCode Swap Nodes in Pairs  交换结点对（单链表）](https://www.cnblogs.com/xcw0754/p/4679343.html)
题意：给一个单链表，将其每两个结点交换，只改尾指针，不改元素值。
思路：迭代法和递归法都容易写，就写个递归的了。
4ms
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
11     ListNode* swapPairs(ListNode* head) {
12         if(!head || !head->next )   return head;
13 
14         ListNode *t=head->next;
15         head->next=swapPairs(t->next);
16         return t->next=head, t;
17     }
18 
19 };
AC代码
```


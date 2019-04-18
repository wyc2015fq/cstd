# LeetCode Intersection of Two Linked Lists  （找交叉点） - xcw0754 - 博客园
# [LeetCode Intersection of Two Linked Lists  （找交叉点）](https://www.cnblogs.com/xcw0754/p/4639416.html)
题意：
　　给两个链表，他们的后部分可能有公共点。请返回第一个公共节点的地址？若不重叠就返回null。
思路：
　　用时间O(n)和空间O(1)的做法。此题数据弱有些弱。
　　方法（1）假设两个链表A和B，用两个指针分别按顺序遍历AB和BA，这AB和BA肯定等长的。如果他们有公共点，那么按照这样走必定会在某个点相遇。所以只要预先判断是否有公共点，这只需要用两个指针分别指向A和B的链尾判断是否相同即可（这一步可以使用更简单的方法，增加1个计数器，判断指针1是否已经遍历过AB了）。
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
11     ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
12         ListNode *L1=headA;
13         ListNode *L2=headB;
14         //若无交点，且AB等长，那么他们会在null处相遇，退出。
15         //先判断是否有交点先。
16         while(L1&&L1->next) L1=L1->next;
17         while(L2&&L2->next) L2=L2->next;
18         if(L1!=L2)  return NULL;
19         L1=headA;
20         L2=headB;
21         while(L1!=L2)
22         {
23             if(L1)  L1=L1->next;
24             else    L1=headB;
25 
26             if(L2)  L2=L2->next;
27             else L2=headA;
28         }
29         return L1;
30     }
31 };
AC代码
```
 　　方法（2）先统计链A和B的长度，假设A长，那么指针1先走|A|-|B|步，然后再同时走，若有公共点必定会相遇。


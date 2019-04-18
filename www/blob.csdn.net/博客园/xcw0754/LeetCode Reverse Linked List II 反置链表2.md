# LeetCode  Reverse Linked List II  反置链表2 - xcw0754 - 博客园
# [LeetCode  Reverse Linked List II  反置链表2](https://www.cnblogs.com/xcw0754/p/4685426.html)
题意：将指定的一段位置[m,n]的链表反置，返回链表头。
思路：主要麻烦在链表头，如果要从链表头就开始，比较特殊。
　　目前用DFS实现，先找到m-1的位置，再找到n+1的位置，中间这段就是否要反置的，交给DFS解决，用个计数器来统计已经反置的个数即可。
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
11     ListNode* DFS(ListNode* t,ListNode* far, int num)
12     {
13         if(num==0)  return far;
14         ListNode* tmp=t->next;
15         t->next=far;
16         return DFS(tmp , t, num-1);
17     }
18 
19     ListNode* reverseBetween(ListNode* head, int m, int n) {
20         if(m==n)    return head;
21         ListNode* t=head, *r=head;
22 
23         for(int i=0; i<n; i++)  r=r->next;     //找到第n+1个
24         if(m==1)    return DFS(head, r, n-m+1);
25 
26         for(int i=1; i<m-1; i++)  t=t->next;   //找到第m-1个
27         t->next=DFS(t->next, r, n-m+1);
28         return head;
29     }
30 };
AC代码
```


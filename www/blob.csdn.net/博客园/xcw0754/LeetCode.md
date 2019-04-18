# LeetCode - xcw0754 - 博客园
# [LeetCode](https://www.cnblogs.com/xcw0754/p/4667739.html)
c++的速度
![](https://images0.cnblogs.com/blog2015/641737/201507/221603487403911.png)
c的速度
![](https://images0.cnblogs.com/blog2015/641737/201507/221604415066557.png)
题意：将单链表重新编序为 *L*0→*L**n*→*L*1→*L**n*-1→*L*2→*L**n*-2→… 。
思路：先将链表后半段反置，变成2个链表，再递归将一个左链元素和一个右链元素合并。
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
11     ListNode * rev_List(ListNode *t, ListNode *far) //反置链表
12     {
13         if(!t)  return far;
14         ListNode *tmp = t->next;    //临时存储
15         t->next=far;
16         return rev_List(tmp,t);
17     }
18 
19     ListNode * remerge(ListNode *cur1,ListNode *cur2)   //两个两个操作
20     {
21         if(!cur2)   return cur1;    //要么一块为空，要么cur2先空
22 
23         ListNode *tmp=cur1->next;    //保存左边的后继
24         cur1->next=cur2;            //改变指向
25         cur2->next= remerge(tmp, cur2->next);
26         return cur1;
27     }
28 
29 
30     void reorderList(ListNode* head) {
31         if(!head||!head->next)  return;
32         //将后半反置
33         ListNode *mid=head,*last=head;
34         while(last&&last->next) //二分找中点
35         {
36             mid=mid->next;
37             last=last->next->next;
38         }
39         ListNode *rev_head =rev_List(mid->next,0);
40         mid->next=0;
41 
42         //递归合并链
43         head=remerge(head,rev_head);
44     }
45 };
AC代码
```


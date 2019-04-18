# LeetCode Linked List Cycle II 单链表环2 （找循环起点） - xcw0754 - 博客园
# [LeetCode Linked List Cycle II 单链表环2 （找循环起点）](https://www.cnblogs.com/xcw0754/p/4681989.html)
题意：给一个单链表，若其有环，返回环的开始处指针，若无环返回NULL。
思路：
　　（1）依然用两个指针的追赶来判断是否有环。在确定有环了之后，指针1跑的路程是指针2的一半，而且他们曾经跑过一段重叠的路（即1跑过，2也跑过），就是那段（环开始处，相遇处），那么指针2开始到环开始处的距离与head到指针相遇处是等长的喔~，那么再跑一次每次一步的就必定会相遇啦。画个图图好方便看~
　　（2）其实还有另一个直观的思路，就是指针1和2相遇后，p指向他们的next，在他们相遇处的next给置空，再跑一遍那个“找两个链表后半段重叠的开始处”那道题就行了。
（1）代码
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
11         ListNode *detectCycle(ListNode *head) {
12             if(!head)   return 0;
13             ListNode *one=head, *two=head->next;
14             while(two&&two->next&&one!=two)
15             {
16                 one=one->next;
17                 two=two->next->next;
18             }
19             if(!two||!two->next) return 0;  //无环
20             two=two->next;//此时他们已经相遇了，two后移一步，使two与head同时到one等长。
21             while(head!=two)//必定会相遇
22             {
23                 head=head->next;
24                 two=two->next;
25             }
26             return head;
27         }
28 };
AC代码
```
（2）代码
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
11     ListNode *detectCycle(ListNode *head) {
12         if(!head)   return 0;
13         ListNode *one=head, *two=head->next;
14         while(two&&two->next&&one!=two)
15         {
16             one=one->next;
17             two=two->next->next;
18         }
19         if(!two||!two->next) return 0;  //无环
20         one=one->next;  //此时two还在断口处
21         two->next=0;
22 
23         ListNode * p1=one, *p2=head;
24         while(p1 && p2 && p1!=p2 )//两链表找重叠处~即使p1p2到开始重叠处不等长也能解决
25         {
26             p1=p1->next;
27             p2=p2->next;
28 
29             if(!p1) p1=head;
30             if(!p2) p2=one;
31         }
32         two->next=one;
33         return p1;
34     }
35 };
AC代码
```


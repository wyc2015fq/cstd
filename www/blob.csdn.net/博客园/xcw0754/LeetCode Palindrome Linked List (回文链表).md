# LeetCode Palindrome Linked List (回文链表) - xcw0754 - 博客园
# [LeetCode Palindrome Linked List (回文链表)](https://www.cnblogs.com/xcw0754/p/4638309.html)
题意：给个单链表，判断是否为回文。
思路：有点BT，处理不好都是死循环。一般思路是，二分查找中心点，根据奇偶个元素，反置前半部分，再判断是否回文，再恢复前半部分。
步骤：
　　（1）在二分查找中心点时判断顺便反置前半部分链表。
　　（2）对奇偶数处理好剩下的工作。这是重点
　　（3）两个指针来判断是否回文。
　　（4）将前半段恢复起来，比较简单，有个head作为界限标志呢。
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
11     bool isPalindrome(ListNode* head) {
12         if(!head || !head->next)   return true;   //保证起码两个
13         bool ans=false;//答案要记录。
14         ListNode *mid=head, *e=head->next;  //用于找中点
15         ListNode *p=0, *q=mid, *s=0;        //用于反置前半链表
16 
17         while(e && e->next)   //找中间点时顺便反置前部分链表。以减少创建指针。
18         {
19             mid=mid->next;
20             e=e->next->next;
21             q->next=p;
22             p=q;
23             q=mid;
24         }
25         s=mid->next;//s一定要处于后半段的欲比较部分。
26         if(e)     q->next=p, p=q, mid=mid->next;            //偶数个，q在前半段的尾部，还没有完全反好链表。
27         else      q=p;                                      //奇数个，q会在中点位置，要挪向前。
28 
29         while(s && p->val==s->val)   p=p->next,s=s->next;//判断是否回文
30         if(!s)  ans=true;
31 
32         //q为前段链表头。mid为后段链表头，只有后段才可能奇数个。
33         //恢复前半段, q已经连着那条链的链头。
34 
35         while(q!=head)
36         {
37             s=q->next;
38             q->next=mid;
39             mid=q;
40             q=s;
41         }
42         return ans;
43     }
44 };
AC代码
```


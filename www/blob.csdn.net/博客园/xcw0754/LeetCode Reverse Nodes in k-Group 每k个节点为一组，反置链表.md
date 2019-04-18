# LeetCode  Reverse Nodes in k-Group  每k个节点为一组，反置链表 - xcw0754 - 博客园
# [LeetCode  Reverse Nodes in k-Group  每k个节点为一组，反置链表](https://www.cnblogs.com/xcw0754/p/4682120.html)
题意：给一个单链表，每k个节点就将这k个节点反置，若节点数不是k的倍数，则后面不够k个的这一小段链表不必反置。
思路：递归法。每次递归就将k个节点反置，将k个之后的链表头递归下去解决。利用原来的函数接口即可，不用重新定义。
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
11     ListNode* reverseKGroup(ListNode* head, int k) {
12         if(k==1)    return head;
13         ListNode* p=head;
14         int cnt=0;
15         while(cnt!=k && p)    //找到下一段的链表头
16         {
17             cnt++;
18             p=p->next;
19         }
20         if(cnt==k)  //进行反置
21         {
22             ListNode* nextp=head->next, frontp=head, qq;
23             while(nextp!=p) //p是下一段链表的开头
24             {
25                 qq=nextp->next;
26                 nextp->next=frontp;
27                 frontp=nextp;
28                 nextp=qq;
29             }
30             head->next=reverseKGroup( p ,k );
31             return frontp;  //返回新链表的头指针
32         }
33         else    return head;    //不够k个
34     }
35 };
AC代码
```


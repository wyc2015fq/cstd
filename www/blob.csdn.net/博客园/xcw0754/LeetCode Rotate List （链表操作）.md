# LeetCode Rotate List （链表操作） - xcw0754 - 博客园
# [LeetCode Rotate List （链表操作）](https://www.cnblogs.com/xcw0754/p/4970471.html)
题意：
　　将链表的后面k个剪出来，拼接到前面，比如 1->2->null 变成2->1->null。数字代表一段的意思。
思路：
　　k有3种可能，k>n，k<n，k=n。理想情况就是k<n，这样就好操作，而当k>n时，k%=n即可，而当k=n时，无需操作。链表可能为空！　　
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
11     ListNode* rotateRight(ListNode* head, int k) {
12         if(k==0 || head==NULL)    return head;
13 
14         ListNode *first=head, *second=head, *t=head;
15         int cnt=0;
16         while(t)    cnt++,t=t->next;//数一下
17         k%=cnt;
18 
19         while(k--)  first=first->next;//将first移动到相应位置
20         while(first->next)    //两指针同时走
21         {
22             first=first->next;
23             second=second->next;
24         }
25         first->next=head;//拼接操作
26         head=second->next;
27         second->next=NULL;
28         return head;
29     }
30 };
AC代码
```


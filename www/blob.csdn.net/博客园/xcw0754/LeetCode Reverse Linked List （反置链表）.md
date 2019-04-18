# LeetCode Reverse Linked List （反置链表） - xcw0754 - 博客园
# [LeetCode Reverse Linked List （反置链表）](https://www.cnblogs.com/xcw0754/p/4621527.html)
![](https://images0.cnblogs.com/blog2015/641737/201507/042359468219830.png)
题意：
　　将单恋表反转。
思路：
　　两种方法：迭代和递归。
　　递归
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
11     ListNode* give_me_your_son( ListNode* far, ListNode* son)
12     {
13         if(!son)    return far;     //far就是链表尾了
14         ListNode* tmp=son->next;
15         son->next=far;
16         return give_me_your_son( son, tmp);
17     }
18 
19     ListNode* reverseList(ListNode* head) {
20         if(!head||!head->next)   return head;
21         return give_me_your_son(0,head);
22     }
23 };
AC代码
```
python3
　　迭代
```
1 # Definition for singly-linked list.
 2 # class ListNode(object):
 3 #     def __init__(self, x):
 4 #         self.val = x
 5 #         self.next = None
 6 
 7 class Solution(object):
 8     def reverseList(self, head):
 9         """
10         :type head: ListNode
11         :rtype: ListNode
12         """
13         top=None
14         while head:
15              top,head.next,head=head,top,head.next
16         return top
AC代码
```
　　递归
```
1 # Definition for singly-linked list.
 2 # class ListNode(object):
 3 #     def __init__(self, x):
 4 #         self.val = x
 5 #         self.next = None
 6 
 7 class Solution(object):
 8     def reverseList(self, head, pre=None):
 9         """
10         :type head: ListNode
11         :rtype: ListNode
12         """
13         if not head:    return pre
14         back=head.next
15         head.next=pre    
16         return self.reverseList(back, head)
AC代码
```


# LeetCode Merge Two Sorted Lists 归并排序 - xcw0754 - 博客园
# [LeetCode Merge Two Sorted Lists 归并排序](https://www.cnblogs.com/xcw0754/p/4125349.html)
题意：
　　将两个有序的链表归并为一个有序的链表。
思路：
　　设合并后的链表为head，现每次要往head中加入一个元素，该元素要么属于L1，要么属于L2，可想而知，此元素只能是L1或者L2的首个元素，那么进行一次比较就可以知道是谁了。操作到L1或L2其中一个已经没有元素为止，剩下的直接加到head后面。
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
11     ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
12         if(l1==0)    return l2;
13         if(l2==0)    return l1;
14         ListNode *start=0,*end=0;
15         if(l1->val<l2->val){
16             start=end=l1;
17             l1=l1->next;
18         }
19         else{
20             start=end=l2;
21             l2=l2->next;
22         }
23         while(l1!=0&&l2!=0){
24             if(l1->val<l2->val){
25                 end->next=l1;
26                 l1=l1->next;
27             }
28             else{
29                 end->next=l2;
30                 l2=l2->next;
31             }
32             end=end->next;
33         }
34         if(l1==0)
35             end->next=l2;
36         else
37             end->next=l1;
38         return start;   
39     }
40 };
AC代码
```
python3
```
1 # Definition for singly-linked list.
 2 # class ListNode(object):
 3 #     def __init__(self, x):
 4 #         self.val = x
 5 #         self.next = None
 6 
 7 class Solution(object):
 8     def mergeTwoLists(self, l1, l2):
 9         """
10         :type l1: ListNode
11         :type l2: ListNode
12         :rtype: ListNode
13         """
14         head=cur=ListNode(0)
15         while l1 and l2:
16             if l1.val<l2.val:    cur.next, l1=l1, l1.next
17             else:                cur.next, l2=l2, l2.next
18             cur=cur.next
19         if l1:    cur.next=l1
20         if l2:    cur.next=l2
21         return head.next
AC代码
```


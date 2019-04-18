# LeetCode  Remove Duplicates from Sorted List 删除有序链表中的重复结点 - xcw0754 - 博客园
# [LeetCode  Remove Duplicates from Sorted List 删除有序链表中的重复结点](https://www.cnblogs.com/xcw0754/p/4107985.html)
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
11 ListNode *deleteDuplicates(ListNode *head) {
12     if(head==0)    return 0;
13     ListNode *s,*e;
14     s=head;        //s指向前一个
15     e=head->next;        //e指向后一个
16     while(e){
17         if(e->val==s->val){
18             s->next=e->next;
19             delete(e);
20             e=s->next;
21         }
22         else{
23             s=s->next;
24             e=e->next;
25         }
26     }
27     return head;    
28 }
29 };
```
题意：给出一条链表，已经排序好了，如果有重复的就删除重复的（即每个元素只允许出现一次）。
思路：因为有序了，所以要删的肯定是相邻的几个。把第一个出现的元素保留，第2次以上出现的就删除。两个指针，一个指向前一元素，一个指向后一元素。逐个判断到结束。
注意：此题用一个指针也是可以解决的，当然，一个两个所占内存区别不大，就用两个比较直观了。


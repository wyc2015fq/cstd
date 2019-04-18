# LeetCode Insertion Sort List 链表插入排序 - xcw0754 - 博客园
# [LeetCode Insertion Sort List 链表插入排序](https://www.cnblogs.com/xcw0754/p/4663128.html)
题意：给一个链表，实现插入排序。
思路：O（1）空间，O(n*n)复杂度。将排好的用另一个链表头串起来，那个链表头最后删掉，再返回链表。
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
11     ListNode* insertionSortList(ListNode* head) {
12         if(!head)   return 0;
13         ListNode *tmp_h=new  ListNode(0);
14         tmp_h->next= head;    //另外一个链
15         ListNode *p=head->next;
16         head->next=0;
17         while(p)
18         {
19             ListNode *cur=p;    //将要插入的对象
20             p=p->next;          //移到下一个对象
21             ListNode * left=tmp_h;  //遍历指针
22             while(left->next&&left->next->val<cur->val)   left=left->next;
23             cur->next=left->next;
24             left->next=cur;
25         }
26         head=tmp_h->next;
27         free(tmp_h);
28         return head;
29     }
30 };
AC代码
```


# LeetCode Delete Node in a Linked List （删除链表中的元素） - xcw0754 - 博客园
# [LeetCode Delete Node in a Linked List （删除链表中的元素）](https://www.cnblogs.com/xcw0754/p/4654618.html)
题意：给一个将要删除的位置的指针，要删除掉该元素。被删元素不会是链尾（不可能删得掉）。
思路：将要找到前面的指针是不可能了，但是可以将后面的元素往前移1位，再删除最后一个元素。
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
11     void deleteNode(ListNode* node) {
12         while(node->next->next)//保证node能够在倒数第2个元素上退出
13         {
14             node->val=node->next->val;
15             node=node->next;
16         }
17         node->val=node->next->val;
18         delete(node->next);
19         node->next=0;//注意尾指针清空
20     }
21 };
AC代码
```


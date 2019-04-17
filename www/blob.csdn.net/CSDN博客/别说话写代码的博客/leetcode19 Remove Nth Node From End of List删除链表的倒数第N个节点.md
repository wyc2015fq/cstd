# leetcode19. Remove Nth Node From End of List删除链表的倒数第N个节点 - 别说话写代码的博客 - CSDN博客





2018年12月20日 10:49:00[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：37标签：[Remove Nth Node From End of Li																[删除链表的倒数第N个节点																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=删除链表的倒数第N个节点&t=blog)




题目：
|Given a linked list, remove the *n*-th node from the end of list and return its head.**Example:**Given linked list: **1->2->3->4->5**, and ***n* = 2**.After removing the second node from the end, the linked list becomes **1->2->3->5**.**Note:**Given *n* will always be valid.|给定一个链表，删除链表的倒数第 *n *个节点，并且返回链表的头结点。**示例：**给定一个链表: **1->2->3->4->5**, 和 ***n* = 2**.当删除了倒数第二个节点后，链表变为 **1->2->3->5**.**说明：**给定的 *n* 保证是有效的。|
|----|----|

思路：两个指针，cur先移动n步，pre指向第一个节点。然后两个一起移动，直到cur移动到最后一个元素，pre所指向的就是倒数第n个节点。注意的是链表长度为n删除倒数第n个几点， 那就直接返回head->next;

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if(head==NULL) return NULL;
        ListNode *cur=head,*pre=head;
        for(int i=0;i<n;++i) cur=cur->next;
        if(cur==NULL) return head->next;
        while(cur->next)
        {
            cur=cur->next;
            pre=pre->next;
        }
        pre->next=pre->next->next;
        return head;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Remove Nth Node From End of Li&t=blog)





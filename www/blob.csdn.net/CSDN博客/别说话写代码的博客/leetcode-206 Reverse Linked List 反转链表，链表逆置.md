# leetcode-206. Reverse Linked List 反转链表，链表逆置 - 别说话写代码的博客 - CSDN博客





2019年01月21日 14:13:01[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：39标签：[链表逆置																[反转链表																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=反转链表&t=blog)



|Reverse a singly linked list.**Example:****Input:** 1->2->3->4->5->NULL**Output:** 5->4->3->2->1->NULL**Follow up:**A linked list can be reversed either iteratively or recursively. Could you implement both?|反转一个单链表。**示例:****输入:** 1->2->3->4->5->NULL**输出:** 5->4->3->2->1->NULL**进阶:**			你可以迭代或递归地反转链表。你能否用两种方法解决这道题？|
|----|----|

思路：题目要求用迭代或递归求解

迭代法，从前往后遍历元素，将每个元素从原始链表摘下来，然后挂到第一个上，这里注意一个细节就是需要新生成一个新的节点作为链表的头结点，这样可以统一后面的操作

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
    ListNode* reverseList(ListNode* head) {
        if(!head || !head->next) return head;
        ListNode *newhead = new ListNode(-1);
        newhead->next=head;
        ListNode *cur=head,*pr=head;
        while(cur->next)
        {
            pr = cur->next;
            cur->next=pr->next;
            pr->next=newhead->next;
            newhead->next = pr;
        } return newhead->next;
    }
};
```](https://so.csdn.net/so/search/s.do?q=链表逆置&t=blog)





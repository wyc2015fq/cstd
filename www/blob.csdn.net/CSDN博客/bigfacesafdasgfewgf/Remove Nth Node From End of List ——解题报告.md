# Remove Nth Node From End of List ——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月08日 09:47:32[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：537标签：[LeetCode																[链表																[两指针法																[删除节点](https://so.csdn.net/so/search/s.do?q=删除节点&t=blog)
个人分类：[LeetCode																[Algorithm Probelm](https://blog.csdn.net/puqutogether/article/category/2812137)](https://blog.csdn.net/puqutogether/article/category/2832605)








    【题目】

    Given a linked list, remove the nth node from the end of list and return its head.


For example,


Given linked list: 1->2->3->4->5, and n = 2.


After removing the second node from the end, the linked list becomes 1->2->3->5.

Note:

Given n will always be valid.

Try to do this in one pass.




    【分析】

** 两指针法。先让first领先n-1步，然后first和second一起移动，当first到底的时候，second即为倒数第k个节点。题目需要删除，所以我们再额外记录second的前节点。**

    注意其中特殊情况的考虑。




    【代码】



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
        if(head == NULL || n <= 0)
            return head;
        
        ListNode* first = head;
        ListNode* second = head;
        ListNode* pre = NULL;
        for(int i = 0; i < n - 1; i++) // 中间间隔N-1的距离
            first = first->next;
        while(first->next != NULL)
        {
            pre = second;  // pre是倒数第k个节点的前一个节点，用来删除
            second = second->next;  // second是倒数第k个节点
            first = first->next;
        }
        
        if(pre == NULL)  // 如果倒数第k个节点就是head，那么pre会保留原来的NULL，这时head直接是倒数第k个节点second的下一个节点
            head = second->next;
        else
            pre->next = second->next;
        
        return head;
    }
};
```](https://so.csdn.net/so/search/s.do?q=两指针法&t=blog)](https://so.csdn.net/so/search/s.do?q=链表&t=blog)](https://so.csdn.net/so/search/s.do?q=LeetCode&t=blog)





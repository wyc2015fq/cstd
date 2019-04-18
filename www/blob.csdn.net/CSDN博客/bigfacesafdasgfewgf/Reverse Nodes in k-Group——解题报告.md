# Reverse Nodes in k-Group——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月12日 09:38:00[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：646











    【题目】




Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.


If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.


You may not alter the values in the nodes, only nodes itself may be changed.


Only constant memory is allowed.


For example,

Given this linked list: `1->2->3->4->5`


For k = 2, you should return: `2->1->4->3->5`


For k = 3, you should return: `3->2->1->4->5`


    【分析】

    先找到第k个节点，然后递归反转第k节点后面的链表，然后反转head到第k个节点的部分（此时使用非递归方式反转更加方便）。




    【题目】

    运行时间30ms



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
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* p = getKthNode(head, k);
        if(head == NULL || p == NULL)
            return head;
            
        ListNode* res = reverseKGroup(p->next, k);
        p->next = NULL;
        // 从head到res反转
        while(head)
        {
            ListNode* cur = head->next;
            head->next = res;
            res = head;
            head = cur;
        }
        return res;
    }
    
    ListNode* getKthNode(ListNode* head, int k)
    {
        for(int i = 0; head && i < k - 1; i++)
        {
            head = head->next;
        }
        return head;
    }

};
```









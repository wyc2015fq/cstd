# leetcode-148. Sort List 链表排序 - 别说话写代码的博客 - CSDN博客





2019年01月18日 22:12:05[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：35标签：[sort list																[leetcode																[链表排序](https://so.csdn.net/so/search/s.do?q=链表排序&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Sort a linked list in *O*(*n* log *n*) time using constant space complexity.**Example 1:****Input:** 4->2->1->3**Output:** 1->2->3->4**Example 2:****Input:** -1->5->3->4->0**Output:** -1->0->3->4->5|在 *O*(*n* log *n*) 时间复杂度和常数级空间复杂度下，对链表进行排序。**示例 1:****输入:** 4->2->1->3**输出:** 1->2->3->4**示例 2:****输入:** -1->5->3->4->0**输出:** -1->0->3->4->5|
|----|----|

思路：时间为nlogn， 所以必须想到快排，堆排序，归并。那就想到用归并对链表 排序，147题用插入对链表排序时间为n^2.

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
    ListNode* sortList(ListNode* head) {
        if(!head || !head->next) return head;
        ListNode *cur=head,*slow=head,*quick=head;
        while(quick && quick->next)
        {
            cur=slow;
            slow=slow->next;
            quick=quick->next->next;
        }
        cur->next=NULL;
        return merge(sortList(head),sortList(slow));
    }
    // ListNode* merge(ListNode* l1,ListNode* l2)   //非递归版本
    // {
    //     ListNode * newhead = new ListNode(-1);
    //     ListNode * newnode = newhead;
    //     while(l1 && l2)
    //     {
    //         if(l1->val < l2->val)
    //         {
    //             newnode->next=l1;
    //             l1=l1->next;
    //         }else{
    //             newnode->next=l2;
    //             l2=l2->next;
    //         }
    //         newnode=newnode->next;
    //     }
    //     if(l1) newnode->next=l1;
    //     if(l2) newnode->next=l2;
    //     return newhead->next;
    // }
    ListNode * merge(ListNode* l1,ListNode * l2)   //递归版本
    {
        if(!l1) return l2;
        if(!l2) return l1;
        if(l1->val <l2->val)
        {
            l1->next = merge(l1->next,l2);
            return l1;
        }else{
            l2->next = merge(l1,l2->next);
            return l2;
        }
    }
};
```](https://so.csdn.net/so/search/s.do?q=sort list&t=blog)





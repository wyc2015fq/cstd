# leetcode-21.Merge Two Sorted Lists 合并两个有序链表 - 别说话写代码的博客 - CSDN博客





2018年12月21日 10:45:52[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：36标签：[leetcode																[两个有序链表合并																[Merge Two Sorted Lists](https://so.csdn.net/so/search/s.do?q=Merge Two Sorted Lists&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=两个有序链表合并&t=blog)




题目：
|Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.**Example:****Input:** 1->2->4, 1->3->4**Output:** 1->1->2->3->4->4|将两个有序链表合并为一个新的有序链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。 **示例：****输入：**1->2->4, 1->3->4**输出：**1->1->2->3->4->4|
|----|----|

思路：不要生成新节点，直接返回，利用递归思路。

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
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if(l1==NULL) return l2;
        if(l2==NULL) return l1;
        if(l1->val < l2->val)
        {
            l1->next = mergeTwoLists(l1->next,l2);
            return l1;
        }
            
        else {
            l2->next = mergeTwoLists(l1,l2->next);
            return l2;
        }
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)





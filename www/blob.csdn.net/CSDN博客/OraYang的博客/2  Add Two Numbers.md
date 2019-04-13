
# 2. Add Two Numbers - OraYang的博客 - CSDN博客

2017年09月01日 10:44:59[OraYang](https://me.csdn.net/u010665216)阅读数：110标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You are given twonon-emptylinked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.
You may assume the two numbers do not contain any leading zero, except the number 0 itself.
Input:(2 -> 4 -> 3) + (5 -> 6 -> 4)
Output:7 -> 0 -> 8
思路：本题，考察了按位加法与链表的构建，思路很简单直接，按位相加，判断有无进位，再构建链表
代码：

```python
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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int sum,carry = 0;
        ListNode* temp = new ListNode(0);
        ListNode* head = temp;
        while(l1||l2)
        {
            if(l1!=NULL&&l2!=NULL)
            {
                sum = l1->val+l2->val+carry;//按位相加
                carry = sum / 10;
                ListNode* l = new ListNode(sum%10);
                temp->next = l ;//构建链表（后向链表）
                temp = l;
                l1 = l1->next;
                l2 = l2->next;
            }
            
            else if(!l1&&l2!=NULL)
            {
                sum = l2->val+carry;
                carry = sum / 10;
                ListNode* l = new ListNode(sum%10);
                temp->next = l ;
                temp = l;  
                l2 = l2->next;
            }
            else if(!l2&&l1!=NULL)
            {
                sum = l1->val+carry;
                carry = sum / 10;
                ListNode* l = new ListNode(sum%10);
                temp->next = l ;
                temp = l;  
                l1 = l1->next;
            } 
            
        }
        if(carry!=0)
        {
            ListNode* l = new ListNode(carry);
            temp->next = l ;
            temp = l; 
        }
        temp->next = NULL;
        head = head->next;
        return head;
    }
};
```


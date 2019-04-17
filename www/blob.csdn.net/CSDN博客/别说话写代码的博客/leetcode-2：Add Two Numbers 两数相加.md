# leetcode-2：Add Two Numbers 两数相加 - 别说话写代码的博客 - CSDN博客





2018年12月01日 15:04:50[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：155
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)









**题目**：

You are given two **non-empty** linked lists representing two non-negative integers. The digits are stored in **reverse order** and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

**Example:**
**Input:** (2 -> 4 -> 3) + (5 -> 6 -> 4)
**Output:** 7 -> 0 -> 8
**Explanation:** 342 + 465 = 807.
给出两个 **非空** 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 **逆序** 的方式存储的，并且它们的每个节点只能存储 **一位** 数字。

如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。

您可以假设除了数字 0 之外，这两个数都不会以 0 开头。

**示例：**
**输入：**(2 -> 4 -> 3) + (5 -> 6 -> 4)
**输出：**7 -> 0 -> 8
**原因：**342 + 465 = 807
**思路**：两个链表逐一 相加，carry用来保存进位，将相加之和保存到新建节点中，直到两个链表都为空。 如果一个为空另一个不为空我们可以令为空的 值为0，一直处理。

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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *res = new ListNode(-1);
        ListNode *cur = res;
        int carry =0,n1=0,n2=0;
        while(l1 || l2)
        {
            if(l1==NULL) n1=0;
            else n1=l1->val;
            if(l2==NULL) n2=0;
            else n2=l2->val;
            int sum = n1 + n2 +carry;
            carry = sum/10;
            cur->next = new ListNode(sum%10);
            cur = cur->next;
            if(l1) l1=l1->next;
            if(l2) l2=l2->next;
        }
        if(carry) cur->next = new ListNode(carry);
        return res->next;
    }
};
```

注意坑：一定要 判断链表是否为空

延伸：如果 两个链表值是 顺序的，求他们 的和，可以先对他们进行逆序。在用这种方法操作。

参考：[http://www.cnblogs.com/grandyang/p/4662599.html](http://www.cnblogs.com/grandyang/p/4662599.html)




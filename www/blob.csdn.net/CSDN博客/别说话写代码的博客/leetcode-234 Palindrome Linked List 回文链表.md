# leetcode-234. Palindrome Linked List 回文链表 - 别说话写代码的博客 - CSDN博客





2019年01月21日 14:54:49[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：52标签：[Palindrome Linked List 																[回文链表																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=回文链表&t=blog)



|Given a singly linked list, determine if it is a palindrome.**Example 1:****Input:** 1->2**Output:** false**Example 2:****Input:** 1->2->2->1**Output:** true**Follow up:**			Could you do it in O(n) time and O(1) space?|请判断一个链表是否为回文链表。**示例 1:****输入:** 1->2**输出:** false**示例 2:****输入:** 1->2->2->1**输出:** true**进阶：**			你能否用 O(n) 时间复杂度和 O(1) 空间复杂度解决此题？|
|----|----|

思路：

**第一种**：用栈，将前半部分 链表放入栈中，与后面逐个比 一下。可以用 快慢指针找出栈的前后半部分

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
    bool isPalindrome(ListNode* head) {
        if(!head || !head->next) return true;
        ListNode * quick=head,*slow=head;
        stack<int> stk;
        stk.push(head->val);
        while(quick->next && quick->next->next)
        {
            slow=slow->next;
            quick=quick->next->next;
            stk.push(slow->val);
        }
        if(!quick->next) stk.pop();   //odd elem
        while(slow->next)
        {
            slow = slow->next;
            if(slow->val != stk.top()) return false;
            else stk.pop();
        }return true;
    }
};
```

**第二种**：找到 前后半部分，把后半部分反转一下就可以了

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
    bool isPalindrome(ListNode* head) {
        if(!head || !head->next) return true;
        ListNode * quick=head,*slow=head;
        while(quick->next && quick->next->next)  //
        {
            slow=slow->next;
            quick=quick->next->next;
        }
        ListNode * last = slow->next,*pre=head;  //slow为中间节点
        while(last->next)    //后半部分元素逆置
        {
            ListNode * pcur = last->next;
            last->next = pcur->next;
            pcur->next= slow->next;
            slow->next= pcur;
        }
        while(slow->next)   //比较
        {
            slow=slow->next;
            if(pre->val != slow->val) return false;
            pre = pre->next;
        }return true;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Palindrome Linked List &t=blog)





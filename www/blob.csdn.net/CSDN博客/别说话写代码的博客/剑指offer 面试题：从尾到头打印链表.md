# 剑指offer 面试题：从尾到头打印链表 - 别说话写代码的博客 - CSDN博客





2018年12月29日 21:59:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：21标签：[剑指offer																[从尾到头打印链表](https://so.csdn.net/so/search/s.do?q=从尾到头打印链表&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：输入一个链表，按链表值从尾到头的顺序返回一个ArrayList。

思路：有多种放法。（1）先反转链表，再打印链表。（2）使用栈。

```cpp
/**
*  struct ListNode {
*        int val;
*        struct ListNode *next;
*        ListNode(int x) :
*              val(x), next(NULL) {
*        }
*  };
*/
class Solution {
public:
    vector<int> printListFromTailToHead(ListNode* head) {
        vector<int> res;
        if(head==nullptr) return {};
        ListNode* phead=new ListNode(-1);
        ListNode* cur=head,*tail=head;
        while(cur!=nullptr)
        {
            tail=cur->next;
            cur->next=phead->next;
            phead->next=cur;
            cur=tail;
        }
        cur=phead->next;
        while(cur!=nullptr)
        {
            res.push_back(cur->val);
            cur=cur->next;
        }
        return res;
    }
};
```

```cpp
/**
*  struct ListNode {
*        int val;
*        struct ListNode *next;
*        ListNode(int x) :
*              val(x), next(NULL) {
*        }
*  };
*/
class Solution {
public:
    vector<int> printListFromTailToHead(ListNode* head) {
        vector<int> res;
        stack<int> stk;
        ListNode * cur=head;
        while(cur)
        {
            stk.push(cur->val);
            cur=cur->next;
        }
        while(stk.size())
        {
            res.push_back(stk.top());
            stk.pop();
        }return res;
    }
};
```







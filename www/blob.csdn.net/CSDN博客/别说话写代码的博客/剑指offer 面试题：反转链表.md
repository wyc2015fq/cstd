# 剑指offer 面试题：反转链表 - 别说话写代码的博客 - CSDN博客





2019年03月09日 10:17:32[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：14标签：[剑指offer																[翻转链表](https://so.csdn.net/so/search/s.do?q=翻转链表&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：输入一个链表，反转链表后，输出新链表的表头。

思路：把每个节点摘下来插入到头结点的位置

```cpp
/*
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};*/
class Solution {
public:
    ListNode* ReverseList(ListNode* pHead) {
        if(!pHead) return nullptr;
        ListNode *head = new ListNode(-1);
        head->next = nullptr;
        ListNode *cur=pHead;
        while(cur)
        {
            ListNode* nxt = cur->next;
            cur->next = head->next;
            head->next = cur;
            cur = nxt;
        }return head->next;
    }
};
```







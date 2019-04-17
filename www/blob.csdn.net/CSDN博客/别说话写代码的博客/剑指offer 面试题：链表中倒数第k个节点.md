# 剑指offer 面试题：链表中倒数第k个节点 - 别说话写代码的博客 - CSDN博客





2019年03月09日 09:48:57[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：9标签：[剑指offer																[链表中倒数第k个节点](https://so.csdn.net/so/search/s.do?q=链表中倒数第k个节点&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：输入一个链表，输出该链表中倒数第k个结点。

思路：让一个指针先走k-1步，然后另一个指针从头开始，两个指针一起走，当第一个指针结尾，第二个指针就到末尾了。

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
    ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) {
        if(k<1 || !pListHead) return nullptr;
        ListNode* p=pListHead,*q=pListHead;
        for(int i=1;i<k;++i)
        {
            if(!p->next) return nullptr;  //这里注意k大于链表长度
            p=p->next;
        }
            
        while(p->next)
        {
            q=q->next;
            p=p->next;
        }return q;
    }
};
```







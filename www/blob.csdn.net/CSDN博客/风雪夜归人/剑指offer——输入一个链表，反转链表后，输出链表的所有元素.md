# 剑指offer——输入一个链表，反转链表后，输出链表的所有元素 - 风雪夜归人 - CSDN博客
2018年05月03日 20:52:27[cdjccio](https://me.csdn.net/qq_34624951)阅读数：163
Q：输入一个链表，反转链表后，输出链表的所有元素。
A：1、先在原链表的头节点前再添加一个头结点；
      2、使用头插法逆置新加入头结点后的所有节点。
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
        if(NULL == pHead || NULL == pHead->next)
        {
            return pHead;
        }
        
        ListNode *p, *q;
        ListNode *tmp = new ListNode(0);
        tmp->next = pHead;
        p = tmp->next;
        tmp->next = NULL;
        
        while(p)
        {
            q = p;
            p = p->next;
            q->next = tmp->next;
            tmp->next = q;
        }
        return q;
    }
};
```

# 剑指offer——输入一个链表，输出该链表中倒数第k个结点 - 风雪夜归人 - CSDN博客
2018年05月03日 19:35:58[cdjccio](https://me.csdn.net/qq_34624951)阅读数：52
Q；输入一个链表，输出该链表中倒数第k个结点。
A：定义两个指针：
　　（1）第一个指针从链表的头指针开始遍历向前走k-1，第二个指针保持不动；
　　（2）从第k步开始，第二个指针也开始从链表的头指针开始遍历；
　　（3）由于两个指针的距离保持在k-1，当第一个（走在前面的）指针到达链表的尾结点时，第二个指针（走在后面的）指针正好是倒数第k个结点。
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
        ListNode* p1 = pListHead;
        ListNode* p2 = pListHead;
        if(NULL == pListHead || k <= 0)
        {
            return NULL;
        }
        
        for(int i = 0; i<k - 1; i++)
        {
            if(p1->next == NULL)
            {
                return NULL;
            }
            else
            {
                p1 = p1->next;
            }
        }
        while(p1->next != NULL)
        {
            p1 = p1->next;
            p2 = p2->next;
        }
        return p2;
    }
};
```

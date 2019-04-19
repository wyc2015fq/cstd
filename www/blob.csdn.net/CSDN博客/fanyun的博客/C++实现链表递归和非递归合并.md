# C++实现链表递归和非递归合并 - fanyun的博客 - CSDN博客
2018年10月30日 07:21:18[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：54
1.使用递归和非递归实现链表的合并
输入两个单调递增的链表，输出两个链表合成后的链表，要求合成后的链表满足单调不减规则。
2.代码实现
```cpp
struct ListNode {
    int val;
    struct ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
/*递归实现合并两个排序链表*/
ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
{
    if(pHead1 == NULL)
        return pHead2;
    if(pHead2 == NULL)
        return pHead1;
    if(pHead1->val < pHead2->val)
    {
        pHead1->next = Merge(pHead1->next, pHead2);
        return pHead1;
    }
    else
    {
        pHead2->next = Merge(pHead1, pHead2->next);
        return pHead2;
    }
}
/*非递归实现合并两个排序链表*/
ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
{
    ListNode *pHead = new ListNode(0);
    ListNode *p = pHead;
    while(pHead1 != NULL && pHead2 != NULL)
    {
        if(pHead1->val < pHead2->val)
        {
            p->next = pHead1;
            pHead1 = pHead1->next;
        }
        else
        {
            p->next = pHead2;
            pHead2 = pHead2->next;
        }
        p = p->next;
    }
    if(pHead1 != NULL)
    {
        p->next = pHead1;
    }
    if(pHead2 != NULL)
    {
        p->next = pHead2;
    }
    return pHead->next;
}
```

# 【剑指offer】题37：链表公共节点 - xiaxzhou的博客 - CSDN博客





2017年07月04日 20:36:27[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：84








```
ListNode* FindFirstCommonNode(ListNode* pHead1, ListNode* pHead2)
{
    if (pHead1==NULL||pHead2==NULL)
    {
        return NULL;
    }
    int k1(0),k2(0);
    ListNode * pCur1(pHead1);
    ListNode * pCur2(pHead2);

    while (pCur1!=NULL)
    {
        k1++;
        pCur1 = pCur1->next;
    }
    while (pCur2 != NULL)
    {
        k2++;
        pCur2 = pCur2->next;
    }
    pCur1 = pHead1;
    pCur2 = pHead2;
    while (k1>k2)
    {
        pCur1 = pCur1->next;
        k1--;
    }
    while (k1 < k2)
    {
        pCur2 = pCur2->next;
        k2--;
    }
    while (pCur1!=pCur2)
    {
        pCur1 = pCur1->next;
        pCur2 = pCur2->next;
    }
    return pCur1;
}
```




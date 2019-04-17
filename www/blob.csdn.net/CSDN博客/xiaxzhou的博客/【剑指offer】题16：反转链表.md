# 【剑指offer】题16：反转链表 - xiaxzhou的博客 - CSDN博客





2017年06月22日 21:41:51[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：132








```
if (pHead == NULL || pHead->next == NULL)
    {
        return pHead;
    }
    ListNode * pPre = NULL;
    ListNode * pCur = pHead;
    ListNode * pNex = pCur->next;
    while (pNex != NULL)
    {
        pCur->next = pPre;
        pPre = pCur;
        pCur = pNex;
        pNex = pNex->next;
    }
    pCur->next = pPre;
    return pCur;
```




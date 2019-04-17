# 【剑指offer】题57：删除链表中的重复的结点 - xiaxzhou的博客 - CSDN博客





2017年07月06日 22:29:28[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：163
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









```
ListNode* deleteDuplication(ListNode* pHead)
{
    if (pHead == NULL || pHead->next == NULL)
    {
        return pHead;
    }
    ListNode dummy(0);//虚拟头结点
    dummy.next = pHead;

    ListNode * pre(&dummy);
    ListNode * pleft, * pright;

    while (pre->next!=NULL)
    {
        pleft = pre->next;
        pright = pleft;
        while (pright!=NULL && pright->val ==pleft->val)
        {
            pright = pright->next;
        }

        if (pleft->next == pright)
        {
            pre = pleft;
        }
        else
        {
            pre->next = pright;
            while (pleft!=pright)
            {
                auto tmp = pleft->next;
                delete pleft;
                pleft = tmp;
            }
        }
    }
    return dummy.next;
}
```




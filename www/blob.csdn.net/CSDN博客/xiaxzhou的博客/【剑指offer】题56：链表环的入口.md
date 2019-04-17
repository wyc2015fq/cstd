# 【剑指offer】题56：链表环的入口 - xiaxzhou的博客 - CSDN博客





2017年07月06日 22:01:46[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：130
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









使用哈希表记录出现过的节点

```cpp
ListNode* EntryNodeOfLoop(ListNode* pHead)
{
    if (pHead==NULL)
    {
        return NULL;
    }
    set<ListNode*> my_set;
    //hash_set<ListNode*> my_set;
    ListNode * curnode(pHead);
    while (1)
    {
        if (my_set.find(curnode)!=my_set.end())
        {
            return curnode;
        }
        my_set.insert(curnode);
        if (curnode == NULL)
        {
            return NULL;
        }
        curnode = curnode->next;
    }

}
```




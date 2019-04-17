# 【剑指offer】题26：复杂链表的复制 - xiaxzhou的博客 - CSDN博客





2017年07月02日 19:47:58[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：235








空间换时间：

```cpp
RandomListNode* Clone(RandomListNode* pHead)
    {
        vector<RandomListNode*> vec;
        vector<int> rand_index;
        RandomListNode * tmp(pHead);
        while(tmp!=NULL)
        {
            vec.push_back(tmp);
            tmp = tmp->next;
        }

        tmp = pHead;
        while (tmp!=NULL)
        {
            rand_index.push_back(find(vec.begin(), vec.end(), tmp->random)-vec.begin());
            tmp = tmp->next;
        }

        vector<RandomListNode*> new_vec;
        for (auto i = 0; i < vec.size();++i)
        {
            RandomListNode* newnode = new RandomListNode(vec[i]->label);
            new_vec.push_back(newnode);
        }


        new_vec.push_back(NULL);
        for (auto i = 0; i < new_vec.size()-1;++i)
        {
            new_vec[i]->next = new_vec[i + 1];
            new_vec[i]->random = new_vec[rand_index[i]];
        }
        return new_vec[0];
    }
```

不用辅助空间的标准解法：

```
void copynode(RandomListNode* pHead)
{
    RandomListNode* curNode(pHead);
    RandomListNode* newnode;

    while (curNode!=NULL)
    {
        newnode = new RandomListNode(curNode->label);
        newnode->next = curNode->next;
        curNode->next = newnode;
        curNode = newnode->next;
    }
}

void copyrandom(RandomListNode* pHead)
{
    RandomListNode* curnode(pHead);
    RandomListNode* newnode(pHead->next);
    while (1)
    {
        if (curnode->random!=NULL)
        {
            newnode->random = curnode->random->next;
        }
        if (newnode->next == NULL)
        {
            break;
        }
        curnode = curnode->next->next;
        newnode = newnode->next->next;
    }
}
RandomListNode* fixnext(RandomListNode* pHead)
{
    RandomListNode* newhead = pHead->next;

    RandomListNode* prenode(pHead);
    RandomListNode* curnode(pHead->next);

    while (1)
    {
        prenode->next = curnode->next;
        prenode = curnode;
        if (curnode->next == NULL)
        {
            break;
        }
        curnode = curnode->next;
    }
    return newhead;
}

RandomListNode* Clone(RandomListNode* pHead)
{
    if (pHead == NULL)
    {
        return NULL;
    }
    copynode(pHead);
    copyrandom(pHead);
    return fixnext(pHead);
}
```




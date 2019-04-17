# 【剑指offer】题15：链表倒数第k个节点 - xiaxzhou的博客 - CSDN博客





2017年06月22日 20:58:24[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：127
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









> 
输入一个链表，输出该链表中倒数第k个结点。


考虑测试样例：
- 链表为空
- 第k个节点为头结点
- 第k个节点为尾结点
- 链表节点数少于k个
- **k==0**

1 **防御性编程**

最简单最实用的防御性编程即在函数入口处验证输入是否合法

2 **虚拟头结点**的使用

```
ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) 
{
    if (pListHead == NULL||k<=0)
    {
        return NULL;
    }

    ListNode virtualnode(0);
    virtualnode.next = pListHead;
    ListNode* left(&virtualnode), *right(&virtualnode);
    while (k!=0 && right!=NULL)
    {
        right = right->next;
        k--;
    }
    if (k!=0||right==NULL)
    {
        return NULL;
    }
    while (right!=NULL)
    {
        right = right->next;
        left = left->next;
    }
    return left;
}
```




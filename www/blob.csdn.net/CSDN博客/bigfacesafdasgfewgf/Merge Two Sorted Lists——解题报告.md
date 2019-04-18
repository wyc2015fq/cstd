# Merge Two Sorted Lists——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月08日 13:01:33[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：883标签：[LeetCode																[链表																[合并																[递归																[指针](https://so.csdn.net/so/search/s.do?q=指针&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)








    【题目】

    Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.




    【分析】

    不要忘记先判断两个链表是否有空链表。其余的使用递归和非递归方式，都可以实现。




    【代码】




    递归方式：



```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* mergedList = NULL; 
        
        if(l1 == NULL)
            return l2;
        if(l2 == NULL)
            return l1;
        
        if(l1->val < l2->val)
        {
            mergedList = l1;
            mergedList->next = mergeTwoLists(l1->next, l2);
        }
        else
        {
            mergedList = l2;
            mergedList->next = mergeTwoLists(l1, l2->next);
        }
        
        return mergedList;
        
    }
};
```



    非递归方式：



```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* mergedList = NULL; 
        
        if(l1 == NULL)
            return l2;
        if(l2 == NULL)
            return l1;
        
        if(l1->val < l2->val)
        {
            mergedList = l1;
            mergedList->next = NULL;
            l1 = l1->next;
        }
        else
        {
            mergedList = l2; 
            mergedList->next = NULL;
            l2 = l2->next;
        }
        
        ListNode* p = mergedList;
        while(l1 != NULL && l2 != NULL)
        {
            if(l1->val < l2->val)
            {
                p->next = l1;
                l1 = l1->next;
                p = p->next;
                p->next = NULL;
            }
            else
            {
                p->next = l2; 
                l2 = l2->next;
                p = p->next;
                p->next = NULL;
            }
        }
        
        if(l1 != NULL)
            p->next = l1; 
        else if(l2 != NULL)
            p->next = l2;
        
        return mergedList;
        
    }
};
```](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=合并&t=blog)](https://so.csdn.net/so/search/s.do?q=链表&t=blog)](https://so.csdn.net/so/search/s.do?q=LeetCode&t=blog)





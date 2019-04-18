# Swap Nodes in Pairs ——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月12日 08:12:38[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：643标签：[LeetCode																[链表																[递归																[反转](https://so.csdn.net/so/search/s.do?q=反转&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)








    【题目】

    Given a linked list, swap every two adjacent nodes and return its head.


For example,

Given 1->2->3->4, you should return the list as 2->1->4->3.


Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed.




    【分析】

    使用递归解决，首先跳出递归的条件一定是链表剩余长度为0或1，即head == NULL，head->next == NULL. 其次，**一定要注意在链表还有剩余节点的情况下倒转两个节点，不然，head->next都为空了，如何反转后面节点呢？具体见下面注释。**




    【代码】

    运行时间4ms






```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        
        if(head == NULL || head->next == NULL)  // 递归跳出条件
            return head;
        ListNode* tmp = swapPairs(head->next->next);  //一定要在前面递归，如果head->next都为空了，递归进去是可以看到的，但是下面直接去head->next就判断不了。
        ListNode* res = head->next;
        res->next = head;
        res->next->next = tmp;
        return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=链表&t=blog)](https://so.csdn.net/so/search/s.do?q=LeetCode&t=blog)





# 剑指Offer - 从尾到头打印链表 - curryche的博客 - CSDN博客





2018年07月31日 21:53:13[curryche](https://me.csdn.net/whwan11)阅读数：28标签：[笔试																[剑指offer](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)](https://so.csdn.net/so/search/s.do?q=笔试&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

从尾到头打印链表 

时间限制：1秒 空间限制：32768K 热度指数：586802 

本题知识点： 链表
题目描述 

输入一个链表，按链表值从尾到头的顺序返回一个ArrayList。

## 解法

代码

```cpp
/**
*  struct ListNode {
*        int val;
*        struct ListNode *next;
*        ListNode(int x) :
*              val(x), next(NULL) {
*        }
*  };
*/
class Solution {
public:
    vector<int> printListFromTailToHead(ListNode* head) {

        vector<int> result,sub_result;
        if(head==NULL)
            return result;

        sub_result=printListFromTailToHead(head->next);
        result.insert(result.end(),sub_result.begin(),sub_result.end());
        result.push_back(head->val);
        return result;

    }
};
```






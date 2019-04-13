
# 24. Swap Nodes in Pairs - OraYang的博客 - CSDN博客

2017年09月12日 11:02:31[OraYang](https://me.csdn.net/u010665216)阅读数：95个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a linked list, swap every two adjacent nodes and return its head.
For example,
Given 1->2->3->4, you should return the list as 2->1->4->3.
Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed.
## 思路
本题是链表操作，每两个指针就互换一次，这里要注意一个小技巧就是，建立一个指针头，这样可以避免处理head的边界情况，这个是真的很重要啊，一开始自己就是因为犯懒，没指针头，导致各种if-else语句来对临界情况判断，超烦的。
## 代码
```python
/**
 * Definition
```
```python
for
```
```python
singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *
```
```python
next
```
```python
;
 *     ListNode(int x) : val(x),
```
```python
next
```
```python
(
```
```python
NULL
```
```python
) {}
 * };
 */
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
```
```python
if
```
```python
(head ==
```
```python
NULL
```
```python
)
```
```python
return
```
```python
NULL
```
```python
;  
    ListNode* helper = new ListNode(
```
```python
0
```
```python
);  
    helper->
```
```python
next
```
```python
= head;  //辅助表头，可以避免处理head的边界情况
    ListNode* pre = helper;  
    ListNode* cur = head;
```
```python
while
```
```python
(cur!=
```
```python
NULL
```
```python
&& cur->
```
```python
next
```
```python
!=
```
```python
NULL
```
```python
)  
    {  
        ListNode*
```
```python
next
```
```python
= cur->
```
```python
next
```
```python
->
```
```python
next
```
```python
;  
        cur->
```
```python
next
```
```python
->
```
```python
next
```
```python
= cur;  
        pre->
```
```python
next
```
```python
= cur->
```
```python
next
```
```python
;
```
```python
if
```
```python
(
```
```python
next
```
```python
!=
```
```python
NULL
```
```python
&&
```
```python
next
```
```python
->
```
```python
next
```
```python
!=
```
```python
NULL
```
```python
)  
            cur->
```
```python
next
```
```python
=
```
```python
next
```
```python
->
```
```python
next
```
```python
;
```
```python
else
```
```python
cur->
```
```python
next
```
```python
=
```
```python
next
```
```python
;  
        pre = cur;  
        cur =
```
```python
next
```
```python
;  
    }
```
```python
return
```
```python
helper->
```
```python
next
```
```python
;  
    }
};
```


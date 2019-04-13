
# 19. Remove Nth Node From End of List - OraYang的博客 - CSDN博客

2017年09月12日 11:19:16[OraYang](https://me.csdn.net/u010665216)阅读数：148个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a linked list, remove the nth node from the end of list and return its head.
For example,
Given linked list: 1->2->3->4->5, and n = 2.
After removing the second node from the end, the linked list becomes 1->2->3->5.
Note:
Given n will always be valid.
Try to do this in one pass.
## 思路
本题和上一道题目同是链表操作的题目，但是明显比上一掉题目[Swap Nodes in Pairs](http://blog.csdn.net/u010665216/article/details/77942807)简单，但是小技巧：建立指针头都是不变滴~。具体思路如下：
遍历链表，获得结点个数num，根据n找到需要删除的结点，将要删除结点的前一个结点指向要删除的结点的后一个结点。
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
 *
```
```python
int
```
```python
val;
 *     ListNode *
```
```python
next
```
```python
;
 *     ListNode(
```
```python
int
```
```python
x) : val(x),
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
```
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
    ListNode* removeNthFromEnd(ListNode* head,
```
```python
int
```
```python
n) {
        ListNode* Head = head;
        ListNode* helper =
```
```python
new
```
```python
ListNode(
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
= head;
        ListNode* res = helper;
```
```python
int
```
```python
num=
```
```python
0
```
```python
;
```
```python
while
```
```python
(head)
        {
            num++;
            head = head->
```
```python
next
```
```python
;
        }
```
```python
int
```
```python
diff = num -n;
```
```python
while
```
```python
(diff)
        {
            diff--;
            helper = helper->
```
```python
next
```
```python
;
            Head = Head->
```
```python
next
```
```python
;
        }
        helper->
```
```python
next
```
```python
= Head->
```
```python
next
```
```python
;
        return res->
```
```python
next
```
```python
;
    }
};
```


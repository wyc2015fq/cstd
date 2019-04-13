
# 86. Partition List - OraYang的博客 - CSDN博客

2017年10月30日 16:46:00[OraYang](https://me.csdn.net/u010665216)阅读数：468标签：[list																](https://so.csdn.net/so/search/s.do?q=list&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a linked list and a value x, partition it such that all nodes less than x come before nodes greater than or equal to x.
You should preserve the original relative order of the nodes in each of the two partitions.
For example,
Given 1->4->3->2->5->2 and x = 3,
return 1->2->2->4->3->5.
## 思路
本题没有必要在原链表上进行插入删除操作，可以额外建立两个链表，比x小的为一边，反之在另一边
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
    ListNode* partition(ListNode* head,
```
```python
int
```
```python
x) {
    ListNode node1(
```
```python
0
```
```python
), node2(
```
```python
0
```
```python
);
    ListNode *p1 = &node1, *p2 = &node2;
```
```python
while
```
```python
(head) {
```
```python
if
```
```python
(head->val < x)
            p1 = p1->
```
```python
next
```
```python
= head;
```
```python
else
```
```python
p2 = p2->
```
```python
next
```
```python
= head;
        head = head->
```
```python
next
```
```python
;
    }
    p2->
```
```python
next
```
```python
=
```
```python
NULL
```
```python
;
    p1->
```
```python
next
```
```python
= node2.
```
```python
next
```
```python
;
    return node1.
```
```python
next
```
```python
;
    }
};
```


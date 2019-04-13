
# 61. Rotate List - OraYang的博客 - CSDN博客

2017年09月21日 12:10:25[OraYang](https://me.csdn.net/u010665216)阅读数：297个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a list, rotate the list to the right by k places, where k is non-negative.
For example:
Given 1->2->3->4->5->NULL and k = 2,
return 4->5->1->2->3->NULL.
## 思路
本题，右旋，比较简单，考察了指针的操作
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
    ListNode* rotateRight(ListNode* head,
```
```python
int
```
```python
k) {
```
```python
if
```
```python
(head==
```
```python
NULL
```
```python
)
            return head;
        ListNode* Head =
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
        Head->
```
```python
next
```
```python
= head;
        ListNode* headcopy = head;
```
```python
int
```
```python
count =
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
(headcopy->
```
```python
next
```
```python
)
        {
            count++;
            headcopy = headcopy->
```
```python
next
```
```python
;
        }
        count++;
        k %=count;
```
```python
if
```
```python
(k==
```
```python
0
```
```python
)
            return head;
        count -= k;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
2
```
```python
;i<=count;i++)
        {
            head = head->
```
```python
next
```
```python
;
        }
        ListNode* temp = Head->
```
```python
next
```
```python
;
        Head->
```
```python
next
```
```python
= head->
```
```python
next
```
```python
;
        head->
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
        headcopy->
```
```python
next
```
```python
= temp;
        return Head->
```
```python
next
```
```python
;      
    }
};
```


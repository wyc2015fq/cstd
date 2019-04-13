
# 82. Remove Duplicates from Sorted List II - OraYang的博客 - CSDN博客

2017年10月30日 16:32:15[OraYang](https://me.csdn.net/u010665216)阅读数：560所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a sorted linked list, delete all nodes that have duplicate numbers, leaving only distinct numbers from the original list.
For example,
Given 1->2->3->3->4->4->5, return 1->2->5.
Given 1->1->1->2->3, return 2->3.
## 思路
本题采用递归的思路，代码清晰简单
## 代码
```python
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
```
```python
class
```
```python
Solution
```
```python
{
```
```python
public:
    ListNode* deleteDuplicates(ListNode* head) {
```
```python
if
```
```python
(!head)
```
```python
return
```
```python
0
```
```python
;
```
```python
if
```
```python
(!head->next)
```
```python
return
```
```python
head;
        int
```
```python
val
```
```python
= head->
```
```python
val
```
```python
;
        ListNode* p = head->next;
```
```python
if
```
```python
(p->
```
```python
val
```
```python
!=
```
```python
val
```
```python
) {
            head->next = deleteDuplicates(p);
```
```python
return
```
```python
head;
        }
```
```python
else
```
```python
{
```
```python
while
```
```python
(p && p->
```
```python
val
```
```python
==
```
```python
val
```
```python
) p = p->next;
```
```python
return
```
```python
deleteDuplicates(p);
        }
    }
};
```


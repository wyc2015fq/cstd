
# 【LeetCode】237. Delete Node in a Linked List（链表） - jiange_zh的博客 - CSDN博客


2016年01月07日 11:55:39[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：565标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[链表																](https://so.csdn.net/so/search/s.do?q=链表&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[算法与数据结构																](https://blog.csdn.net/jiange_zh/article/category/5787591)
所属专栏：[C++后台开发面试题](https://blog.csdn.net/column/details/cppinterview.html)



## Question
Write a function to delete a node (except the tail) in a singly linked list, given only access to that node.
Supposed the linked list is 1 -> 2 -> 3 -> 4 and you are given the third node with value 3, the linked list should become 1 -> 2 -> 4 after calling your function.
## Analyze
思路：用当前节点的下一个节点覆盖掉当前节点，之后释放下一个节点的内存空间。
这个方案不是直接的删除该节点，而需要间接来做。
因为，如果想直接删除该节点，由于上一个节点的next指向当前节点，我们需要把上一个节点的next修改为当前节点的next。然而我们并无法访问到上一个节点，也就无法对其next进行修改。故直接删除是不可行的。
也就是说，对于A->B->C->null，表面上我们是删除了B节点，实际上释放的是C节点的空间。
## My code
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
class Solution {
```
```python
public
```
```python
:
```
```python
void
```
```python
deleteNode
```
```python
(ListNode* node) {
```
```python
if
```
```python
(node) {
```
```python
//move the next node to current position,then delete the next node
```
```python
//with this, we can delete the current node
```
```python
ListNode* nxt = node->next;
            node->val = nxt->val;
            node->next = nxt->next;
            delete nxt;
        }
    }
};
```
## More
本题目不考虑尾节点的删除，同时由于题目的特殊背景，也并不涉及删除头节点引发的问题。
如果更一般性的，我们应该注意一些边界问题：
空节点；
尾节点；
头节点的删除可能会影响到head指针



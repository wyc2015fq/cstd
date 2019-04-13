
# 138. Copy List with Random Pointer - OraYang的博客 - CSDN博客

2018年01月13日 11:26:20[OraYang](https://me.csdn.net/u010665216)阅读数：95标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[hashtable																](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 题目
A linked list is given such that each node contains an additional random pointer which could point to any node in the list or null.
Return a deep copy of the list.
# 思路
这道题目是要复制一个副本，这里必须要保证两个链表不占用同一个空间，意思就是要新开辟一个空间，因此我们采用hashmap的思路，来存储对应node之间的关系。
# 代码
```python
/**
 * Definition for singly-linked list with a random pointer.
 * struct RandomListNode {
 *     int label;
 *     RandomListNode *next, *random;
 *     RandomListNode(int x) : label(x), next(NULL), random(NULL) {}
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
    RandomListNode *copyRandomList(RandomListNode *head) {
```
```python
if
```
```python
(head==NULL)
```
```python
return
```
```python
NULL;
```
```python
unordered_map
```
```python
<RandomListNode*,RandomListNode*>
```
```python
mp;
        RandomListNode* node = head;
```
```python
//loop1:copy and construct
```
```python
while
```
```python
(node)
        {
            mp[node] =
```
```python
new
```
```python
RandomListNode(node->label);
            node = node->next;
        }
        node = head;
```
```python
//loop2:assigh node
```
```python
while
```
```python
(node)
        {
            mp[node]->next = mp[node->next];
            mp[node]->random = mp[node->random];
            node = node->next;
        }
```
```python
return
```
```python
mp[head];
    }
};
```


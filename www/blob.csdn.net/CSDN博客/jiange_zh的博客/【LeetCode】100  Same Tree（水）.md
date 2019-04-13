
# 【LeetCode】100. Same Tree（水） - jiange_zh的博客 - CSDN博客


2016年01月08日 09:28:13[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：458标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[算法与数据结构																](https://blog.csdn.net/jiange_zh/article/category/5787591)



## Question
Given two binary trees, write a function to check if they are equal or not.
Two binary trees are considered equal if they are structurally identical and the nodes have the same value.
## Code
```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
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
    bool isSameTree(TreeNode* p, TreeNode* q) {
```
```python
if
```
```python
(!p && !q)
```
```python
return
```
```python
true
```
```python
;
```
```python
else
```
```python
if
```
```python
(!p || !q)
```
```python
return
```
```python
false
```
```python
;
```
```python
else
```
```python
return
```
```python
(p->
```
```python
val
```
```python
== q->
```
```python
val
```
```python
&& isSameTree(p->left, q->left) && isSameTree(p->right, q->right));
    }
};
```
更简洁的Answer：
```python
bool isSameTree(TreeNode
```
```python
*
```
```python
p, TreeNode
```
```python
*
```
```python
q) {
```
```python
if
```
```python
(
```
```python
!
```
```python
p
```
```python
||
```
```python
!
```
```python
q)
```
```python
return
```
```python
q
```
```python
==
```
```python
p;
```
```python
return
```
```python
p
```
```python
->
```
```python
val
```
```python
==
```
```python
q
```
```python
->
```
```python
val
```
```python
&&
```
```python
isSameTree(p
```
```python
->
```
```python
left, q
```
```python
->
```
```python
left)
```
```python
&&
```
```python
isSameTree(p
```
```python
->
```
```python
right, q
```
```python
->
```
```python
right);
}
```


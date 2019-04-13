
# 【LeetCode】226. Invert Binary Tree - jiange_zh的博客 - CSDN博客


2016年01月07日 20:44:09[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：309标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[算法与数据结构																](https://blog.csdn.net/jiange_zh/article/category/5787591)



## Question
Invert a binary tree.
```python
4
   /
```
```python
\
```
```python
2     7
 /
```
```python
\
```
```python
/
```
```python
\
```
```python
1   3 6   9
```
to
```python
4
   /
```
```python
\
```
```python
7     2
 /
```
```python
\
```
```python
/
```
```python
\
```
```python
9   6 3   1
```
## Code
BFS:
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
Solution {
```
```python
public
```
```python
:
    TreeNode* invertTree(TreeNode* root) {
```
```python
if
```
```python
(root==NULL)
```
```python
return
```
```python
root;
```
```python
queue
```
```python
<TreeNode*>
```
```python
q;
        q.push(root);
```
```python
while
```
```python
(!q.empty()) {
            TreeNode* cur = q.front();
            q.pop();
            TreeNode* tmp = cur->left;
            cur->left = cur->right;
            cur->right = tmp;
```
```python
if
```
```python
(cur->left)  q.push(cur->left);
```
```python
if
```
```python
(cur->right) q.push(cur->right);
        }
```
```python
return
```
```python
root;
    }
};
```
There is a simple recursion solution:
```python
TreeNode
```
```python
*
```
```python
invertTree(TreeNode
```
```python
*
```
```python
root) {
```
```python
if
```
```python
(root
```
```python
==
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
    TreeNode
```
```python
*
```
```python
tmp
```
```python
=
```
```python
root
```
```python
->
```
```python
left;
    root
```
```python
->
```
```python
left
```
```python
=
```
```python
invertTree(root
```
```python
->
```
```python
right);
    root
```
```python
->
```
```python
right
```
```python
=
```
```python
invertTree(tmp);
```
```python
return
```
```python
root;
}
```
## Note
Be careful for the NULL pointer！
When you use a pointer, please make sure that it is availble!


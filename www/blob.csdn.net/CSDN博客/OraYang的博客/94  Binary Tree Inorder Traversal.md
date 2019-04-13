
# 94. Binary Tree Inorder Traversal - OraYang的博客 - CSDN博客

2018年01月13日 09:59:28[OraYang](https://me.csdn.net/u010665216)阅读数：70标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[hashtable																](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 题目描述
Given a binary tree, return the*inorder*traversal of its nodes’ values.
For example:
Given binary tree`[1,null,2,3]`,
1
    \
     2
    /
   3
return`[1,3,2]`.
**Note:**Recursive solution is trivial, could you do it iteratively?

# 思路
本题的目的是将一个二叉树结构进行中序遍历输出
**递归**的思想就是：先左节点，再根节点，最后右节点
在这里给大家介绍下，编写递归代码时最重要的三点：
> 递归总有一个最简单的情况——方法的第一条语句总是一个包含return的条件语句。

> 递归调用总是去尝试解决一个规模更小的子问题，这样递归才能收敛到最简单的情况。

> 递归调用的父问题和尝试解决的子问题之间不应该有交集。
**迭代**的思想就是：利用一个栈，两个嵌套循环，先遍历左节点，压栈，再出栈，遍历右节点。
迭代需要注意的问题是：
迭代的每一次过程都是输出作为输入：
```python
root
```
```python
=
```
```python
root->left;
```
# 代码
## 递归
```python
//recursive solution
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
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
inorderTraversal(TreeNode* root) {
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
res;
        inorder(res,root);
```
```python
return
```
```python
res;

    }
```
```python
private
```
```python
:
```
```python
void
```
```python
inorder(
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
& res,TreeNode* root){
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
;
        inorder(res,root->left);
        res.push_back(root->val);
        inorder(res,root->right);
    }
};
```
## 迭代思路
```python
// iteratively
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
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
inorderTraversal(TreeNode* root) {
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
res;
```
```python
stack
```
```python
<TreeNode*>
```
```python
treeroot;
```
```python
while
```
```python
(root!=NULL||!treeroot.empty())
        {
```
```python
while
```
```python
(root!=NULL)
            {
                treeroot.push(root);
                root=root->left;
            }
            root = treeroot.top();
            treeroot.pop();
            res.push_back(root->val);
            root = root->right;
        }
```
```python
return
```
```python
res;
    }
};
```


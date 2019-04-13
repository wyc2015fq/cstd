
# 106. Construct Binary Tree from Inorder and Postorder Traversal - OraYang的博客 - CSDN博客

2017年12月18日 10:00:30[OraYang](https://me.csdn.net/u010665216)阅读数：106标签：[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given inorder and postorder traversal of a tree, construct the binary tree.
Note:
You may assume that duplicates do not exist in the tree.
# 思路
本题思路和我前面一篇结题报告：[105. Construct Binary Tree from Preorder and Inorder Traversal](http://blog.csdn.net/u010665216/article/details/78701038)思路一致。可以说是没有差别。这里就不细致分析了
# 代码
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
    TreeNode* buildTree(
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
& inorder,
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
& postorder) {
```
```python
//step1:construct hash-tab
```
```python
if
```
```python
(postorder.size()==
```
```python
0
```
```python
)
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
<
```
```python
int
```
```python
,
```
```python
int
```
```python
>
```
```python
mapIndex;
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
0
```
```python
;i<inorder.size();i++)
            mapIndex[inorder[i]] = i;
```
```python
return
```
```python
helpTree(postorder,
```
```python
0
```
```python
,inorder.size(),
```
```python
0
```
```python
,mapIndex);
    }
```
```python
private
```
```python
:
    TreeNode* helpTree(
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
& postorder,
```
```python
int
```
```python
start,
```
```python
int
```
```python
len,
```
```python
int
```
```python
offest,
```
```python
unordered_map
```
```python
<
```
```python
int
```
```python
,
```
```python
int
```
```python
>
```
```python
& mapIndex)
    {
```
```python
if
```
```python
(len<=
```
```python
0
```
```python
)
```
```python
return
```
```python
NULL;
```
```python
int
```
```python
rootval = postorder[len-
```
```python
1
```
```python
+start];
```
```python
//vist root
```
```python
int
```
```python
i = mapIndex[rootval]-offest;
```
```python
//compute len of left tree
```
```python
TreeNode* root =
```
```python
new
```
```python
TreeNode(rootval);
        root->left = helpTree(postorder,start,i,offest,mapIndex);
```
```python
//construct left subtree
```
```python
root->right = helpTree(postorder,start+i,len-i-
```
```python
1
```
```python
,offest+i+
```
```python
1
```
```python
,mapIndex);
```
```python
//construct right subtree
```
```python
return
```
```python
root;
    }
};
```


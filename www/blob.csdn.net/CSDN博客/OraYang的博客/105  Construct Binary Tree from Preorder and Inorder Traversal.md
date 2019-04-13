
# 105. Construct Binary Tree from Preorder and Inorder Traversal - OraYang的博客 - CSDN博客

2017年12月03日 13:07:40[OraYang](https://me.csdn.net/u010665216)阅读数：289标签：[preorder																](https://so.csdn.net/so/search/s.do?q=preorder&t=blog)[inorder																](https://so.csdn.net/so/search/s.do?q=inorder&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=inorder&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=inorder&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=inorder&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=preorder&t=blog)



## 题目
Given preorder and inorder traversal of a tree, construct the binary tree.
Note:
You may assume that duplicates do not exist in the tree.
## 思路
这道题目，翻译一下就是：如何根据先序和中序遍历的数组，来构造二叉树。其中Note部分说，假设不存在重复项。为什么这样说呢？因为如果存在重复项，那么构造出来的树就不是唯一的。
比如说数组{7，7}就有两种树形结构：
```python
7
```
```python
7
```
```python
/
```
```python
or
```
```python
\
```
```python
7
```
```python
7
```
那么接下来我们来举个例子：存在先序遍历数组、中序遍历数组如下所示：
```python
preorder =
```
```python
{7,10,4,3,1,2,8,11}
```
```python
inorder =
```
```python
{4,10,3,1,7,11,8,2}
```
观察数组构造，我们会发现中序遍历中的根节点必然是先序遍历的第一个元素。我们根据先序遍历中的第一个元素7找到根节点，然后在中序遍历数组中找到7的索引是4，然后我们发现索引4的左边是左子树，右边是右子树。然后我们继续根据左子树的根节点10找到中虚遍历中的索引位置1，然后又发现了索引1的左边{1}是左子树，索引1的右边{3,1}是右子树。这样我们就能用递归的方法构造出对应的二叉树。
在上述思路中，我们根据先序遍历数组里的根节点找到中序遍历数组里的索引，这个索引的查找，如果是顺序查找，最长时间复杂度是$O(n)$，如果该树是平衡二叉树，那么平均时间复杂度是$O(logn)$。
因此为了缩减构造树的时间复杂度，我们可以通过建立哈希表，这样查找索引的时间就是$O(1)$。因此构造二叉树的时间就是$O(N)$，线性时间复杂度！
## 代码
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
& preorder,
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
& inorder) {
```
```python
//step1:construct hash-tab
```
```python
if
```
```python
(preorder.size()==
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
helpTree(preorder,
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
& preorder,
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
rootval = preorder[start];
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
        start++;
        root->left = helpTree(preorder,start,i,offest,mapIndex);
```
```python
//construct left subtree
```
```python
root->right = helpTree(preorder,start+i,len-i-
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


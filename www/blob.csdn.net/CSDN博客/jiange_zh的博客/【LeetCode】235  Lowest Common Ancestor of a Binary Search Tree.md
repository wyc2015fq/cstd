
# 【LeetCode】235. Lowest Common Ancestor of a Binary Search Tree - jiange_zh的博客 - CSDN博客


2016年01月09日 17:43:39[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：372标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[算法与数据结构																](https://blog.csdn.net/jiange_zh/article/category/5787591)



## Question
Given a binary search tree (BST), find the lowest common ancestor (LCA) of two given nodes in the BST.
According to the definition of LCA on Wikipedia: “The lowest common ancestor is defined between two nodes v and w as the lowest node in T that has both v and w as descendants (where we allow a node to be a descendant of itself).”
```python
_______6______
       /
```
```python
\
```
```python
___2__          ___8__
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
0      _4       7       9
         /
```
```python
\
```
```python
3   5
```
For example, the lowest common ancestor (LCA) of nodes 2 and 8 is 6. Another example is LCA of nodes 2 and 4 is 2, since a node can be a descendant of itself according to the LCA definition.
## Solution
一开始脑抽了，没有看到是查找树，以为是一般的树，所以复杂了一点，下面是对所有树都通用的。
思路是：
找到第一个目标节点A；
以该目标节点为根dfs查找第二个目标节点B；如果找得到，说明两者的LCA是节点A，否则不做其他事，返回上一层；
如果LCA不是节点A，由于A是找到的第一个目标节点，按照我们的搜索顺序，第二个目标节点B必然在A的某个祖先节点的右子树上。故对这些祖先节点，即满足dfs(root->left, val,  val2, ans)的节点，调用dfsr(root, val2)查找结点B即可。由递归的性质，我们可以保证第一个满足条件的节点必然为LCA。
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
class Solution {
```
```python
public
```
```python
:
    TreeNode*
```
```python
lowestCommonAncestor
```
```python
(TreeNode* root, TreeNode* p, TreeNode* q) {
        TreeNode* ans = NULL;
        TreeNode* v1 = p;
```
```python
//Be careful don't write like this:TreeNode* v1 = p, v2 = q;
```
```python
TreeNode* v2 = q;
```
```python
//You could write like this:TreeNode *v1 = p, *v2 = q;
```
```python
dfs(root, v1, v2, &ans);
```
```python
return
```
```python
ans;
    }
```
```python
private
```
```python
:
    //Find the first value
    bool
```
```python
dfs
```
```python
(TreeNode* root, TreeNode* &v1, TreeNode* &v2, TreeNode** ans) {
```
```python
if
```
```python
(!root)
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
if
```
```python
(root == v1 || root == v2) {
```
```python
if
```
```python
(root == v2) {
```
```python
//Let val2 be the second value
```
```python
TreeNode* tmp = v1;
                v1 = v2;
                v2 = tmp;
            }
```
```python
if
```
```python
(dfsr(root, v2)) *ans = root;
```
```python
//if current node is LCA
```
```python
return
```
```python
true
```
```python
;
        }
```
```python
if
```
```python
(dfs(root->left, v1,  v2, ans)) {
```
```python
//the second node must be in the right-subtree of one of ancestor of the first node
```
```python
if
```
```python
(!(*ans) && dfsr(root, v2)) *ans = root;
```
```python
return
```
```python
true
```
```python
;
        }
```
```python
if
```
```python
(dfs(root->right, v1, v2, ans))
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
return
```
```python
false
```
```python
;
    }
    bool dfsr(TreeNode* root, TreeNode* v) {
```
```python
if
```
```python
(!root)
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
if
```
```python
(root == v)
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
if
```
```python
(dfsr(root->left, v))
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
if
```
```python
(dfsr(root->right, v))
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
return
```
```python
false
```
```python
;
    }
};
```
如果是二叉查找树，那么就简单很多，只需要将A,B值分别与当前节点比较，如果在同一边，则可以递归下去，如果不在同一边，说明当前节点已经是LCA了。
```python
TreeNode
```
```python
*
```
```python
lowestCommonAncestor(TreeNode
```
```python
*
```
```python
root, TreeNode
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
root)
```
```python
return
```
```python
root;
```
```python
if
```
```python
((root
```
```python
->
```
```python
val
```
```python
-
```
```python
p
```
```python
->
```
```python
val)
```
```python
*
```
```python
(root
```
```python
->
```
```python
val
```
```python
-
```
```python
q
```
```python
->
```
```python
val)
```
```python
<=
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
root;
```
```python
if
```
```python
((root
```
```python
->
```
```python
val
```
```python
-
```
```python
p
```
```python
->
```
```python
val)
```
```python
>
```
```python
0
```
```python
&&
```
```python
(root
```
```python
->
```
```python
val
```
```python
-
```
```python
q
```
```python
->
```
```python
val)
```
```python
>
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
lowestCommonAncestor(root
```
```python
->
```
```python
left,p,q);
```
```python
if
```
```python
((root
```
```python
->
```
```python
val
```
```python
-
```
```python
p
```
```python
->
```
```python
val)
```
```python
<
```
```python
0
```
```python
&&
```
```python
(root
```
```python
->
```
```python
val
```
```python
-
```
```python
q
```
```python
->
```
```python
val)
```
```python
<
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
lowestCommonAncestor(root
```
```python
->
```
```python
right,p,q);
    }
```
Note：看题仔细，不要脑抽！不过作为练习，考虑一般情况是可以的。
意外收获：
发现236. Lowest Common Ancestor of a Binary Tree
是本题的进阶版，一般情况下的树~直接提交啦~


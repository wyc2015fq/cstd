
# 572. Subtree of Another Tree - OraYang的博客 - CSDN博客

2017年08月27日 13:18:48[OraYang](https://me.csdn.net/u010665216)阅读数：170标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given two non-empty binary treessandt, check whether treethas exactly the same structure and node values with a subtree ofs.
 A subtree ofsis a tree consists of a node insand all of this node's descendants. The treescould also be considered as a subtree of itself.
Example 1:
Given tree s:
3
    / \
   4   5
  / \
 1   2Given tree t:4 
  / \
 1   2Returntrue,
 because t has the same structure and node values with a subtree of s.
Example 2:
Given tree s:
3
    / \
   4   5
  / \
 1   2
    /
   0Given tree t:4
  / \
 1   2Returnfalse.
思路：
本题采用递归的思路，遍历s的每个结点，判断以当前结点为根结点的树是否与待比较的树一样，一样返回true,否则返回false
代码：

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
class Solution {
public:
    bool isSubtree(TreeNode* s, TreeNode* t) {
        if (!s) return false;
        if (isSame(s, t)) return true;
        return isSubtree(s->left, t) || isSubtree(s->right, t);
    }
    bool isSame(TreeNode* s, TreeNode* t) {
        if (!s && !t) return true;
        if (!s || !t) return false;
        if (s->val != t->val) return false;
        return isSame(s->left, t->left) && isSame(s->right, t->right);
    }
};
```



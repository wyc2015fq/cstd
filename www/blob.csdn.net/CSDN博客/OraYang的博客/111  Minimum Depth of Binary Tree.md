
# 111. Minimum Depth of Binary Tree - OraYang的博客 - CSDN博客

2017年07月23日 09:46:23[OraYang](https://me.csdn.net/u010665216)阅读数：111所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary tree, find its minimum depth.
The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.
思路：本题思路递归，为了求最小值，那么在递归返回时只要返回左右子树的的较小值即可，但是要注意特殊情况即存在左或右子树不存在的情况，此时递归返回值要返回左右子树较大的值。代码很简单如下：
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
    int minDepth(TreeNode* root) {
        if(root==NULL)
            return 0;
        int leftDepth = minDepth(root->left);
        int rightDepth = minDepth(root->right);
        if(rightDepth==0||leftDepth==0)
            return max(leftDepth,rightDepth)+1;
        else
            return min(leftDepth,rightDepth)+1;
    }
};
```



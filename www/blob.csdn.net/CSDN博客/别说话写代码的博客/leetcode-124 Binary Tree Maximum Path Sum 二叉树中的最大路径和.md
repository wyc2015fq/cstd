# leetcode-124. Binary Tree Maximum Path Sum 二叉树中的最大路径和 - 别说话写代码的博客 - CSDN博客





2019年03月14日 14:55:26[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：20标签：[Binary Tree Maximum Path Sum																[二叉树中的最大路径和																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=二叉树中的最大路径和&t=blog)



|Given a **non-empty** binary tree, find the maximum path sum.For this problem, a path is defined as any sequence of nodes from some starting node to any node in the tree along the parent-child connections. The path must contain **at least one node** and does not need to go through the root.**Example 1:****Input:** [1,2,3]       **1****/ \****2****3****Output:** 6**Example 2:****Input:** [-10,9,20,null,null,15,7]   -10   / \  9  **20****/  \****15   7****Output:** 42|给定一个**非空**二叉树，返回其最大路径和。本题中，路径被定义为一条从树中任意节点出发，达到任意节点的序列。该路径**至少包含一个**节点，且不一定经过根节点。**示例 1:****输入:** [1,2,3]       **1****/ \****2****3****输出:** 6**示例 2:****输入:** [-10,9,20,null,null,15,7]   -10   / \  9  **20****/  \****15   7****输出:** 42|
|----|----|

思路：参考[http://www.cnblogs.com/grandyang/p/4280120.html](http://www.cnblogs.com/grandyang/p/4280120.html)

```cpp
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
    int maxPathSum(TreeNode* root) {
        int res=INT_MIN;
        trival(root,res);
        return res;
    }
    int trival(TreeNode* root,int &res)
    {
        if(!root) return 0;
        int left = max(trival(root->left,res),0); //获取左子树最大值，小于0不取
        int right= max(trival(root->right,res),0);  
        res = max(res,left+right+root->val);   //res为遍历的所有结果的最大值
        return root->val + max(left,right);   //返回值定义为以当前节点为终点的path之和
    }
};
```](https://so.csdn.net/so/search/s.do?q=Binary Tree Maximum Path Sum&t=blog)





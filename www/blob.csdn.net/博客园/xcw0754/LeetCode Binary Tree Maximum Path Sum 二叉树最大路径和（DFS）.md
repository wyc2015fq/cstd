# LeetCode  Binary Tree Maximum Path Sum  二叉树最大路径和（DFS） - xcw0754 - 博客园
# [LeetCode  Binary Tree Maximum Path Sum  二叉树最大路径和（DFS）](https://www.cnblogs.com/xcw0754/p/4685090.html)
题意：给一棵二叉树，要求找出任意两个节点（也可以只是一个点）的最大路径和，至少1个节点，返回路径和。（点权有负的。）
思路：DFS解决，返回值是，经过从某后代节点上来到当前节点且路径和最大的值。要注意如果子树传来的如果是负值，是可以同时丢弃的，但至少要将当前节点的val更新答案。
```
1 /**
 2  * Definition for a binary tree node.
 3  * struct TreeNode {
 4  *     int val;
 5  *     TreeNode *left;
 6  *     TreeNode *right;
 7  *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 8  * };
 9  */
10 class Solution {
11 public:
12     int maxPathSum(TreeNode* root) {
13         ans=-2147483648;
14         DFS(root);
15         return ans;
16     }
17 private:
18     int ans;
19     int DFS(TreeNode* t) {
20         if(!t)  return 0;
21         int a=max(0,DFS(t->left));  //小于0的直接丢弃
22         int b=max(0,DFS(t->right));
23         ans=max(ans, a+b+t->val);   //更新，必须以当前节点为中转
24         return max(a, b)+t->val;    //只能返回一条到叶子的路径
25     }
26 };
AC代码
```


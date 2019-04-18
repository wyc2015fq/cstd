# LeetCode Maximum Depth of Binary Tree (求树的深度) - xcw0754 - 博客园
# [LeetCode Maximum Depth of Binary Tree (求树的深度)](https://www.cnblogs.com/xcw0754/p/4638361.html)
题意：给一棵二叉树，求其深度。
思路：递归比较简洁，先求左子树深度，再求右子树深度，比较其结果，返回：max_one+1。
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
12     int maxDepth(TreeNode* root) {
13         if(!root)   return 0;
14         int a=maxDepth(root->left);
15         int b=maxDepth(root->right);
16         return a>b?a+1:b+1;
17     }
18 };
AC代码
```


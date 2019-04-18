# LeetCode  Sum Root to Leaf Numbers（DFS） - xcw0754 - 博客园
# [LeetCode  Sum Root to Leaf Numbers（DFS）](https://www.cnblogs.com/xcw0754/p/4970166.html)
题意：
　　给一棵二叉树，每个节点上有一个数字，范围是0～9，将从根到叶子的所有数字作为一个串，求所有串的和。
思路：
　　普通常规的DFS。
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
12     int sumNumbers(TreeNode* root) {
13         if(root==NULL)    return 0; 
14         return DFS(root,0);
15     }
16     int DFS(TreeNode* t,int sum)
17     {
18         sum=sum*10+t->val;
19         if(!t->left && !t->right)    return sum;
20         int left=0, right=0;
21         if(t->left)        left=DFS(t->left,sum);
22         if(t->right)    right=DFS(t->right,sum);
23         return left+right;
24     }
25 };
AC代码
```


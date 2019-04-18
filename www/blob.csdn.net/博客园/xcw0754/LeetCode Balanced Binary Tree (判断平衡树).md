# LeetCode Balanced Binary Tree (判断平衡树) - xcw0754 - 博客园
# [LeetCode Balanced Binary Tree (判断平衡树)](https://www.cnblogs.com/xcw0754/p/4638441.html)
题意：如题，平衡树是指任意一个节点（除了叶子），其左子树的高度与右子树的高度相差不超过1。
思路：递归解决，但是提供的函数不满足递归的要求啊，我们至少得知道高度，又得返回真假，所以另开个函数解决。
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
12     bool ans;//答案会记录在此。
13     int DFS(TreeNode*  root)
14     {
15         if(!root || !ans)   return 0;  //已经错了，不必要再比较下去
16         int b=DFS(root->right);        
17         int a=DFS(root->left);
18         if(abs(a - b)>1)  ans=false;
19         return max(a,b)+1;
20     }
21 
22     bool isBalanced(TreeNode* root) {
23         ans=true;//初始化答案
24         DFS(root);
25         return ans;
26     }
27 };
AC代码
```


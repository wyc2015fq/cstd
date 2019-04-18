# LeetCode Binary Tree Paths（简单题） - xcw0754 - 博客园
# [LeetCode Binary Tree Paths（简单题）](https://www.cnblogs.com/xcw0754/p/4934110.html)
题意：　　
　　给出一个二叉树，输出根到所有叶子节点的路径。
思路：
　　直接DFS一次，只需要判断是否到达了叶子，是就收集答案。
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
11     vector<string> ans;
12 public:
13     void DFS(string path,TreeNode* t)
14     {
15         if(t->left==NULL&&t->right==NULL)    
16         {
17             ans.push_back(path);
18             return ;
19         }
20 
21         if(t->left)        
22             DFS(path+"->"+to_string(t->left->val),t->left);
23         if(t->right)    
24             DFS(path+"->"+to_string(t->right->val),t->right);
25     }
26     vector<string> binaryTreePaths(TreeNode* root) {
27         if(root!=NULL)    DFS(to_string(root->val),root);
28         return ans;
29     }
30 };
AC代码
```


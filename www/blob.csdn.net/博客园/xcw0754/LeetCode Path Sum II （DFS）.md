# LeetCode Path Sum II  （DFS） - xcw0754 - 博客园
# [LeetCode Path Sum II  （DFS）](https://www.cnblogs.com/xcw0754/p/4976296.html)
题意：
　　给一棵二叉树，每个叶子到根的路径之和为sum的，将所有可能的路径装进vector返回。
思路：
　　节点的值可能为负的。这样子就必须到了叶节点才能判断，而不能中途进行剪枝。
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
10 
11 
12 class Solution {
13 public:
14     vector<vector<int> > pathSum(TreeNode* root, int sum) {
15         if(root==NULL)  return ans;
16         DFS(root,0,sum,tmp);
17         return ans;
18     }
19     void DFS(TreeNode* t,int n,const int& sum,vector<int>& num)
20     {
21         num.push_back(t->val);
22         n+=t->val;
23 
24         if(t->left==NULL&&t->right==NULL)
25         {
26             if(n==sum)  ans.push_back(num);
27         }
28         else
29         {
30             if(t->left)     DFS(t->left,n,sum,num);
31             if(t->right)    DFS(t->right,n,sum,num);
32         }
33         num.pop_back();
34     }
35 private:
36     vector<vector<int>> ans;
37     vector<int> tmp;
38 };
AC代码
```


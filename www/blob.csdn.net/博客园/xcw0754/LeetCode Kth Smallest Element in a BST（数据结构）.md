# LeetCode  Kth Smallest Element in a BST（数据结构） - xcw0754 - 博客园
# [LeetCode  Kth Smallest Element in a BST（数据结构）](https://www.cnblogs.com/xcw0754/p/4937197.html)
题意：
　　寻找一棵BST中的第k小的数。
思路：
　　递归比较方便。
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
12     int countNode(TreeNode* root)//计算子树有多少个节点，其实还可以将这个函数直接融到下面的函数中，用K来标记功能即可。
13     {
14         if(root==NULL)    return 0;
15         int L=countNode(root->left);
16         int R=countNode(root->right);
17         return L+R+1;
18     }
19     int kthSmallest(TreeNode* root, int k) 
20     {
21         int cnt=countNode(root->left);
22         if(cnt+1==k)    return root->val;
23         if(cnt>=k)    kthSmallest(root->left,k);
24         else    kthSmallest(root->right,k-cnt-1);    
25     }
26 };
AC代码
```


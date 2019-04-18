# LeetCode Path Sum 判断树的路径之和 - xcw0754 - 博客园
# [LeetCode Path Sum 判断树的路径之和](https://www.cnblogs.com/xcw0754/p/4084055.html)
```
1 /**
 2  * Definition for binary tree
 3  * struct TreeNode {
 4  *     int val;
 5  *     TreeNode *left;
 6  *     TreeNode *right;
 7  *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 8  * };
 9  */
10 class Solution {
11 public:
12     bool hasPathSum(TreeNode *root, int sum) {
13      if(root==0)        //针对根结点，非根节点不应会执行到这里
14          return false;
15 
16      if( root->left==0 && root->right==0 )        //是叶子结点，且刚好将sum变为0
17          return root->val==sum ? true : false ;
18 
19      if( ( root->left!=0 && hasPathSum( root->left , sum-root->val) ) || ( root->right!=0 && hasPathSum( root->right , sum-root->val ) ) )//判断左叉或右叉中是否有一条从上往下满足要求的路径
20         return true;
21 
22      return false;
23     }
24 };
```
这次终于感觉代码够简洁的了。哈哈
题意是：有没有一条这样一条路径，从根开始到叶子结点上的值之和为所提供的数字。
　　本来挺不愿意用递归的，递归很有局限性，但用起来又特别爽。像此题，想半个小时没想到怎么设计算法会快一点。如果有非递归算法，且是较好的代码，请不吝分享一下吧！
解题需考虑的是：
1.根结点为空
2.递归到叶子结点了，要设计其作为递归出口
3.非叶子结点要解决sum的问题。只要左子树或者右子树中有一条路径满足要求，那么就判断结束。


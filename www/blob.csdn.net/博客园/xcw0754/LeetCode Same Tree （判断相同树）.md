# LeetCode Same Tree （判断相同树） - xcw0754 - 博客园
# [LeetCode Same Tree （判断相同树）](https://www.cnblogs.com/xcw0754/p/4634546.html)
题意：如题
思路：递归解决，同判断对称树的原理差不多。先保证当前两个结点是相等的，再递归保证两左结点是相等的，再递归保证右结点是相等的。
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
12     bool isSameTree(TreeNode* p, TreeNode* q) {
13         if(!p&&!q)  return true;//都是空
14         if(!p&&q||p&&!q)    return false;//其中1个为空
15         return (p->val==q->val&&isSameTree(p->left,q->left)&&isSameTree(p->right,q->right))?true:false;
16     }
17 };
AC代码
```


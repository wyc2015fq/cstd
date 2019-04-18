# LeetCode Lowest Common Ancestor of a Binary Search Tree （LCA最近公共祖先） - xcw0754 - 博客园
# [LeetCode Lowest Common Ancestor of a Binary Search Tree （LCA最近公共祖先）](https://www.cnblogs.com/xcw0754/p/4638594.html)
题意：
　　给一棵二叉排序树，找p和q的LCA。
思路：
　　给的是BST（无相同节点），那么每个节点肯定大于左子树中的最大，小于右子树种的最小。根据这个特性，找LCA就简单多了。
　　分三种情况：
　　（1）p和q都在root左边，那么往root左子树递归。
　　（2）在右同理。
　　（3）一左一右的，那么root->val肯定大于其中的1个，小于另一个。
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
12     TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {  //这是一棵排序树啊！
13         if( (root->val - p->val ) * (root->val - q->val) <=0  )    //一大一小必会产生负的，或者root为其中1个，那么就是0
14             return root;
15         if( max(p->val, q->val) < root->val )              //都在左边
16             return lowestCommonAncestor(root->left, p, q);
17         else            //都在右边
18             return lowestCommonAncestor(root->right, p, q);
19     }
20 };
AC代码
```
python3
　　迭代
```
1 class Solution(object):
 2     def lowestCommonAncestor(self, root, p, q):
 3         """
 4         :type root: TreeNode
 5         :type p: TreeNode
 6         :type q: TreeNode
 7         :rtype: TreeNode
 8         """
 9         while root:
10             if (p.val-root.val)*(q.val-root.val)<=0:    #一左一右
11                 return root
12             elif p.val<root.val:    #左
13                 root=root.left
14             else:
15                 root=root.right    
AC代码
```
 　　递归
```
1 class Solution(object):
 2     def lowestCommonAncestor(self, root, p, q):
 3         """
 4         :type root: TreeNode
 5         :type p: TreeNode
 6         :type q: TreeNode
 7         :rtype: TreeNode
 8         """
 9         if (p.val-root.val)*(q.val-root.val)<=0:    #一左一右
10             return root
11         elif p.val<root.val:    #左
12             return self.lowestCommonAncestor(root.left, p, q)
13         else:
14             return self.lowestCommonAncestor(root.right, p, q)
AC代码
```


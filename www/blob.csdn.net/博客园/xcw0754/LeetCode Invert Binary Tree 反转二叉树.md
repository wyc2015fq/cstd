# LeetCode Invert Binary Tree  反转二叉树 - xcw0754 - 博客园
# [LeetCode Invert Binary Tree  反转二叉树](https://www.cnblogs.com/xcw0754/p/4619712.html)
![](https://images0.cnblogs.com/blog2015/641737/201507/032210070168584.png)
思路：递归解决，在返回root前保证该点的两个孩子已经互换了。注意可能给一个Null。
C++
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
12     TreeNode* invertTree(TreeNode* root) {
13         if(!root)   return 0;//重要在这而已
14         if(root->left)    invertTree(root->left);
15         if(root->right)   invertTree(root->right);
16         TreeNode* tmp=root->right;
17         root->right=root->left;
18         root->left=tmp;
19         return root;
20     }
21 };
AC代码
```
python3
　　递归
```
1 # Definition for a binary tree node.
 2 # class TreeNode(object):
 3 #     def __init__(self, x):
 4 #         self.val = x
 5 #         self.left = None
 6 #         self.right = None
 7 
 8 class Solution(object):
 9     def invertTree(self, root):
10         """
11         :type root: TreeNode
12         :rtype: TreeNode
13         """
14         if root!=None:
15             root.left, root.right= self.invertTree(root.right), self.invertTree(root.left)
16         return root
AC代码
```
　　迭代
```
1 # Definition for a binary tree node.
 2 # class TreeNode(object):
 3 #     def __init__(self, x):
 4 #         self.val = x
 5 #         self.left = None
 6 #         self.right = None
 7 
 8 class Solution(object):
 9     def invertTree(self, root):
10         """
11         :type root: TreeNode
12         :rtype: TreeNode
13         """
14         stack=[root]
15         while stack!=[]:
16             back=stack.pop()
17             if back!=None:
18                 back.left, back.right= back.right, back.left
19                 stack.extend([back.left,back.right])#也可以写成stack+=back.left,back.right
20     
21         return root
AC代码
```


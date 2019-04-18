# LeetCode OJ Symmetric Tree 判断是否为对称树（AC代码） - xcw0754 - 博客园
# [LeetCode OJ Symmetric Tree 判断是否为对称树（AC代码）](https://www.cnblogs.com/xcw0754/p/4076153.html)
思路：
主要判断左子树与右子树。
在判断左时，循环下去肯定会到达叶子结点中最左边的结点与最右边的结点比较。
到了这一步因为他们都没有左（右）子树了，所以得开始判断这两个结点的右（左）子树了。
当某个结点对称了，它的左子树也对称了，右子树也对称了，那才是真的对称了。
C++
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
12     bool isSymmetric(TreeNode *root) {
13         if(!root)    
14             return true;    //无根结点
15         else if(root->left==(void*)0 && root->right==(void*)0)        //只有根结点的树
16             return true;
17         else if(root->left!=(void*)0 && root->right!=(void*)0 && isSymmetric(root->left,root->right))    //两个子树非空,左右子树为对称
18             return true;
19         else
20             return false;
21     }
22     bool isSymmetric(TreeNode *left,TreeNode *right) {
23         if(left->val==right->val){        //左右结点值相等
24             if(left->left==(void*)0 && right->right==(void*)0 || left->left!=(void*)0 && right->right!=(void*)0 && isSymmetric(left->left,right->right)){        //左右子树都为空、或者对称
25                 if(left->right==(void*)0 && right->left==(void*)0 ||left->right!=(void*)0 && right->left!=(void*)0 && isSymmetric(left->right,right->left))      //右左子树都为空、或对称
26                     return true;
27                 else
28                     return false;
29             }
30             else    //左右子树不对称
31                 return false;
32         }
33         else //左右结点值不等
34             return false;
35     }
36 };
AC代码
```
python3：　　两种方法
　　增加默认参数。
```
1 # Definition for a binary tree node.
 2 # class TreeNode(object):
 3 #     def __init__(self, x):
 4 #         self.val = x
 5 #         self.left = None
 6 #         self.right = None
 7 
 8 class Solution(object):
 9     def isSymmetric(self, root, right=None, isroot=True):
10         """
11         :type root: TreeNode
12         :rtype: bool
13         """
14         F=self.isSymmetric
15         if isroot==True:    #根
16             if root==None:    return True
17             return F(root.left, root.right, False)
18         elif root==None and right==None:    #都是空
19             return True
20         elif root and right and root.val==right.val:    #都不空
21             return F(root.left, right.right, False) and F(root.right, right.left, False)
22         return False
AC代码
```
　　在函数中定义辅助函数。
```
1 # Definition for a binary tree node.
 2 # class TreeNode(object):
 3 #     def __init__(self, x):
 4 #         self.val = x
 5 #         self.left = None
 6 #         self.right = None
 7 
 8 class Solution(object):
 9     def isSymmetric(self, root):
10         """
11         :type root: TreeNode
12         :rtype: bool
13         """
14         def help(L, R):
15             if not L and not R:
16                 return True
17             elif L and R and L.val==R.val:
18                 return help(L.left,R.right) and help(L.right,R.left)
19             else:
20                 return False
21         if root==None:    return True
22         return help(root.left, root.right)
AC代码
```


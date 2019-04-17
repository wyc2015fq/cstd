# lintcode - 克隆二叉树 - guoziqing506的博客 - CSDN博客





2016年09月05日 08:46:31[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1939
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：深度复制一个二叉树。给定一个二叉树，返回一个他的克隆品 。




题目把意思已经说得很清楚了，所以不再举例了。




非常简单，用一个简单的递归就可以解决，这个思路有点类似于二叉树的前序遍历的递归算法（详见：[二叉树的遍历](http://blog.csdn.net/guoziqing506/article/details/51058024)）

因为太简单，所以直接给出代码：



```python
"""
Definition of TreeNode:
class TreeNode:
    def __init__(self, val):
        this.val = val
        this.left, this.right = None, None
"""
class Solution:
    """
    @param {TreeNode} root: The root of binary tree
    @return {TreeNode} root of new tree
    """
    def cloneTree(self, root):
        if root:
            new_root = TreeNode(root.val)
        else:
            return None
        new_root.left = self.cloneTree(root.left)
        new_root.right = self.cloneTree(root.right)
        return new_root
        # Write your code here
```








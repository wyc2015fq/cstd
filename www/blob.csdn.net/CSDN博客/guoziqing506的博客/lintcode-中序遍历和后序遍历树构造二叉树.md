# lintcode - 中序遍历和后序遍历树构造二叉树 - guoziqing506的博客 - CSDN博客





2016年10月24日 17:50:41[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：945
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：根据中序遍历和后序遍历树构造二叉树




这是典型的递归算法的应用。




什么是中序遍历、后序遍历，我想无需多说了。直接看看遍历之后列表的结构。




中序遍历：[left subtree, root, right subtree]

后序遍历：[left subtree,right subtree, root]

其中，每种遍历中的左右子树，也是按照同样的遍历模式排列的。也就是说，从结构上讲，这两种遍历的结果就具有“递归”的特性。




看看具体怎么处理这个问题。




既然遍历之后，列表的结构时上面列的那样，很显然，我们可以知道，这棵树的根节点就是后序遍历中的最后一位。然后，在中序遍历的列表中找到这个根节点所在的位置，我们记为pos。那么，pos前面的节点就是左子树的所有节点，而pos后面的节点就是右子树的所有节点，这样，我们能推出2个关键的量：左右子树的节点个数。通过这2个量，也就不难得到后续遍历中left subtree和right subtree分别在哪一段。




接下来就是递归发挥作用的时刻了。既然左右子树的节点在前序和中序2个列表中已经被找到，那自然可以按照上面的方法继续找每个子树的左右子树。递归触底的条件是：某个节点的左右子树都是空（也就是扫描到了叶子）。




代码如下：






```python
"""
Definition of TreeNode:
class TreeNode:
    def __init__(self, val):
        self.val = val
        self.left, self.right = None, None
"""


class Solution:
    """
    @param inorder : A list of integers that inorder traversal of a tree
    @param postorder : A list of integers that postorder traversal of a tree
    @return : Root of a tree
    """
    def buildTree(self, inorder, postorder):
        
        if len(inorder) == 0:
            return

        inBegin, inEnd = 0, len(inorder) - 1
        postBegin, postEnd = 0, len(postorder) - 1
        
        return self.helper(inorder, postorder, inBegin, inEnd, postBegin, postEnd)

    def helper(self, inorder, postorder, inBegin, inEnd, postBegin, postEnd):

        root = TreeNode(postorder[postEnd])
        pos = inorder.index(postorder[postEnd])

        leftLen = pos - inBegin
        rightLen = inEnd - pos

        if leftLen > 0:
            leftChild = self.helper(inorder, postorder, inBegin, pos - 1, postBegin, postBegin + leftLen - 1)
            root.left = leftChild

        if rightLen > 0:
            rightChild = self.helper(inorder, postorder, pos + 1, inEnd, postBegin + leftLen, postEnd - 1)
            root.right = rightChild

        return root
```

需要注意的是helper()函数的参数，分别是：中序列表，后续列表，中序开始位置，中序结束位置，后续开始位置，后续结束位置。








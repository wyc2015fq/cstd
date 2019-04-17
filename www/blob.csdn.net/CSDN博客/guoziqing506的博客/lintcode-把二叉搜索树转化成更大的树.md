# lintcode - 把二叉搜索树转化成更大的树 - guoziqing506的博客 - CSDN博客





2018年03月20日 13:36:24[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：156
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给定二叉搜索树(BST)，将其转换为更大的树，使原始BST上每个节点的值都更改为在原始树中大于等于该节点值的节点值之和(包括该节点)。

样例： 

Given a binary search Tree `{5,2,13}｀:

```
5
        /   \
       2     13
```

Return the root of new tree

```
18
        /   \
      20     13
```

解题思路：首先注意到是一棵二叉搜索树，那么基本的规律是“右>中>左”，这可以想到采用类似二叉树的中序遍历（可以参照我的博客：[二叉树的中序遍历](http://blog.csdn.net/guoziqing506/article/details/51159022)）的方法，依次处理节点。只不过需要对中序遍历稍加修改。我们知道中序遍历为“左中右”，现在换一下，换成“右中左”。这样得到的节点序列有一个特点：其所含值的大小是由大到小排列的。

处理由大到小排列的节点序列，对于本题要求的转换成更大的树，是有好处的：因为每次只需要考虑前一个处理的节点即可。拿样例来说，13最大，第一个被处理，没有前一个节点，啥都不用考虑；5次之，只需考虑13，加上就是18；2最小，加上18，就得到20了。所以，解题的方法就是按修改后的中序遍历方法访问树中节点，对每一个节点的值做相应的加法操作就行。有关于中序遍历的内容我已经在上面给出的博客链接中详细说明了，此处略。代码如下：

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
    @param root: the root of binary tree
    @return: the new root
    """
    def convertBST(self, root):
        stack = []
        cur = root
        temp = 0

        while cur or len(stack) != 0:

            if cur:
                stack.append(cur)
                cur = cur.right
            else:
                cur = stack.pop()
                cur.val += temp
                temp = cur.val
                cur = cur.left
        return root
```

有关于lintcode题目的答案代码，我打算整理一个完整的版本，并放在我的github上。已经在博客中写过的，我也会在后期慢慢加入。另外，一些我觉得过于简单的题目就不再博客中特别记录了。不过github上会有相关的代码。欢迎大家批评指正。网址：[Lintcode-Answer](https://github.com/guoziqingbupt/Lintcode-Answer)




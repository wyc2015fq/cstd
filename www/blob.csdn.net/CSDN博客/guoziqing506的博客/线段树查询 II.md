# 线段树查询 II - guoziqing506的博客 - CSDN博客





2016年07月31日 09:37:49[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：549标签：[lintcode																[递归](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=lintcode&t=blog)
个人分类：[lintcode](https://blog.csdn.net/guoziqing506/article/category/6289140)

所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)








题目描述：对于一个数组，我们可以对其建立一棵 线段树, 每个结点存储一个额外的值 count 来代表这个结点所指代的数组区间内的元素个数. (数组中并不一定每个位置上都有元素) 

实现一个 query 的方法，该方法接受三个参数 root, start 和 end, 分别代表线段树的根节点和需要查询的区间，找到数组中在区间[start, end]内的元素个数。




样例：

对于数组 [0, 空，2, 3], 对应的线段树为：

![](https://img-blog.csdn.net/20160731093018098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


query(1, 1), return 0

query(1, 2), return 1

query(2, 3), return 2

query(0, 2), return 2




跟“线段树的查询是一样的”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/52074586)），只不过此处求的是count，而那一道题求的是max，所以，很容易先想到最后对于count的处理时+，而不是对max的处理那样用max()函数。




除此之外，基本也就没什么不同了，对了，还有一点是当查询区间与节点区间不交时，返回0，而不是空，这一点与上题不同。




我们仿照上一题的思路写出代码：因为和上一题的思路太一致，所以，不再赘述



```python
"""
Definition of SegmentTreeNode:
class SegmentTreeNode:
    def __init__(self, start, end, count):
        self.start, self.end, self.count = start, end, count
        self.left, self.right = None, None
"""

class Solution: 
    # @param root, start, end: The root of segment tree and 
    #                          an segment / interval
    # @return: The count number in the interval [start, end] 
    def query(self, root, start, end):
        if root is None or start > root.end or end < root.start:
            return 0
        elif start <= root.start and end >= root.end:
            return root.count
        else:
            return self.query(root.left, start, end) + self.query(root.right, start, end)
        # write your code here
```







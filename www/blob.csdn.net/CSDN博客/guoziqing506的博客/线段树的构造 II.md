# 线段树的构造 II - guoziqing506的博客 - CSDN博客





2016年07月30日 10:51:49[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：518
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：线段树是一棵二叉树，他的每个节点包含了两个额外的属性start和end用于表示该节点所代表的区间。start和end都是整数，并按照如下的方式赋值:

根节点的 start 和 end 由 build 方法所给出。

对于节点 A 的左儿子，有 start=A.left, end=(A.left + A.right) / 2。

对于节点 A 的右儿子，有 start=(A.left + A.right) / 2 + 1, end=A.right。

如果 start 等于 end, 那么该节点是叶子节点，不再有左右儿子。

对于给定数组设计一个build方法，构造出线段树




样例：给出[3,2,1,4]，线段树将被这样构造


![](https://img-blog.csdn.net/20160730103542007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





和上一道题“线段树的构造”是一样的（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/52066168)），只不过此处稍微复杂一点，用到了一个数组，并且加了一个值max存储在某段区间内数组的最大值。那么道理和上一题是一样的，区间的构造还是通过递归的方式，而最大值的求取则需要我们控制数组的区间，用max()函数求取某个区间内的最大值。

为了方便对区间的控制，我们可以再写一个辅助函数helper()



```python
"""
Definition of SegmentTreeNode:
class SegmentTreeNode:
    def __init__(self, start, end, max):
        self.start, self.end, self.max = start, end, max
        self.left, self.right = None, None
"""

class Solution:
    # @oaram A: a list of integer
    # @return: The root of Segment Tree
    def build(self, A):
        start, end = 0, len(A) - 1
        if start > end:
            return
        return self.helper(A, start, end)

    def helper(self, A, start, end):
        if start == end:
            return SegmentTreeNode(start, end, A[start])
        root = SegmentTreeNode(start, end, max(A[start: end + 1]))
        mid = (start + end) // 2
        root.left = self.helper(A, start, mid)
        root.right = self.helper(A, mid + 1, end)
        return root
        # write your code here
```


其中，对于区间的构造就不多说了，不明白的可以回看上我的一篇博文，但是此时需要注意的是辅助函数的构造。为什么要重新构造一个helper()函数来完成最大值的求取，而不是直接将数组的切片带入max()函数？这里，其实需要特别小心，因为当我的函数是由三个参数构成的时候，执行完第23行代码，执行第24行的root.right = self.helper(A, mid + 1, end)的时候，mid和end的值与23行代码执行前是一样的，因为是以函数参数带入，而反过来讲，如果直接向max()函数传数组切片，则是传入了一个参数，mid和end会变，就会出错。所以，所有这种将数组分段的递归函数，最好都通过一个辅助函数将数组的起点，终点作为参数传入，防止出错。



比如，很早之前我们写快速排序的程序时，就是这样做的（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/50952002)）




# lintcode - 旋转图像 - guoziqing506的博客 - CSDN博客





2016年08月29日 15:00:13[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：689
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给定一个N×N的二维矩阵表示图像，90度顺时针旋转图像。

样例：给出一个矩形[[1,2],[3,4]]，90度顺时针旋转后，返回[[3,1],[4,2]]




具体怎么转化，大家都知道，现在看看能不能有什么技巧能够使得代码写起来更容易，同时能够支持原地翻转。

实际上，对于这种翻转90度的情况，可以这样处理：先按副对角线翻转元素，再按水平中线翻转矩阵的行。如下图所示：

![](https://img-blog.csdn.net/20160829145520875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


道理很简单了，接下来的事情就是具体代码实现：



```python
class Solution:
    """
    @param matrix: A list of lists of integers
    @return: Nothing
    """
    def rotate(self, matrix):
        m = len(matrix)
        if m == 0:
            return
        n = len(matrix[0])

        # 延副对角线翻转元素
        for i in range(m - 1):
            for j in range(n - 1 - i):
                matrix[i][j], matrix[m - j - 1][n - i - 1] = matrix[m - j - 1][n - i - 1], matrix[i][j]
                
        # 延副水平中线翻转元素
        for i in range(m // 2):
            matrix[i], matrix[m - i - 1] =  matrix[m - i - 1], matrix[i]
```

从18行起可以直接对矩阵的行进行翻转，而不是一个个的元素，这样写将代码简化了很多。





# 不同的路径 II - guoziqing506的博客 - CSDN博客





2016年06月14日 15:00:50[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：995
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述："不同的路径" 的跟进问题：现在考虑网格中有障碍物，那样将会有多少条不同的路径？网格中的障碍和空位置分别用 1 和 0 来表示。

样例：如下所示在3x3的网格中有一个障碍物：

[

  [0,0,0],

  [0,1,0],

  [0,0,0]

]

上一道“不同的路径”的题目是这道题的背景（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51648682)），所以我在这里直接进入正题了，如果你不知道上一道题是怎么做的，建议先去做一下，再来看本题。

还是动态规划的方法，通过建立一个二维的表格记录走到网格每个位置的路径数。与上一道题几乎是一模一样的，不同仅仅在于有障碍，所以我们只需啊在上一题的基础上加上一条判断语句就行：

1. 如果当前网格的值为0，和上一题一样

2. 如果当前网格的值为1，那么，到此网格的路径数归0（不能到嘛，因为有障碍）

所以，对上一题的代码略作修改：



```python
class Solution:
    """
    @param obstacleGrid: An list of lists of integers
    @return: An integer
    """
    def uniquePathsWithObstacles(self, obstacleGrid):
        m = len(obstacleGrid)
        if m == 0 or obstacleGrid[0][0] == 1:
            return 0
        n = len(obstacleGrid[0])
        record = [[1 for i in range(n)] for j in range(m)]
        for i in range(m):
            for j in range(n):
                if obstacleGrid[i][j] == 1:
                    record[i][j] = 0
                elif j == 0:
                    record[i][j] = record[i - 1][j]
                elif i == 0:
                    record[i][j] = record[i][j - 1]
                else:
                    record[i][j] = record[i - 1][j] + record[i][j - 1]
        return record[m - 1][n - 1]
        # write your code here
```






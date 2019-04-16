# LeetCode中的两道动态规划题目 - 白马负金羁 - CSDN博客





2017年12月28日 13:43:40[白马负金羁](https://me.csdn.net/baimafujinji)阅读数：1015








动态规划是一种非常重要的算法设计思想。历史上有很多著名的算法都是基于这种思想设计而来的，例如：Needleman–Wunsch算法、CYK算法、FFT算法、维特比算法等等。动态规划的核心思想有两个：首先是将一个大问题拆解为若干子问题；其次是将曾经计算过的结果储存起来以备多次使用。




动态规划非常重要，因为它是各种IT公司笔试面试题目中常常被考察的重点，而且它也是所有希望信息学竞赛的选手所必备的知识。但动态规划对于很多初学者来说，是相对较难掌握的一种技巧，LeetCode上的动态规划题目大部分属于Median和Hard程度。但通常来说，一般的动态规划问题都是有套路的，通过掌握这种套路，其实可以解决相当一部分的动态规划问题。





下面通过两个LeetCode题目来做进一步说明。首先是编号为#62的Unique Paths问题，该问题描述如下：


![](https://img-blog.csdn.net/20171228124651207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpbWFmdWppbmpp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


大意是给定一个矩阵，问你从左上角到右下角一共有多少种不同的路径。其中Robot移动的方式只能是要么向下移动一格、要么是向右移动一格。




解决动态规划问题的套路中，第一个步骤就是写出一个递归求解的“初稿”。就当前这个问题来说，要想移动到格子[m,n]，要么是从[m,n-1]中（也就是左面一格）移动而来，要么是从[m-1,n]中（也就是上面一格）移动而来。于是，我们知道到达格子[m,n]的路径数目就是到达格子[m,n-1]的路径数目+到达格子[m-1,n]的路径数目，也就是最核心的递归关系（通常在动态规划问题中我们称其为状态转移方程）。然后，对于一个递归的问题，你还需要考虑到一些base condition以保证你的递归分解到原子问题时可以正确返回。根据上述思路，我们来首先写一个递归的版本（注意其中并未使用动态规划）：



```cpp
class Solution {
public:

    int uniquePaths(int m, int n) {
        
        if (m==1 && n==1) return 1;
        
        if(m==1) 
            return uniquePaths(m, n-1);
        
        if(n==1) 
            return uniquePaths(m-1, n);
         
        return (uniquePaths(m-1, n) + uniquePaths(m, n-1));
    }
};
```
但是，上面这个版本直接提交LeetCode，会显示超时！因为其中会存在大量的重复计算。为此，需要使用解决动态规划问题的套路中之第二个步骤：定义一个数组或矩阵来存储已经得到的结果。当再次需要使用它们时，直接查表，而不用重新计算。






就当前这个问题来说，不妨定义一个矩阵。因为题目中已经言明：m和n的最大值是100，而且地图的初始位置是[1,1]，所以我们定义的矩阵大小为101×101，当然其中最左边一列和最上边一行并不会被用到。然后，我们把整个矩阵初始化，每个位置都是-1，表示该位置未被计算过。然后把，[1,1]这个位置初始化为1，表示如果地图只有一个格子那么路线也就只有1条。由此而得之代码如下：




```cpp
class Solution {
public:
    
    int matrix[101][101];
    
    Solution(){
        for(int i = 0; i<101; i++)
            for(int j = 0; j<101; j++)
                matrix[i][j] = -1;
    
        matrix[1][1] = 1;
    }

    int uniquePaths(int m, int n) {
        
        if (matrix[m][n] != -1)
            return matrix[m][n];
        if(m==1) 
        {
            matrix[m][n] = uniquePaths(m, n-1);
            return matrix[m][n];
        }
        
        if(n==1) 
        {
            matrix[m][n] = uniquePaths(m-1, n);
            return matrix[m][n];
        }
        
        matrix[m][n] = uniquePaths(m-1, n) + uniquePaths(m, n-1);
        return (matrix[m][n]);
        
    }
};
```
上述实现可以满足题目要求。当然，由于为了演示之用，我们并为对其进行优化，你还可以改进它，以实现更高的效率。






LeetCode中还提供了另外一个变型题目，其编号为#63。大意是初始条件中给定一个矩阵用来表示地图，其中如果值为1则表示一个障碍物。然后再假定存在障碍物的情况下问你有多少条不同的路径。


![](https://img-blog.csdn.net/20171228125706179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpbWFmdWppbmpp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其实如果你能解决#62题，那么这一道题目其实是非常简单的。我们主要在递归过程中，增加一个base condition，即如果目标点是一个障碍物，那么通向它的路径就为0。基于这种小改动而得之代码如下：




```cpp
class Solution {
public:
    
    int matrix[101][101];
    
    Solution(){
        for(int i = 0; i<101; i++)
            for(int j = 0; j<101; j++)
                matrix[i][j] = -1; 
    }
    
    int uniquePaths(int m, int n, vector<vector<int>>& obstacleGrid) {
        
        if (matrix[m][n] != -1)
            return matrix[m][n];
        
        if(obstacleGrid[m-1][n-1] == 1)
        {
            matrix[m][n] = 0;
            return 0;
        }
        
        if(m==1) 
        {
            matrix[m][n] = uniquePaths(m, n-1, obstacleGrid);
            return matrix[m][n];
        }
        
        if(n==1) 
        {
            matrix[m][n] = uniquePaths(m-1, n, obstacleGrid);
            return matrix[m][n];
        }
        
        matrix[m][n] = uniquePaths(m-1, n, obstacleGrid) + uniquePaths(m, n-1, obstacleGrid);
        return (matrix[m][n]);
        
    }
    
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        
        matrix[1][1]= obstacleGrid[0][0]==1? 0:1;
        
        return uniquePaths(m, n, obstacleGrid);
    }
};
```

唯一需要注意的是作为参数而提供的新矩阵是从[0,0]开始的（而非从[1,1]开始）。所以在初始化上会有一点点的小变化。






（全文完）






**本博客中已经讨论过的LeetCode题目列表**

- 
# [LeetCode中的两道动态规划题目（#62、#63）](http://blog.csdn.net/baimafujinji/article/details/78920814)

- 
# [LeetCode中的动态规划题目解答（2）(#64)](http://blog.csdn.net/baimafujinji/article/details/78928845)

- 
# [LeetCode中的动态规划题目解答（3）（#72、#718）](http://blog.csdn.net/baimafujinji/article/details/78930037)

- 
# [最大连续子序列和问题（#53）](http://blog.csdn.net/baimafujinji/article/details/78238497)

- 
# [看看你是否真正掌握了Binary Search(#35)](http://blog.csdn.net/baimafujinji/article/details/6477055)

- 
# [ZigZag排列问题与经典笔试面试题目解析（#6）](http://blog.csdn.net/baimafujinji/article/details/50388584)

- 
# [括号匹配问题与经典笔试面试题目解析（#20、#32）](http://blog.csdn.net/baimafujinji/article/details/50465482)

- 
# [牛顿迭代法与一道经典编程问题（#69）](http://blog.csdn.net/baimafujinji/article/details/50390841)

- 
# [三道tricky的Leetcode面试题目解析（#48、#169、#231）](http://blog.csdn.net/baimafujinji/article/details/50478012)

- 
# [道关于哈希的Leetcode题目解析（#187、#389）](http://blog.csdn.net/baimafujinji/article/details/6488201)

- 
# [杨辉三角与一道经典笔试面试题目（#118、#119）](http://blog.csdn.net/baimafujinji/article/details/50436170)

- 
# [波兰表达式（Reverse Polish Notation）（#150）](http://blog.csdn.net/baimafujinji/article/details/6471037)






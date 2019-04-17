# leetcode-62. Unique Paths 不同路径 - 别说话写代码的博客 - CSDN博客





2019年01月16日 23:32:03[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：37标签：[leetcode																[unique path																[不同路径](https://so.csdn.net/so/search/s.do?q=不同路径&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=unique path&t=blog)



|A robot is located at the top-left corner of a *m* x *n* grid (marked 'Start' in the diagram below).The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).How many possible unique paths are there?![](https://assets.leetcode.com/uploads/2018/10/22/robot_maze.png)			Above is a 7 x 3 grid. How many possible unique paths are there?**Note:***m* and *n* will be at most 100.**Example 1:****Input:** m = 3, n = 2**Output:** 3**Explanation:**From the top-left corner, there are a total of 3 ways to reach the bottom-right corner:1. Right -> Right -> Down2. Right -> Down -> Right3. Down -> Right -> Right**Example 2:****Input:** m = 7, n = 3**Output:** 28|一个机器人位于一个 *m x n *网格的左上角 （起始点在下图中标记为“Start” ）。机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为“Finish”）。问总共有多少条不同的路径？![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/10/22/robot_maze.png)例如，上图是一个7 x 3 的网格。有多少可能的路径？**说明：***m* 和 *n *的值均不超过 100。**示例 1:****输入:** m = 3, n = 2**输出:** 3**解释:**从左上角开始，总共有 3 条路径可以到达右下角。1. 向右 -> 向右 -> 向下2. 向右 -> 向下 -> 向右3. 向下 -> 向右 -> 向右**示例 2:****输入:** m = 7, n = 3**输出:** 28|
|----|----|

思路：其实只要想到当前路径可能数dp[i][j]=dp[i-1][j]+dp[i][j-1],也就是说当前路径可能数等于上面的可能加上 这个位置左边的可能。dp搞定。可以 生成二维数组，为了节省空间用一维数组也可以

```cpp
class Solution {
public:
    int uniquePaths(int m, int n) {
        if(m<1 || n<1) return 0;
        vector<int> dp(n,1);
        for(int i=1;i<m;++i)
            for(int j=1;j<n;++j)
                dp[j]+=dp[j-1];
        return dp[n-1];
    }
};
```

![](https://img-blog.csdnimg.cn/20190116233140554.png)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)





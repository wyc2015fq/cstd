# leetcode-63. Unique Paths II 不同路径2 - 别说话写代码的博客 - CSDN博客





2019年01月16日 23:46:43[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：38标签：[leetcode																[unique path ll																[不同路径2](https://so.csdn.net/so/search/s.do?q=不同路径2&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=unique path ll&t=blog)



|A robot is located at the top-left corner of a *m* x *n* grid (marked 'Start' in the diagram below).The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).Now consider if some obstacles are added to the grids. How many unique paths would there be?![](https://assets.leetcode.com/uploads/2018/10/22/robot_maze.png)An obstacle and empty space is marked as `1` and `0` respectively in the grid.**Note:***m* and *n* will be at most 100.**Example 1:****Input:**[  [0,0,0],  [0,1,0],  [0,0,0]]**Output:** 2**Explanation:**There is one obstacle in the middle of the 3x3 grid above.There are two ways to reach the bottom-right corner:1. Right -> Right -> Down -> Down2. Down -> Down -> Right -> Right|一个机器人位于一个 *m x n *网格的左上角 （起始点在下图中标记为“Start” ）。机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为“Finish”）。现在考虑网格中有障碍物。那么从左上角到右下角将会有多少条不同的路径？![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/10/22/robot_maze.png)网格中的障碍物和空位置分别用 `1` 和 `0` 来表示。**说明：***m* 和 *n *的值均不超过 100。**示例 1:****输入:**[  [0,0,0],  [0,1,0],  [0,0,0]]**输出:** 2**解释:**3x3 网格的正中间有一个障碍物。从左上角到右下角一共有 `2` 条不同的路径：1. 向右 -> 向右 -> 向下 -> 向下2. 向下 -> 向下 -> 向右 -> 向右|
|----|----|

思路：和上一题类似。只不过处理障碍物时 只需要把当前可能路径置为0就行，其他和上一题一样。dp就完了，注意的是可以申请个二维数组，也可以申请一个一维数组。

```cpp
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        if(obstacleGrid.size()==0 || obstacleGrid[0].size()==0 || obstacleGrid[0][0]==1)
            return 0;
        vector<vector<int>> dp(obstacleGrid.size(),vector<int>  (obstacleGrid[0].size(),0));
        for(int i=0;i<obstacleGrid.size();++i)
        {
            for(int j=0;j<obstacleGrid[0].size();++j)
            {
                if(obstacleGrid[i][j]==1) dp[i][j]=0;
                else if(i==0 && j==0) dp[i][j]=1;
                else if(i==0 && j>0) dp[i][j]=dp[i][j-1];
                else if(i>1 && j==0) dp[i][j]=dp[i-1][j];
                else dp[i][j] = dp[i-1][j]+dp[i][j-1];
            }
        }
        return dp.back().back();
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)





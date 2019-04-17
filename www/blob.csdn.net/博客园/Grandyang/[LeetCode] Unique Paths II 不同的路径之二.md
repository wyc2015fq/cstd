# [LeetCode] Unique Paths II 不同的路径之二 - Grandyang - 博客园







# [[LeetCode] Unique Paths II 不同的路径之二](https://www.cnblogs.com/grandyang/p/4353680.html)







A robot is located at the top-left corner of a *m* x *n* grid (marked 'Start' in the diagram below).

The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).

Now consider if some obstacles are added to the grids. How many unique paths would there be?

![](https://assets.leetcode.com/uploads/2018/10/22/robot_maze.png)

An obstacle and empty space is marked as `1` and `0` respectively in the grid.

Note: *m* and *n* will be at most 100.

Example 1:
Input:
[
  [0,0,0],
  [0,1,0],
  [0,0,0]
]
Output: 2
Explanation:
There is one obstacle in the middle of the 3x3 grid above.
There are two ways to reach the bottom-right corner:
1. Right -> Right -> Down -> Down
2. Down -> Down -> Right -> Right


这道题是之前那道 [Unique Paths](http://www.cnblogs.com/grandyang/p/4353555.html) 的延伸，在路径中加了一些障碍物，还是用动态规划Dynamic Programming来解，我们使用一个二维的dp数组，大小为 (m+1) x (n+1)，这里的dp[i][j] 表示到达 (i-1, j-1) 位置的不同路径的数量，那么i和j需要更新的范围就是[1, m] 和 [1, n]。状态转移方程跟之前那道题是一样的，因为每个位置只能由其上面和左面的位置移动而来，所以也是由其上面和左边的dp值相加来更新当前的dp值，即：

dp[i][j] = dp[i-1][j] + dp[i][j-1]

这里就能看出来我们初始化dp数组的大小为 (m+1) x (n+1)，是为了handle边缘情况，当i或j为0时，减1可能会出错。当某个位置是障碍物时，其dp值为0，我们直接跳过该位置即可。我们还需要初始化dp数组的某个值，使得其能正常累加。当起点不是障碍物时，其dp值应该为值，即dp[1][1] = 1，由于其是由 dp[0][1] + dp[1][0] 更新而来，所以二者中任意一个初始化为1即可。由于之后LeetCode更新了这道题的test case，使得使用int型的dp数组会有溢出的错误，所以我们改为使用long型的数组来避免overflow，代码如下：



解法一：

```
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        if (obstacleGrid.empty() || obstacleGrid[0].empty() || obstacleGrid[0][0] == 1) return 0;
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        vector<vector<long>> dp(m + 1, vector<long>(n + 1, 0));
        dp[0][1] = 1;
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (obstacleGrid[i - 1][j - 1] != 0) continue;
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }
        return dp[m][n];
    }
};
```



或者我们也可以使用一维dp数组来解，省一些空间，参见代码如下：



解法二：

```
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        if (obstacleGrid.empty() || obstacleGrid[0].empty() || obstacleGrid[0][0] == 1) return 0;
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        vector<long> dp(n, 0);
        dp[0] = 1;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (obstacleGrid[i][j] == 1) dp[j] = 0;
                else if (j > 0) dp[j] += dp[j - 1];
            }
        }
        return dp[n - 1];
    }
};
```



类似题目：

[Unique Paths](http://www.cnblogs.com/grandyang/p/4353555.html)



参考资料：

[https://leetcode.com/problems/unique-paths-ii/](https://leetcode.com/problems/unique-paths-ii/)

[https://leetcode.com/problems/unique-paths-ii/discuss/23250/Short-JAVA-solution](https://leetcode.com/problems/unique-paths-ii/discuss/23250/Short-JAVA-solution)

[https://leetcode.com/problems/unique-paths-ii/discuss/23248/My-C%2B%2B-Dp-solution-very-simple!](https://leetcode.com/problems/unique-paths-ii/discuss/23248/My-C%2B%2B-Dp-solution-very-simple!)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













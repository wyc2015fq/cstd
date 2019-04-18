# LeetCode Minimum Path Sum （简单DP） - xcw0754 - 博客园
# [LeetCode Minimum Path Sum （简单DP）](https://www.cnblogs.com/xcw0754/p/4931544.html)
题意：
　　给一个n*m的矩阵，每次可以往下或右走，经过的格子中的数字之和就是答案了，答案最小为多少？
思路：
　　比较水，只是各种空间利用率而已。
　　如果可以在原空间上作修改。
```
1 class Solution {
 2 public:
 3     int minPathSum(vector<vector<int>>& grid) {
 4         int n=grid.size()-1;
 5         int m=grid[n].size()-1;
 6         for(int j=1; j<=m; j++)    
 7             grid[0][j]+=grid[0][j-1];
 8         for(int i=1; i<=n; i++)
 9         {
10             grid[i][0]+=grid[i-1][0];    
11             for(int j=1; j<=m; j++)
12                 grid[i][j]+=min(grid[i-1][j], grid[i][j-1]);
13         }
14         return grid[n][m];
15     }
16 };
AC代码
```
　　至少也要用O(m)的空间吧。
```
1 class Solution {
 2 public:
 3     int minPathSum(vector<vector<int>>& grid) {
 4         vector<int> dp(grid[0].begin(),grid[0].end());
 5         int n=grid.size()-1, m=grid[n].size()-1;
 6         for(int j=1; j<=m; j++)    
 7             dp[j]+=dp[j-1];
 8         for(int i=1; i<=n; i++)
 9         {
10             dp[0]+=grid[i][0];    
11             for(int j=1; j<=m; j++)
12                 dp[j]=grid[i][j]+min(dp[j-1], dp[j]);
13         }
14         return dp[m];
15     }
16 };
AC代码
```


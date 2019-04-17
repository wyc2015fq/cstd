# [LeetCode] Out of Boundary Paths 出界的路径 - Grandyang - 博客园







# [[LeetCode] Out of Boundary Paths 出界的路径](https://www.cnblogs.com/grandyang/p/6927921.html)







There is an m by n grid with a ball. Given the start coordinate (i,j) of the ball, you can move the ball to adjacent cell or cross the grid boundary in four directions (up, down, left, right). However, you can at most move N times. Find out the number of paths to move the ball out of grid boundary. The answer may be very large, return it after mod 109 + 7.

Example 1:
Input:m = 2, n = 2, N = 2, i = 0, j = 0
Output: 6
Explanation:

![](https://leetcode.com/static/images/problemset/out_of_boundary_paths_1.png)



Example 2:
Input:m = 1, n = 3, N = 3, i = 0, j = 1
Output: 12
Explanation:

![](https://leetcode.com/static/images/problemset/out_of_boundary_paths_2.png)



Note:
- Once you move the ball out of boundary, you cannot move it back.
- The length and height of the grid is in range [1,50].
- N is in range [0,50].



这道题给了我们一个二维的数组，某个位置放个足球，每次可以在上下左右四个方向中任意移动一步，总共可以移动N步，问我们总共能有多少种移动方法能把足球移除边界，由于结果可能是个巨大的数，所以让我们对一个大数取余。那么我们知道对于这种结果很大的数如果用递归解法很容易爆栈，所以最好考虑使用DP来解。那么我们使用一个三维的DP数组，其中dp[k][i][j]表示总共走k步，从(i,j)位置走出边界的总路径数。那么我们来找递推式，对于dp[k][i][j]，走k步出边界的总路径数等于其周围四个位置的走k-1步出边界的总路径数之和，如果周围某个位置已经出边界了，那么就直接加上1，否则就在dp数组中找出该值，这样整个更新下来，我们就能得出每一个位置走任意步数的出界路径数了，最后只要返回dp[N][i][j]就是所求结果了，参见代码如下：



解法一：

```
class Solution {
public:
    int findPaths(int m, int n, int N, int i, int j) {
        vector<vector<vector<int>>> dp(N + 1, vector<vector<int>>(m, vector<int>(n, 0)));
        for (int k = 1; k <= N; ++k) {
            for (int x = 0; x < m; ++x) {
                for (int y = 0; y < n; ++y) {
                    long long v1 = (x == 0) ? 1 : dp[k - 1][x - 1][y];
                    long long v2 = (x == m - 1) ? 1 : dp[k - 1][x + 1][y];
                    long long v3 = (y == 0) ? 1 : dp[k - 1][x][y - 1];
                    long long v4 = (y == n - 1) ? 1 : dp[k - 1][x][y + 1];
                    dp[k][x][y] = (v1 + v2 + v3 + v4) % 1000000007;
                }
            }
        } 
        return dp[N][i][j];
    }
};
```



下面这种方法虽然也是用的DP解法，但是DP数组的定义和上面的不一样，这种解法相当于使用了BFS搜索，以(i, j)为起始点，其中dp[k][x][y]表示用了k步，进入(x, y)位置的路径数，由于dp[k][x][y]只依赖于dp[k-1][x][y]，所以我们可以用一个二维dp数组来代替，初始化dp[i][j]为1，总共N步，进行N次循环，每次都新建一个mxn大小的临时数组t，然后就是对于遍历到的每个位置，都遍历其四个相邻位置，如果相邻位置越界了，那么我们用当前位置的dp值更新结果res，因为此时dp值的意义就是从(i,j)到越界位置的路径数。如果没有，我们将当前位置的dp值赋给t数组的对应位置，这样在遍历完所有的位置时，将数组t整个赋值给dp，然后进入下一步的循环，参加代码如下：



解法二：

```
class Solution {
public:
    int findPaths(int m, int n, int N, int i, int j) {
        int res = 0;
        vector<vector<int>> dp(m, vector<int>(n, 0));
        dp[i][j] = 1;
        vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
        for (int k = 0; k < N; ++k) {
            vector<vector<int>> t(m, vector<int>(n, 0));
            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    for (auto dir : dirs) {
                        int x = r + dir[0], y = c + dir[1];
                        if (x < 0 || x >= m || y < 0 || y >= n) {
                            res = (res + dp[r][c]) % 1000000007;
                        } else {
                            t[x][y] = (t[x][y] + dp[r][c]) % 1000000007;
                        }
                    }
                }
            }
            dp = t;
        } 
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/88492/c-6-lines-dp-o-n-m-n-6-ms](https://discuss.leetcode.com/topic/88492/c-6-lines-dp-o-n-m-n-6-ms)

[https://discuss.leetcode.com/topic/88570/java-solution-dp-with-space-compression](https://discuss.leetcode.com/topic/88570/java-solution-dp-with-space-compression)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













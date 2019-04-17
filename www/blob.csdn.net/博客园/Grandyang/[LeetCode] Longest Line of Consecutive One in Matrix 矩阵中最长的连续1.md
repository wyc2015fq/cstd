# [LeetCode] Longest Line of Consecutive One in Matrix 矩阵中最长的连续1 - Grandyang - 博客园







# [[LeetCode] Longest Line of Consecutive One in Matrix 矩阵中最长的连续1](https://www.cnblogs.com/grandyang/p/6900866.html)







Given a 01 matrix M, find the longest line of consecutive one in the matrix. The line could be horizontal, vertical, diagonal or anti-diagonal.

Example:
Input:
[[0,1,1,0],
 [0,1,1,0],
 [0,0,0,1]]
Output: 3



Hint: The number of elements in the given matrix will not exceed 10,000.



这道题给了我们一个二维矩阵，让我们求矩阵中最长的连续1，连续方向任意，可以是水平，竖直，对角线或者逆对角线均可。那么最直接最暴力的方法就是四个方向分别来统计最长的连续1，其中水平方向和竖直方向都比较容易，就是逐行逐列的扫描，使用一个计数器，如果当前位置是1，则计数器自增1，并且更新结果res，否则计数器清零。对于对角线和逆对角线需要进行些坐标转换，对于一个mxn的矩阵，对角线和逆对角线的排数都是m+n-1个，难点在于我们要确定每一排上的数字的坐标，如果i是从0到m+n-1之间遍历，j是在i到0之间遍历，那么对角线的数字的坐标就为(i-j, j)，逆对角线的坐标就为(m-1-i+j, j)，这是博主千辛万苦试出来的T.T，如果能直接记住，效果肯定棒！那么有了坐标转换，求对角线和逆对角线的连续1也就不是啥难事了，参见代码如下：



解法一：

```
class Solution {
public:
    int longestLine(vector<vector<int>>& M) {
        if (M.empty() || M[0].empty()) return 0;
        int res = 0, m = M.size(), n = M[0].size();
        for (int i = 0; i < m; ++i) { // Check horizontal
            int cnt = 0;
            for (int j = 0; j < n; ++j) {
                if (M[i][j] == 1) res = max(res, ++cnt);
                else cnt = 0;
            }
        }
        for (int j = 0; j < n; ++j) {
            int cnt = 0;
            for (int i = 0; i < m; ++i) { // Check vertical
                if (M[i][j] == 1) res = max(res, ++cnt);
                else cnt = 0;
            }
        }
        for (int i = 0; i < m + n - 1; ++i) {
            int cnt1 = 0, cnt2 = 0;
            for (int j = i; j >= 0; --j) {
                if (i - j < m && j < n) { // Check diagonal
                    if (M[i - j][j] == 1) res = max(res, ++cnt1);
                    else cnt1 = 0;
                }
                int t = m - 1 - i + j;
                if (t >= 0 && t < m && j < n ) { // Check anti-diagonal
                    if(M[t][j] == 1) res = max(res, ++cnt2);
                    else cnt2 = 0;
                }
            }
        }
        return res;
    }
};
```



如果上面的解法的坐标转换不好想的话，我们也可以考虑用DP解法来做，我们建立一个三维dp数组，其中dp[i][j][k]表示从开头遍历到数字nums[i][j]为止，第k种情况的连续1的个数，k的值为0，1，2，3，分别对应水平，竖直，对角线和逆对角线这四种情况。之后就是更新dp数组的过程了，如果如果数字为0的情况直接跳过，然后水平方向就加上前一个的dp值，竖直方向加上上面一个数字的dp值，对角线方向就加上右上方数字的dp值，逆对角线就加上左上方数字的dp值，然后每个值都用来更新结果res，参见代码如下：



解法二：

```
class Solution {
public:
    int longestLine(vector<vector<int>>& M) {
        if (M.empty() || M[0].empty()) return 0;
        int m = M.size(), n = M[0].size(), res = 0;
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(4)));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (M[i][j] == 0) continue;
                for (int k = 0; k < 4; ++k) dp[i][j][k] = 1;
                if (j > 0) dp[i][j][0] += dp[i][j - 1][0]; // horizonal
                if (i > 0) dp[i][j][1] += dp[i - 1][j][1]; // vertical
                if (i > 0 && j < n - 1) dp[i][j][2] += dp[i - 1][j + 1][2]; // diagonal
                if (i > 0 && j > 0) dp[i][j][3] += dp[i - 1][j - 1][3]; // anti-diagonal
                res = max(res, max(dp[i][j][0], dp[i][j][1]));
                res = max(res, max(dp[i][j][2], dp[i][j][3]));
            }
        }
        return res;
    }
};
```



下面我们来优化空间复杂度，用一种类似于DFS的思路来解决问题，我们在遍历到为1的点时，对其水平方向，竖直方向，对角线方向和逆对角线方向分别不停遍历，直到越界或者遇到为0的数字，同时用计数器来累计1的个数，这样就可以用来更新结果res了，就不用把每个中间结果都保存下来了，参见代码如下：



解法三：

```
class Solution {
public:
    int longestLine(vector<vector<int>>& M) {
        if (M.empty() || M[0].empty()) return 0;
        int m = M.size(), n = M[0].size(), res = 0;
        vector<vector<int>> dirs{{1,0},{0,1},{-1,-1},{-1,1}};
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (M[i][j] == 0) continue;
                for (int k = 0; k < 4; ++k) {
                    int cnt = 0, x = i, y = j;
                    while (x >= 0 && x < m && y >= 0 && y < n && M[x][y] == 1) {
                        x += dirs[k][0];
                        y += dirs[k][1];
                        ++cnt;
                    }
                    res = max(res, cnt);
                }
            }
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/87231/dfs-straightforward](https://discuss.leetcode.com/topic/87231/dfs-straightforward)

[https://discuss.leetcode.com/topic/87197/java-o-nm-time-dp-solution](https://discuss.leetcode.com/topic/87197/java-o-nm-time-dp-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













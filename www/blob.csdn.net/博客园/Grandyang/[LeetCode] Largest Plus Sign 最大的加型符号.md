# [LeetCode] Largest Plus Sign 最大的加型符号 - Grandyang - 博客园







# [[LeetCode] Largest Plus Sign 最大的加型符号](https://www.cnblogs.com/grandyang/p/8679286.html)









In a 2D `grid` from (0, 0) to (N-1, N-1), every cell contains a `1`, except those cells in the given list `mines` which are `0`. What is the largest axis-aligned plus sign of `1`s contained in the grid? Return the order of the plus sign. If there is none, return 0.

An "*axis-aligned plus sign of `1`s* of order k" has some center `grid[x][y] = 1` along with 4 arms of length `k-1`going up, down, left, and right, and made of `1`s. This is demonstrated in the diagrams below. Note that there could be `0`s or `1`s beyond the arms of the plus sign, only the relevant area of the plus sign is checked for 1s.



Examples of Axis-Aligned Plus Signs of Order k:
Order 1:
000
010
000

Order 2:
00000
00100
01110
00100
00000

Order 3:
0000000
0001000
0001000
0111110
0001000
0001000
0000000



Example 1:
Input: N = 5, mines = [[4, 2]]
Output: 2
Explanation:
11111
11111
11111
11111
11011
In the above grid, the largest plus sign can only be order 2.  One of them is marked in bold.



Example 2:
Input: N = 2, mines = []
Output: 1
Explanation:
There is no plus sign of order 2, but there is of order 1.



Example 3:
Input: N = 1, mines = [[0, 0]]
Output: 0
Explanation:
There is no plus sign, so return 0.



Note:
- `N` will be an integer in the range `[1, 500]`.
- `mines` will have length at most `5000`.
- `mines[i]` will be length 2 and consist of integers in the range `[0, N-1]`.
- *(Additionally, programs submitted in C, C++, or C# will be judged with a slightly smaller time limit.)*





这道题给了我们一个数字N，表示一个NxN的二位数字，初始化均为1，又给了一个mines数组，里面是一些坐标，表示数组中这些位置都为0，然后让我们找最大的加型符号。所谓的加型符号是有数字1组成的一个十字型的加号，题目中也给出了长度分别为1，2，3的加型符号的样子。好，理解了题意以后，我们来想想该如何破题。首先，最简单的就是考虑暴力搜索啦，以每个1为中心，向四个方向分别去找，只要任何一个方向遇到了0就停止，然后更新结果res。令博主感到惊讶的是，此题的OJ居然允许Brute Force的解法通过，还是比较大度的，参见代码如下：



解法一：

```
class Solution {
public:
    int orderOfLargestPlusSign(int N, vector<vector<int>>& mines) {
        int res = 0;
        vector<vector<int>> mat(N, vector<int>(N, 1));
        for (auto mine : mines) mat[mine[0]][mine[1]] = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int k = 0;
                while (canExpand(mat, N, i, j, k)) ++k;
                res = max(res, k);
            }
        }
        return res;
    }
    bool canExpand(vector<vector<int>>& mat, int N, int x, int y, int k) {
        if (x - k < 0 || y - k < 0 || x + k >= N || y + k >= N) return false;
        return mat[x - k][y] && mat[x][y + k] && mat[x + k][y] && mat[x][y - k];
    }
};
```



如果我们只想出暴力搜索的解法，就不再管这道题了的话，那在面试的时候就比较悬了。毕竟立方级的时间复杂度实在是太高了，我们必须要进行优化。暴力搜索的时间复杂度之所以高的原因是因为对于每一个1都要遍历其上下左右四个方向，有大量的重复计算，我们为了提高效率，可以对于每一个点，都计算好其上下左右连续1的个数。博主最先用的方法是建立四个方向的dp数组，dp[i][j]表示 (i, j) 位置上该特定方向连续1的个数，那么就需要4个二维dp数组，举个栗子，比如：

原数组：

```
1  0  1  0
1  1  1  1
1  0  1  1
```

那么我们建立left数组是当前及其左边连续1的个数，如下所示：

```
1  0  1  0
1  2  3  4
1  0  1  2
```

right数组是当前及其右边连续1的个数，如下所示：

```
1  0  1  0
4  3  2  1
1  0  2  1
```

up数组是当前及其上边连续1的个数，如下所示：

```
1  0  1  0
2  1  2  1
3  0  3  2
```

down数组是当前及其下边连续1的个数，如下所示：

```
3  0  3  0
2  1  2  2
1  0  1  1
```

我们需要做的是在这四个dp数组中的相同位置的四个值中取最小的一个，然后在所有的这些去除的最小值中选最大一个返回即可。为了节省空间，我们不用四个二维dp数组，而只用一个就可以了，因为对于每一个特定位置，我们只需要保留较小值，所以在更新的时候，只需要跟原来值相比取较小值即可。在计算down数组的时候，我们就可以直接更新结果res了，因为四个值都已经计算过了，我们就不用再重新在外面开for循环了，参见代码如下：



解法二：

```
class Solution {
public:
    int orderOfLargestPlusSign(int N, vector<vector<int>>& mines) {
        int res = 0, cnt = 0;
        vector<vector<int>> dp(N, vector<int>(N, 0));
        unordered_set<int> s;
        for (auto mine : mines) s.insert(mine[0] * N + mine[1]);
        for (int j = 0; j < N; ++j) {
            cnt = 0;
            for (int i = 0; i < N; ++i) { // up
                cnt = s.count(i * N + j) ? 0 : cnt + 1;
                dp[i][j] = cnt;
            }
            cnt = 0;
            for (int i = N - 1; i >= 0; --i) { // down
                cnt = s.count(i * N + j) ? 0 : cnt + 1;
                dp[i][j] = min(dp[i][j], cnt);
            }
        }
        for (int i = 0; i < N; ++i) {
            cnt = 0;
            for (int j = 0; j < N; ++j) { // left
                cnt = s.count(i * N + j) ? 0 : cnt + 1;
                dp[i][j] = min(dp[i][j], cnt);
            }
            cnt = 0;
            for (int j = N - 1; j >= 0; --j) { // right
                cnt = s.count(i * N + j) ? 0 : cnt + 1;
                dp[i][j] = min(dp[i][j], cnt);
                res = max(res, dp[i][j]);
            }
        }
        return res;
    }
};
```



我们可以进一步的压缩代码，使其更加简洁，我们发现其实只要分别用四个变量l，r，u，d来表示四个方向连续1的个数，既可以将for循环糅合在一起。注意里面内嵌的for循环其实是两个for循环，由j和k分别控制，那么只要弄清i，j，k坐标的位置，就可以同时更新四个方向的dp值了，最后dp数组更新好了之后，我们再秀一波，只用一个for循环来遍历二维数组，其实就是把二维坐标压缩成了一个数字，再解压缩，参见代码如下：



解法三：

```
class Solution {
public:
    int orderOfLargestPlusSign(int N, vector<vector<int>>& mines) {
        int res = 0;
        vector<vector<int>> dp(N, vector<int>(N, N));
        for (auto mine : mines) dp[mine[0]][mine[1]] = 0;
        for (int i = 0; i < N; ++i) {
            int l = 0, r = 0, u = 0, d = 0;
            for (int j = 0, k = N - 1; j < N; ++j, --k) {
                dp[i][j] = min(dp[i][j], l = (dp[i][j] ? l + 1 : 0));
                dp[j][i] = min(dp[j][i], u = (dp[j][i] ? u + 1 : 0));
                dp[i][k] = min(dp[i][k], r = (dp[i][k] ? r + 1 : 0));
                dp[k][i] = min(dp[k][i], d = (dp[k][i] ? d + 1 : 0));
            }
        }
        for (int k = 0; k < N * N; ++k) res = max(res, dp[k / N][k % N]);
        return res;
    }
};
```



类似题目：

Cheapest Flights Within K Stops

Minimum Swaps To Make Sequences Increasing

Soup Servings



参考资料：

[https://leetcode.com/problems/largest-plus-sign/solution/](https://leetcode.com/problems/largest-plus-sign/solution/)

[https://leetcode.com/problems/largest-plus-sign/discuss/113314/JavaC++Python-O(N2)-solution-using-only-one-grid-matrix](https://leetcode.com/problems/largest-plus-sign/discuss/113314/JavaC++Python-O(N2)-solution-using-only-one-grid-matrix)

[https://leetcode.com/problems/largest-plus-sign/discuss/113350/C++-simple-brute-force-easy-to-understand-with-detailed-explanation](https://leetcode.com/problems/largest-plus-sign/discuss/113350/C++-simple-brute-force-easy-to-understand-with-detailed-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













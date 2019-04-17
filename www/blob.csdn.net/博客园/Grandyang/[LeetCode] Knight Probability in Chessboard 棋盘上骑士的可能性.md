# [LeetCode] Knight Probability in Chessboard 棋盘上骑士的可能性 - Grandyang - 博客园







# [[LeetCode] Knight Probability in Chessboard 棋盘上骑士的可能性](https://www.cnblogs.com/grandyang/p/7639153.html)








On an `N`x`N` chessboard, a knight starts at the `r`-th row and `c`-th column and attempts to make exactly `K` moves. The rows and columns are 0 indexed, so the top-left square is `(0, 0)`, and the bottom-right square is `(N-1, N-1)`.

A chess knight has 8 possible moves it can make, as illustrated below. Each move is two squares in a cardinal direction, then one square in an orthogonal direction.
![](https://leetcode.com/static/images/problemset/knight.png)
Each time the knight is to move, it chooses one of eight possible moves uniformly at random (even if the piece would go off the chessboard) and moves there.

The knight continues moving until it has made exactly `K` moves or has moved off the chessboard. Return the probability that the knight remains on the board after it has stopped moving.

Example:
Input: 3, 2, 0, 0
Output: 0.0625
Explanation: There are two moves (to (1,2), (2,1)) that will keep the knight on the board.
From each of those positions, there are also two moves that will keep the knight on the board.
The total probability the knight stays on the board is 0.0625.



Note:
- `N` will be between 1 and 25.
- `K` will be between 0 and 100.
- The knight always initially starts on the board.



这道题给了我们一个大小为NxN国际象棋棋盘，上面有个骑士，相当于我们中国象棋中的马，能走‘日’字，给了我们一个起始位置，然后说允许我们走K步，问走完K步之后还能留在棋盘上的概率是多少。那么要求概率，我们必须要先分别求出分子和分母，其中分子是走完K步还在棋盘上的走法，分母是没有限制条件的总共的走法。那么分母最好算，每步走有8种跳法，那么K步就是8的K次方种了。关键是要求出分子，博主开始向的方法是以给定位置为起始点，然后进行BFS，每步遍历8种情况，遇到在棋盘上的就计数器加1，结果TLE了。上论坛看大家的解法，结果发现都是换了一个角度来解决问题的，并不很关心骑士的起始位置，而是把棋盘上所有位置上经过K步还留在棋盘上的走法总和都算出来，那么最后直接返回需要的值即可。跟之前那道[Out of Boundary Paths](http://www.cnblogs.com/grandyang/p/6927921.html)没啥本质上的区别，又是换了一个马甲就不会了系列。还是要用DP来做，我们可以用三维DP数组，也可以用二维DP数组来做，这里为了省空间，我们就用二维DP数组来做，其中dp[i][j]表示在棋盘(i, j)位置上走完当前步骤还留在棋盘上的走法总和，初始化为1，我们其实将步骤这个维度当成了时间维度在不停更新。好，下面我们先写出8种‘日’字走法的位置的坐标，就像之前遍历迷宫上下左右四个方向坐标一样，这不过这次位置变了而已。然后我们一步一步来遍历，每一步都需要完整遍历一遍棋盘的每个位置，新建一个临时数组t，大小和dp数组相同，但是初始化为0，然后对于遍历到的棋盘上的每一个格子，我们都遍历8中解法，如果新的位置不在棋盘上了，直接跳过，否则就加上上一步中的dp数组中对应的值，遍历完棋盘后，将dp数组更新为这个临时数组t，参见代码如下：



解法一：

```
class Solution {
public:
    double knightProbability(int N, int K, int r, int c) {
        if (K == 0) return 1;
        vector<vector<double>> dp(N, vector<double>(N, 1));
        vector<vector<int>> dirs{{-1,-2},{-2,-1},{-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2}};
        for (int m = 0; m < K; ++m) {
            vector<vector<double>> t(N, vector<double>(N, 0));
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    for (auto dir : dirs) {
                        int x = i + dir[0], y = j + dir[1];
                        if (x < 0 || x >= N || y < 0 || y >= N) continue;
                        t[i][j] += dp[x][y];
                    }
                }
            }
            dp = t;
        }
        return dp[r][c] / pow(8, K);
    }
};
```



我们也可以使用有memo数组优化的递归来做，避免重复运算，从而能通过OJ。递归下的memo数组其实就是迭代下的dp数组，这里我们用三维的数组，初始化为0。在递归函数中，如果k为0了，说明已经走了k步，返回 1。如果memo[k][r][c]不为0，说明这种情况之前已经计算过，直接返回。然后遍历8种走法，计算新的位置，如果不在棋盘上就跳过；然后更新memo[k][r][c]，使其加上对新位置调用递归的返回值，注意此时带入k-1和新的位置，退出循环后返回memo[k][r][c]即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> dirs{{-1,-2},{-2,-1},{-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2}};
    double knightProbability(int N, int K, int r, int c) {
        vector<vector<vector<double>>> memo(K + 1, vector<vector<double>>(N, vector<double>(N, 0.0)));
        return helper(memo, N, K, r, c) / pow(8, K);
    }
    double helper(vector<vector<vector<double>>>& memo, int N, int k, int r, int c) {
        if (k == 0) return 1.0;
        if (memo[k][r][c] != 0.0) return memo[k][r][c];
        for (auto dir : dirs) {
            int x = r + dir[0], y = c + dir[1];
            if (x < 0 || x >= N || y < 0 || y >= N) continue;
            memo[k][r][c] += helper(memo, N, k - 1, x, y);
        }
        return memo[k][r][c];
    }
};
```



类似题目：

[Out of Boundary Paths](http://www.cnblogs.com/grandyang/p/6927921.html)



参考资料：

[https://discuss.leetcode.com/topic/105571/my-accepted-dp-solution](https://discuss.leetcode.com/topic/105571/my-accepted-dp-solution)

[https://discuss.leetcode.com/topic/105597/c-java-dp-concise-solution](https://discuss.leetcode.com/topic/105597/c-java-dp-concise-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














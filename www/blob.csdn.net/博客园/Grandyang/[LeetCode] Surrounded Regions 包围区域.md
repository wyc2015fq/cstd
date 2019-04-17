# [LeetCode] Surrounded Regions 包围区域 - Grandyang - 博客园







# [[LeetCode] Surrounded Regions 包围区域](https://www.cnblogs.com/grandyang/p/4555831.html)







Given a 2D board containing `'X'` and `'O'`, capture all regions surrounded by `'X'`.

A region is captured by flipping all `'O'`s into `'X'`s in that surrounded region.

 For example,

X X X X
X O O X
X X O X
X O X X

After running your function, the board should be:
X X X X
X X X X
X X X X
X O X X



这是道关于XXOO的题，有点像围棋，将包住的O都变成X，但不同的是边缘的O不算被包围，跟之前那道 [Number of Islands](http://www.cnblogs.com/grandyang/p/4402656.html) 很类似，都可以用DFS来解。刚开始我的思路是DFS遍历中间的O，如果没有到达边缘，都变成X，如果到达了边缘，将之前变成X的再变回来。但是这样做非常的不方便，在网上看到大家普遍的做法是扫面矩阵的四条边，如果有O，则用DFS遍历，将所有连着的O都变成另一个字符，比如\$，这样剩下的O都是被包围的，然后将这些O变成X，把$变回O就行了。代码如下：



解法一：

```
class Solution {
public:
    void solve(vector<vector<char> >& board) {
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[i].size(); ++j) {
                if ((i == 0 || i == board.size() - 1 || j == 0 || j == board[i].size() - 1) && board[i][j] == 'O')
                    solveDFS(board, i, j);
            }
        }
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == 'O') board[i][j] = 'X';
                if (board[i][j] == '$') board[i][j] = 'O';
            }
        }
    }
    void solveDFS(vector<vector<char> > &board, int i, int j) {
        if (board[i][j] == 'O') {
            board[i][j] = '$';
            if (i > 0 && board[i - 1][j] == 'O') 
                solveDFS(board, i - 1, j);
            if (j < board[i].size() - 1 && board[i][j + 1] == 'O') 
                solveDFS(board, i, j + 1);
            if (i < board.size() - 1 && board[i + 1][j] == 'O') 
                solveDFS(board, i + 1, j);
            if (**j > 1** && board[i][j - 1] == 'O') 
                solveDFS(board, i, j - 1);
        }
    }
};
```



有网友提问上面的代码中红色部分为啥是j > 1 而不是j > 0，为啥j > 0无法通过OJ的最后一个大数据集合，我开始也不知道其中奥秘，直到被另一个网友提醒在本地机子上可以通过最后一个大数据集合，于是我也写了一个程序来验证，请参见[验证LeetCode Surrounded Regions 包围区域的DFS方法](http://www.cnblogs.com/grandyang/p/4759189.html)。发现j > 0是正确的，可以得到相同的结果。

下面这种解法还是DFS解法，只是递归函数的写法稍有不同，但是本质上并没有太大的区别，参见代码如下：



解法二：

```
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty() || board[0].empty()) return;
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == 0 || i == m - 1 || j == 0 || j == n - 1) {
                    if (board[i][j] == 'O') dfs(board, i , j);
                }
            }   
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (board[i][j] == 'O') board[i][j] = 'X';
                if (board[i][j] == '$') board[i][j] = 'O';
            }
        }
    }
    void dfs(vector<vector<char>> &board, int x, int y) {
        int m = board.size(), n = board[0].size();
        vector<vector<int>> dir{{0,-1},{-1,0},{0,1},{1,0}};
        board[x][y] = '$';
        for (int i = 0; i < dir.size(); ++i) {
            int dx = x + dir[i][0], dy = y + dir[i][1];
            if (dx >= 0 && dx < m && dy > 0 && dy < n && board[dx][dy] == 'O') {
                dfs(board, dx, dy);
            }
        }
    }
};
```



我们也可以使用迭代的解法，但是整体的思路还是一样的，我们在找到边界上的O后，然后利用队列queue进行BFS查找和其相连的所有O，然后都标记上美元号。最后的处理还是先把所有的O变成X，然后再把美元号变回O即可，参见代码如下：



解法三：

```
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty() || board[0].empty()) return;
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != 0 && i != m - 1 && j != 0 && j != n - 1) continue;
                if (board[i][j] != 'O') continue;
                board[i][j] = '$';
                queue<int> q{{i * n + j}};
                while (!q.empty()) {
                    int t = q.front(), x = t / n, y = t % n; q.pop();
                    if (x >= 1 && board[x - 1][y] == 'O') {board[x - 1][y] = '$'; q.push(t - n);}
                    if (x < m - 1 && board[x + 1][y] == 'O') {board[x + 1][y] = '$'; q.push(t + n);}
                    if (y >= 1 && board[x][y - 1] == 'O') {board[x][y - 1] = '$'; q.push(t - 1);}
                    if (y < n - 1 && board[x][y + 1] == 'O') {board[x][y + 1] = '$'; q.push(t + 1);}
                }
            }
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (board[i][j] == 'O') board[i][j] = 'X';
                if (board[i][j] == '$') board[i][j] = 'O';
            }
        }
    }
};
```



类似题目：

[Number of Islands](http://www.cnblogs.com/grandyang/p/4402656.html)

[Walls and Gates](http://www.cnblogs.com/grandyang/p/5285868.html)



参考资料：

[https://leetcode.com/problems/surrounded-regions/](https://leetcode.com/problems/surrounded-regions/)

[https://leetcode.com/problems/surrounded-regions/discuss/41895/Share-my-clean-Java-Code](https://leetcode.com/problems/surrounded-regions/discuss/41895/Share-my-clean-Java-Code)

[https://leetcode.com/problems/surrounded-regions/discuss/41825/Simple-BFS-solution-easy-to-understand](https://leetcode.com/problems/surrounded-regions/discuss/41825/Simple-BFS-solution-easy-to-understand)

[https://leetcode.com/problems/surrounded-regions/discuss/41612/A-really-simple-and-readable-C%2B%2B-solutionuff0conly-cost-12ms](https://leetcode.com/problems/surrounded-regions/discuss/41612/A-really-simple-and-readable-C%2B%2B-solutionuff0conly-cost-12ms)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













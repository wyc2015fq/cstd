# [LeetCode] Max Area of Island 岛的最大面积 - Grandyang - 博客园







# [[LeetCode] Max Area of Island 岛的最大面积](https://www.cnblogs.com/grandyang/p/7712724.html)








Given a non-empty 2D array `grid` of 0's and 1's, an island is a group of `1`'s (representing land) connected 4-directionally (horizontal or vertical.) You may assume all four edges of the grid are surrounded by water.

Find the maximum area of an island in the given 2D array. (If there is no island, the maximum area is 0.)

Example 1:
[[0,0,1,0,0,0,0,1,0,0,0,0,0],
 [0,0,0,0,0,0,0,1,1,1,0,0,0],
 [0,1,1,0,1,0,0,0,0,0,0,0,0],
 [0,1,0,0,1,1,0,0,1,0,1,0,0],
 [0,1,0,0,1,1,0,0,1,1,1,0,0],
 [0,0,0,0,0,0,0,0,0,0,1,0,0],
 [0,0,0,0,0,0,0,1,1,1,0,0,0],
 [0,0,0,0,0,0,0,1,1,0,0,0,0]]

Given the above grid, return `6`. Note the answer is not 11, because the island must be connected 4-directionally.



Example 2:
[[0,0,0,0,0,0,0,0]]
Given the above grid, return `0`.



Note: The length of each dimension in the given `grid` does not exceed 50.



这道题跟之前的那两道[Number of Islands](http://www.cnblogs.com/grandyang/p/4402656.html)和[Number of Distinct Islands](http://www.cnblogs.com/grandyang/p/7698778.html)是同一个类型的，只不过这次需要统计出每个岛的大小，再来更新结果res。先用递归来做，遍历grid，当遇到为1的点，我们调用递归函数，在递归函数中，我们首先判断i和j是否越界，还有grid[i][j]是否为1，我们没有用visited数组，而是直接修改了grid数组，遍历过的标记为-1。如果合法，那么cnt自增1，并且更新结果res，然后对其周围四个相邻位置分别调用递归函数即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), res = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] != 1) continue;
                int cnt = 0;
                helper(grid, i, j, cnt, res);
            }
        }
        return res;
    }
    void helper(vector<vector<int>>& grid, int i, int j, int& cnt, int& res) {
        int m = grid.size(), n = grid[0].size();
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] <= 0) return;
        res = max(res, ++cnt);
        grid[i][j] *= -1;
        for (auto dir : dirs) {
            helper(grid, i + dir[0], j + dir[1], cnt, res);
        }
    }
};
```



下面是迭代的写法，BFS遍历，使用queue来辅助运算，思路没啥太大区别，都是套路，都是模版，往里套就行了，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), res = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] != 1) continue;
                int cnt = 0;
                queue<pair<int, int>> q{{{i, j}}};
                grid[i][j] *= -1;
                while (!q.empty()) {
                    auto t = q.front(); q.pop();
                    res = max(res, ++cnt);
                    for (auto dir : dirs) {
                        int x = t.first + dir[0], y = t.second + dir[1];
                        if (x < 0 || x >= m || y < 0 || y >= n || grid[x][y] <= 0) continue;
                        grid[x][y] *= -1;
                        q.push({x, y});
                    }
                }
            }
        }
        return res;
    }
};
```



类似题目：

[Number of Distinct Islands](http://www.cnblogs.com/grandyang/p/7698778.html)

[Island Perimeter](http://www.cnblogs.com/grandyang/p/6096138.html)

[Number of Islands](http://www.cnblogs.com/grandyang/p/4402656.html)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













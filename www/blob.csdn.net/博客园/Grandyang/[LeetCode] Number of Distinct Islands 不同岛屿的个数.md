# [LeetCode] Number of Distinct Islands 不同岛屿的个数 - Grandyang - 博客园







# [[LeetCode] Number of Distinct Islands 不同岛屿的个数](https://www.cnblogs.com/grandyang/p/7698778.html)







Given a non-empty 2D array `grid` of 0's and 1's, an island is a group of `1`'s (representing land) connected 4-directionally (horizontal or vertical.) You may assume all four edges of the grid are surrounded by water.

Count the number of distinct islands. An island is considered to be the same as another if and only if one island can be translated (and not rotated or reflected) to equal the other.

Example 1:
11000
11000
00011
00011

Given the above grid map, return `1`.



Example 2:
11011
10000
00001
11011
Given the above grid map, return `3`.

Notice that:
11
1

and
 1
11

are considered different island shapes, because we do not consider reflection / rotation.



Note: The length of each dimension in the given `grid` does not exceed 50.



这道题让我们求不同岛屿的个数，是之前那道[Number of Islands](http://www.cnblogs.com/grandyang/p/4402656.html)的拓展，这道题的难点是如何去判断两个岛屿是否是不同的岛屿，首先1的个数肯定是要相同，但是1的个数相同不能保证一定是相同的岛屿，比如例子2中的那两个岛屿的就不相同，就是说两个相同的岛屿通过平移可以完全重合，但是不能旋转。那么我们如何来判断呢，我们发现可以通过相对位置坐标来判断，比如我们使用岛屿的最左上角的1当作基点，那么基点左边的点就是(0,-1)，右边的点就是(0,1), 上边的点就是(-1,0)，下面的点就是(1,0)。那么例子1中的两个岛屿都可以表示为[(0,0), (0,1), (1,0), (1,1)]，点的顺序是基点-右边点-下边点-右下点。通过这样就可以判断两个岛屿是否相同了，下面这种解法我们没有用数组来存，而是encode成了字符串，比如这四个点的数组就存为"0_0_0_1_1_0_1_1_"，然后把字符串存入集合unordered_set中，利用其自动去重复的特性，就可以得到不同的岛屿的数量啦，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
    int numDistinctIslands(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        unordered_set<string> res;
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 1 && !visited[i][j]) {
                    set<string> s;
                    helper(grid, i, j, i, j, visited, s);
                    string t = "";
                    for (auto str : s) t += str + "_";
                    res.insert(t);
                }
            }
        }
        return res.size();
    }
    void helper(vector<vector<int>>& grid, int x0, int y0, int i, int j, vector<vector<bool>>& visited, set<string>& s) {
        int m = grid.size(), n = grid[0].size();
        visited[i][j] = true;
        for (auto dir : dirs) {
            int x = i + dir[0], y = j + dir[1];
            if (x < 0 || x >= m || y < 0 || y >= n || grid[x][y] == 0 || visited[x][y]) continue;
            string str = to_string(x - x0) + "_" + to_string(y - y0);
            s.insert(str);
            helper(grid, x0, y0, x, y, visited, s);
        }
    }
};
```



当然我们也可以不encode字符串，直接将相对坐标存入数组中，然后把整个数组放到集合set中，还是会去掉相同的数组，而且这种解法直接在grid数组上标记访问过的位置，写起来更加简洁了，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
    int numDistinctIslands(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        set<vector<pair<int, int>>> res;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] != 1) continue;
                vector<pair<int, int>> v;
                helper(grid, i, j, i, j, v);
                res.insert(v);
            }
        }
        return res.size();
    }
    void helper(vector<vector<int>>& grid, int x0, int y0, int i, int j, vector<pair<int, int>>& v) {
        int m = grid.size(), n = grid[0].size();
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] <= 0) return;
        grid[i][j] *= -1;
        v.push_back({i - x0, j - y0});
        for (auto dir : dirs) {
            helper(grid, x0, y0, i + dir[0], j + dir[1], v);
        }
    }
};
```



既然递归DFS可以，那么迭代的BFS就坐不住了，其实思路没什么区别，这种类似迷宫遍历的题都是一个套路，整体框架都很像，细枝末节需要改改就行了，参见代码如下：



解法三：

```
class Solution {
public:
    vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
    int numDistinctIslands(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        set<vector<pair<int, int>>> res;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] != 1) continue;
                vector<pair<int, int>> v;
                queue<pair<int, int>> q{{{i, j}}};
                grid[i][j] *= -1;
                while (!q.empty()) {
                    auto t = q.front(); q.pop();
                    for (auto dir : dirs) {
                        int x = t.first + dir[0], y = t.second + dir[1];
                        if (x < 0 || x >= m || y < 0 || y >= n || grid[x][y] <= 0) continue;
                        q.push({x, y});
                        grid[x][y] *= -1;
                        v.push_back({x - i, y - j});
                    }
                }
                res.insert(v);
            }
        }
        return res.size();
    }
};
```



类似题目：

[Number of Islands](http://www.cnblogs.com/grandyang/p/4402656.html)



参考资料：

[https://discuss.leetcode.com/topic/106264/java-c-clean-code](https://discuss.leetcode.com/topic/106264/java-c-clean-code)

[https://discuss.leetcode.com/topic/106276/c-bfs-set-with-explanation](https://discuss.leetcode.com/topic/106276/c-bfs-set-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













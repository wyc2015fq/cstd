# [LeetCode] Swim in Rising Water 在上升的水中游泳 - Grandyang - 博客园







# [[LeetCode] Swim in Rising Water 在上升的水中游泳](https://www.cnblogs.com/grandyang/p/9017300.html)







On an N x N `grid`, each square `grid[i][j]` represents the elevation at that point `(i,j)`.

Now rain starts to fall. At time `t`, the depth of the water everywhere is `t`. You can swim from a square to another 4-directionally adjacent square if and only if the elevation of both squares individually are at most `t`. You can swim infinite distance in zero time. Of course, you must stay within the boundaries of the grid during your swim.

You start at the top left square `(0, 0)`. What is the least time until you can reach the bottom right square `(N-1, N-1)`?

Example 1:
Input: [[0,2],[1,3]]
Output: 3
Explanation:
At time `0`, you are in grid location `(0, 0)`.
You cannot go anywhere else because 4-directionally adjacent neighbors have a higher elevation than t = 0.

You cannot reach point `(1, 1)` until time `3`.
When the depth of water is `3`, we can swim anywhere inside the grid.

Example 2:
Input: [[0,1,2,3,4],[24,23,22,21,5],[12,13,14,15,16],[11,17,18,19,20],[10,9,8,7,6]]
Output: 16
Explanation:
** 0  1  2  3  4**
24 23 22 21  **5****12 13 14 15 16****11** 17 18 19 20
**10  9  8  7  6**

The final route is marked in bold.
We need to wait until time 16 so that (0, 0) and (4, 4) are connected.

Note:
- `2 <= N <= 50`.
- grid[i][j] is a permutation of [0, ..., N*N - 1].



这道题给了我们一个二维数组，可以看作一个水池，这里不同数字的高度可以看作台阶的高度，只有当水面升高到台阶的高度时，我们才能到达该台阶，起始点在左上角位置，问我们水面最低升到啥高度就可以到达右下角的位置。这是一道蛮有意思的题目。对于这种类似迷宫遍历的题，一般都是DFS或者BFS。而如果有极值问题存在的时候，一般都是优先考虑BFS的，但是这道题比较特别，有一个上升水面的设定，我们可以想象一下，比如洪水爆发了，大坝垮了，那么愤怒汹涌的水流冲了出来，地势低洼处就会被淹没，而地势高的地方，比如山峰啥的，就会绕道而过。这里也是一样，随着水面不断的上升，低于水平面的地方就可以到达，直到水流到了右下角的位置停止。因为水流要向周围低洼处蔓延，所以BFS仍是一个不错的选择，由于水是向低洼处蔓延的，而低洼处的位置又是不定的，所以我们希望每次取出最低位置进行遍历，那么使用最小堆就是一个很好的选择，这样高度低的就会被先处理。在每次取出高度最小的数字时，我们用此高度来更新结果res，如果当前位置已经是右下角了，则我们直接返回结果res，否则就遍历当前位置的周围位置，如果未越界且未被访问过，则标记已经访问过，并且加入队列，参见代码如下：



解法一：

```
class Solution {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int res = 0, n = grid.size();
        unordered_set<int> visited{0};
        vector<vector<int>> dirs{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
        auto cmp = [](pair<int, int>& a, pair<int, int>& b) {return a.first > b.first;};
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp) > q(cmp);
        q.push({grid[0][0], 0});
        while (!q.empty()) {
            int i = q.top().second / n, j = q.top().second % n; q.pop();
            res = max(res, grid[i][j]);
            if (i == n - 1 && j == n - 1) return res;
            for (auto dir : dirs) {
                int x = i + dir[0], y = j + dir[1];
                if (x < 0 || x >= n || y < 0 || y >= n || visited.count(x * n + y)) continue;
                visited.insert(x * n + y);
                q.push({grid[x][y], x * n + y});
            }
        }
        return res;
    }
};
```



我们也可以使用DP+DFS来做，这里使用一个二维dp数组，其中 dp[i][j] 表示到达 (i, j) 位置所需要的最低水面高度，均初始化为整型数最大值，我们的递归函数函数需要知道当前的位置 (x, y)，还有当前的水高cur，同时传入grid数组和需要不停更新的dp数组，如果当前位置越界了，或者是当前水高和 grid[x][y] 中的较大值大于等于 dp[x][y] 了，直接跳过，因为此时的dp值更小，不需要被更新了。否则 dp[x][y] 更新为较大值，然后对周围四个位置调用递归函数继续更新dp数组，最终返回右下位置的dp值即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> dirs{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();
        vector<vector<int>> dp(n, vector<int>(n, INT_MAX));
        helper(grid, 0, 0, grid[0][0], dp);
        return dp[n - 1][n - 1];
    }
    void helper(vector<vector<int>>& grid, int x, int y, int cur, vector<vector<int>>& dp) {
        int n = grid.size();
        if (x < 0 || x >= n || y < 0 || y >= n || max(cur, grid[x][y]) >= dp[x][y]) return;
        dp[x][y] = max(cur, grid[x][y]);
        for (auto dir : dirs) {
            helper(grid, x + dir[0], y + dir[1], dp[x][y], dp);
        }
    }
};
```



其实这道题还可以使用二分搜索法来做，属于博主的总结帖中[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)的第四类，用子函数当作判断关系。由于题目中给定了数字的范围，那么二分搜索法的左右边界就有了，然后我们计算一个中间值mid，调用子函数来看这个水面高度下能否到达右下角，如果不能的话，说明水面高度不够，则 left = mid+1，如果能到达的话，有可能水面高度过高了，则right = mid，最终会到达的临界点就是能到达右下角的最低水面高度。那么来看子函数怎么写，其实就是个迷宫遍历问题，我们可以使用BFS或者DFS，这里使用了stack辅助的迭代形式的DFS来遍历，当然我们也可以使用queue辅助的迭代形式的BFS来遍历，都一样，如果在mid的水面高度下，遍历到了右下角，则返回true，否则返回false，参见代码如下：



解法三：

```
class Solution {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();
        int left = grid[0][0], right = n * n;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (!helper(grid, mid)) left = mid + 1;
            else right = mid;
        }
        return left;
    }
    bool helper(vector<vector<int>>& grid, int mid) {
        int n = grid.size();
        unordered_set<int> visited{0};
        vector<vector<int>> dirs{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
        stack<int> st{{0}};
        while (!st.empty()) {
            int i = st.top() / n, j = st.top() % n; st.pop();
            if (i == n - 1 && j == n - 1) return true;
            for (auto dir : dirs) {
                int x = i + dir[0], y = j + dir[1];
                if (x < 0 || x >= n || y < 0 || y >= n || visited.count(x * n + y) || grid[x][y] > mid) continue;
                st.push(x * n + y);
                visited.insert(x * n + y);
            }
        }
        return false;
    }
};
```



参考资料：

[https://leetcode.com/problems/swim-in-rising-water/solution/#](https://leetcode.com/problems/swim-in-rising-water/solution/#)

[https://leetcode.com/problems/swim-in-rising-water/discuss/113743/JAVA-DP-+-DFS](https://leetcode.com/problems/swim-in-rising-water/discuss/113743/JAVA-DP-+-DFS)

[https://leetcode.com/problems/swim-in-rising-water/discuss/113765/Easy-and-Concise-Solution-using-Binary-Search-PythonC++](https://leetcode.com/problems/swim-in-rising-water/discuss/113765/Easy-and-Concise-Solution-using-Binary-Search-PythonC++)

[https://leetcode.com/problems/swim-in-rising-water/discuss/113758/C++-two-solutions-Binary-Search+DFS-and-Dijkstra+BFS-O(n2logn)-11ms](https://leetcode.com/problems/swim-in-rising-water/discuss/113758/C++-two-solutions-Binary-Search+DFS-and-Dijkstra+BFS-O(n2logn)-11ms)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













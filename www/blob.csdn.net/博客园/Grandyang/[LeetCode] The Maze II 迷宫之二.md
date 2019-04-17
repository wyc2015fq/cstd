# [LeetCode] The Maze II 迷宫之二 - Grandyang - 博客园







# [[LeetCode] The Maze II 迷宫之二](https://www.cnblogs.com/grandyang/p/6725380.html)







There is a ball in a maze with empty spaces and walls. The ball can go through empty spaces by rolling up, down, left or right, but it won't stop rolling until hitting a wall. When the ball stops, it could choose the next direction.

Given the ball's start position, the destination and the maze, find the shortest distance for the ball to stop at the destination. The distance is defined by the number of empty spaces traveled by the ball from the start position (excluded) to the destination (included). If the ball cannot stop at the destination, return -1.

The maze is represented by a binary 2D array. 1 means the wall and 0 means the empty space. You may assume that the borders of the maze are all walls. The start and destination coordinates are represented by row and column indexes.

Example 1
Input 1: a maze represented by a 2D array

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

Input 2: start coordinate (rowStart, colStart) = (0, 4)
Input 3: destination coordinate (rowDest, colDest) = (4, 4)

Output: 12
Explanation: One shortest way is : left -> down -> left -> down -> right -> down -> right.
             The total distance is 1 + 1 + 3 + 1 + 2 + 2 + 2 = 12.

![](https://leetcode.com/static/images/problemset/maze_1_example_1.png)



Example 2
Input 1: a maze represented by a 2D array

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

Input 2: start coordinate (rowStart, colStart) = (0, 4)
Input 3: destination coordinate (rowDest, colDest) = (3, 2)

Output: -1
Explanation: There is no way for the ball to stop at the destination.

![](https://leetcode.com/static/images/problemset/maze_1_example_2.png)



Note:
- There is only one ball and one destination in the maze.
- Both the ball and the destination exist on an empty space, and they will not be at the same position initially.
- The given maze does not contain border (like the red rectangle in the example pictures), but you could assume the border of the maze are all walls.
- The maze contains at least 2 empty spaces, and both the width and height of the maze won't exceed 100.



这道题是之前那道 [The Maze](http://www.cnblogs.com/grandyang/p/6381458.html) 的拓展，那道题只让我们判断能不能在终点位置停下，而这道题让我们求出到达终点的最少步数。其实本质都是一样的，难点还是在于对于一滚到底的实现方法，唯一不同的是，这里我们用一个二位数组dists，其中dists[i][j]表示到达(i,j)这个位置时需要的最小步数，我们都初始化为整型最大值，在后在遍历的过程中不断用较小值来更新每个位置的步数值，最后我们来看终点位置的步数值，如果还是整型最大值的话，说明没法在终点处停下来，返回-1，否则就返回步数值。注意在压入栈的时候，我们对x和y进行了判断，只有当其不是终点的时候才压入栈，这样是做了优化，因为如果小球已经滚到终点了，我们就不要让它再滚了，就不把终点位置压入栈，免得它还滚，参见代码如下：



解法一：

```
class Solution {
public:
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> dists(m, vector<int>(n, INT_MAX));
        vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
        queue<pair<int, int>> q;
        q.push({start[0], start[1]});
        dists[start[0]][start[1]] = 0;
        while (!q.empty()) {
            auto t = q.front(); q.pop();
            for (auto d : dirs) {
                int x = t.first, y = t.second, dist = dists[t.first][t.second];
                while (x >= 0 && x < m && y >= 0 && y < n && maze[x][y] == 0) {
                    x += d[0];
                    y += d[1];
                    ++dist;
                }
                x -= d[0];
                y -= d[1];
                --dist;
                if (dists[x][y] > dist) {
                    dists[x][y] = dist;
                    if (x != destination[0] || y != destination[1]) q.push({x, y});
                }
            }
        }
        int res = dists[destination[0]][destination[1]];
        return (res == INT_MAX) ? -1 : res;
    }
};
```



下面这种写法是DFS的解法，可以看出来思路基本上跟上面的解法没有啥区别，写法上稍有不同，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> dists(m, vector<int>(n, INT_MAX));
        dists[start[0]][start[1]] = 0;
        helper(maze, start[0], start[1], destination, dists);
        int res = dists[destination[0]][destination[1]];
        return (res == INT_MAX) ? -1 : res;
    }
    void helper(vector<vector<int>>& maze, int i, int j, vector<int>& destination, vector<vector<int>>& dists) {
        if (i == destination[0] && j == destination[1]) return;
        int m = maze.size(), n = maze[0].size();
        for (auto d : dirs) {
            int x = i, y = j, dist = dists[x][y];
            while (x >= 0 && x < m && y >= 0 && y < n && maze[x][y] == 0) {
                x += d[0];
                y += d[1];
                ++dist;
            }
            x -= d[0];
            y -= d[1];
            --dist;
            if (dists[x][y] > dist) {
                dists[x][y] = dist;
                helper(maze, x, y, destination, dists);
            }
        }
    }
};
```



我们还可以使用迪杰克斯特拉算法Dijkstra Algorithm来做，LeetCode中能使用到此类高级算法的时候并不多，[Network Delay Time](http://www.cnblogs.com/grandyang/p/8278115.html) 就是一次。该算法是主要是在有向权重图中计算单源最短路径，即单个点到任意点到最短路径。因为这里起点只有一个，所以适用，然后迷宫中的每个格子都可以看作是图中的一个结点，权重可以都看成是1，那么就可以当作是有向权重图来处理。Dijkstra算法的核心是松弛操作Relaxtion，当有对边 (u, v) 是结点u到结点v，如果 dist(v) > dist(u) + w(u, v)，那么 dist(v) 就可以被更新，这是所有这些的算法的核心操作。Dijkstra算法是以起点为中心，向外层层扩展，直到扩展到终点为止。根据这特性，用BFS来实现时再好不过了。为了加快运算速度，我们使用一个优先队列（最小堆）来代替普通的queue，这样我们就能尽量先更新离起点近的位置的dp值，优先队列里同时也存了该点到起点的距离，这个距离不一定是最短距离，可能还能松弛。但是如果其dp值已经小于优先队列中保存的距离，那么就不必更新其周围位置的距离了，因为优先队列中保存的这个距离值不是最短的，使用它来更新周围的dp值没有意义。这相当于利用了松弛操作来进行剪枝，大大提高了运算效率，之后就是类似于之前的BFS的操作了，遍历其周围的四个位置，尝试去更新其dp值。最后还是跟之前一样，如果遍历到了终点，就不要再排入队列了，因为已经得到需要的结果了，参见代码如下：



解法三：

```
class Solution {
public:
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> dists(m, vector<int>(n, INT_MAX));
        vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
        auto cmp = [](vector<int>& a, vector<int>& b) {
            return a[2] > b[2];
        };
        priority_queue<vector<int>, vector<vector<int>>, decltype(cmp) > pq(cmp);
        pq.push({start[0], start[1], 0});
        dists[start[0]][start[1]] = 0;
        while (!pq.empty()) {
            auto t = pq.top(); pq.pop();
            if (dists[t[0]][t[1]] < t[2]) continue;
            for (auto dir : dirs) {
                int x = t[0], y = t[1], dist = dists[t[0]][t[1]];
                while (x >= 0 && x < m && y >= 0 && y < n && maze[x][y] == 0) {
                    x += dir[0];
                    y += dir[1];
                    ++dist;
                }
                x -= dir[0];
                y -= dir[1];
                --dist;
                if (dists[x][y] > dist) {
                    dists[x][y] = dist;
                    if (x != destination[0] || y != destination[1]) pq.push({x, y, dist});
                }
            }
        }
        int res = dists[destination[0]][destination[1]];
        return (res == INT_MAX) ? -1 : res;
    }
};
```



类似题目：

[The Maze](http://www.cnblogs.com/grandyang/p/6381458.html)

[The Maze III](http://www.cnblogs.com/grandyang/p/6746528.html)



参考资料：

[https://leetcode.com/problems/the-maze-ii/](https://leetcode.com/problems/the-maze-ii/)

[https://leetcode.com/problems/the-maze-ii/discuss/98401/java-accepted-dfs](https://leetcode.com/problems/the-maze-ii/discuss/98401/java-accepted-dfs)

[https://leetcode.com/problems/the-maze-ii/discuss/98456/simple-c-bfs-solution](https://leetcode.com/problems/the-maze-ii/discuss/98456/simple-c-bfs-solution)

[https://leetcode.com/problems/the-maze-ii/discuss/98427/2-Solutions:-BFS-and-Dijkstra's.-Detailed-explanation..-But-why-is-BFS-faster](https://leetcode.com/problems/the-maze-ii/discuss/98427/2-Solutions:-BFS-and-Dijkstra's.-Detailed-explanation..-But-why-is-BFS-faster)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













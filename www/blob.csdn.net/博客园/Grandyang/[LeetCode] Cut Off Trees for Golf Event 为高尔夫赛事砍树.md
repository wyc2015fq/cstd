# [LeetCode] Cut Off Trees for Golf Event 为高尔夫赛事砍树 - Grandyang - 博客园







# [[LeetCode] Cut Off Trees for Golf Event 为高尔夫赛事砍树](https://www.cnblogs.com/grandyang/p/8379506.html)









You are asked to cut off trees in a forest for a golf event. The forest is represented as a non-negative 2D map, in this map:
- `0` represents the `obstacle` can't be reached.
- `1` represents the `ground` can be walked through.
- `The place with number bigger than 1` represents a `tree` can be walked through, and this positive number represents the tree's height.



You are asked to cut off all the trees in this forest in the order of tree's height - always cut off the tree with lowest height first. And after cutting, the original place has the tree will become a grass (value 1).

You will start from the point (0, 0) and you should output the minimum steps you need to walk to cut off all the trees. If you can't cut off all the trees, output -1 in that situation.

You are guaranteed that no two `trees` have the same height and there is at least one tree needs to be cut off.

Example 1:
Input: 
[
 [1,2,3],
 [0,0,4],
 [7,6,5]
]
Output: 6



Example 2:
Input: 
[
 [1,2,3],
 [0,0,0],
 [7,6,5]
]
Output: -1



Example 3:
Input: 
[
 [2,3,4],
 [0,0,5],
 [8,7,6]
]
Output: 6
Explanation: You started from the point (0,0) and you can cut off the tree in (0,0) directly without walking.



Hint: size of the given matrix will not exceed 50x50.





这道题让我们砍掉所有高度大于1的树，而且要求是按顺序从低到高来砍，那么本质实际上还是要求任意两点之间的最短距离啊。对于这种类似迷宫遍历求最短路径的题，BFS是不二之选啊。那么这道题就对高度相邻的两棵树之间调用一个BFS，所以我们可以把BFS的内容放倒子函数helper中来使用。那么我们首先就要将所有的树从低到高进行排序，我们遍历原二位矩阵，将每棵树的高度及其横纵坐标取出来，组成一个三元组，然后放到vector中，之后用sort对数组进行排序，因为sort默认是以第一个数字排序，这也是为啥我们要把高度放在第一个位置。然后我们就遍历我们的trees数组，我们的起始位置是（0，0），终点位置是从trees数组中取出的树的位置，然后调用BFS的helper函数，这个BFS的子函数就是很基本的写法，没啥过多需要讲解的地方，会返回最短路径的值，如果无法到达目标点，就返回-1。所以我们先检查，如果helper函数返回-1了，那么我们就直接返回-1，否则就将cnt加到结果res中。然后更新我们的起始点为当前树的位置，然后循环取下一棵树即可，参见代码如下：



```
class Solution {
public:
    int cutOffTree(vector<vector<int>>& forest) {
        int m = forest.size(), n = forest[0].size(), res = 0, row = 0, col = 0;
        vector<vector<int>> trees;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (forest[i][j] > 1) trees.push_back({forest[i][j], i, j});
            }
        }
        sort(trees.begin(), trees.end());
        for (int i = 0; i < trees.size(); ++i) {
            int cnt = helper(forest, row, col, trees[i][1], trees[i][2]);
            if (cnt == -1) return -1;
            res += cnt;
            row = trees[i][1];
            col = trees[i][2];
        }
        return res;
    }
    int helper(vector<vector<int>>& forest, int row, int col, int treeRow, int treeCol) {
        if (row == treeRow && col == treeCol) return 0;
        int m = forest.size(), n = forest[0].size(), cnt = 0;
        queue<int> q{{row * n + col}};
        vector<vector<int>> visited(m, vector<int>(n));
        vector<int> dir{-1, 0, 1, 0, -1};
        while (!q.empty()) {
            ++cnt;
            for (int i = q.size(); i > 0; --i) {
                int r = q.front() / n, c = q.front() % n; q.pop();
                for (int k = 0; k < 4; ++k) {
                    int x = r + dir[k], y = c + dir[k + 1];
                    if (x < 0 || x >= m || y < 0 || y >= n || visited[x][y] == 1 || forest[x][y] == 0) continue;
                    if (x == treeRow && y == treeCol) return cnt;
                    visited[x][y] = 1;
                    q.push(x * n + y);
                }
            }
        }
        return -1;
    }
};
```



参考资料：

[https://leetcode.com/problems/cut-off-trees-for-golf-event/](https://leetcode.com/problems/cut-off-trees-for-golf-event/)

[https://leetcode.com/problems/cut-off-trees-for-golf-event/discuss/107403/c-sort-bfs-with-explanation](https://leetcode.com/problems/cut-off-trees-for-golf-event/discuss/107403/c-sort-bfs-with-explanation)

[https://leetcode.com/problems/cut-off-trees-for-golf-event/discuss/107404/java-solution-priorityqueue-bfs](https://leetcode.com/problems/cut-off-trees-for-golf-event/discuss/107404/java-solution-priorityqueue-bfs)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













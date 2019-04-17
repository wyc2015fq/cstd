# [LeetCode] Contain Virus 包含病毒 - Grandyang - 博客园







# [[LeetCode] Contain Virus 包含病毒](https://www.cnblogs.com/grandyang/p/8424780.html)







A virus is spreading rapidly, and your task is to quarantine the infected area by installing walls.

The world is modeled as a 2-D array of cells, where `0` represents uninfected cells, and `1` represents cells contaminated with the virus. A wall (and only one wall) can be installed between any two 4-directionally adjacent cells, on the shared boundary.

Every night, the virus spreads to all neighboring cells in all four directions unless blocked by a wall. Resources are limited. Each day, you can install walls around only one region -- the affected area (continuous block of infected cells) that threatens the most uninfected cells the following night. There will never be a tie.

Can you save the day? If so, what is the number of walls required? If not, and the world becomes fully infected, return the number of walls used.



Example 1:
Input: grid = 
[[0,1,0,0,0,0,0,1],
 [0,1,0,0,0,0,0,1],
 [0,0,0,0,0,0,0,1],
 [0,0,0,0,0,0,0,0]]
Output: 10
Explanation:
There are 2 contaminated regions.
On the first day, add 5 walls to quarantine the viral region on the left. The board after the virus spreads is:

[[0,1,0,0,0,0,1,1],
 [0,1,0,0,0,0,1,1],
 [0,0,0,0,0,0,1,1],
 [0,0,0,0,0,0,0,1]]

On the second day, add 5 walls to quarantine the viral region on the right. The virus is fully contained.



Example 2:
Input: grid = 
[[1,1,1],
 [1,0,1],
 [1,1,1]]
Output: 4
Explanation: Even though there is only one cell saved, there are 4 walls built.
Notice that walls are only built on the shared boundary of two different cells.



Example 3:
Input: grid = 
[[1,1,1,0,0,0,0,0,0],
 [1,0,1,0,1,1,1,1,1],
 [1,1,1,0,0,0,0,0,0]]
Output: 13
Explanation: The region on the left only builds two new walls.



Note:
- The number of rows and columns of `grid` will each be in the range `[1, 50]`.
- Each `grid[i][j]` will be either `0` or `1`.
- Throughout the described process, there is always a contiguous viral region that will infect strictly moreuncontaminated squares in the next round.



这道题给了我们一个由0和1组成的二维数组，其中0表示健康细胞，而1表示病毒细胞，多个相邻的1组成了病毒细胞群，每天都会向周围扩散一圈，除非我们在其跟健康细胞之间建立隔离墙，这样其才会不会扩散。我们每天只能给一个病毒细胞群来建立隔离墙，其他的细胞群会进行扩散。问最终我们需要多少个隔离墙。这道题真的挺难，博主研究了好久才弄明白题目的意思。首先要明白一点，病毒细胞只会向上下左右四个方向相邻的健康细胞扩散。需要注意的一点是，需要的隔离墙的数量可能大于周围相邻的健康细胞，最明显的就是例子2中，只有一个健康细胞，但是需要四个隔离墙才能保护这个健康细胞不被感染。还有就是，我们需要隔离某个病毒细胞群的判定依据是其能感染的健康细胞的数量，而不是需要建的墙的数量或者病毒细胞的个数，这点很重要，博主之前没有注意这一点，导致fail了一个test case。所以我们要做的就是要求出每个病毒细胞群能感染的健康细胞的数量，其周围能建墙的地方，以及每个病毒细胞的位置。我们再其中选择能感染最多健康细胞的病毒细胞群进行建墙，建完墙后，我们将该群中的所有病毒细胞标记为-1，跟其他细胞区分出来。对于其他所有的病毒细胞群，将其周围能建墙的地方(即健康细胞)都标记为1，表示其现在已经被感染成了病毒细胞。然后再进行新的一轮循环检测，直到无法找出新的病毒细胞为止。

我们先找值为1的点，找到后，以其作为起点，进行BFS遍历，将和其相连的所有为1的点都找出来，在BFS遍历的过程中，如果我们检测到周围位置值为0，将其加入walls数组，表示这里可以建隔离墙，如果检测到周围位置为1，将其加入virus数组，表示这里是病毒细胞，注意起始位置也要提前加入virus数组。我们这里为了节省维度，将二维的坐标都encode成了一个int数字。BFS遍历结束后，我们根据walls数组来算出能感染的健康细胞的个数，因为我们前面提到过建隔离墙的位置可能大于健康细胞的个数，所以我们只要去除wall数组的重复项即可，利用HashSet的去重复项原理，然后将剩下的个数放入cells数组中。把cells，walls，和virus数组放入一个vector中，表示一个病毒细胞群的信息，再放入一个大数组all中，这样我们收集了所有病毒细胞群的信息后，可以根据可感染的健康细胞个数由多到少来排序，这样我们就把第一个病毒细胞群中所有virus数组的位置值变为-1，并且把可感染的健康细胞个数累加到结果res中。然后把后面所有的病毒细胞群中walls的位置值都变为1即可。当all数组为空时，跳出循环，表示没有检测到病毒细胞群或者全部都被感染了，参见代码如下：



```
class Solution {
public:
    int containVirus(vector<vector<int>>& grid) {
        int res = 0, m = grid.size(), n = grid[0].size();
        vector<vector<int>> dirs{{-1,0},{0,1},{1,0},{0,-1}};
        while (true) {
            unordered_set<int> visited;
            vector<vector<vector<int>>> all;
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (grid[i][j] == 1 && !visited.count(i * n + j)) {
                        queue<int> q{{i * n + j}};
                        vector<int> virus{i * n + j};
                        vector<int> walls;
                        visited.insert(i * n + j);
                        while (!q.empty()) {
                            auto t = q.front(); q.pop();
                            for (auto dir : dirs) {
                                int x = (t / n) + dir[0], y = (t % n) + dir[1];
                                if (x < 0 || x >= m || y < 0 || y >= n || visited.count(x * n + y)) continue;
                                if (grid[x][y] == -1) continue;
                                else if (grid[x][y] == 0) walls.push_back(x * n + y);
                                else if (grid[x][y] == 1) {
                                    visited.insert(x * n + y);
                                    virus.push_back(x * n + y);
                                    q.push(x * n + y);
                                }
                            }
                        }
                        unordered_set<int> s(walls.begin(), walls.end());
                        vector<int> cells{(int)s.size()};
                        all.push_back({cells ,walls, virus});
                    }
                }
            }
            if (all.empty()) break;
            sort(all.begin(), all.end(), [](vector<vector<int>> &a, vector<vector<int>> &b) {return a[0][0] > b[0][0];});
            for (int i = 0; i < all.size(); ++i) {
                if (i == 0) {
                    vector<int> virus = all[0][2];
                    for (int idx : virus) grid[idx / n][idx % n] = -1;
                    res += all[0][1].size();
                } else {
                    vector<int> wall = all[i][1];
                    for (int idx : wall) grid[idx / n][idx % n] = 1;
                }
            }
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/114208/c-dfs-12ms](https://discuss.leetcode.com/topic/114208/c-dfs-12ms)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













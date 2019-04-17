# [LeetCode] Max Increase to Keep City Skyline 保持城市天际线的最大增高 - Grandyang - 博客园







# [[LeetCode] Max Increase to Keep City Skyline 保持城市天际线的最大增高](https://www.cnblogs.com/grandyang/p/9383482.html)









In a 2 dimensional array `grid`, each value `grid[i][j]` represents the height of a building located there. We are allowed to increase the height of any number of buildings, by any amount (the amounts can be different for different buildings). Height 0 is considered to be a building as well. 

At the end, the "skyline" when viewed from all four directions of the grid, i.e. top, bottom, left, and right, must be the same as the skyline of the original grid. A city's skyline is the outer contour of the rectangles formed by all the buildings when viewed from a distance. See the following example.

What is the maximum total sum that the height of the buildings can be increased?
Example:
Input: grid = [[3,0,8,4],[2,4,5,7],[9,2,6,3],[0,3,1,0]]
Output: 35
Explanation: 
The grid is:
[ [3, 0, 8, 4], 
  [2, 4, 5, 7],
  [9, 2, 6, 3],
  [0, 3, 1, 0] ]

The skyline viewed from top or bottom is: [9, 4, 8, 7]
The skyline viewed from left or right is: [8, 7, 9, 3]

The grid after increasing the height of buildings without affecting skylines is:

gridNew = [ [8, 4, 8, 7],
            [7, 4, 7, 7],
            [9, 4, 8, 7],
            [3, 3, 3, 3] ]


Notes:
- `1 < grid.length = grid[0].length <= 50`.
- All heights `grid[i][j]` are in the range `[0, 100]`.
- All buildings in `grid[i][j]` occupy the entire grid cell: that is, they are a `1 x 1 x grid[i][j]`rectangular prism.





这道题给了我们一个二维数组，说是数组中的每个数字代表了一栋建筑的高度，那么从四个方向看去，就会有城市的天际线，这个天际线就是由这些建筑的最高的边形成的，现在让我们在不改变天际线的前提下，问最多可以增高建筑的总高度。那么既然不能改变天际线，我们就要清楚天际线是由啥组成的，是最高的建筑物，那么就是说每行或每列的最高那个建筑不能变，而其他建筑物在不超过该行该列中最高建筑的高度情况下是可以有升高空间的。那么思路就该很清晰了，我们首先需要求出各行各列的最大值，成为一个限制高度，然后就遍历每个建筑，每一个建筑的高度都有可能影响该行或者该列的天际线，那么该行该列中的较小值应该是该建筑物的高度极限，如果超过了这个值，那么原来的天际线就会被破坏，所以这个极限值和当前的高度之差就是可以增加的高度，我们累计所有建筑的可增加的高度，就是所求的最大增高，参见代码如下：



```
class Solution {
public:
    int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), res = 0;
        vector<int> row(m, 0), col(n, 0);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                row[i] = max(row[i], grid[i][j]);
                col[j] = max(col[j], grid[i][j]);
            }
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                res += min(row[i] - grid[i][j], col[j] - grid[i][j]);
            }
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/max-increase-to-keep-city-skyline/solution/](https://leetcode.com/problems/max-increase-to-keep-city-skyline/solution/)

[https://leetcode.com/problems/max-increase-to-keep-city-skyline/discuss/120681/Easy-and-Concise-Solution-C++JavaPython](https://leetcode.com/problems/max-increase-to-keep-city-skyline/discuss/120681/Easy-and-Concise-Solution-C++JavaPython)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













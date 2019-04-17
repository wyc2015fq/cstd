# [LeetCode] Magic Squares In Grid 网格中的神奇正方形 - Grandyang - 博客园







# [[LeetCode] Magic Squares In Grid 网格中的神奇正方形](https://www.cnblogs.com/grandyang/p/10404157.html)









A 3 x 3 magic square is a 3 x 3 grid filled with distinct numbers from 1 to 9 such that each row, column, and both diagonals all have the same sum.

Given an `grid` of integers, how many 3 x 3 "magic square" subgrids are there?  (Each subgrid is contiguous).



Example 1:
Input: [[4,3,8,4],
        [9,5,1,9],
        [2,7,6,2]]
Output: 1
Explanation: 
The following subgrid is a 3 x 3 magic square:
438
951
276

while this one is not:
384
519
762

In total, there is only one magic square inside the given grid.

Note:
- `1 <= grid.length <= 10`
- `1 <= grid[0].length <= 10`
- `0 <= grid[i][j] <= 15`





这道题定义了一种神奇正方形，是一个3x3大小，且由1到9中到数字组成，各行各列即对角线和都必须相等。那么其实这个神奇正方形的各行各列及对角线之和就已经被限定了，必须是15才行，而且最中间的位置必须是5，否则根本无法组成满足要求的正方形。博主也没想出啥特别巧妙的方法，就老老实实的遍历所有的3x3大小的正方形呗，我们写一个子函数来检测各行各列及对角线的和是否为15，在调用子函数之前，先检测一下中间的数字是否为5，是的话再进入子函数。在子函数中，先验证下该正方形中的数字是否只有1到9中的数字，且不能由重复出现，使用一个一维数组来标记出现过的数字，若当前数字已经出现了，直接返回true。之后便是一次计算各行各列及对角线之和是否为15了，若全部为15，则返回true，参见代码如下：



```
class Solution {
public:
    int numMagicSquaresInside(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), res = 0;
        for (int i = 0; i < m - 2; ++i) {
            for (int j = 0; j < n - 2; ++j) {
                if (grid[i + 1][j + 1] == 5 && isValid(grid, i, j)) ++res;
            }
        }
        return res;
    }
    bool isValid(vector<vector<int>>& grid, int i, int j) {
        vector<int> cnt(10);
        for (int x = i; x < i + 2; ++x) {
            for (int y = j; y < j + 2; ++y) {
                int k = grid[x][y];
                if (k < 1 || k > 9 || cnt[k] == 1) return false;
                cnt[k] = 1;
            }
        }
        if (15 != grid[i][j] + grid[i][j + 1] + grid[i][j + 2]) return false;
        if (15 != grid[i + 1][j] + grid[i + 1][j + 1] + grid[i + 1][j + 2]) return false;
        if (15 != grid[i + 2][j] + grid[i + 2][j + 1] + grid[i + 2][j + 2]) return false;
        if (15 != grid[i][j] + grid[i + 1][j] + grid[i + 2][j]) return false;
        if (15 != grid[i][j + 1] + grid[i + 1][j + 1] + grid[i + 2][j + 1]) return false;
        if (15 != grid[i][j + 2] + grid[i + 1][j + 2] + grid[i + 2][j + 2]) return false;
        if (15 != grid[i][j] + grid[i + 1][j + 1] + grid[i + 2][j + 2]) return false;
        if (15 != grid[i + 2][j] + grid[i + 1][j + 1] + grid[i][j + 2]) return false;
        return true;
    }
};
```



参考资料：

[https://leetcode.com/problems/magic-squares-in-grid/](https://leetcode.com/problems/magic-squares-in-grid/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













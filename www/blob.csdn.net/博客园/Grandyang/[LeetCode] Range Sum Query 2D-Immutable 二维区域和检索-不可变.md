# [LeetCode] Range Sum Query 2D - Immutable 二维区域和检索 - 不可变 - Grandyang - 博客园







# [[LeetCode] Range Sum Query 2D - Immutable 二维区域和检索 - 不可变](https://www.cnblogs.com/grandyang/p/4958789.html)







Given a 2D matrix *matrix*, find the sum of the elements inside the rectangle defined by its upper left corner (*row*1, *col*1) and lower right corner (*row*2, *col*2).

![Range Sum Query 2D](https://leetcode.com/static/images/courses/range_sum_query_2d.png)
The above rectangle (with the red border) is defined by (row1, col1) = **(2, 1)** and (row2, col2) = **(4, 3)**, which contains sum = **8**.

**Example:**

Given matrix = [
  [3, 0, 1, 4, 2],
  [5, 6, 3, 2, 1],
  [1, 2, 0, 1, 5],
  [4, 1, 0, 1, 7],
  [1, 0, 3, 0, 5]
]

sumRegion(2, 1, 4, 3) -> 8
sumRegion(1, 1, 2, 2) -> 11
sumRegion(1, 2, 2, 4) -> 12

**Note:**

- You may assume that the matrix does not change.
- There are many calls to *sumRegion* function.
- You may assume that *row*1 ≤ *row*2 and *col*1 ≤ *col*2.



这道题让我们求一个二维区域和的检索，是之前那道题[Range Sum Query - Immutable 区域和检索](http://www.cnblogs.com/grandyang/p/4952464.html)的延伸。有了之前那道题的基础，我们知道这道题其实也是换汤不换药，还是要建立一个累计区域和的数组，然后根据边界值的加减法来快速求出给定区域之和。这里我们维护一个二维数组dp，其中dp[i][j]表示累计区间(0, 0)到(i, j)这个矩形区间所有的数字之和，那么此时如果我们想要快速求出(r1, c1)到(r2, c2)的矩形区间时，只需dp[r2][c2] - dp[r2][c1 - 1] - dp[r1 - 1][c2] + dp[r1 - 1][c1 - 1]即可，下面的代码中我们由于用了辅助列和辅助行，所以下标会有些变化，参见代码如下：



```
class NumMatrix {
public:
    NumMatrix(vector<vector<int> > &matrix) {
        if (matrix.empty() || matrix[0].empty()) return;
        dp.resize(matrix.size() + 1, vector<int>(matrix[0].size() + 1, 0));
        for (int i = 1; i <= matrix.size(); ++i) {
            for (int j = 1; j <= matrix[0].size(); ++j) {
                dp[i][j] = dp[i][j - 1] + dp[i - 1][j] - dp[i - 1][j - 1] + matrix[i - 1][j - 1];
            }
        }
    }
    int sumRegion(int row1, int col1, int row2, int col2) {
        return dp[row2 + 1][col2 + 1] - dp[row1][col2 + 1] - dp[row2 + 1][col1] + dp[row1][col1];
    }
    
private:
    vector<vector<int> > dp;
};
```



类似题目：

[Range Sum Query 2D - Mutable](http://www.cnblogs.com/grandyang/p/5300458.html)

[Range Sum Query - Immutable](http://www.cnblogs.com/grandyang/p/4952464.html)

[Range Sum Query - Mutable](http://www.cnblogs.com/grandyang/p/4985506.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)















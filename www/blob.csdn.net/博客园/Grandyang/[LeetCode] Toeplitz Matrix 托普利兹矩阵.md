# [LeetCode] Toeplitz Matrix 托普利兹矩阵 - Grandyang - 博客园







# [[LeetCode] Toeplitz Matrix 托普利兹矩阵](https://www.cnblogs.com/grandyang/p/8729459.html)







A matrix is *Toeplitz* if every diagonal from top-left to bottom-right has the same element.

Now given an `M x N` matrix, return `True` if and only if the matrix is *Toeplitz*.


Example 1:
Input: matrix = [[1,2,3,4],[5,1,2,3],[9,5,1,2]]
Output: True
Explanation:
1234
5123
9512

In the above grid, the diagonals are "[9]", "[5, 5]", "[1, 1, 1]", "[2, 2, 2]", "[3, 3]", "[4]", and in each diagonal all elements are the same, so the answer is True.

Example 2:
Input: matrix = [[1,2],[2,2]]
Output: False
Explanation:
The diagonal "[1, 2]" has different elements.

Note:
- `matrix` will be a 2D array of integers.
- `matrix` will have a number of rows and columns in range `[1, 20]`.
- `matrix[i][j]` will be integers in range `[0, 99]`.



这道题让我们验证一个矩阵是否是托普利兹矩阵Toeplitz Matrix，所谓的这个托普利兹矩阵，就是看每条从左上到右下的对角线是否是值都相等。注意矩阵的行数列数不一定相等，要验证所有的对角线。那么其实这道题的本质是让我们斜向遍历矩阵，就是按对角线来。那么博主最先想到的方法就是按照对角线来遍历矩阵，起点是最左下的数字，对于mxn的矩阵，最左下角数字的坐标为(m-1, 0)，然后我们开始往右下角遍历，我们先记录每条对角线左上角的数字为val，然后再往右下角遍历的时候，如果同一条对角线上的数字不等于val，直接返回false。当我们遍历完一条对角线的时候，切换一条对角线的时候，是根据起点数字的坐标移动的，如果细心观察会发现，起点位置是先从第一列往上移动，然后在第一行往右移动，那么只要根据起点位置的行坐标是否为0来判断移动的方向即可，比如对于题目中的例子1:

12 3 4
5  1  2  3
9  5  1  2

起点移动的方向是9 -> 5 -> 1 -> 2 -> 3 -> 4，参见代码如下：



解法一：

```
class Solution {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size(), p = m - 1, q = 0;
        while (p >= 0 && q < n) {
            int val = matrix[p][q], i = p, j = q;
            while (i + 1 < m && j + 1 < n) {
                if (matrix[++i][++j] != val) return false;
            }
            (p > 0) ? --p : ++q;
        }
        return true;
    }
};
```



其实并不需要像上面解法写的那么复杂，我们还可以按正常顺序来遍历数组，对于每个遍历到的数字，都跟其右下方的数字对比，如果不相同，直接返回false即可。为了防止越界，我们不遍历最后一行和最后一列，遍历完成后，返回true，参见代码如下：



解法二：

```
class Solution {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        for (int i = 0; i < matrix.size() - 1; ++i) {
            for (int j = 0; j < matrix[i].size() - 1; ++j) {
                if (matrix[i][j] != matrix[i + 1][j + 1]) return false;
            }
        }
        return true;
    }
};
```



类似题目：

[Valid Word Square](http://www.cnblogs.com/grandyang/p/5991673.html)



参考资料：

[https://leetcode.com/problems/toeplitz-matrix/discuss/113417/Java-solution-4-liner.](https://leetcode.com/problems/toeplitz-matrix/discuss/113417/Java-solution-4-liner.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













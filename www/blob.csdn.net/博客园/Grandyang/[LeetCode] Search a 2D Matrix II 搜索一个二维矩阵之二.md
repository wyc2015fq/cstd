# [LeetCode] Search a 2D Matrix II 搜索一个二维矩阵之二 - Grandyang - 博客园







# [[LeetCode] Search a 2D Matrix II 搜索一个二维矩阵之二](https://www.cnblogs.com/grandyang/p/4669134.html)







Write an efficient algorithm that searches for a value in an *m* x *n* matrix. This matrix has the following properties:


- Integers in each row are sorted in ascending from left to right.
- Integers in each column are sorted in ascending from top to bottom.

For example,

Consider the following matrix:
[
  [1,   4,  7, 11, 15],
  [2,   5,  8, 12, 19],
  [3,   6,  9, 16, 22],
  [10, 13, 14, 17, 24],
  [18, 21, 23, 26, 30]
]

Given **target** = `5`, return `true`.

Given **target** = `20`, return `false`.



突然发现LeetCode很喜欢从LintCode上盗题，这是逼我去刷LintCode的节奏么?! 这道题让我们在一个二维数组中快速的搜索的一个数字，这个二维数组各行各列都是按递增顺序排列的，是之前那道[Search a 2D Matrix 搜索一个二维矩阵](http://www.cnblogs.com/grandyang/p/4323301.html)的延伸，那道题的不同在于每行的第一个数字比上一行的最后一个数字大，是一个整体蛇形递增的数组。所以那道题可以将二维数组展开成一个一位数组用一次二查搜索。而这道题没法那么做，这道题有它自己的特点。如果我们观察题目中给的那个例子，我们可以发现有两个位置的数字很有特点，左下角和右上角的数。左下角的18，往上所有的数变小，往右所有数增加，那么我们就可以和目标数相比较，如果目标数大，就往右搜，如果目标数小，就往上搜。这样就可以判断目标数是否存在。当然我们也可以把起始数放在右上角，往左和下搜，停止条件设置正确就行。代码如下：



```
class Solution {
public:
    bool searchMatrix(vector<vector<int> > &matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;
        if (target < matrix[0][0] || target > matrix.back().back()) return false;
        int x = matrix.size() - 1, y = 0;
        while (true) {
            if (matrix[x][y] > target) --x;
            else if (matrix[x][y] < target) ++y;
            else return true;
            if (x < 0 || y >= matrix[0].size()) break;
        }
        return false;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)















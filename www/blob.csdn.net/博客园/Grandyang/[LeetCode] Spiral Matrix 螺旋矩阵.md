# [LeetCode] Spiral Matrix 螺旋矩阵 - Grandyang - 博客园







# [[LeetCode] Spiral Matrix 螺旋矩阵](https://www.cnblogs.com/grandyang/p/4362675.html)







Given a matrix of *m* x *n* elements (*m* rows, *n*columns), return all elements of the matrix in spiral order.

Example 1:
Input:
[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
Output: [1,2,3,6,9,8,7,4,5]

Example 2:
Input:
[
  [1, 2, 3, 4],
  [5, 6, 7, 8],
  [9,10,11,12]
]
Output: [1,2,3,4,8,12,11,10,9,5,6,7]


这道题让我们搓一个螺旋丸，将一个矩阵按照螺旋顺序打印出来，我们只能一条边一条边的打印，首先我们要从给定的mxn的矩阵中算出按螺旋顺序有几个环，注意最终间的环可以是一个数字，也可以是一行或者一列。环数的计算公式是 min(m, n) / 2，知道了环数，我们可以对每个环的边按顺序打印，比如对于题目中给的那个例子，个边生成的顺序是(用颜色标记了数字) Red -> Green -> Blue -> Yellow -> Black 

1　2　3

4　5　6

7　89



我们定义p，q为当前环的高度和宽度，当p或者q为1时，表示最后一个环只有一行或者一列，可以跳出循环。此题的难点在于下标的转换，如何正确的转换下标是解此题的关键，我们可以对照着上面的3x3的例子来完成下标的填写，代码如下：



解法一：

```
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int> > &matrix) {
        if (matrix.empty() || matrix[0].empty()) return {};
        int m = matrix.size(), n = matrix[0].size();
        vector<int> res;
        int c = m > n ? (n + 1) / 2 : (m + 1) / 2;
        int p = m, q = n;
        for (int i = 0; i < c; ++i, p -= 2, q -= 2) {
            for (int col = i; col < i + q; ++col) 
                res.push_back(matrix[i][col]);
            for (int row = i + 1; row < i + p; ++row)
                res.push_back(matrix[row][i + q - 1]);
            if (p == 1 || q == 1) break;
            for (int col = i + q - 2; col >= i; --col)
                res.push_back(matrix[i + p - 1][col]);
            for (int row = i + p - 2; row > i; --row) 
                res.push_back(matrix[row][i]);
        }
        return res;
    }
};
```



如果觉得上面解法中的下标的转换比较难弄的话，我们可以使用下面这种坐标稍稍简洁一些的方法。对于这种螺旋遍历的方法，重要的是要确定上下左右四条边的位置，那么初始化的时候，上边up就是0，下边down就是m-1，左边left是0，右边right是n-1。然后我们进行while循环，先遍历上边，将所有元素加入结果res，然后上边下移一位，如果此时上边大于下边，说明此时已经遍历完成了，直接break。同理对于下边，左边，右边，依次进行相对应的操作，这样就会使得坐标很有规律，并且不易出错，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return {};
        int m = matrix.size(), n = matrix[0].size();
        vector<int> res;
        int up = 0, down = m - 1, left = 0, right = n - 1;
        while (true) {
            for (int j = left; j <= right; ++j) res.push_back(matrix[up][j]);
            if (++up > down) break;
            for (int i = up; i <= down; ++i) res.push_back(matrix[i][right]);
            if (--right < left) break;
            for (int j = right; j >= left; --j) res.push_back(matrix[down][j]);
            if (--down < up) break;
            for (int i = down; i >= up; --i) res.push_back(matrix[i][left]);
            if (++left > right) break;
        }
        return res;
    }
};
```



类似题目：

[Spiral Matrix II](http://www.cnblogs.com/grandyang/p/4362813.html)



参考资料：

[https://leetcode.com/problems/spiral-matrix/](https://leetcode.com/problems/spiral-matrix/)

[https://leetcode.com/problems/spiral-matrix/discuss/20719/0ms-Clear-C%2B%2B-Solution](https://leetcode.com/problems/spiral-matrix/discuss/20719/0ms-Clear-C%2B%2B-Solution)

[https://leetcode.com/problems/spiral-matrix/discuss/20599/Super-Simple-and-Easy-to-Understand-Solution](https://leetcode.com/problems/spiral-matrix/discuss/20599/Super-Simple-and-Easy-to-Understand-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













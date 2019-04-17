# [LeetCode] Pascal's Triangle 杨辉三角 - Grandyang - 博客园







# [[LeetCode] Pascal's Triangle 杨辉三角](https://www.cnblogs.com/grandyang/p/4032449.html)







Given a non-negative integer *numRows*, generate the first *numRows* of Pascal's triangle.

![](https://upload.wikimedia.org/wikipedia/commons/0/0d/PascalTriangleAnimated2.gif)
In Pascal's triangle, each number is the sum of the two numbers directly above it.

Example:
Input: 5
Output:
[
     [1],
    [1,1],
   [1,2,1],
  [1,3,3,1],
 [1,4,6,4,1]
]


杨辉三角是二项式系数的一种写法，如果熟悉杨辉三角的五个性质，那么很好生成，可参见我的上一篇博文[Pascal's Triangle II](http://www.cnblogs.com/grandyang/p/4031536.html)。具体生成算是：每一行的首个和结尾一个数字都是1，从第三行开始，中间的每个数字都是上一行的左右两个数字之和。代码如下：



```
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> res(numRows, vector<int>());
        for (int i = 0; i < numRows; ++i) {
            res[i].resize(i + 1, 1);
            for (int j = 1; j < i; ++j) {
                res[i][j] = res[i - 1][j - 1] + res[i - 1][j];
            }
        }
        return res;
    }
};
```



类似题目：

[Pascal's Triangle II](http://www.cnblogs.com/grandyang/p/4031536.html)



参考资料：

[https://leetcode.com/problems/pascals-triangle/](https://leetcode.com/problems/pascals-triangle/)

[https://leetcode.com/problems/pascals-triangle/discuss/38150/My-C%2B%2B-code-0ms](https://leetcode.com/problems/pascals-triangle/discuss/38150/My-C%2B%2B-code-0ms)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













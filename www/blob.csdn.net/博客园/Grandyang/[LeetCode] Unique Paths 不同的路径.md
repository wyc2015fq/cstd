# [LeetCode] Unique Paths 不同的路径 - Grandyang - 博客园







# [[LeetCode] Unique Paths 不同的路径](https://www.cnblogs.com/grandyang/p/4353555.html)







A robot is located at the top-left corner of a *m* x *n* grid (marked 'Start' in the diagram below).

The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).

How many possible unique paths are there?

![](https://assets.leetcode.com/uploads/2018/10/22/robot_maze.png)
Above is a 7 x 3 grid. How many possible unique paths are there?

Note: *m* and *n* will be at most 100.

Example 1:
Input: m = 3, n = 2
Output: 3
Explanation:
From the top-left corner, there are a total of 3 ways to reach the bottom-right corner:
1. Right -> Right -> Down
2. Right -> Down -> Right
3. Down -> Right -> Right

Example 2:
Input: m = 7, n = 3
Output: 28


这道题让求所有不同的路径的个数，一开始还真把我难住了，因为之前好像没有遇到过这类的问题，所以感觉好像有种无从下手的感觉。在网上找攻略之后才恍然大悟，原来这跟之前那道 [Climbing Stairs](http://www.cnblogs.com/grandyang/p/4079165.html) 很类似，那道题是说可以每次能爬一格或两格，问到达顶部的所有不同爬法的个数。而这道题是每次可以向下走或者向右走，求到达最右下角的所有不同走法的个数。那么跟爬梯子问题一样，我们需要用动态规划Dynamic Programming来解，我们可以维护一个二维数组dp，其中dp[i][j]表示到当前位置不同的走法的个数，然后可以得到递推式为: dp[i][j] = dp[i - 1][j] + dp[i][j - 1]，这里为了节省空间，我们使用一维数组dp，一行一行的刷新也可以，代码如下：



解法一：

```
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 1);
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                dp[j] += dp[j - 1]; 
            }
        }
        return dp[n - 1];
    }
};
```



这道题其实还有另一种很数学的解法，参见网友[Code Ganker的博客](http://blog.csdn.net/linhuanmars/article/details/22126357)，实际相当于机器人总共走了m + n - 2步，其中m - 1步向右走，n - 1步向下走，那么总共不同的方法个数就相当于在步数里面m - 1和n - 1中较小的那个数的取法，实际上是一道组合数的问题，写出代码如下:



解法二：

```
class Solution {
public:
    int uniquePaths(int m, int n) {
        double num = 1, denom = 1;
        int small = m > n ? n : m;
        for (int i = 1; i <= small - 1; ++i) {
            num *= m + n - 1 - i;
            denom *= i;
        }
        return (int)(num / denom);
    }
};
```



类似题目：

[Unique Paths II](http://www.cnblogs.com/grandyang/p/4353680.html)

[Minimum Path Sum](http://www.cnblogs.com/grandyang/p/4353255.html)

[Dungeon Game](http://www.cnblogs.com/grandyang/p/4233035.html)



参考资料：

[https://leetcode.com/problems/unique-paths/](https://leetcode.com/problems/unique-paths/)

[https://leetcode.com/problems/unique-paths/discuss/22981/My-AC-solution-using-formula](https://leetcode.com/problems/unique-paths/discuss/22981/My-AC-solution-using-formula)

[https://leetcode.com/problems/unique-paths/discuss/22954/0ms-5-lines-DP-Solution-in-C%2B%2B-with-Explanations](https://leetcode.com/problems/unique-paths/discuss/22954/0ms-5-lines-DP-Solution-in-C%2B%2B-with-Explanations)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













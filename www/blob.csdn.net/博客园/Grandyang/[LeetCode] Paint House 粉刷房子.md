# [LeetCode] Paint House 粉刷房子 - Grandyang - 博客园







# [[LeetCode] Paint House 粉刷房子](https://www.cnblogs.com/grandyang/p/5319384.html)







There are a row of *n* houses, each house can be painted with one of the three colors: red, blue or green. The cost of painting each house with a certain color is different. You have to paint all the houses such that no two adjacent houses have the same color.

The cost of painting each house with a certain color is represented by a `*n* x *3*` cost matrix. For example, `costs[0][0]` is the cost of painting house 0 with color red; `costs[1][2]` is the cost of painting house 1 with color green, and so on... Find the minimum cost to paint all houses.

Note:
All costs are positive integers.



这道题说有n个房子，每个房子可以用红绿蓝三种颜色刷，每个房子的用每种颜色刷的花费都不同，限制条件是相邻的房子不能用相同的颜色来刷，现在让我们求刷完所有的房子的最低花费是多少。这题跟[House Robber II](http://www.cnblogs.com/grandyang/p/4518674.html)和[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)很类似，不过那题不是每个房子都抢，相邻的房子不抢，而这道题是每个房子都刷，相邻的房子不能刷同一种颜色。而[Paint Fence](http://www.cnblogs.com/grandyang/p/5231220.html)那道题主要考察我们有多少种刷法，这几道题很类似，但都不一样，需要我们分别区分。但是它们的解题思想都一样，需要用动态规划Dynamic Programming来做，这道题我们需要维护一个二维的动态数组dp，其中dp[i][j]表示刷到第i+1房子用颜色j的最小花费，递推式为:

dp[i][j] = dp[i][j] + min(dp[i - 1][(j + 1) % 3], dp[i - 1][(j + 2) % 3])；

这个也比较好理解，如果当前的房子要用红色刷，那么上一个房子只能用绿色或蓝色来刷，那么我们要求刷到当前房子，且当前房子用红色刷的最小花费就等于当前房子用红色刷的钱加上刷到上一个房子用绿色和刷到上一个房子用蓝色的较小值，这样当我们算到最后一个房子时，我们只要取出三个累计花费的最小值即可，参见代码如下：



解法一：

```
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        if (costs.empty() || costs[0].empty()) return 0;
        vector<vector<int>> dp = costs;
        for (int i = 1; i < dp.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                dp[i][j] += min(dp[i - 1][(j + 1) % 3], dp[i - 1][(j + 2) % 3]);
            }
        }
        return min(min(dp.back()[0], dp.back()[1]), dp.back()[2]);
    }
};
```



由于只有红绿蓝三张颜色，所以我们就可以分别写出各种情况，这样写可能比上面的写法更加一目了然一些，更容易理解一点吧：



解法二：

```
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        if (costs.empty() || costs[0].empty()) return 0;
        vector<vector<int>> dp = costs;
        for (int i = 1; i < dp.size(); ++i) {
            dp[i][0] += min(dp[i - 1][1], dp[i - 1][2]);
            dp[i][1] += min(dp[i - 1][0], dp[i - 1][2]);
            dp[i][2] += min(dp[i - 1][0], dp[i - 1][1]);
        }
        return min(min(dp.back()[0], dp.back()[1]), dp.back()[2]);
    }
};
```



类似题目：

[House Robber II](http://www.cnblogs.com/grandyang/p/4518674.html)

[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)

[Paint Fence](http://www.cnblogs.com/grandyang/p/5231220.html)



参考资料：

[https://leetcode.com/discuss/51721/simple-java-dp-solution](https://leetcode.com/discuss/51721/simple-java-dp-solution)

[https://leetcode.com/discuss/63863/simple-15-line-code-with-o-n-time-and-o-1-memory-solution-java](https://leetcode.com/discuss/63863/simple-15-line-code-with-o-n-time-and-o-1-memory-solution-java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [LeetCode] Best Time to Buy and Sell Stock III 买股票的最佳时间之三 - Grandyang - 博客园







# [[LeetCode] Best Time to Buy and Sell Stock III 买股票的最佳时间之三](https://www.cnblogs.com/grandyang/p/4281975.html)







Say you have an array for which the *i*th element is the price of a given stock on day *i*.

Design an algorithm to find the maximum profit. You may complete at most *two* transactions.

Note: You may not engage in multiple transactions at the same time (i.e., you must sell the stock before you buy again).

Example 1:
Input: [3,3,5,0,0,3,1,4]
Output: 6
Explanation: Buy on day 4 (price = 0) and sell on day 6 (price = 3), profit = 3-0 = 3.
             Then buy on day 7 (price = 1) and sell on day 8 (price = 4), profit = 4-1 = 3.
Example 2:
Input: [1,2,3,4,5]
Output: 4
Explanation: Buy on day 1 (price = 1) and sell on day 5 (price = 5), profit = 5-1 = 4.
             Note that you cannot buy on day 1, buy on day 2 and sell them later, as you are
             engaging multiple transactions at the same time. You must sell before buying again.

Example 3:
Input: [7,6,4,3,1]
Output: 0
Explanation: In this case, no transaction is done, i.e. max profit = 0.


这道是买股票的最佳时间系列问题中最难最复杂的一道，前面两道 [Best Time to Buy and Sell Stock](http://www.cnblogs.com/grandyang/p/4280131.html) 和 [Best Time to Buy and Sell Stock II](http://www.cnblogs.com/grandyang/p/4280803.html) 的思路都非常的简洁明了，算法也很简单。而这道是要求最多交易两次，找到最大利润，还是需要用动态规划Dynamic Programming来解，而这里我们需要两个递推公式来分别更新两个变量local和global，参见网友[Code Ganker的博客](http://blog.csdn.net/linhuanmars/article/details/23236995)，我们其实可以求至少k次交易的最大利润，找到通解后可以设定 k = 2，即为本题的解答。我们定义local[i][j]为在到达第i天时最多可进行j次交易并且最后一次交易在最后一天卖出的最大利润，此为局部最优。然后我们定义global[i][j]为在到达第i天时最多可进行j次交易的最大利润，此为全局最优。它们的递推式为：

local[i][j] = max(global[i - 1][j - 1] + max(diff, 0), local[i - 1][j] + diff)

global[i][j] = max(local[i][j], global[i - 1][j])

其中局部最优值是比较前一天并少交易一次的全局最优加上大于0的差值，和前一天的局部最优加上差值中取较大值，而全局最优比较局部最优和前一天的全局最优，代码如下：



解法一：

```
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        if (prices.empty()) return 0;
        int n = prices.size(), g[n][3] = {0}, l[n][3] = {0};
        for (int i = 1; i < prices.size(); ++i) {
            int diff = prices[i] - prices[i - 1];
            for (int j = 1; j <= 2; ++j) {
                l[i][j] = max(g[i - 1][j - 1] + max(diff, 0), l[i - 1][j] + diff);
                g[i][j] = max(l[i][j], g[i - 1][j]);
            }
        }
        return g[n - 1][2];
    }
};
```



下面这种解法用一维数组来代替二维数组，可以极大的节省了空间，由于覆盖的顺序关系，我们需要j从2到1，这样可以取到正确的g[j-1]值，而非已经被覆盖过的值，参见代码如下：



解法二：

```
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        if (prices.empty()) return 0;
        int g[3] = {0};
        int l[3] = {0};
        for (int i = 0; i < prices.size() - 1; ++i) {
            int diff = prices[i + 1] - prices[i];
            for (int j = 2; j >= 1; --j) {
                l[j] = max(g[j - 1] + max(diff, 0), l[j] + diff);
                g[j] = max(l[j], g[j]);
            }
        }
        return g[2];
    }
};
```



我们如果假设prices数组为1, 3, 2, 9, 那么我们来看每次更新时local 和 global 的值：

第一天两次交易：　　　　　　第一天一次交易：

local:　　  0 0 0 　　　　　　local:　　  0 0 0 

global:　　0 0 0　　　　　 　global:　　0 0 0

第二天两次交易：　　　　　　第二天一次交易：

local:　　  0 0 2 　　　　　　local:　　  0 2 2 

global:　　0 0 2　　　　　 　global:　　0 2 2

第三天两次交易：　　　　　　第三天一次交易：

local:　　  0 2 2 　　　　　　local:　　  0 1 2 

global:　　0 2 2　　　　 　　global:　　0 2 2

第四天两次交易：　　　　　　第四天一次交易：

local:　　  0 1 9 　　　　　　local:　　  0 8 9 

global:　　0 2 9　　　　　 　global:　　0 8 9



在网友@[loveahnee](http://home.cnblogs.com/u/1221269/)的提醒下，发现了其实上述的递推公式关于local[i][j]的可以稍稍化简一下，我们之前定义的local[i][j]为在到达第i天时最多可进行j次交易并且最后一次交易在最后一天卖出的最大利润，然后网友@[fgvlty](http://home.cnblogs.com/u/985421/)解释了一下第 i 天卖第 j 支股票的话，一定是下面的一种：

1. 今天刚买的
那么 Local(i, j) = Global(i-1, j-1)
相当于啥都没干

2. 昨天买的
那么 Local(i, j) = Global(i-1, j-1) + diff
等于Global(i-1, j-1) 中的交易，加上今天干的那一票

3. 更早之前买的
那么 Local(i, j) = Local(i-1, j) + diff
昨天别卖了，留到今天卖

但其实第一种情况是不需要考虑的，因为当天买当天卖不会增加利润，完全是重复操作，这种情况可以归纳在global[i-1][j-1]中，所以我们就不需要max(0, diff)了，那么由于两项都加上了diff，所以我们可以把diff抽到max的外面，所以更新后的递推公式为：

local[i][j] = max(global[i - 1][j - 1], local[i - 1][j]) + diff

global[i][j] = max(local[i][j], global[i - 1][j])



类似题目：

[Best Time to Buy and Sell Stock with Cooldown](http://www.cnblogs.com/grandyang/p/4997417.html)

[Best Time to Buy and Sell Stock IV](http://www.cnblogs.com/grandyang/p/4295761.html)

[Best Time to Buy and Sell Stock II](http://www.cnblogs.com/grandyang/p/4280803.html)

[Best Time to Buy and Sell Stock](http://www.cnblogs.com/grandyang/p/4280131.html)



参考资料：

[https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













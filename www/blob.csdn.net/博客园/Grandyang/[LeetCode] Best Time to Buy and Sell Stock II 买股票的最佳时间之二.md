# [LeetCode] Best Time to Buy and Sell Stock II  买股票的最佳时间之二 - Grandyang - 博客园







# [[LeetCode] Best Time to Buy and Sell Stock II  买股票的最佳时间之二](https://www.cnblogs.com/grandyang/p/4280803.html)







Say you have an array for which the *i*th element is the price of a given stock on day *i*.

Design an algorithm to find the maximum profit. You may complete as many transactions as you like (ie, buy one and sell one share of the stock multiple times). However, you may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy again).



这道跟之前那道[Best Time to Buy and Sell Stock 买卖股票的最佳时间](http://www.cnblogs.com/grandyang/p/4280131.html)很类似，但都比较容易解答。这道题由于可以无限次买入和卖出。我们都知道炒股想挣钱当然是低价买入高价抛出，那么这里我们只需要从第二天开始，如果当前价格比之前价格高，则把差值加入利润中，因为我们可以昨天买入，今日卖出，若明日价更高的话，还可以今日买入，明日再抛出。以此类推，遍历完整个数组后即可求得最大利润。代码如下：



C++ 解法：

```
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int res = 0, n = prices.size();
        for (int i = 0; i < n - 1; ++i) {
            if (prices[i] < prices[i + 1]) {
                res += prices[i + 1] - prices[i];
            }
        }
        return res;
    }
};
```



Java 解法：

```
public class Solution {
    public int maxProfit(int[] prices) {
        int res = 0;
        for (int i = 0; i < prices.length - 1; ++i) {
            if (prices[i] < prices[i + 1]) {
                res += prices[i + 1] - prices[i];
            }
        }
        return res;
    }
}
```



类似题目：

[Best Time to Buy and Sell Stock with Cooldown](http://www.cnblogs.com/grandyang/p/4997417.html)

[Best Time to Buy and Sell Stock IV](http://www.cnblogs.com/grandyang/p/4295761.html)

[Best Time to Buy and Sell Stock III](http://www.cnblogs.com/grandyang/p/4281975.html)

[Best Time to Buy and Sell Stock](http://www.cnblogs.com/grandyang/p/4280131.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













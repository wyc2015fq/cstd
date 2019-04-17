# [LeetCode] Best Time to Buy and Sell Stock  买卖股票的最佳时间 - Grandyang - 博客园







# [[LeetCode] Best Time to Buy and Sell Stock  买卖股票的最佳时间](https://www.cnblogs.com/grandyang/p/4280131.html)







Say you have an array for which the *i*th element is the price of a given stock on day *i*.

If you were only permitted to complete at most one transaction (ie, buy one and sell one share of the stock), design an algorithm to find the maximum profit.



这道题相当简单，感觉达不到Medium的难度，只需要遍历一次数组，用一个变量记录遍历过数中的最小值，然后每次计算当前值和这个最小值之间的差值最为利润，然后每次选较大的利润来更新。当遍历完成后当前利润即为所求，代码如下：



C++ 解法：

```
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int res = 0, buy = INT_MAX;
        for (int price : prices) {
            buy = min(buy, price);
            res = max(res, price - buy);
        }
        return res;
    }
};
```



Java 解法：

```
public class Solution {
    public int maxProfit(int[] prices) {
        int res = 0, buy = Integer.MAX_VALUE;
        for (int price : prices) {
            buy = Math.min(buy, price);
            res = Math.max(res, price - buy);
        }
        return res;
    }
}
```



类似题目：

[Best Time to Buy and Sell Stock with Cooldown](http://www.cnblogs.com/grandyang/p/4997417.html)

[Best Time to Buy and Sell Stock IV](http://www.cnblogs.com/grandyang/p/4295761.html)

[Best Time to Buy and Sell Stock III](http://www.cnblogs.com/grandyang/p/4281975.html)

[Best Time to Buy and Sell Stock II](http://www.cnblogs.com/grandyang/p/4280803.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













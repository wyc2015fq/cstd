# leetcode(20):Best Time to Buy and Sell Stock（买卖股票的最佳时机） - 刘炫320的博客 - CSDN博客
2018年07月24日 20:39:51[刘炫320](https://me.csdn.net/qq_35082030)阅读数：144标签：[股票预测																[leetcode																[Best Time](https://so.csdn.net/so/search/s.do?q=Best Time&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)](https://so.csdn.net/so/search/s.do?q=股票预测&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)[算法思想培养指南](https://blog.csdn.net/column/details/36524.html)
好久没刷算法了，也很久没来CSDN了，今天又重新刷一下。
这次我们还是从最熟悉的股票买卖开始。
# 1.leetcode#121.Best Time to Buy and Sell Stock
## 1.1问题描述
> 
Say you have an array for which the ith element is the price of a given stock on day i.
If you were only permitted to complete at most one transaction (i.e., buy one and sell one share of the stock), design an algorithm to find the maximum profit.
Note that you cannot sell a stock before you buy one.
Example
> 
Example1:
Input: [7,1,5,3,6,4]
Output: 5
Explanation: Buy on day 2 (price = 1) and sell on day 5 (price = 6), profit = 6-1 = 5.
Not 7-1 = 6, as selling price needs to be larger than buying price.
Example 2:
Input: [7,6,4,3,1]
Output: 0
Explanation: In this case, no transaction is done, i.e. max profit = 0.
## 1.2思路
这是最简单的动态规划的思路，只需要从前往后记录局部最优值和历史最优值的大小即可，另外一点就是要及时更换最小值。
## 1.3代码
```java
public int maxProfit(int[] prices) {
        if (prices.length == 0) {
            return 0;
        }
        int min = prices[0];
        int profit = 0;
        for (int i = 1; i < prices.length; i++) {
            //如果当前值大于最小值的话，取最大利润
            if (prices[i] > min) {
                profit = Math.max(profit, prices[i] - min);
            }
            //否则的话当前值为最小值 
            else {
                min = prices[i];
            }
        }
        return profit;
    }
```
# 2.leetcode#122.Best Time to Buy and Sell Stock2
## 2.1问题描述
> 
Say you have an array for which the ith element is the price of a given stock on day i.
Design an algorithm to find the maximum profit. You may complete as many transactions as you like (i.e., buy one and sell one share of the stock multiple times).
Note: You may not engage in multiple transactions at the same time (i.e., you must sell the stock before you buy again).
Example
> 
Input: [7,1,5,3,6,4]
Output: 7
Explanation: Buy on day 2 (price = 1) and sell on day 3 (price = 5), profit = 5-1 = 4.
Then buy on day 4 (price = 3) and sell on day 5 (price = 6), profit = 6-3 = 3.
## 2.2思路
我们先说正经想法，有了上一个的想法，我们就可以想象第二个，其实就是我们只记录单调递增的部分即可，我们首先找到最小值，其次找到最大值。如果i没有越界（没有找到符合条件的）,那么此次利润就是最大值减去最小值，不然为0。然后往后面找下去。
## 2.3代码
```java
public int maxProfit(int[] prices) {
        int profit = 0, i = 0;
	    //不断循环
	    while (i < prices.length) {
	        // 找到最小值
	        while (i < prices.length-1 && prices[i+1] <= prices[i]) i++;
	        int min = prices[i++]; 
	        // 找到最大值
	        while (i < prices.length-1 && prices[i+1] >= prices[i]) i++;
	        //计算此次利润
	        profit += i < prices.length ? prices[i++] - min : 0;
	    }
	    return profit;
    }
```
## 2.4钻漏洞
这里我们对于题目的审题上有一个约束，at the same time其实应当严格指的是同一天不能又买又卖。如果没有这个约束的话，其实是非常简单的，在每一步上我们都考虑是否是递增的即可，就像我们股市一样每天买了就卖即可完成赚钱，如果下跌的话，就不买。
## 2.5代码
```java
public int maxProfit(int[] prices) {
        int total = 0;
	    for (int i=0; i< prices.length-1; i++) {
	        if (prices[i+1]>prices[i]) total += prices[i+1]-prices[i];
	    }
	    return total;
    }
```
## 2.6小结
好久没刷题，感觉都不太一样了。有些迟钝了，还是需要经常刷一刷。

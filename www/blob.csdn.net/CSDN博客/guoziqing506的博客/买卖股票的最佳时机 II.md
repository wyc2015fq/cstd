# 买卖股票的最佳时机 II - guoziqing506的博客 - CSDN博客





2016年05月17日 14:15:40[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1867
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：假设有一个数组，它的第i个元素是一个给定的股票在第i天的价格。设计一个算法来找到最大的利润。你可以完成尽可能多的交易(多次买卖股票)。然而,你不能同时参与多个交易(你必须在再次购买前出售股票)。

样例：给出一个数组样例[2,1,2,0,1], 返回 2

这道题比上一道题“买卖股票的最佳时机”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51434777)）还要简单。

因为有不定次数的交易，所以，最“贪心”的办法就是只要明天的比今天价格高，我就不卖，而如果低了，一定卖。

放在程序中，可以这样描述：

1. 设置一个整型变量count，初始化为0

2. 遍历数组，发现下一个数，比当前的数大，就令count加上两个的差值，继续遍历

3. 如果下一个数比当前数小了，什么都不做就行，继续遍历

照这个思路写出代码就行：



```python
class Solution:
    """
    @param prices: Given an integer array
    @return: Maximum profit
    """
    def maxProfit(self, prices):
        n = len(prices)
        if n <= 1:
            return 0
        count = 0
        i = 1
        while i != n:
            diff = prices[i] - prices[i - 1]
            if diff > 0:
                count += diff
            i += 1
        return count
        # write your code here
```






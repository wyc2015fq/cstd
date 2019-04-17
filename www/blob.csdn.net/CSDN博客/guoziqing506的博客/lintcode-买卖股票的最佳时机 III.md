# lintcode - 买卖股票的最佳时机 III - guoziqing506的博客 - CSDN博客





2016年09月08日 09:57:10[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1461
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：假设你有一个数组，它的第i个元素是一支给定的股票在第i天的价格。设计一个算法来找到最大的利润。你最多可以完成两笔交易。

样例：给出一个样例数组 [4,4,6,1,1,4,2,5], 返回 6




之前，关于“买卖股票”的故事，我们已经讲过两集：[买卖股票的最佳时机](http://blog.csdn.net/guoziqing506/article/details/51434777)，[买卖股票的最佳时机 II](http://blog.csdn.net/guoziqing506/article/details/51435452)。

我默认你看这篇文章的时候，已经对前面两道题的方法彻底明白了。前面的第一道题关键是只能进行一次交易，第二道题的关键是可以进行任意次交易，而现在的问题是要进行两次交易，且只能进行两次交易。只能一次交易的时候，是根据数组特性，使用贪心法，任意次交易时，就更简单了，只要股票涨价，就交易，求总和即可。但是现在这种只能交易两次的情况，无疑，比前面的两道题都难。




可以这样来思考：既然我们已知进行一次交易的最大值了，那么，想要两次交易获得最大值，就存在以下可能的三种情况：在此之前，为了方便描述，我假设完成一次利润最大交易的开始时间为left，结束时间为right

1. 在left之前，进行一次交易，使得利润最大，这个最大利润加上left-right之间的利润，就是最终最大利润

2. 在right之后，进行一次交易，使得利润最大，这个最大利润加上left-right之间的利润，就是最终最大利润

3. 在left-right之间去掉最大损失，这样把left-right分成两段，这两段的利润和就是最大利润




换句话说，我们需要做的是求出上面3中情况的值，求他们的最大值。而left-right之间的最大损失的计算方法与最大利润的计算方法是极为相似的，不再赘述。




代码如下：



```python
class Solution:
    """
    @param prices: Given an integer array
    @return: Maximum profit
    """
    def maxProfit(self, prices):

        result1 = self.profit(prices)
        left, right = result1[0], result1[1]

        r1 = self.profit(prices[:left])[-1] + result1[-1]
        r2 = self.profit(prices[right + 1:])[-1] + result1[-1]
        r3 = result1[-1] - self.loss(prices[left + 1: right])

        return max(r1, r2, r3)

    def profit(self, prices):

        n = len(prices)
        if n < 2:
            return [0, 0, 0]

        sum_val, max_val = 0, 0
        left, right = 0, 1

        # 将可能成为left的值存储为temp
        temp = left

        for i in range(n - 1):

            sum_val += prices[i + 1] - prices[i]

            # 获得更大利润了，更新left, right以及max_val
            if sum_val > max_val:
                right = i + 1
                left = temp
                max_val = sum_val

            sum_val = max(0, sum_val)

            # 说明上一步中，sum_val <= 0，需更新temp
            if sum_val == 0:
                temp = i + 1

        # 返回左右值及最大利润
        return [left, right, max_val]

    def loss(self, prices):

        n = len(prices)
        if n < 2:
            return 0

        sum_val, min_val = 0, 0

        for i in range(n - 1):
            sum_val += prices[i + 1] - prices[i]
            min_val = min(min_val, sum_val)
            sum_val = min(0, sum_val)
            
        return min_val
```








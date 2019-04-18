# LeetCode  Best Time to Buy and Sell Stock  买卖股票的最佳时机 （DP） - xcw0754 - 博客园
# [LeetCode  Best Time to Buy and Sell Stock  买卖股票的最佳时机 （DP）](https://www.cnblogs.com/xcw0754/p/4679913.html)
题意：给定一个序列，第i个元素代表第i天这支股票的价格，问在最佳时机买入和卖出能赚多少钱？只买一次，且仅1股，假设本钱无限。
思路：要找一个最低价的时候买入，在最高价的时候卖出利润会最大。但是时间是不能冲突的，比如说在明天买入，今天卖出。因此，对于今天的价格，应该要找到今天之前的该股的最低价，买入，今天卖出。
　　其实就是要为序列中的一个元素A[k]，找到另一个元素A[e]，位置满足e<k，结果使得A[k]-A[e]最大。
　　用动态规划解决，从左扫起，遇到一个元素就更新当前最小值，再用当前元素去减这个最小值。扫完就知道结果了。
```
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int small=2147483647, ans=0;
        for(int i=0; i<prices.size(); i++)
        {
            small=min(small, prices[i]);    //找到i之前的最小值
            ans=max(  prices[i]-small,  ans );
        }
        return ans;
    }
};
AC代码
```


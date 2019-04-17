# leetcode-121. Best Time to Buy and Sell Stock 买卖股票的最佳时机 - 别说话写代码的博客 - CSDN博客





2019年01月26日 22:34:45[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：18标签：[买卖股票的最佳时机																[Best Time to Buy and Sell Stoc																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Best Time to Buy and Sell Stoc&t=blog)



|Say you have an array for which the *i*th element is the price of a given stock on day *i*.If you were only permitted to complete at most one transaction (i.e., buy one and sell one share of the stock), design an algorithm to find the maximum profit.Note that you cannot sell a stock before you buy one.**Example 1:****Input:** [7,1,5,3,6,4]**Output:** 5**Explanation:** Buy on day 2 (price = 1) and sell on day 5 (price = 6), profit = 6-1 = 5.             Not 7-1 = 6, as selling price needs to be larger than buying price.**Example 2:****Input:** [7,6,4,3,1]**Output:** 0**Explanation:** In this case, no transaction is done, i.e. max profit = 0.|给定一个数组，它的第 *i* 个元素是一支给定股票第 *i* 天的价格。如果你最多只允许完成一笔交易（即买入和卖出一支股票），设计一个算法来计算你所能获取的最大利润。注意你不能在买入股票前卖出股票。**示例 1:****输入:** [7,1,5,3,6,4]**输出:** 5**解释: **在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5 。     注意利润不能是 7-1 = 6, 因为卖出价格需要大于买入价格。**示例 2:****输入:** [7,6,4,3,1]**输出:** 0**解释: **在这种情况下, 没有交易完成, 所以最大利润为 0。|
|----|----|

思路：遍历一遍就能得出答案。用mn表示当前最小元素，用res表示当前元素 与最小元素差的最大值。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if(prices.size()<2) return 0;
        int mn=prices[0],res=0;
        for(int i=1;i<prices.size();++i)
        {
            mn=min(mn,prices[i]);
            res=max(res,prices[i]-mn);
        }return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=买卖股票的最佳时机&t=blog)





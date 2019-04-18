# LeetCode Best Time to Buy and Sell Stock II （简单题） - xcw0754 - 博客园
# [LeetCode Best Time to Buy and Sell Stock II （简单题）](https://www.cnblogs.com/xcw0754/p/4918527.html)
题意：
　　股票买卖第2题。给出每天的股票价格，每次最多买一股，可以多次操作，但是每次在买之前必须保证身上无股票。问最大的利润？
思路：
　　每天的股票价格可以看成是一条曲线，能卖掉就卖掉，那么肯定是在上升的时候就可以卖掉，但是在不卖的时候要保证自己身上的那只股票的价格是最低价买进的。
```
1 class Solution {
 2 public:
 3     int maxProfit(vector<int>& prices)
 4     {
 5         int pre=2147483647, ans=0;
 6         for(int i=0; i<prices.size(); i++)
 7         {
 8             if( pre<2147483647 && pre<prices[i] )
 9             {
10                 ans+=prices[i]-pre;
11                 pre=prices[i];            
12             }
13             else
14                 pre=min(pre,prices[i]);
15         }
16         return ans;
17     }
18 };
AC代码
```


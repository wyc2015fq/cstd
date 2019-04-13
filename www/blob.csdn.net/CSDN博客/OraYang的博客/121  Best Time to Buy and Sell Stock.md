
# 121. Best Time to Buy and Sell Stock - OraYang的博客 - CSDN博客

2017年07月24日 10:48:10[OraYang](https://me.csdn.net/u010665216)阅读数：135标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Say you have an array for which theithelement is the price of a given stock on dayi.
If you were only permitted to complete at most one transaction (ie, buy one and sell one share of the stock), design an algorithm to find the maximum profit.
Example 1:
Input: [7, 1, 5, 3, 6, 4]
Output: 5
max. difference = 6-1 = 5 (not 7-1 = 6, as selling price needs to be larger than buying price)
Example 2:
Input: [7, 6, 4, 3, 1]
Output: 0
In this case, no transaction is done, i.e. max profit = 0.思路：这道题目关键是要对数组进行变换，股票操作都是后面的售出价减去前面的购买价，那么我们可以先将数组相邻两项相减，观察数组结构[-6,4,-2,3,-2]，然后通过观察发现，原数据的最大利润就是求变换后数组的最大子数组的和。而求一个数组的最大子数组的和就是个典型的动态规划问题，具体见上一篇博客[53.
 Maximum Subarray](http://blog.csdn.net/u010665216/article/details/75999633)代码：

```python
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        vector<int> res;
        if(prices.size()<=1)
            return 0;
        for(int i=0;i<prices.size()-1;i++)
            res.push_back(prices[i+1]-prices[i]);
        int result = maxSubArray(res);
        return result<0?0:result;
        
    }
private:
    int maxSubArray(vector<int>& nums) {
        int num = nums.size();
        int maxSum[num]; 
        int i;
        maxSum[0] = nums[0];
        for(i=1;i<num;i++)
        {
            maxSum[i] =max(maxSum[i-1]+nums[i],nums[i]);
        }
          for(i=1;i<num;i++)
        {
          if(maxSum[0]<maxSum[i])
              maxSum[0] = maxSum[i];
    }
        
        return maxSum[0];    
        
    }
};
```




# C++实现买卖股票的最佳时机算法 - fanyun的博客 - CSDN博客
2018年12月31日 17:56:14[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：280标签：[股票买卖最佳时机算法](https://so.csdn.net/so/search/s.do?q=股票买卖最佳时机算法&t=blog)
个人分类：[C++																[算法](https://blog.csdn.net/fanyun_01/article/category/7113106)](https://blog.csdn.net/fanyun_01/article/category/6102444)
1.要求，给定一个数组，它的第 *i* 个元素是一支给定股票第 *i* 天的价格。如果你最多只允许完成一笔交易（即买入和卖出一支股票），设计一个算法来计算你所能获取的最大利润。注意你不能在买入股票前卖出股票。
2.实现思路：
这是一个经典的动态规化问题，关键点是当前最大收益可以在线处理，所以只用O(n)的时间复杂度就可以解决。
示例1：
输入: [7,1,5,3,6,4]
输出: 5
解释: 在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5 。
注意利润不能是 7-1 = 6, 因为卖出价格需要大于买入价格。
示例 2:
输入: [7,6,4,3,1]
输出: 0
解释: 在这种情况下, 没有交易完成, 所以最大利润为 0。:
3.代码实现
我们需要找出给定数组中两个数字之间的最大差值（即，最大利润）。此外，第二个数字（卖出价格）必须大于第一个数字（买入价格）。
形式上，对于每组 ii 和 jj（其中 j > ij>i）我们需要找出 \max(prices[j] - prices[i])max(prices[j]−prices[i])。
1).暴力法
复杂度分析
时间复杂度：O(n^2)O(n2)。循环运行 \dfrac{n (n-1)}{2}2n(n−1)​ 次。
空间复杂度：O(1)O(1)。只使用了两个变量 —— \text{maxprofit}maxprofit 和 \text{profit}profit。 
```cpp
int maxProfit(int* prices, int pricesSize)
{
	int max = 0;
	for(int i = 0; i < pricesSize; i++)
	{
		for(int j = i + 1; j < pricesSize; j++)
		{
			int tmp = prices[j] - prices[i];
			if(tmp > max)
			{
				max = tmp;
			}
		}
	}
	return max;
}
```
2).一次遍历法
假设给定的数组为：[7, 1, 5, 3, 6, 4]。如果我们在图表上绘制给定数组中的数字，找到最小的谷之后的最大的峰。 我们可以维持两个变量——minprice 和 maxprofit，它们分别对应迄今为止所得到的最小的谷值和最大的利润（卖出价格与最低价格之间的最大差值）。
**复杂度分析**
时间复杂度：O(n)O(n)。只需要遍历一次。
空间复杂度：O(1)O(1)。只使用了两个变量。
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) 
    {
       int minprice = INT_MAX;
       int maxprofit = 0;
       int n = prices.size();
       for(int i=0;i<n;i++)
       {
           if(prices[i]<minprice)
           {
               minprice = prices[i];
           }
           else if(prices[i]-minprice>maxprofit)
           {
               maxprofit = prices[i] - minprice;
           }
       }
        return maxprofit;
    }
};
```

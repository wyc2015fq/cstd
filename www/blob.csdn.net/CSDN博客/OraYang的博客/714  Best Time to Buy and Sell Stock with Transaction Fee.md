
# 714. Best Time to Buy and Sell Stock with Transaction Fee - OraYang的博客 - CSDN博客

2017年12月25日 10:15:44[OraYang](https://me.csdn.net/u010665216)阅读数：133标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 714. Best Time to Buy and Sell Stock with Transaction Fee
标签（空格分隔）： leetcode array medium
---题目

---Your are given an array of integers

---prices

---, for which the

---i

----th element is the price of a given stock on day

---i

---; and a non-negative integer

---fee

---representing a transaction fee.

---You may complete as many transactions as you like, but you need to pay the transaction fee for each transaction.  You may not buy more than 1 share of a stock at a time (ie. you must sell the stock share before you buy again.)

---Return the maximum profit you can make.

---Example 1:


---Input:

---prices = [1, 3, 2, 8, 4, 9], fee = 2

---Output:

---8

---Explanation:

---The maximum profit can be achieved by:

---Buying at prices[0] = 1

---Selling at prices[3] = 8

---Buying at prices[4] = 4

---Selling at prices[5] = 9

---The total profit is ((8 - 1) - 2) + ((9 - 4) - 2) = 8.

---Note:

---0 < prices.length <= 50000

---.

---0 < prices[i] < 50000

---.

---0 <= fee < 50000

---.


---思路

---本题是个典型的股票题，由于股票可以买卖多次，因此我们定义两个变量：s0,s1;其中s0和s1的定义如下：

---s0：代表买完股票的利润

---s1：代表卖完股票的利润

---更新规则：

---update s0 by selling the stock from s1, so s0 = max(s0, s1+p);

---update s1 by buying the stock from s0, so s1 = max(s1, s0-p-fee);

---代码

---class

---Solution {

---public

---:

---int

---maxProfit(

---vector

---<

---int

--->

---& prices,

---int

---fee) {

---int

---s0=

---0

---,s1=INT_MIN,tmp;

---for

---(

---auto

---p:prices)
        {
            tmp=s0;
            s0=max(s0,s1+p);
            s1=max(s1,tmp-p-fee);
        }

---return

---s0;
    }
};



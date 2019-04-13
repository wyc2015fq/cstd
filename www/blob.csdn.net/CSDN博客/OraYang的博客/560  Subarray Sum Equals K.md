
# 560. Subarray Sum Equals K - OraYang的博客 - CSDN博客

2017年12月22日 11:09:41[OraYang](https://me.csdn.net/u010665216)阅读数：410标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 560. Subarray Sum Equals K
标签（空格分隔）： leetcode array medium
---题目


---Given an array of integers and an integer

---k

---, you need to find the total number of continuous subarrays whose sum equals to

---k

---.

---Example 1:


---Input:

---nums = [1,1,1], k = 2

---Output:

---2

---Note:


---The length of the array is in range [1, 20,000].

---The range of numbers in the array is [-1000, 1000] and the range of the integer

---k

---is [-1e7, 1e7].


---思路

---这道题目我们一开始最简单的思路一定是平方算法，就是暴力计算下[i,j]之间的数值和。时间复杂度是

---O(N^2)

---,很明显这个会造成TLE。如果有人看过算法珠玑第八章的程序设计。一定会有这样的思路，就是计算出前i个元素的累加和。那么要判断[i,j]之间数值和就是计算cum[j]-cum[i-1]的数值是否为k。我们可以建立一个hashmap来存储cum，具体看代码。

---代码

---class

---Solution {

---public

---:

---int

---subarraySum(

---vector

---<

---int

--->

---& nums,

---int

---k) {

---unordered_map

---<

---int

---,

---int

--->

---presum;

---int

---cum=

---0

---,count=

---0

---;
        presum[

---0

---]++;

---for

---(

---auto

---num:nums){
            cum+=num;
            count+=presum[cum-k];
            presum[cum]++;
        }

---return

---count;
    }
};



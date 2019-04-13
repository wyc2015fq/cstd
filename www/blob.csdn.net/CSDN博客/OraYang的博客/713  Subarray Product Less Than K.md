
# 713. Subarray Product Less Than K - OraYang的博客 - CSDN博客

2017年12月18日 11:05:19[OraYang](https://me.csdn.net/u010665216)阅读数：108标签：[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 713. Subarray Product Less Than K
---题目

---Your are given an array of positive integers

---nums

---.

---Count and print the number of (contiguous) subarrays where the product of all the elements in the subarray is less than

---k

---.

---Example 1:


---Input:

---nums = [10, 5, 2, 6], k = 100

---Output:

---8

---Explanation:

---The 8 subarrays that have product less than 100 are: [10], [5], [2], [6], [10, 5], [5, 2], [2, 6], [5, 2, 6].
Note that [10, 5, 2] is not included as the product of 100 is not strictly less than k.

---Note:

---0 < nums.length <= 50000

---.

---0 < nums[i] < 1000

---.

---0 <= k < 10^6

---.


---思路

---本题思路比较巧妙，维护两个索引j和i，每次统计当乘积小于k时j-i的数量。

---代码

---class

---Solution {

---public

---:

---int

---numSubarrayProductLessThanK(

---vector

---<

---int

--->

---& nums,

---int

---k) {

---if

---(k<=

---1

---)

---return

---0

---;

---int

---product=

---1

---,i=

---0

---,j=

---0

---,count=

---0

---;

---while

---(j<nums.size())
        {
            product*=nums[j++];

---while

---(product>=k)
                product/=nums[i++];

---cout

---<<(j-i)<<endl;
            count+=j-i;
        }

---return

---count;
    }
};



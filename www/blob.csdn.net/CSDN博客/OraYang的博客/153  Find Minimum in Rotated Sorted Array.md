
# 153. Find Minimum in Rotated Sorted Array - OraYang的博客 - CSDN博客

2017年12月22日 15:05:32[OraYang](https://me.csdn.net/u010665216)阅读数：408标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 153. Find Minimum in Rotated Sorted Array
标签（空格分隔）： leetcode array medium
---题目

---Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.

---(i.e.,

---0 1 2 4 5 6 7

---might become

---4 5 6 7 0 1 2

---).

---Find the minimum element.

---You may assume no duplicate exists in the array.


---思路

---本题考察的是二分搜索，这里需要建立两个中间变量middle1,middle2,代码很好理解。

---代码

---class

---Solution {

---public

---:

---int

---findMin(

---vector

---<

---int

--->

---& nums) {

---int

---l=

---0

---,r=nums.size()-

---1

---,middle1,middle2;

---//判断只有一个元素的情况

---if

---(r==

---0

---)

---return

---nums[

---0

---];

---//判断没有发生反转的情况

---if

---(nums[r]>nums[l])

---return

---nums[l];

---while

---(l<r)
        {
            middle1 =(l+r)/

---2

---;
            middle2 =(l+r)/

---2

---+

---1

---;

---if

---(nums[middle1]<nums[middle2]){

---//判断l,r更新的值

---if

---(nums[middle1]>nums[

---0

---])
                    l=middle2;

---else

---r=middle1;
            }

---if

---(nums[middle1]>nums[middle2])
            {

---return

---nums[middle2];
            }
        }

---return

---nums[l];    
    }
};



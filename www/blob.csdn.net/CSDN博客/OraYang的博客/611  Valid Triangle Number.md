
# 611. Valid Triangle Number - OraYang的博客 - CSDN博客

2017年12月25日 09:16:01[OraYang](https://me.csdn.net/u010665216)阅读数：170标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



\# 611. Valid Triangle Number
标签（空格分隔）： leetcode array medium
—



\# 题目Given an array consists of non-negative integers,  your task is to count the number of triplets chosen from the array that can make triangles if we take them as side lengths of a triangle.**Example 1:**
**Input:**[2,2,3,4]**Output:**3**Explanation:**Valid combinations are: 
2,3,4 (using the first 2)
2,3,4 (using the second 2)
2,2,3
**Note:**
The length of the given array won’t exceed 1000.
The integers in the given array are in the range of [0, 1000].

---思路

---本题判断的规则是最小两边之和大于第三边。我们实现了一个时间复杂度为

---O(n^2)

---，空间复杂度为O(1)的方法。基本思路就是先排序，再维护头尾指针遍历。

---代码

---class

---Solution {

---public

---:

---int

---triangleNumber(

---vector

---<

---int

--->

---& nums) {

---//step1:先排序

---sort(nums.begin(),nums.end());

---int

---count=

---0

---,num=nums.size(),l,r;

---//遍历

---for

---(

---int

---i=num-

---1

---;i>=

---2

---;i--){
            l=

---0

---,r=i-

---1

---;

---while

---(l<r)
            {

---if

---(nums[l]+nums[r]>nums[i])
                {
                    count+=r-l;
                    r--;
                }

---else

---l++;
            }
        }

---return

---count;
    }
};



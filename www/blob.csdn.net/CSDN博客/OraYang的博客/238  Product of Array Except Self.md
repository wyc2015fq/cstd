
# 238. Product of Array Except Self - OraYang的博客 - CSDN博客

2017年12月29日 15:50:23[OraYang](https://me.csdn.net/u010665216)阅读数：467标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 238. Product of Array Except Self
标签（空格分隔）： leetcode array medium
---题目

---Given an array of

---n

---integers where

---n

---> 1,

---nums

---, return an array

---output

---such that

---output[i]

---is equal to the product of all the elements of

---nums

---except

---nums[i]

---.

---Solve it

---without division

---and in O(

---n

---).

---For example, given

---[1,2,3,4]

---, return

---[24,12,8,6]

---.

---Follow up:

---Could you solve it with constant space complexity? (Note: The output array

---does not

---count as extra space for the purpose of space complexity analysis.)


---思路

---为了便于理解我们举个例子。

---给定数组

---[2, 3, 4, 5]

---, 指定第三个数字4, 那么除了4之外的乘积就是

---2*3*5

---上述乘积由两部分组成: left

---2*3

---and right

---5

---. 这个乘积就是

---left*right

---. 我们可以通过下面的步骤获得左右两部分的值:

---Numbers:

---2

---3

---4

---5
Lefts:

---2

---2*3 2*3*4
Rights:  3*4*5  4*5

---5

---我们将空白部分填入数值

---Numbers:

---2

---3

---4

---5
Lefts:

---1

---2

---2*3 2*3*4
Rights:  3*4*5  4*5

---5

---1

---我们可以通过两个循环获得该乘积. 时间复杂度是 O(n).

---—\# 代码

---class

---Solution {

---public

---:

---vector

---<

---int

--->

---productExceptSelf(

---vector

---<

---int

--->

---& nums) {

---vector

---<

---int

--->

---res(nums.size());

---int

---left =

---1

---;
        res[

---0

---]=

---1

---;

---for

---(

---int

---i=

---1

---;i<nums.size();i++)
        {
            res[i] = res[i-

---1

---]*nums[i-

---1

---];
        }

---int

---right =

---1

---;

---for

---(

---int

---j=nums.size()-

---1

---;j>=

---0

---;j--)
        {
            res[j] *=right;
            right *=nums[j];
        }

---return

---res;
    }
};




# 442. Find All Duplicates in an Array - OraYang的博客 - CSDN博客

2017年12月12日 17:57:39[OraYang](https://me.csdn.net/u010665216)阅读数：106标签：[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given an array of integers, 1 ≤ a[i] ≤ n (n = size of array), some elements appear twice and others appear once.
Find all the elements that appear twice in this array.
Could you do it without extra space and in O(n) runtime?
Example:
> Input:

> [4,3,2,7,8,2,3,1]

> Output:

> [2,3]

> 思路

> 本题找重复项，在大小为n的数组中1 ≤ a[i] ≤ n的前提下，我们如何判断一个数是重复项？有一个很巧妙的思路，首先遍历数组，当找到数值i的时候，如果nums[abs(i)-1]<0，我们就认为数值i是重复值，否则就将nums[abs(i)-1]置为负数。

> 代码

> class
> Solution {
> public
> :
> vector
> <
> int
> >
> findDuplicates(
> vector
> <
> int
> >
> & nums) {
> vector
> <
> int
> >
> res;
> for
> (
> int
> i=
> 0
> ;i<nums.size();i++)
        {
> int
> index =
> abs
> (nums[i])-
> 1
> ;
> if
> (nums[index]<
> 0
> )
            {
                res.push_back(index+
> 1
> );
            }
> else
> nums[index]=-nums[index];
        }
> return
> res;    
    }
};


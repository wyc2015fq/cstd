
# 31. Next Permutation - OraYang的博客 - CSDN博客

2017年09月13日 10:42:09[OraYang](https://me.csdn.net/u010665216)阅读数：118个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.
If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).
The replacement must be in-place, do not allocate extra memory.
Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.
1,2,3 → 1,3,2
3,2,1 → 1,2,3
1,1,5 → 1,5,1
## 思路
本题的思路是基于对题目的理解，解题思路如下：
> 有如下的一个数组

> 1　　2　　7　　4　　3　　1

> 下一个排列为：

> 1　　3　　1　　2　　4　　7

> 那么是如何得到的呢，我们通过观察原数组可以发现，如果从末尾往前看，数字逐渐变大，到了2时才减小>的，然后我们再从后往前找第一个比2大的数字，是3，那么我们交换2和3，再把此时3后面的所有数字转

> 置一下即可，步骤如下：

> 1
> 2
> 7　　4　　3　　1

> 1
> 2
> 7　　4
> 3
> 1

> 1
> 3
> 7　　4
> 2
> 1

> 1　　3　　1
> 2
> 4
> 7

> 代码

> class
> Solution {
> public
> :
> void
> nextPermutation(
> vector
> <
> int
> >
> & nums) {
> int
> i,j,n = nums.size();
> for
> (i=n-
> 2
> ;i>=
> 0
> ;--i)
        {
> if
> (nums[i+
> 1
> ]>nums[i])
            {
> for
> (j=n-
> 1
> ;j>=i;--j)
                {
> if
> (nums[j]>nums[i])
> break
> ;
                }
> cout
> <<nums[i]<<endl;
                swap(nums[i],nums[j]);    
                reverse(nums.begin()+i+
> 1
> ,nums.end());
> return
> ; 
            }
        }
        reverse(nums.begin(),nums.end());
    }
};


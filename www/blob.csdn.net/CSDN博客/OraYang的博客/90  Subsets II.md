
# 90. Subsets II - OraYang的博客 - CSDN博客

2017年12月03日 10:37:52[OraYang](https://me.csdn.net/u010665216)阅读数：229标签：[Subset																](https://so.csdn.net/so/search/s.do?q=Subset&t=blog)[Array																](https://so.csdn.net/so/search/s.do?q=Array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Subset&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=Subset&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=Subset&t=blog)



## 题目
Given a collection of integers that might contain duplicates, nums, return all possible subsets (the power set).
Note: The solution set must not contain duplicate subsets.
For example,
If nums = [1,2,2], a solution is:
> [

> [2],

> [1],

> [1,2,2],

> [2,2],

> [1,2],

> []

> ]

> 思路

> 这道题目是典型的数组类型的题目，求带有重复项数组的所有子集。在解决这个问题之前我们先明确一个问题。假如一个大小为N的数组不含重复项，那么他的所有子集的个数就是
> 2^N
> 个。这个是很好理解的，因为对于数组里每个元素，我们都有两种选择，选取或不选取，N个元素就总共有
> 2^N
> 个选择了。

> 但是现在问题来了，题目中要求数组中可以含有重复项，那么我们该如何解决这个问题呢？其实很简单，以题目中的例子为例，对于数组
> nums = [1,2,2]
> ，里面的重复项是2，这里我们并不把2看成是两个数字，而是把2看成一个特殊的数字，但是对2有三种选择：不把2加入子集；把一个2加入子集；把两个2加入子集。怎么样？是不是思路很清晰。那么接下来我们的算法步骤就是：

> 初始化空字典

> 循环遍历数组

> 对于每次遇到的元素，统计重复项个数count

> 按元素选择次数加入数组，并加入字典中

> 退出循环，返回字典

> 代码

> class
> Solution {
> public
> :
> vector
> <
> vector
> <
> int
> >
> >
> subsetsWithDup(
> vector
> <
> int
> >
> & nums) {
> //step1:sort
> sort(nums.begin(),nums.end());
> //step2:loop
> vector
> <
> vector
> <
> int
> >
> >
> res = {{}};
> //cout<<res.size()<<endl;
> for
> (
> int
> i=
> 0
> ;i<nums.size();)
        {
> //step3:find numbers of duplicates
> int
> count =
> 0
> ;
> while
> ((count+i)<nums.size()&&nums[i+count]==nums[i])
            {
                count++;
            }
> //step4:extend subsets
> int
> num = res.size();
> for
> (
> int
> j=
> 0
> ;j<num;j++)
            {
> vector
> <
> int
> >
> instance = res[j];
> for
> (
> int
> y=
> 0
> ;y<count;y++)
                {
                    instance.push_back(nums[i]);
                    res.push_back(instance);
                }  
            }
            i+=count;
        }
> return
> res; 
    }
};


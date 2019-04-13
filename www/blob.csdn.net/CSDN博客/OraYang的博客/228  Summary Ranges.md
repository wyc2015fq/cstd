
# 228. Summary Ranges - OraYang的博客 - CSDN博客

2017年12月15日 13:54:50[OraYang](https://me.csdn.net/u010665216)阅读数：249标签：[Summary																](https://so.csdn.net/so/search/s.do?q=Summary&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Summary&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=Summary&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=Summary&t=blog)



# 题目
Given a sorted integer array without duplicates, return the summary of its ranges.
**Example 1:**
> Input: [0,1,2,4,5,7]

> Output: [“0->2”,”4->5”,”7”]

> Example 2:

> Input: [0,2,3,4,6,8,9]

> Output: [“0”,”2->4”,”6”,”8->9”]

> 思路

> 本题按照题意需要将数组中连续数字转换成用箭头表示的范围。观察Example1及Example2就能明白。思路比较简单。这里介绍一个O(n)时间复杂度的算法。维护两个变量，分别代表范围数字起始数字索引start及结尾数字索引end。然后扫描一遍数组，并按照”start->end”or “start”的形式保存在字符串数组中。

> 代码

> class
> Solution {
> public
> :
> vector
> <
> string
> >
> summaryRanges(
> vector
> <
> int
> >
> & nums) {
> vector
> <
> string
> >
> res;
> if
> (nums.size()<=
> 0
> )
> return
> res;
> int
> start =
> 0
> ,end=
> 0
> ;
> for
> (
> int
> i=
> 1
> ;i<nums.size();i++)
        {
> if
> (nums[i-
> 1
> ]+
> 1
> ==nums[i])
            {
                end = i;
> continue
> ;
            }
> else
> if
> (end>start)
                res.push_back(to_string(nums[start])+
> "->"
> +to_string(nums[end]));
> else
> res.push_back(to_string(nums[start]));    
            start = i;
            end = i;
        }
> if
> (end>start)
            res.push_back(to_string(nums[start])+
> "->"
> +to_string(nums[end]));
> else
> res.push_back(to_string(nums[start]));
> return
> res;
    }
};


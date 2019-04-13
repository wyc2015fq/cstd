
# 553. Optimal Division - OraYang的博客 - CSDN博客

2017年12月06日 11:55:11[OraYang](https://me.csdn.net/u010665216)阅读数：158标签：[math																](https://so.csdn.net/so/search/s.do?q=math&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=math&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a list of positive integers, the adjacent integers will perform the float division. For example, [2,3,4] -> 2 / 3 / 4.
However, you can add any number of parenthesis at any position to change the priority of operations. You should find out how to add parenthesis to get the maximum result, and return the corresponding expression in string format. Your expression should NOT contain redundant parenthesis.
Example:
> Input: [1000,100,10,2]

> Output: “1000/(100/10/2)”

> Explanation:

> 1000/(100/10/2) = 1000/((100/10)/2) = 200

> However, the bold parenthesis in “1000/((100/10)/2)” are redundant,

> since they don’t influence the operation priority. So you should return “1000/(100/10/2)”.

> Other cases:

> 1000/(100/10)/2 = 50

> 1000/(100/(10/2)) = 50

> 1000/100/10/2 = 0.5

> 1000/100/(10/2) = 2

> Note:

> The length of the input array is [1, 10].

> Elements in the given array will be in range [2, 1000].

> There is only one optimal division for each test case.

> 思路

> 这道题目，给你一个数组，里面都是正整数，默认相邻整数之间做整除运算”/“。你可以给这些算式添加括号，但不能重复添加，或者添加不必要的括号。使得你添加括号后这个算式的结果最大。这道题目把许多人难住了，包括我。我拿到这个题目，第一个想法是，我应该从上述式子中找规律。我猜想这个式子应该满足某种规律从而使得添加括号后数值最大。但是不幸的是，在我看了没10秒钟，我突然有种强烈的感觉，这道题目应该用动态规划来做，于是在dp的路上越走越远。。。其实大家如果多花点时间观察算式，就能发现，无论如何，算式
> X_1/X_2/X_3.../X_n
> 的分子都是X_1，要想算式值最大，分母就要最小，在分母全是正整数的情况下
> X_2/X_3/X_4.../X_n
> 的值就是最小的！所以问题很简单，代码的思路也有了，唯一要注意的是注意特殊情况，当数组大小小于等于2时需要特殊处理。

> 代码

> class
> Solution {
> public
> :
> string
> optimalDivision(
> vector
> <
> int
> >
> & nums) {
> string
> res;
> if
> (nums.size()==
> 0
> )
> return
> res;
> else
> if
> (nums.size()==
> 1
> )
        {
            res = to_string(nums[
> 0
> ]);
> return
> res;
        }
> else
> if
> (nums.size()==
> 2
> )
            res = to_string(nums[
> 0
> ])+
> "/"
> +to_string(nums[
> 1
> ]);
> else
> {
            res = to_string(nums[
> 0
> ])+
> "/("
> +to_string(nums[
> 1
> ]);
> for
> (
> int
> i=
> 2
> ;i<nums.size();i++)
                res+=
> "/"
> +to_string(nums[i]);
            res+=
> ")"
> ;
        }
> return
> res;
    }
};


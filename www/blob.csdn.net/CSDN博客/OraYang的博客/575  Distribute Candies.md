
# 575. Distribute Candies - OraYang的博客 - CSDN博客

2017年10月06日 20:37:53[OraYang](https://me.csdn.net/u010665216)阅读数：173个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given an integer array with even length, where different numbers in this array represent different kinds of candies. Each number means one candy of the corresponding kind. You need to distribute these candies equally in number to brother and sister. Return the maximum number of kinds of candies the sister could gain.
> Example 1:

> Input: candies = [1,1,2,2,3,3]

> Output: 3

> Explanation:

> There are three different kinds of candies (1, 2 and 3), and two candies for each kind.

> Optimal distribution: The sister has candies [1,2,3] and the brother has candies [1,2,3], too.

> The sister has three different kinds of candies.

> Example 2:

> Input: candies = [1,1,2,3]

> Output: 2

> Explanation: For example, the sister has candies [2,3] and the brother has candies [1,1].

> The sister has two different kinds of candies, the brother has only one kind of candies.

> Note:

> The length of the given array is in range [2, 10,000], and will be even.

> The number in given array is in range [-100,000, 100,000].

> 思路

> 这道题目很长,但是！是真的很简单，只要一个逻辑判断就足以解决，首先求出数组里面有几个不同的数，赋值给num2,然后将原数组大小赋值给num1,如果num2>=num1/2，那么返回num1/2;否则返回num2（是不是真的很简单？）

> 代码

> class
> Solution {
> public
> :
> int
> distributeCandies(
> vector
> <
> int
> >
> & candies) {
> unordered_map
> <
> int
> ,
> int
> >
> mp;
> int
> num1 = candies.size(),num2;
> for
> (
> int
> i=
> 0
> ;i<candies.size();i++)
        {
            mp[candies[i]]++;
        }
        num2 = mp.size();
> if
> (num2>=num1/
> 2
> )
> return
> num1/
> 2
> ;
> else
> return
> num2;
    }
};



# 229. Majority Element II - OraYang的博客 - CSDN博客

2017年12月14日 17:09:06[OraYang](https://me.csdn.net/u010665216)阅读数：308标签：[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[majority																](https://so.csdn.net/so/search/s.do?q=majority&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=majority&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=array&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=array&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)



# 题目
Given an integer array of size n, find all elements that appear more than ⌊ n/3 ⌋ times. The algorithm should run in linear time and in O(1) space.
# 思路
这道题目求频繁项，且数字次数大于 ⌊ n/3 ⌋ ，这个限制条件就限制了频繁项的个数一定小于等于2。又因为题目要求使用O(1)的空间和O(n)的时间，因此我们可以借鉴多数投票表决算法。Hesselink在2005年发表了一篇文章，文章中提出了一种[The Boyer-Moore Majority Vote Algorithm with a majority of voting rabbits](http://www.cs.rug.nl/~wim/pub/whh348.pdf)的方法。
> 我们先来看下算法要解决的问题是：

> 假设你有一个没有排序的整数数组。 你想知道列表中是否存在列表中超过一半的值。 如果存在，这样的值是什么？ 如果不存在，你需要确定不存在。 你需要足够高效地完成解决这个问题。

> 该问题一般性的思路就是，先将数组排序，如果存在绝大多数项，那么中间数必定是绝大多数项，最后只需要扫描一遍数组，确定是否重复次数超过数组大小的一半。

> 最后Hesselink提出的多数投票表决法，完美的解决了上述问题，时间复杂度为O(n)，空间复杂度为O(1)。该算法只需要扫描数组两遍：

> 第一遍：如果存在绝大多数项，找到绝大多数项

> 第二遍：统计第一遍找到的数值出现的次数，是否满足题目要求

> 如果整数列表中存在超过一半的值，这个值有且只有一个

> 很明显算法的核心部分在第一遍遍历寻找重复项的过程中：

> 在第一遍遍历中，我们需要2个值：

> candidate1，最初可以设置为任何值。

> count1，最初设置为0。

> 对于我们输入数组中的每个元素，我们首先检查count1。 如果count1等于0，我们将候选值设置为当前元素的值。 接下来，首先比较元素的值和当前的candidate1。 如果他们是一样的，我们count1加1.如果他们不同，我们count1减1。

> 最后我们再遍历数组求candidate1的出现次数，来验证是否满足题目要求。

> 而在本题中，因为如果出现满足题目要求的数，那么这个数的个数一定小于等于2；因此我们维护两个值：

> candidate1，candidate2，最初可以设置为任何值。

> count1，count2，最初设置为0。

> note
> ：如果看不明白，可以结合代码与测试例子来加深理解

> 代码

> class
> Solution {
> public
> :
> vector
> <
> int
> >
> majorityElement(
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
> if
> (nums.size()<
> 0
> )
> return
> res;
> int
> candidate1=
> 0
> ,candidate2=
> 0
> ,count1=
> 0
> ,count2=
> 0
> ;
> for
> (
> auto
> item:nums)
        {
> if
> (item==candidate1)
                count1++;
> else
> if
> (item==candidate2)
                count2++;
> else
> if
> (count1==
> 0
> )
            {
                candidate1=item;
                count1=
> 1
> ;
            }
> else
> if
> (count2==
> 0
> )
            {
                candidate2 = item;
                count2=
> 1
> ;
            }
> else
> {
                count1--;
                count2--;
            }
        }
        count1=
> 0
> ;
        count2=
> 0
> ;
> for
> (
> auto
> item:nums)
        {
> if
> (candidate1==item)
                count1++;
> if
> (candidate2==item)
                count2++;
        }
> if
> (candidate1!=candidate2)
        {
> if
> (count1>nums.size()/
> 3
> )
                res.push_back(candidate1);
> if
> (count2>nums.size()/
> 3
> )
                res.push_back(candidate2);
> return
> res;
        }
> else
> {
> if
> (count1>nums.size()/
> 3
> )
                res.push_back(candidate1);
> return
> res;
        }
    }
};


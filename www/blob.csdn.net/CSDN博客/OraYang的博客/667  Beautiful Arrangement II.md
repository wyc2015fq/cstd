
# 667. Beautiful Arrangement II - OraYang的博客 - CSDN博客

2017年12月29日 17:09:02[OraYang](https://me.csdn.net/u010665216)阅读数：358标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 667. Beautiful Arrangement II
标签（空格分隔）： leetcode array medium
---题目

---Given two integers

---n

---and

---k

---, you need to construct a list which contains

---n

---different positive integers ranging from

---1

---to

---n

---and obeys the following requirement:

---Suppose this list is [a

---1

---, a

---2

---, a

---3

---, … , a

---n

---], then the list [|a

---1

---- a

---2

---|, |a

---2

---- a

---3

---|, |a

---3

---- a

---4

---|, … , |a

---n-1

---- a

---n

---|] has exactly

---k

---distinct integers.

---If there are multiple answers, print any of them.

---Example 1:


---Input:

---n = 3, k = 1

---Output:

---[1, 2, 3]

---Explanation:

---The [1, 2, 3] has three different positive integers ranging from 1 to 3, and the [1, 1] has exactly 1 distinct integer: 1.

---Example 2:


---Input:

---n = 3, k = 2

---Output:

---[1, 3, 2]

---Explanation:

---The [1, 3, 2] has three different positive integers ranging from 1 to 3, and the [2, 1] has exactly 2 distinct integers: 1 and 2.

---Note:


---The

---n

---and

---k

---are in the range 1 <= k < n <= 10

---4

---.


---思路

---首先假设你有个数字n,那么k的最大取值是n-1;如果n是9，那么k就是8

---我们可以通过头尾交叉插入数字来实现，如下所示：

---// start from i = 1, j = n;
// i++, j--, i++, j--, i++, j--
1

---2

---3

---4

---5

---9

---8

---7

---6
out:

---1

---9

---2

---8

---3

---7

---6

---4 5
dif:

---8

---7

---6

---5

---4

---3

---2

---1

---此时如果k < n-1 , 比如说k=5 , 那么有

---i++ j-- i++ j--  i++ i++ i++ ...
out:

---1

---9

---2

---8

---3

---4

---5

---6

---7
dif:

---8

---7

---6

---5

---1

---1

---1

---1

---代码

---class

---Solution {

---public

---:

---vector

---<

---int

--->

---constructArray(

---int

---n,

---int

---k) {

---vector

---<

---int

--->

---res;

---for

---(

---int

---i=

---1

---,j=n;i<=j;k--)
        {

---if

---(k>

---1

---)
            {
                res.push_back(k%

---2

---?i++:j--);
            }

---else

---res.push_back(i++);
        }

---return

---res;
    }
};



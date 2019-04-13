
# 216. Combination Sum III - OraYang的博客 - CSDN博客

2017年12月29日 09:29:31[OraYang](https://me.csdn.net/u010665216)阅读数：173标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 216. Combination Sum III
标签（空格分隔）： leetcode array medium
---题目

---Find all possible combinations of

---k

---numbers that add up to a number

---n

---, given that only numbers from 1 to 9 can be used and each combination should be a unique set of numbers.

---Example 1:

---Input:

---k

---= 3,

---n

---= 7

---Output:


---[[1,2,4]]


---Example 2:

---Input:

---k

---= 3,

---n

---= 9

---Output:


---[[1,2,6], [1,3,5], [2,3,4]]

---思路

---本题本质上是个搜索问题，使用回溯法。

---代码

---class

---Solution {

---public

---:

---vector

---<

---vector

---<

---int

--->

--->

---combinationSum3(

---int

---k,

---int

---n) {

---vector

---<

---vector

---<

---int

--->

--->

---res;

---vector

---<

---int

--->

---tmp;
        combination(res,tmp,k,n);

---return

---res;
    }

---private

---:

---void

---combination(

---vector

---<

---vector

---<

---int

--->

--->

---& res,

---vector

---<

---int

--->

---tmp,

---int

---k,

---int

---n)
    {

---if

---(tmp.size()==k&n==

---0

---)
        {
            res.push_back(tmp);

---return

---;
        }

---if

---(tmp.size()<k)
        {

---for

---(

---int

---i=tmp.empty()?

---1

---:tmp.back()+

---1

---;i<=

---9

---;i++)
        {

---if

---(n-i<

---0

---)

---break

---;
            tmp.push_back(i);
            combination(res,tmp,k,n-i);
            tmp.pop_back();
        }
        }
    }
};



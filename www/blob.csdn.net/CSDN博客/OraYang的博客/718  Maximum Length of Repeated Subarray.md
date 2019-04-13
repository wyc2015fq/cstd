
# 718. Maximum Length of Repeated Subarray - OraYang的博客 - CSDN博客

2017年12月21日 11:22:46[OraYang](https://me.csdn.net/u010665216)阅读数：191标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 718. Maximum Length of Repeated Subarray
标签（空格分隔）： leetcode array medium
---题目

---Given two integer arrays

---A

---and

---B

---, return the maximum length of an subarray that appears in both arrays.

---Example 1:


---Input:

---A: [1,2,3,2,1]
B: [3,2,1,4,7]

---Output:

---3

---Explanation:

---The repeated subarray with maximum length is [3, 2, 1].

---Note:


---1 <= len(A), len(B) <= 1000

---0 <= A[i], B[i] < 100


---思路

---本题是个简单的动态规划问题，定义一个数组dp[j]来存储以A[i]为开头的最大子数组长。

---代码

---//本题是个简单的动态规划问题

---class

---Solution {

---public

---:

---int

---findLength(

---vector

---<

---int

--->

---& A,

---vector

---<

---int

--->

---& B) {

---int

---maxn=

---0

---;

---vector

---<

---int

--->

---dp(B.size()+

---1

---);

---//默认初始化为0

---for

---(

---int

---i=A.size()-

---1

---;i>=

---0

---;i--)

---for

---(

---int

---j=

---0

---;j<B.size();j++)
            {

---if

---(A[i]==B[j])
                {
                    dp[j]=

---1

---+dp[j+

---1

---];
                    maxn = max(maxn,dp[j]);
                }

---else

---dp[j]=

---0

---;
            }

---return

---maxn;
    }
};



# leetcode-70. Climbing Stairs 爬楼梯 - 别说话写代码的博客 - CSDN博客





2019年01月28日 22:00:29[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：18标签：[爬楼梯																[Climbing Stairs																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q= Climbing Stairs&t=blog)



|You are climbing a stair case. It takes *n* steps to reach to the top.Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?**Note:** Given *n* will be a positive integer.**Example 1:****Input:** 2**Output:** 2**Explanation:** There are two ways to climb to the top.1. 1 step + 1 step2. 2 steps**Example 2:****Input:** 3**Output:** 3**Explanation:** There are three ways to climb to the top.1. 1 step + 1 step + 1 step2. 1 step + 2 steps3. 2 steps + 1 step|假设你正在爬楼梯。需要 *n* 阶你才能到达楼顶。每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？**注意：**给定 *n* 是一个正整数。**示例 1：****输入：** 2**输出：** 2**解释：** 有两种方法可以爬到楼顶。1.  1 阶 + 1 阶2.  2 阶**示例 2：****输入：** 3**输出：** 3**解释：** 有三种方法可以爬到楼顶。1.  1 阶 + 1 阶 + 1 阶2.  1 阶 + 2 阶3.  2 阶 + 1 阶|
|----|----|

 思路：dp，递推公式为dp[i]=dp[i-2]+dp[i-1]

```cpp
class Solution {
public:
    int climbStairs(int n) {
        vector<int> res={1,2};
        for(int i=2;i<n;++i)
            res.push_back(res[i-1]+res[i-2]);
        return res[n-1];
    }
};
```](https://so.csdn.net/so/search/s.do?q=爬楼梯&t=blog)





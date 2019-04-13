
# 70. Climbing Stairs - OraYang的博客 - CSDN博客

2017年07月15日 10:26:26[OraYang](https://me.csdn.net/u010665216)阅读数：129标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You are climbing a stair case. It takesnsteps to reach to the top.
Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
Note:Givennwill be a positive integer.
思路：这道题目本质上是个动态规划的问题，虽然也可以用递归的方法做，但是时间复杂度太高，那么用动态规划就需要建立状态转移方程：X(i) = X(i-1) + X(i-2);每次有两种选择，两种选择之后又是各有两种选择，如下图所示：![](https://img-blog.csdn.net/20170715104041011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码：

```python
//简单的动态规划问题
class Solution {
public:
    int climbStairs(int n) {
        vector<int> res(n+1);
        res[0] = 1;
        res[1] = 1;
        for(int i =2;i<=n;i++)
        {
            res[i] = res[i-1] + res[i-2]; 
        }
        return res[n];
        
        
    }
};
```




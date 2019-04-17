# Leetcode746——Min Cost Climbing Stairs - westbrook1998的博客 - CSDN博客





2018年02月19日 19:01:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：55标签：[Leetcode																[算法																[动态规划](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)
个人分类：[Leetcode](https://blog.csdn.net/westbrook1998/article/category/7459919)





## 题目：

> 
On a staircase, the i-th step has some non-negative cost cost[i] assigned (0 indexed).

Once you pay the cost, you can either climb one or two steps. You need to find minimum cost to reach the top of the floor, and you can either start from the step with index 0, or the step with index 1.

Example 1: 

  Input: cost = [10, 15, 20] 

  Output: 15 

  Explanation: Cheapest is start on cost[1], pay that cost and go to the top. 

  Example 2: 

  Input: cost = [1, 100, 1, 1, 1, 100, 1, 1, 100, 1] 

  Output: 6 

  Explanation: Cheapest is start on cost[0], and only step on 1s, skipping cost[3].   
## 题解：

```
public class Solution {
    public static int minCostClimbingStairs(int[] cost){
        int len=cost.length;
        int[] dp=new int[len+1];
        dp[0]=0;
        dp[1]=0;
        for(int i=2;i<len+1;i++){
            dp[i]=Math.min(dp[i-2]+cost[i-2],dp[i-1]+cost[i-1]);
        }
        return dp[len];
    }
    public static void main(String[] args) {
        int[] cost={10,15,20};
        System.out.println(minCostClimbingStairs(cost));

    }
}
```

动态规划问题，理解题意比写代码还难。。](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=Leetcode&t=blog)





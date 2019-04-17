# [LeetCode] Min Cost Climbing Stairs 爬楼梯的最小损失 - Grandyang - 博客园







# [[LeetCode] Min Cost Climbing Stairs 爬楼梯的最小损失](https://www.cnblogs.com/grandyang/p/8343874.html)








On a staircase, the `i`-th step has some non-negative cost `cost[i]` assigned (0 indexed).

Once you pay the cost, you can either climb one or two steps. You need to find minimum cost to reach the top of the floor, and you can either start from the step with index 0, or the step with index 1.

Example 1:
Input: cost = [10, 15, 20]
Output: 15
Explanation: Cheapest is start on cost[1], pay that cost and go to the top.



Example 2:
Input: cost = [1, 100, 1, 1, 1, 100, 1, 1, 100, 1]
Output: 6
Explanation: Cheapest is start on cost[0], and only step on 1s, skipping cost[3].



Note:
- `cost` will have a length in the range `[2, 1000]`.
- Every `cost[i]` will be an integer in the range `[0, 999]`.




这道题应该算是之前那道 [Climbing Stairs](http://www.cnblogs.com/grandyang/p/4079165.html) 的拓展，这里不是求步数，而是每个台阶上都有一个cost，让我们求爬到顶端的最小cost是多少。换汤不换药，还是用动态规划Dynamic Programming来做。这里我们定义一个一维的dp数组，其中dp[i]表示爬到第i层的最小cost，然后我们来想dp[i]如何推导。我们来思考一下如何才能到第i层呢？是不是只有两种可能性，一个是从第i-2层上直接跳上来，一个是从第i-1层上跳上来。不会再有别的方法，所以我们的dp[i]只和前两层有关系，所以可以写做如下：

dp[i] = min(dp[i- 2] + cost[i - 2], dp[i - 1] + cost[i - 1])

最后我们返回最后一个数字dp[n]即可，参见代码如下：



解法一：

```
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();
        vector<int> dp(n + 1, 0);
        for (int i = 2; i < n + 1; ++i) {
            dp[i] = min(dp[i- 2] + cost[i - 2], dp[i - 1] + cost[i - 1]);
        }
        return dp.back();
    }
};
```



再来看一种DP的解法，跟上面的解法很相近，不同在于dp数组长度为n，其中dp[i]表示到第i+1层的最小cost，分别初始化dp[0]和dp[1]为cost[0]和cost[1]。然后从i=2处开始遍历，此时我们的更新思路是，要爬当前的台阶，肯定需要加上当前的cost[i]，那么我们还是要从前一层或者前两层的台阶上跳上来，那么我们选择dp值小的那个，所以递归式如下：

dp[i] = cost[i] + min(dp[i- 1], dp[i - 2])

最后我们在最后两个dp值中选择一个较小的返回即可，参见代码如下：



解法二：

```
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();
        vector<int> dp(n, 0);
        dp[0] = cost[0]; dp[1] = cost[1];
        for (int i = 2; i < n; ++i) {
            dp[i] = cost[i] + min(dp[i- 1], dp[i - 2]);
        }
        return min(dp[n - 1], dp[n - 2]);
    }
};
```



我们可以对空间复杂度进行优化，通过前面的分析我们可以发现，当前的dp值仅仅依赖前面两个的值，所以我们不必把整个dp数组都记录下来，只需用两个变量a和b来记录前两个值，然后不停的用新得到的值来覆盖它们就好了。我们初始化a和b均为0，然后遍历cost数组，首先将a和b中较小值加上num放入临时变量t中，然后把b赋给a，把t赋给b即可，参见代码如下：



解法三：

```
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int a = 0, b = 0;
        for (int num : cost) {
            int t = min(a, b) + num;
            a = b;
            b = t;
        }
        return min(a, b);
    }
};
```



我们还可以用递归来写，需要优化计算量，即用HashMap来保存已经算过了台阶，用的还是dp的思想，参见代码如下：



解法四：

```
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        unordered_map<int, int> memo;
        return helper(cost, cost.size(), memo);
    }
    int helper(vector<int>& cost, int i, unordered_map<int, int>& memo) {
        if (memo.count(i)) return memo[i];
        if (i <= 1) return memo[i] = cost[i];
        return memo[i] = (i == cost.size() ? 0 : cost[i]) + min(helper(cost, i - 1, memo), helper(cost, i - 2, memo));
    }
};
```



类似题目：

[Climbing Stairs](http://www.cnblogs.com/grandyang/p/4079165.html)



参考资料：

[https://discuss.leetcode.com/topic/114149/c-o-1-space](https://discuss.leetcode.com/topic/114149/c-o-1-space)

[https://discuss.leetcode.com/topic/114144/easy-to-understand-c-using-dp-with-detailed-explanation](https://discuss.leetcode.com/topic/114144/easy-to-understand-c-using-dp-with-detailed-explanation)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














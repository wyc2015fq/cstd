# [LeetCode] Coin Change 2 硬币找零之二 - Grandyang - 博客园







# [[LeetCode] Coin Change 2 硬币找零之二](https://www.cnblogs.com/grandyang/p/7669088.html)








You are given coins of different denominations and a total amount of money. Write a function to compute the number of combinations that make up that amount. You may assume that you have infinite number of each kind of coin.

Note: You can assume that
- 0 <= amount <= 5000
- 1 <= coin <= 5000
- the number of coins is less than 500
- the answer is guaranteed to fit into signed 32-bit integer



Example 1:
Input: amount = 5, coins = [1, 2, 5]
Output: 4
Explanation: there are four ways to make up the amount:
5=5
5=2+2+1
5=2+1+1+1
5=1+1+1+1+1



Example 2:
Input: amount = 3, coins = [2]
Output: 0
Explanation: the amount of 3 cannot be made up just with coins of 2.



Example 3:
Input: amount = 10, coins = [10] 
Output: 1



这道题是之前那道[Coin Change](http://www.cnblogs.com/grandyang/p/5138186.html)的拓展，那道题问我们最少能用多少个硬币组成给定的钱数，而这道题问的是组成给定钱数总共有多少种不同的方法。那么我们还是要使用DP来做，首先我们来考虑最简单的情况，如果只有一个硬币的话，那么给定钱数的组成方式就最多有1种，就看此钱数能否整除该硬币值。那么当有两个硬币的话，那么组成某个钱数的方式就可能有多种，比如可能由每种硬币单独来组成，或者是两种硬币同时来组成。那么我们怎么量化呢，比如我们有两个硬币[1,2]，钱数为5，那么钱数的5的组成方法是可以看作两部分组成，一种是由硬币1单独组成，那么仅有一种情况(1+1+1+1+1)；另一种是由1和2共同组成，说明我们的组成方法中至少需要由一个2，所以此时我们先取出一个硬币2，那么我们只要拼出钱数为3即可，这个3还是可以用硬币1和2来拼，所以就相当于求由硬币[1,2]组成的钱数为3的总方法。是不是不太好理解，多想想。那么我们的需要一个二维的dp数组，其中dp[i][j]表示用前i个硬币组成钱数为j的不同组合方法，那么怎么算才不会重复，也不会漏掉呢？我们采用的方法是一个硬币一个硬币的增加，每增加一个硬币，都从1遍历到amount，对于遍历到的当前钱数j，组成方法就是不加上当前硬币的频发dp[i-1][j]，还要加上，去掉当前硬币值的钱数的组成方法，当然钱数j要大于当前硬币值，那么我们的递推公式也在上面的分析中得到了：

dp[i][j] = dp[i - 1][j] + (j >= coins[i - 1] ? dp[i][j - coins[i - 1]] : 0)

注意我们要初始化每行的第一个位置为0，参见代码如下：      



解法一：

```
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        vector<vector<int>> dp(coins.size() + 1, vector<int>(amount + 1, 0));
        dp[0][0] = 1;
        for (int i = 1; i <= coins.size(); ++i) {
            dp[i][0] = 1;
            for (int j = 1; j <= amount; ++j) {
                dp[i][j] = dp[i - 1][j] + (j >= coins[i - 1] ? dp[i][j - coins[i - 1]] : 0);
            }
        }
        return dp[coins.size()][amount];
    }
};
```



我们可以对空间进行优化，我们发现dp[i][j]仅仅依赖于dp[i - 1][j] 和 dp[i][j - coins[i - 1]] 这两项，那么我们可以使用一个一维dp数组来代替，此时的dp[i]表示组成钱数i的不同方法。其实最开始的时候，博主就想着用一维的dp数组来写，但是博主开始想的方法是把里面两个for循环调换了一个位置，结果计算的种类数要大于正确答案，所以一定要注意for循环的顺序不能搞反，参见代码如下：



解法二：

```
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        vector<int> dp(amount + 1, 0);
        dp[0] = 1;
        for (int coin : coins) {
            for (int i = coin; i <= amount; ++i) {
                dp[i] += dp[i - coin];
            }
        }
        return dp[amount];
    }
};
```



在CareerCup中，有一道极其相似的题[9.8 Represent N Cents 美分的组成](http://www.cnblogs.com/grandyang/p/4840713.html)，书里面用的是那种递归的方法，博主想将其解法直接搬到这道题里，但是失败了，博主发现使用那种的递归的解法必须要有值为1的硬币存在，这点无法在这道题里满足。你以为这样博主就没有办法了吗？当然有，博主加了判断，当用到最后一个硬币时，我们判断当前还剩点钱数是否能整除这个硬币，不能的话就返回0，否则返回1。还有就是用二维数组的memo会TLE，所以博主换成了map，就可以通过啦～



解法三：

```
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        if (amount == 0) return 1;
        if (coins.empty()) return 0;
        map<pair<int, int>, int> memo;
        return helper(amount, coins, 0, memo);
    }
    int helper(int amount, vector<int>& coins, int idx, map<pair<int, int>, int>& memo) {
        if (amount == 0) return 1;
        else if (idx >= coins.size()) return 0;
        else if (idx == coins.size() - 1) return amount % coins[idx] == 0;
        if (memo.count({amount, idx})) return memo[{amount, idx}];
        int val = coins[idx], res = 0;
        for (int i = 0; i * val <= amount; ++i) {
            int rem = amount - i * val;
            res += helper(rem, coins, idx + 1, memo);
        }
        return memo[{amount, idx}] = res;
    }
};
```



类似题目：

[Coin Change](http://www.cnblogs.com/grandyang/p/5138186.html)

[9.8 Represent N Cents 美分的组成](http://www.cnblogs.com/grandyang/p/4840713.html)



参考资料：

[https://discuss.leetcode.com/topic/89238/golang-recursive-solution](https://discuss.leetcode.com/topic/89238/golang-recursive-solution)

[https://discuss.leetcode.com/topic/79071/knapsack-problem-java-solution-with-thinking-process-o-nm-time-and-o-m-space](https://discuss.leetcode.com/topic/79071/knapsack-problem-java-solution-with-thinking-process-o-nm-time-and-o-m-space)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













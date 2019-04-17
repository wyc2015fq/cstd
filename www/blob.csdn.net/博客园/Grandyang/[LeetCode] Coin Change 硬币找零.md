# [LeetCode] Coin Change 硬币找零 - Grandyang - 博客园







# [[LeetCode] Coin Change 硬币找零](https://www.cnblogs.com/grandyang/p/5138186.html)







You are given coins of different denominations and a total amount of money *amount*. Write a function to compute the fewest number of coins that you need to make up that amount. If that amount of money cannot be made up by any combination of the coins, return `-1`.

Example 1:
coins = `[1, 2, 5]`, amount = `11`
return `3` (11 = 5 + 5 + 1)

Example 2:
coins = `[2]`, amount = `3`
return `-1`.

Note:
You may assume that you have an infinite number of each kind of coin.

Credits:
Special thanks to [@jianchao.li.fighter](https://leetcode.com/discuss/user/jianchao.li.fighter) for adding this problem and creating all test cases.



这道题给我们了一些可用的硬币值，又给了一个钱数，问我们最小能用几个硬币来找零。根据题目中的例子可知，不是每次都会给全1,2,5的硬币，有时候没有1分硬币，那么有的钱数就没法找零，需要返回-1。这道题跟CareerCup上的那道 [9.8 Represent N Cents 美分的组成](http://www.cnblogs.com/grandyang/p/4840713.html) 有些类似，那道题给全了所有的美分，25,10,5,1，然后给我们一个钱数，问我们所有能够找零的方法，而这道题只让我们求出最小的那种。没啥特别好的思路就首先来考虑brute force吧，暴力搜索如果也没思路肿么办-.-|||。那我们怎么办，还是来看例子1，如果不考虑代码实现，你怎么手动找出答案。博主会先取出一个最大的数字5，比目标值11要小，由于这里的硬币是可以重复使用的，所以博主会再取个5出来，现在是10，还是比11要小，这是再取5会超，那就往前取，取2，也会超出，于是就取1，刚好是11。那么我们的暴力搜索法也是这种思路，首先要给数组排个序，因为我们想要从最大的开始取，我们的递归函数需要一个变量start，初识化为数组的最后一个位置，当前目标值target，还有当前使用的硬币个数cur，以及最终结果res。在递归函数，我们首先判断如果target小于0了，直接返回。若target为0了，说明我们当前使用的硬币已经组成了目标值，用cur来更新结果res。否则就从start开始往前遍历硬币，对每个硬币都调用递归函数，此时target应该减去当前的硬币值，cur应该自增1，代码参见评论区七楼。但是暴力搜索Brute Force的方法会超时TLE，所以我们考虑一下其他的方法吧。

如果大家刷题有一阵子了的，那么应该会知道，对于求极值问题，我们还是主要考虑动态规划Dynamic Programming来做，好处是保留了一些中间状态的计算值，这样可以避免大量的重复计算。我们维护一个一维动态数组dp，其中dp[i]表示钱数为i时的最小硬币数的找零，注意由于数组是从0开始的，所以我们要多申请一位，数组大小为amount+1，这样最终结果就可以保存在dp[amount]中了。初始化dp[0] = 0，因为目标值若为0时，就不需要硬币了。其他值可以初始化是amount+1，为啥呢，因为最小的硬币是1，所以amount最多需要amount个硬币，amount+1也就相当于当前的最大值了，注意这里不能用整型最大值来初始化，因为在后面的状态转移方程有加1的操作，有可能会溢出，除非你先减个1，这样还不如直接用amount+1舒服呢。好，接下来就是要找状态转移方程了，没思路？不要紧！回归例子1，假设我取了一个值为5的硬币，那么由于目标值是11，所以是不是假如我们知道dp[6]，那么就知道了组成11的dp值了？所以我们更新dp[i]的方法就是遍历每个硬币，如果遍历到的硬币值小于i值（比如我们不能用值为5的硬币去更新dp[3]）时，我们用 dp[i - coins[j]] + 1 来更新dp[i]，所以状态转移方程为：
dp[i] = min(dp[i], dp[i - coins[j]] + 1);
其中coins[j]为第j个硬币，而i - coins[j]为钱数i减去其中一个硬币的值，剩余的钱数在dp数组中找到值，然后加1和当前dp数组中的值做比较，取较小的那个更新dp数组。先来看迭代的写法如下所示：



解法一：

```
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0;
        for (int i = 1; i <= amount; ++i) {
            for (int j = 0; j < coins.size(); ++j) {
                if (coins[j] <= i) {
                    dp[i] = min(dp[i], dp[i - coins[j]] + 1);
                }
            }
        }
        return (dp[amount] > amount) ? -1 : dp[amount];
    }
};
```



迭代的DP解法有一个好基友，就是递归+memo数组的解法，说其是递归形式的DP解法也没错，但博主比较喜欢说成是递归加记忆数组。其目的都是为了保存中间计算结果，避免大量的重复计算，从而提高运算效率，思路都一样，仅仅是写法有些区别：



解法二：

```
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> memo(amount + 1, INT_MAX);
        memo[0] = 0;
        return coinChangeDFS(coins, amount, memo);
    }
    int coinChangeDFS(vector<int>& coins, int target, vector<int>& memo) {
        if (target < 0) return - 1;
        if (memo[target] != INT_MAX) return memo[target];
        for (int i = 0; i < coins.size(); ++i) {
            int tmp = coinChangeDFS(coins, target - coins[i], memo);
            if (tmp >= 0) memo[target] = min(memo[target], tmp + 1);
        }
        return memo[target] = (memo[target] == INT_MAX) ? -1 : memo[target];
    }
};
```



再来看一种使用HashMap来当记忆数组的递归解法：



解法三：

```
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        unordered_map<int, int> memo;
        memo[0] = 0;
        return coinChangeDFS(coins, amount, memo);
    }
    int coinChangeDFS(vector<int>& coins, int target, unordered_map<int, int>& memo) {
        if (target < 0) return - 1;
        if (memo.count(target)) return memo[target];
        int cur = INT_MAX;
        for (int i = 0; i < coins.size(); ++i) {
            int tmp = coinChangeDFS(coins, target - coins[i], memo);
            if (tmp >= 0) cur = min(cur, tmp + 1);
        }
        return memo[target] = (cur == INT_MAX) ? -1 : cur;
    }
};
```



难道这题一定要DP来做吗，我们来看网友hello_world00提供的一种解法，这其实是对暴力搜索的解法做了很好的优化，不仅不会TLE，而且击败率相当的高！对比Brute Force的方法，这里在递归函数中做了很好的优化。首先是判断start是否小于0，因为我们需要从coin中取硬币，不能越界。下面就是优化的核心了，看target是否能整除coins[start]，这是相当叼的一步，比如假如我们的目标值是15，如果我们当前取出了大小为5的硬币，我们做除法，可以立马知道只用大小为5的硬币就可以组成目标值target，那么我们用cur + target/coins[start] 来更新结果res。之后的for循环也相当叼，不像暴力搜索中的那样从start位置开始往前遍历coins中的硬币，而是遍历 target/coins[start] 的次数，由于不能整除，我们只需要对余数调用递归函数，而且我们要把次数每次减1，并且再次求余数。举个例子，比如coins=[1,2,3]，amount=11，那么 11除以3，得3余2，那么我们的i从3开始遍历，这里有一步非常有用的剪枝操作，没有这一步，还是会TLE，而加上了这一步，直接击败百分之九十九以上，可以说是天壤之别。那就是判断若 cur + i >= res - 1 成立，直接break，不调用递归。这里解释一下，cur + i 自不必说，是当前硬币个数cur 加上新加的i个硬币，我们都是知道cur+i如果大于等于res的话，那么res是不会被更新的，那么为啥这里是大于等于res-1呢？因为能运行到这一步，说明之前是无法整除的，那么余数一定存在，所以再次调用递归函数的target不为0，那么如果整除的话，cur至少会加上1，所以又跟res相等了，还是不会使得res变得更小。解释到这里应该比较明白了吧，有疑问的请在下方留言哈，参见代码如下：



解法四：

```
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        int res = INT_MAX, n = coins.size();
        sort(coins.begin(), coins.end());
        helper(coins, n - 1, amount, 0, res);
        return (res == INT_MAX) ? -1 : res;
    }
    void helper(vector<int>& coins, int start, int target, int cur, int& res) {
        if (start < 0) return;
        if (target % coins[start] == 0) {
            res = min(res, cur + target / coins[start]);
            return;
        }
        for (int i = target / coins[start]; i >= 0; --i) {
            if (cur + i >= res - 1) break;
            helper(coins, start - 1, target - i * coins[start], cur + i, res);
        }
    }
};
```



类似题目：

[Coin Change 2](http://www.cnblogs.com/grandyang/p/7669088.html)

[9.8 Represent N Cents 美分的组成](http://www.cnblogs.com/grandyang/p/4840713.html)



参考资料：

[https://leetcode.com/problems/coin-change/](https://leetcode.com/problems/coin-change/)

[https://leetcode.com/problems/coin-change/discuss/77360/C%2B%2B-O(n*amount)-time-O(amount)-space-DP-solution](https://leetcode.com/problems/coin-change/discuss/77360/C%2B%2B-O(n*amount)-time-O(amount)-space-DP-solution)

[https://leetcode.com/problems/coin-change/discuss/77368/*Java*-Both-iterative-and-recursive-solutions-with-explanations](https://leetcode.com/problems/coin-change/discuss/77368/*Java*-Both-iterative-and-recursive-solutions-with-explanations)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













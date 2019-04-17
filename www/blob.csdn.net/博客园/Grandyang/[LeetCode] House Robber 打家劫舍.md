# [LeetCode] House Robber 打家劫舍 - Grandyang - 博客园







# [[LeetCode] House Robber 打家劫舍](https://www.cnblogs.com/grandyang/p/4383632.html)







You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed, the only constraint stopping you from robbing each of them is that adjacent houses have security system connected and it will automatically contact the police if two adjacent houses were broken into on the same night.

Given a list of non-negative integers representing the amount of money of each house, determine the maximum amount of money you can rob tonight without alerting the police.

Example 1:
Input: [1,2,3,1]
Output: 4
Explanation: Rob house 1 (money = 1) and then rob house 3 (money = 3).
             Total amount you can rob = 1 + 3 = 4.
Example 2:
Input: [2,7,9,3,1]
Output: 12
Explanation: Rob house 1 (money = 2), rob house 3 (money = 9) and rob house 5 (money = 1).
             Total amount you can rob = 2 + 9 + 1 = 12.
Credits:
Special thanks to [@ifanchu](https://oj.leetcode.com/discuss/user/ifanchu) for adding this problem and creating all test cases. Also thanks to [@ts](https://oj.leetcode.com/discuss/user/ts) for adding additional test cases.



这道题的本质相当于在一列数组中取出一个或多个不相邻数，使其和最大。那么我们对于这类求极值的问题首先考虑动态规划Dynamic Programming来解，我们维护一个一位数组dp，其中dp[i]表示到i位置时不相邻数能形成的最大和，那么状态转移方程怎么写呢，我们先拿一个简单的例子来分析一下，比如说nums为{3, 2, 1, 5}，那么我们来看我们的dp数组应该是什么样的，首先dp[0]=3没啥疑问，再看dp[1]是多少呢，由于3比2大，所以我们抢第一个房子的3，当前房子的2不抢，所以dp[1]=3，那么再来看dp[2]，由于不能抢相邻的，所以我们可以用再前面的一个的dp值加上当前的房间值，和当前房间的前面一个dp值比较，取较大值当做当前dp值，所以我们可以得到状态转移方程dp[i] = max(num[i] + dp[i - 2], dp[i - 1]), 由此看出我们需要初始化dp[0]和dp[1]，其中dp[0]即为num[0]，dp[1]此时应该为max(num[0], num[1])，代码如下：



解法一：

```
// DP
class Solution {
public:
    int rob(vector<int> &num) {
        if (num.size() <= 1) return num.empty() ? 0 : num[0];
        vector<int> dp = {num[0], max(num[0], num[1])};
        for (int i = 2; i < num.size(); ++i) {
            dp.push_back(max(num[i] + dp[i - 2], dp[i - 1]));
        }
        return dp.back();
    }
};
```



还有一种解法，核心思想还是用DP，分别维护两个变量robEven和robOdd，顾名思义，robEven就是要抢偶数位置的房子，robOdd就是要抢奇数位置的房子。所以我们在遍历房子数组时，如果是偶数位置，那么robEven就要加上当前数字，然后和robOdd比较，取较大的来更新robEven。这里我们就看出来了，robEven组成的值并不是只由偶数位置的数字，只是当前要抢偶数位置而已。同理，当奇数位置时，robOdd加上当前数字和robEven比较，取较大值来更新robOdd，这种按奇偶分别来更新的方法，可以保证组成最大和的数字不相邻，最后别忘了在robEven和robOdd种取较大值返回，代码如下：



解法二：

```
class Solution {
public:
    int rob(vector<int> &nums) {
        int robEven = 0, robOdd = 0, n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (i % 2 == 0) {
                robEven = max(robEven + nums[i], robOdd);
            } else {
                robOdd = max(robEven, robOdd + nums[i]);
            }
        }
        return max(robEven, robOdd);
    }
};
```



上述方法还可以进一步简洁，我们使用两个变量rob和notRob，其中rob表示抢当前的房子，notRob表示不抢当前的房子，那么在遍历的过程中，我们先用两个变量preRob和preNotRob来分别记录更新之前的值，由于rob是要抢当前的房子，由于抢了当前的房子，那么前一个房子一定不能抢，所以使用preNotRob加上当前的数字赋给rob，然后notRob表示不能抢当前的房子，那么之前的房子就可以抢也可以不抢，所以我们将preRob和preNotRob中的较大值赋给notRob，参见代码如下：



解法三：

```
class Solution {
public:
    int rob(vector<int> &nums) {
        int rob = 0, notRob = 0, n = nums.size();
        for (int i = 0; i < n; ++i) {
            int preRob = rob, preNotRob = notRob;
            rob = preNotRob + nums[i];
            notRob = max(preRob, preNotRob);
        }
        return max(rob, notRob);
    }
};
```



类似题目：

[House Robber II](http://www.cnblogs.com/grandyang/p/4518674.html)

[House Robber III](http://www.cnblogs.com/grandyang/p/5275096.html)

[Maximum Product Subarray](http://www.cnblogs.com/grandyang/p/4028713.html)

[Paint House](http://www.cnblogs.com/grandyang/p/5319384.html)

[Paint Fence](http://www.cnblogs.com/grandyang/p/5231220.html)

[Coin Path](http://www.cnblogs.com/grandyang/p/8183477.html)

[Non-negative Integers without Consecutive Ones](http://www.cnblogs.com/grandyang/p/6959585.html)

[Delete and Earn](http://www.cnblogs.com/grandyang/p/8176933.html)



参考资料：

[https://leetcode.com/problems/house-robber/description/](https://leetcode.com/problems/house-robber/description/)

[https://leetcode.com/problems/house-robber/discuss/55681/java-on-solution-space-o1](https://leetcode.com/problems/house-robber/discuss/55681/java-on-solution-space-o1)

[https://leetcode.com/problems/house-robber/discuss/55693/c-1ms-o1space-very-simple-solution](https://leetcode.com/problems/house-robber/discuss/55693/c-1ms-o1space-very-simple-solution)

[https://leetcode.com/problems/house-robber/discuss/55695/java-dp-solution-on-runtime-and-o1-space-with-inline-comment](https://leetcode.com/problems/house-robber/discuss/55695/java-dp-solution-on-runtime-and-o1-space-with-inline-comment)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













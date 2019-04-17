# [LeetCode] House Robber II 打家劫舍之二 - Grandyang - 博客园







# [[LeetCode] House Robber II 打家劫舍之二](https://www.cnblogs.com/grandyang/p/4518674.html)







You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed. All houses at this place are arranged in a circle. That means the first house is the neighbor of the last one. Meanwhile, adjacent houses have security system connected and it will automatically contact the police if two adjacent houses were broken into on the same night.

Given a list of non-negative integers representing the amount of money of each house, determine the maximum amount of money you can rob tonight without alerting the police.

Example 1:
Input: [2,3,2]
Output: 3
Explanation: You cannot rob house 1 (money = 2) and then rob house 3 (money = 2),
             because they are adjacent houses.

Example 2:
Input: [1,2,3,1]
Output: 4
Explanation: Rob house 1 (money = 1) and then rob house 3 (money = 3).
             Total amount you can rob = 1 + 3 = 4.
**Credits:**
Special thanks to [@Freezen](https://oj.leetcode.com/discuss/user/Freezen) for adding this problem and creating all test cases.



这道题是之前那道 [House Robber](http://www.cnblogs.com/grandyang/p/4383632.html) 的拓展，现在房子排成了一个圆圈，则如果抢了第一家，就不能抢最后一家，因为首尾相连了，所以第一家和最后一家只能抢其中的一家，或者都不抢，那我们这里变通一下，如果我们把第一家和最后一家分别去掉，各算一遍能抢的最大值，然后比较两个值取其中较大的一个即为所求。那我们只需参考之前的 [House Robber](http://www.cnblogs.com/grandyang/p/4383632.html) 中的解题方法，然后调用两边取较大值，代码如下：



解法一：

```
// DP
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.size() <= 1) return nums.empty() ? 0 : nums[0];
        return max(rob(nums, 0, nums.size() - 1), rob(nums, 1, nums.size()));
    }
    int rob(vector<int> &nums, int left, int right) {
        if (right - left <= 1) return nums[left];
        vector<int> dp(right, 0);
        dp[left] = nums[left];
        dp[left + 1] = max(nums[left], nums[left + 1]);
        for (int i = left + 2; i < right; ++i) {
            dp[i] = max(nums[i] + dp[i - 2], dp[i - 1]);
        }
        return dp.back();
    }
};
```



当然，我们也可以使用两个变量来代替整个DP数组，讲解请参见之前的帖子 [House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)，参见代码如下：



解法二：

```
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.size() <= 1) return nums.empty() ? 0 : nums[0];
        return max(rob(nums, 0, nums.size() - 1), rob(nums, 1, nums.size()));
    }
    int rob(vector<int> &nums, int left, int right) {
        int robEven = 0, robOdd = 0;
        for (int i = left; i < right; ++i) {
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



另一种更为简洁的写法，讲解请参见之前的帖子 [House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)，参见代码如下：



解法三：

```
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.size() <= 1) return nums.empty() ? 0 : nums[0];
        return max(rob(nums, 0, nums.size() - 1), rob(nums, 1, nums.size()));
    }
    int rob(vector<int> &nums, int left, int right) {
        int rob = 0, notRob = 0;
        for (int i = left; i < right; ++i) {
            int preRob = rob, preNotRob = notRob;
            rob = preNotRob + nums[i];
            notRob = max(preRob, preNotRob);
        }
        return max(rob, notRob);
    }
};
```



类似题目：

[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)

[House Robber III](http://www.cnblogs.com/grandyang/p/5275096.html)

[Paint House](http://www.cnblogs.com/grandyang/p/5319384.html)

[Paint Fence](http://www.cnblogs.com/grandyang/p/5231220.html)

[Non-negative Integers without Consecutive Ones](http://www.cnblogs.com/grandyang/p/6959585.html)

[Coin Path](http://www.cnblogs.com/grandyang/p/8183477.html)



参考资料：

[https://leetcode.com/problems/house-robber-ii/](https://leetcode.com/problems/house-robber-ii/)

[https://leetcode.com/problems/house-robber-ii/discuss/59929/Java-clean-short-solution-DP](https://leetcode.com/problems/house-robber-ii/discuss/59929/Java-clean-short-solution-DP)

[https://leetcode.com/problems/house-robber-ii/discuss/59934/Simple-AC-solution-in-Java-in-O(n)-with-explanation](https://leetcode.com/problems/house-robber-ii/discuss/59934/Simple-AC-solution-in-Java-in-O(n)-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













# [LeetCode] Increasing Triplet Subsequence 递增的三元子序列 - Grandyang - 博客园







# [[LeetCode] Increasing Triplet Subsequence 递增的三元子序列](https://www.cnblogs.com/grandyang/p/5194599.html)







Given an unsorted array return whether an increasing subsequence of length 3 exists or not in the array.

Formally the function should:

> Return true if there exists *i, j, k *
such that *arr[i]* < *arr[j]* < *arr[k]* given 0 ≤ *i* < *j* < *k* ≤ *n*-1 else return false.



Your algorithm should run in O(*n*) time complexity and O(*1*) space complexity.

Examples:
Given `[1, 2, 3, 4, 5]`,
return `true`.

Given `[5, 4, 3, 2, 1]`,
return `false`.

Credits:
Special thanks to [@DjangoUnchained](https://leetcode.com/discuss/user/DjangoUnchained) for adding this problem and creating all test cases.



这道题让我们求一个无序数组中是否有任意三个数字是递增关系的，我最先相处的方法是用一个dp数组，dp[i]表示在i位置之前小于等于nums[i]的数字的个数(包括其本身)，我们初始化dp数组都为1，然后我们开始遍历原数组，对当前数字nums[i]，我们遍历其之前的所有数字，如果之前某个数字nums[j]小于nums[i]，那么我们更新dp[i] = max(dp[i], dp[j] + 1)，如果此时dp[i]到3了，则返回true，若遍历完成，则返回false，参见代码如下：



解法一：

```
// Dumped, brute force
class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        vector<int> dp(nums.size(), 1);
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                    if (dp[i] >= 3) return true;
                }
            }
        }
        return false;
    }
};
```



但是题目中要求我们O(n)的时间复杂度和O(1)的空间复杂度，上面的那种方法一条都没满足，所以白写了。我们下面来看满足题意的方法，这个思路是使用两个指针m1和m2，初始化为整型最大值，我们遍历数组，如果m1大于等于当前数字，则将当前数字赋给m1；如果m1小于当前数字且m2大于等于当前数字，那么将当前数字赋给m2，一旦m2被更新了，说明一定会有一个数小于m2，那么我们就成功的组成了一个长度为2的递增子序列，所以我们一旦遍历到比m2还大的数，我们直接返回ture。如果我们遇到比m1小的数，还是要更新m1，有可能的话也要更新m2为更小的值，毕竟m2的值越小，能组成长度为3的递增序列的可能性越大，参见代码如下：



解法二：

```
class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        int m1 = INT_MAX, m2 = INT_MAX;
        for (auto a : nums) {
            if (m1 >= a) m1 = a;
            else if (m2 >= a) m2 = a;
            else return true;
        }
        return false;
    }
};
```



如果觉得上面的解法不容易想出来，那么如果能想出下面这种解法，估计面试官也会为你点赞。这种方法的虽然不满足常数空间的要求，但是作为对暴力搜索的优化，也是一种非常好的解题思路。这个解法的思路是建立两个数组，forward数组和backward数组，其中forward[i]表示[0, i]之间最小的数，backward[i]表示[i, n-1]之间最大的数，那么对于任意一个位置i，如果满足 forward[i] < nums[i] < backward[i]，则表示这个递增三元子序列存在，举个例子来看吧，比如：

nums:        8  3  5  1  6

foward:      8  3  3  1  1

backward:  8  6  6  6  6

我们发现数字5满足forward[i] < nums[i] < backward[i]，所以三元子序列存在。



解法三：

```
class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        if (nums.size() < 3) return false;
        int n = nums.size();
        vector<int> f(n, nums[0]), b(n, nums.back());
        for (int i = 1; i < n; ++i) {
            f[i] = min(f[i - 1], nums[i]);
        }
        for (int i = n - 2; i >= 0; --i) {
            b[i] = max(b[i + 1], nums[i]);
        }
        for (int i = 0; i < n; ++i) {
            if (nums[i] > f[i] && nums[i] < b[i]) return true;
        }
        return false;
    }
};
```



参考资料：

[https://leetcode.com/discuss/86593/clean-and-short-with-comments-c](https://leetcode.com/discuss/86593/clean-and-short-with-comments-c)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













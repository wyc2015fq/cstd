# [LeetCode] Maximum Average Subarray I 子数组的最大平均值 - Grandyang - 博客园







# [[LeetCode] Maximum Average Subarray I 子数组的最大平均值](https://www.cnblogs.com/grandyang/p/7294585.html)








Given an array consisting of `n` integers, find the contiguous subarray of given length `k` that has the maximum average value. And you need to output the maximum average value.

Example 1:
Input: [1,12,-5,-6,50,3], k = 4
Output: 12.75
Explanation: Maximum average is (12-5-6+50)/4 = 51/4 = 12.75



Note:
- 1 <= `k` <= `n` <= 30,000.
- Elements of the given array will be in the range [-10,000, 10,000].



这道题给了我们一个数组nums，还有一个数字k，让我们找长度为k且平均值最大的子数组。由于子数组必须是连续的，所以我们不能给数组排序。那么怎么办呢，在博主印象中，计算子数组之和的常用方法应该是建立累加数组，然后我们可以快速计算出任意一个长度为k的子数组，用来更新结果res，从而得到最大的那个，参见代码如下：



解法一：

```
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> sums = nums;
        for (int i = 1; i < n; ++i) {
            sums[i] = sums[i - 1] + nums[i];
        }
        double mx = sums[k - 1];
        for (int i = k; i < n; ++i) {
            mx = max(mx, (double)sums[i] - sums[i - k]);
        }
        return mx / k;
    }
};
```



由于这道题子数组的长度k是确定的，所以我们其实没有必要建立整个累加数组，而是先算出前k个数字的和，然后就像维护一个滑动窗口一样，将窗口向右移动一位，即加上一个右边的数字，减去一个左边的数字，就等同于加上右边数字减去左边数字的差值，然后每次更新结果res即可，参见代码如下：



解法二：

```
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        double sum = accumulate(nums.begin(), nums.begin() + k, 0), res = sum;
        for (int i = k; i < nums.size(); ++i) {
            sum += nums[i] - nums[i - k];
            res = max(res, sum);
        }
        return res / k;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/96134/c-simple-sliding-window-solution](https://discuss.leetcode.com/topic/96134/c-simple-sliding-window-solution)

[https://discuss.leetcode.com/topic/96154/java-solution-sum-of-sliding-window](https://discuss.leetcode.com/topic/96154/java-solution-sum-of-sliding-window)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














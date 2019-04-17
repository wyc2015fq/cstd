# [LeetCode] Number of Longest Increasing Subsequence 最长递增序列的个数 - Grandyang - 博客园







# [[LeetCode] Number of Longest Increasing Subsequence 最长递增序列的个数](https://www.cnblogs.com/grandyang/p/7603903.html)








Given an unsorted array of integers, find the number of longest increasing subsequence.

Example 1:
Input: [1,3,5,4,7]
Output: 2
Explanation: The two longest increasing subsequence are [1, 3, 4, 7] and [1, 3, 5, 7].



Example 2:
Input: [2,2,2,2,2]
Output: 5
Explanation: The length of longest continuous increasing subsequence is 1, and there are 5 subsequences' length is 1, so output 5.



Note: Length of the given array will be not exceed 2000 and the answer is guaranteed to be fit in 32-bit signed int.



这道题给了我们一个数组，让我们求最长递增序列的个数，题目中的两个例子也很好的说明了问题。那么对于这种求个数的问题，直觉告诉我们应该要使用DP来做。其实这道题在设计DP数组的时候有个坑，如果我们将dp[i]定义为到i位置的最长子序列的个数的话，那么递推公式不好找。但是如果我们将dp[i]定义为以nums[i]为结尾的递推序列的个数的话，再配上这些递推序列的长度，将会比较容易的发现递推关系。这里我们用len[i]表示以nums[i]为结尾的递推序列的长度，用cnt[i]表示以nums[i]为结尾的递推序列的个数，初始化都赋值为1，只要有数字，那么至少都是1。然后我们遍历数组，对于每个遍历到的数字nums[i]，我们再遍历其之前的所有数字nums[j]，当nums[i]小于等于nums[j]时，不做任何处理，因为不是递增序列。反之，则判断len[i]和len[j]的关系，如果len[i]等于len[j] + 1，说明nums[i]这个数字可以加在以nums[j]结尾的递增序列后面，并且以nums[j]结尾的递增序列个数可以直接加到以nums[i]结尾的递增序列个数上。如果len[i]小于len[j] + 1，说明我们找到了一条长度更长的递增序列，那么我们此时奖len[i]更新为len[j]+1，并且原本的递增序列都不能用了，直接用cnt[j]来代替。我们在更新完len[i]和cnt[i]之后，要更新mx和res，如果mx等于len[i]，则把cnt[i]加到res之上；如果mx小于len[i]，则更新mx为len[i]，更新结果res为cnt[i]，参见代码如下：



解法一：

```
class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int res = 0, mx = 0, n = nums.size();
        vector<int> len(n, 1), cnt(n, 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[i] <= nums[j]) continue;
                if (len[i] == len[j] + 1) cnt[i] += cnt[j];
                else if (len[i] < len[j] + 1) {
                    len[i] = len[j] + 1;
                    cnt[i] = cnt[j];
                }
            }
            if (mx == len[i]) res += cnt[i];
            else if (mx < len[i]) {
                mx = len[i];
                res = cnt[i];
            }
        }
        return res;
    }
};
```



下面这种方法跟上面的解法基本一样，就是把更新结果res放在了遍历完数组之后，我们利用mx来找到所有的cnt[i]，累加到结果res上，参见代码如下：



解法二：

```
class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int res = 0, mx = 0, n = nums.size();
        vector<int> len(n, 1), cnt(n, 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[i] <= nums[j]) continue;
                if (len[i] == len[j] + 1) cnt[i] += cnt[j];
                else if (len[i] < len[j] + 1) {
                    len[i] = len[j] + 1;
                    cnt[i] = cnt[j];
                }
            }
            mx = max(mx, len[i]);
        }
        for (int i = 0; i < n; ++i) {
            if (mx == len[i]) res += cnt[i];
        }
        return res;
    }
};
```



类似题目：

[Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/4938187.html)

[Longest Continuous Increasing Subsequence](http://www.cnblogs.com/grandyang/p/7608976.html)



参考资料：

[https://discuss.leetcode.com/topic/102974/c-dp-with-explanation-o-n-2](https://discuss.leetcode.com/topic/102974/c-dp-with-explanation-o-n-2)

[https://discuss.leetcode.com/topic/103020/java-c-simple-dp-solution-with-explanation](https://discuss.leetcode.com/topic/103020/java-c-simple-dp-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














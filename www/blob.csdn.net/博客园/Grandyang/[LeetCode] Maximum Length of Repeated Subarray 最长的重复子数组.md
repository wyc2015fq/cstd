# [LeetCode] Maximum Length of Repeated Subarray 最长的重复子数组 - Grandyang - 博客园







# [[LeetCode] Maximum Length of Repeated Subarray 最长的重复子数组](https://www.cnblogs.com/grandyang/p/7801533.html)







Given two integer arrays `A` and `B`, return the maximum length of an subarray that appears in both arrays.

**Example 1:**

**Input:**
A: [1,2,3,2,1]
B: [3,2,1,4,7]
**Output:** 3
**Explanation:** 
The repeated subarray with maximum length is [3, 2, 1].

**Note:**

- 1 <= len(A), len(B) <= 1000
- 0 <= A[i], B[i] < 100



这道题给了我们两个数组A和B，让我们返回连个数组的最长重复子数组。那么如果我们将数组换成字符串，实际这道题就是求Longest Common Substring的问题了，而貌似LeetCode上并没有这种明显的要求最长相同子串的题，注意需要跟最长子序列Longest Common Subsequence区分开，关于最长子序列会在follow up中讨论。好，先来看这道题，对于这种求极值的问题，DP是不二之选，我们使用一个二维的DP数组，其中dp[i][j]表示数组A的前i个数字和数组B的前j个数字的最长子数组的长度，如果dp[i][j]不为0，则A中第i个数组和B中第j个数字必须相等，比对于这两个数组[1,2,2]和[3,1,2]，我们的dp数组为：



```
3 1 2
1 0 1 0
2 0 0 2
2 0 0 1
```



我们注意观察，dp值不为0的地方，都是当A[i] == B[j]的地方，而且还要加上左上方的dp值，即dp[i-1][j-1]，所以当前的dp[i][j]就等于dp[i-1][j-1] + 1，而一旦A[i] != B[j]时，直接赋值为0，不用多想，因为子数组是要连续的，一旦不匹配了，就不能再增加长度了。我们每次算出一个dp值，都要用来更新结果res，这样就能得到最长相同子数组的长度了，参见代码如下：



```
class Solution {
public:
    int findLength(vector<int>& A, vector<int>& B) {
        int res = 0;
        vector<vector<int>> dp(A.size() + 1, vector<int>(B.size() + 1, 0));
        for (int i = 1; i < dp.size(); ++i) {
            for (int j = 1; j < dp[i].size(); ++j) {
                dp[i][j] = (A[i - 1] == B[j - 1]) ? dp[i - 1][j - 1] + 1 : 0;
                res = max(res, dp[i][j]);
            }
        }
        return res;
    }
};
```



Follow up：在开始时，博主提到了要跟最长相同子序列Longest Common Subsequence区分开来，虽然LeetCode没有直接求最大相同子序列的题，但有几道题利用到了求该问题的思想，比如[Delete Operation for Two Strings](http://www.cnblogs.com/grandyang/p/7144045.html)和[Minimum ASCII Delete Sum for Two Strings](http://www.cnblogs.com/grandyang/p/7752002.html)等，详细讨论请参见评论区一楼 :)



类似题目：

[Minimum Size Subarray Sum](http://www.cnblogs.com/grandyang/p/4501934.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













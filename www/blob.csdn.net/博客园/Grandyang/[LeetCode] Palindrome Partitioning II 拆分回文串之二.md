# [LeetCode] Palindrome Partitioning II 拆分回文串之二 - Grandyang - 博客园







# [[LeetCode] Palindrome Partitioning II 拆分回文串之二](https://www.cnblogs.com/grandyang/p/4271456.html)







Given a string *s*, partition *s* such that every substring of the partition is a palindrome.

Return the minimum cuts needed for a palindrome partitioning of *s*.

Example:
Input: "aab"
Output: 1
Explanation: The palindrome partitioning ["aa","b"] could be produced using 1 cut.


这道题是让找到把原字符串拆分成回文串的最小切割数，如果我们首先考虑用brute force来做的话就会十分的复杂，因为我们不但要判断子串是否是回文串，而且还要找出最小切割数，情况会非常的多，不好做。所以对于这种玩字符串且是求极值的题，就要祭出旷古神器动态规划Dynamic Programming了，秒天秒地秒空气，DP在手天下我有。好，吹完一波后，开始做题。DP解法的两个步骤，定义dp数组和找状态转移方程。首先来定义dp数组，这里使用最直接的定义方法，一维的dp数组，其中dp[i]表示子串 [0, i] 范围内的最小分割数，那么我们最终要返回的就是 dp[n-1] 了，这里先加个corner case的判断，若s串为空，直接返回0，OJ的test case中并没有空串的检测，但博主认为还是加上比较好，毕竟空串也算是回文串的一种，所以最小分割数为0也说得过去。接下来就是大难点了，如何找出状态转移方程。

如何更新dp[i]呢，前面说过了其表示子串 [0, i] 范围内的最小分割数。那么这个区间的每个位置都可以尝试分割开来，所以就用一个变量j来从0遍历到i，这样就可以把区间 [0, i] 分为两部分，[0, j-1] 和 [j, i]，那么suppose我们已经知道区间 [0, j-1] 的最小分割数 dp[j-1]，因为我们是从前往后更新的，而 j 小于等于 i，所以 dp[j-1] 肯定在 dp[i] 之前就已经算出来了。这样我们就只需要判断区间 [j, i] 内的子串是否为回文串了，是的话，dp[i] 就可以用 1 + dp[j-1] 来更新了。判断子串的方法用的是之前那道 [Palindromic Substrings](http://www.cnblogs.com/grandyang/p/7404777.html) 一样的方法，使用一个二维的dp数组p，其中 p[i][j] 表示区间 [i, j] 内的子串是否为回文串，其状态转移方程为 p[i][j] = (s[i] == s[j]) && p[i+1][j-1]，其中 p[i][j] = true if [i, j]为回文。这样的话，这道题实际相当于同时用了两个DP的方法，确实难度不小呢。

第一个for循环遍历的是i，此时我们现将 dp[i] 初始化为 i，因为对于区间 [0, i]，就算我们每个字母割一刀（怎么听起来像凌迟？！），最多能只用分割 i 次，不需要再多于这个数字。但是可能会变小，所以第二个for循环用 j 遍历区间 [0, j]，根据上面的解释，我们需要验证的是区间 [j, i] 内的子串是否为回文串，那么只要 s[j] == s[i]，并且 i-j < 2 或者 p[j+1][i-1] 为true的话，先更新 p[j][i] 为true，然后在更新 dp[i]，这里需要注意一下corner case，当 j=0 时，我们直接给 dp[i] 赋值为0，因为此时能运行到这，说明 [j, i] 区间是回文串，而 j=0， 则说明 [0, i] 区间内是回文串，这样根本不用分割啊。若 j 大于0，则用 dp[j-1] + 1 来更新 dp[i]，最终返回 dp[n-1] 即可，参见代码如下：



解法一：

```
class Solution {
public:
    int minCut(string s) {
        if (s.empty()) return 0;
        int n = s.size();
        vector<vector<bool>> p(n, vector<bool>(n));
        vector<int> dp(n);
        for (int i = 0; i < n; ++i) {
            dp[i] = i;
            for (int j = 0; j <= i; ++j) {
                if (s[i] == s[j] && (i - j < 2 || p[j + 1][i - 1])) {
                    p[j][i] = true;
                    dp[i] = (j == 0) ? 0 : min(dp[i], dp[j - 1] + 1);
                }
            }
        }
        return dp[n - 1];
    }
};
```



我们也可以反向推，这里的dp数组的定义就刚好跟前面反过来了，dp[i] 表示区间 [i, n-1] 内的最小分割数，所以最终只需要返回 dp[0] 就是区间 [0, n-1] 内的最喜哦啊分割数了，极为所求。然后每次初始化 dp[i] 为 n-1-i 即可，j 的更新范围是 [i, n)，此时我们就只需要用 1 + dp[j+1] 来更新 dp[i] 了，为了防止越界，需要对 j == n-1 的情况单独处理一下，整个思想跟上面的解法一模一样，请参见之前的讲解。



解法二：

```
class Solution {
public:
    int minCut(string s) {
        if (s.empty()) return 0;
        int n = s.size();
        vector<vector<bool>> p(n, vector<bool>(n));
        vector<int> dp(n);
        for (int i = n - 1; i >= 0; --i) {
            dp[i] = n - i - 1;
            for (int j = i; j < n; ++j) {
                if (s[i] == s[j] && (j - i <= 1 || p[i + 1][j - 1])) {
                    p[i][j] = true;
                    dp[i] = (j == n - 1) ? 0 : min(dp[i], dp[j + 1] + 1);
                }
            }
        }
        return dp[0];
    }
};
```



下面这种解法是论坛上的高分解法，没用使用判断区间 [i, j] 内是否为回文串的二维dp数组，节省了空间。但写法上比之前的解法稍微有些凌乱，也算是个 trade-off 吧。这里还是用的一维dp数组，不过大小初始化为了 n+1，这样其定义就稍稍发生了些变化，dp[i] 表示由s串中前 i 个字母组成的子串的最小分割数，这样 dp[n] 极为最终所求。接下来就要找状态转移方程了。这道题的更新方式比较特别，跟之前的都不一样，之前遍历 i 的时候，都是更新的 dp[i]，这道题更新的却是 dp[i+len+1] 和 dp[i+len+2]，其中 len 是以i为中心，总长度为 2*len + 1 的回文串，比如 bob，此时 i=1，len=1，或者是i为中心之一，总长度为 2*len + 2 的回文串，比如 noon，此时 i=1，len=1。中间两个for循环就是分别更新以 i 为中心且长度为 2*len + 1 的奇数回文串，和以 i 为中心之一且长度为 2*len + 2 的偶数回文串的。i-len 正好是奇数或者偶数回文串的起始位置，由于我们定义的 dp[i] 是区间 [0, i-1] 的最小分割数，所以 dp[i-len] 就是区间 [0, i-len-1] 范围内的最小分割数，那么加上奇数回文串长度 2*len + 1，此时整个区间为 [0, i+len]，即需要更新 dp[i+len+1]。如果是加上偶数回文串的长度 2*len + 2，那么整个区间为 [0, i+len+1]，即需要更新 dp[i+len+2]。这就是分奇偶的状态转移方程，参见代码如下：



解法三：

```
class Solution {
public:
    int minCut(string s) {
        if (s.empty()) return 0;
        int n = s.size();
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = -1;
        for (int i = 0; i < n; ++i) {
            for (int len = 0; i - len >= 0 && i + len < n && s[i - len] == s[i + len]; ++len) {
                dp[i + len + 1] = min(dp[i + len + 1], 1 + dp[i - len]);
            }
            for (int len = 0; i - len >= 0 && i + len + 1 < n && s[i - len] == s[i + len + 1]; ++len) {
                dp[i + len + 2] = min(dp[i + len + 2], 1 + dp[i - len]);
            }
        }
        return dp[n];
    }
};
```



类似题目：

[Palindrome Partitioning](http://www.cnblogs.com/grandyang/p/4270008.html)

[Palindromic Substrings](http://www.cnblogs.com/grandyang/p/7404777.html)



参考资料：

[https://leetcode.com/problems/palindrome-partitioning-ii/](https://leetcode.com/problems/palindrome-partitioning-ii/)

[https://leetcode.com/problems/palindrome-partitioning-ii/discuss/42213/Easiest-Java-DP-Solution-(97.36)](https://leetcode.com/problems/palindrome-partitioning-ii/discuss/42213/Easiest-Java-DP-Solution-(97.36))

[https://leetcode.com/problems/palindrome-partitioning-ii/discuss/42199/My-DP-Solution-(-explanation-and-code)](https://leetcode.com/problems/palindrome-partitioning-ii/discuss/42199/My-DP-Solution-(-explanation-and-code))

[https://leetcode.com/problems/palindrome-partitioning-ii/discuss/42212/Two-C%2B%2B-versions-given-(one-DP-28ms-one-Manancher-like-algorithm-10-ms)](https://leetcode.com/problems/palindrome-partitioning-ii/discuss/42212/Two-C%2B%2B-versions-given-(one-DP-28ms-one-Manancher-like-algorithm-10-ms))

[https://leetcode.com/problems/palindrome-partitioning-ii/discuss/42198/My-solution-does-not-need-a-table-for-palindrome-is-it-right-It-uses-only-O(n)-space.](https://leetcode.com/problems/palindrome-partitioning-ii/discuss/42198/My-solution-does-not-need-a-table-for-palindrome-is-it-right-It-uses-only-O(n)-space.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













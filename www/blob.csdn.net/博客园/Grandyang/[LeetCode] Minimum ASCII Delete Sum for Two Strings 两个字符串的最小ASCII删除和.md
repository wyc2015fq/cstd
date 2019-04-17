# [LeetCode] Minimum ASCII Delete Sum for Two Strings 两个字符串的最小ASCII删除和 - Grandyang - 博客园







# [[LeetCode] Minimum ASCII Delete Sum for Two Strings 两个字符串的最小ASCII删除和](https://www.cnblogs.com/grandyang/p/7752002.html)








Given two strings `s1, s2`, find the lowest ASCII sum of deleted characters to make two strings equal.

Example 1:
Input: s1 = "sea", s2 = "eat"
Output: 231
Explanation: Deleting "s" from "sea" adds the ASCII value of "s" (115) to the sum.
Deleting "t" from "eat" adds 116 to the sum.
At the end, both strings are equal, and 115 + 116 = 231 is the minimum sum possible to achieve this.



Example 2:
Input: s1 = "delete", s2 = "leet"
Output: 403
Explanation: Deleting "dee" from "delete" to turn the string into "let",
adds 100[d]+101[e]+101[e] to the sum.  Deleting "e" from "leet" adds 101[e] to the sum.
At the end, both strings are equal to "let", and the answer is 100+101+101+101 = 403.
If instead we turned both strings into "lee" or "eet", we would get answers of 433 or 417, which are higher.



Note:
- `0 < s1.length, s2.length <= 1000`.
- All elements of each string will have an ASCII value in `[97, 122]`.



这道题给了我们两个字符串，让我们删除一些字符使得两个字符串相等，我们希望删除的字符的ASCII码最小。这道题跟之前那道[Delete Operation for Two Strings](http://www.cnblogs.com/grandyang/p/7144045.html)极其类似，那道题让求删除的最少的字符数，这道题换成了ASCII码值。其实很多大厂的面试就是这种改动，虽然很少出原题，但是这种小范围的改动却是很经常的，所以当背题侠是没有用的，必须要完全掌握了解题思想，并能举一反三才是最重要的。看到这种玩字符串又是求极值的题，想都不要想直接上DP，我们建立一个二维数组dp，其中dp[i][j]表示字符串s1的前i个字符和字符串s2的前j个字符变相等所要删除的字符的最小ASCII码累加值。那么我们可以先初始化边缘，即有一个字符串为空的话，那么另一个字符串有多少字符就要删多少字符，才能变空字符串。所以我们初始化dp[0][j]和dp[i][0]，计算方法就是上一个dp值加上对应位置的字符，有点像计算累加数组的方法，由于字符就是用ASCII表示的，所以我们不用转int，直接累加就可以。这里我们把dp[i][0]的计算放入大的循环中计算，是为了少写一个for循环。好，现在我们来看递推公式，需要遍历这个二维数组的每一个位置即dp[i][j]，当对应位置的字符相等时，s1[i-1] == s2[j-1]，(注意由于dp数组的i和j是从1开始的，所以字符串中要减1)，那么我们直接赋值为上一个状态的dp值，即dp[i-1][j-1]，因为已经匹配上了，不用删除字符。如果s1[i-1] != s2[j-1]，那么就有两种情况，我们可以删除s[i-1]的字符，且加上被删除的字符的ASCII码到上一个状态的dp值中，即dp[i-1][j] + s1[i-1]，或者删除s[j-1]的字符，且加上被删除的字符的ASCII码到上一个状态的dp值中，即dp[i][j-1] + s2[j-1]。这不难理解吧，比如sea和eat，当首字符s和e失配了，那么有两种情况，要么删掉s，用ea和eat继续匹配，或者删掉e，用sea和at继续匹配，记住删掉的字符一定要累加到dp值中才行，参见代码如下：



解法一： 

```
class Solution {
public:
    int minimumDeleteSum(string s1, string s2) {
        int m = s1.size(), n = s2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        for (int j = 1; j <= n; ++j) dp[0][j] = dp[0][j - 1] + s2[j - 1];
        for (int i = 1; i <= m; ++i) {
            dp[i][0] = dp[i - 1][0] + s1[i - 1];
            for (int j = 1; j <= n; ++j) {
                dp[i][j] = (s1[i - 1] == s2[j - 1]) ? dp[i - 1][j - 1] : min(dp[i - 1][j] + s1[i - 1], dp[i][j - 1] + s2[j - 1]);
            }
        }
        return dp[m][n];
    }
};
```



我们也可以优化空间复杂度，使用一个一维数组dp，其中dp[i]表示字符串s1和字符串s2的前i个字符变相等所要删除的字符的最小ASCII码累加值。刚开始还是要初始化dp[j]，这里用变量t1和t2保存上一个状态的值，并不断更新。如果面试官没有特别的要求，还是用二维dp数组吧，毕竟逻辑更清晰一些，一维的容易写错～



解法二：

```
class Solution {
public:
    int minimumDeleteSum(string s1, string s2) {
        int m = s1.size(), n = s2.size();
        vector<int> dp(n + 1, 0);
        for (int j = 1; j <= n; ++j) dp[j] = dp[j - 1] + s2[j - 1];
        for (int i = 1; i <= m; ++i) {
            int t1 = dp[0];
            dp[0] += s1[i - 1];
            for (int j = 1; j <= n; ++j) {
                int t2 = dp[j];
                dp[j] = (s1[i - 1] == s2[j - 1]) ? t1 : min(dp[j] + s1[i - 1], dp[j - 1] + s2[j - 1]);
                t1 = t2;
            }
        }
        return dp[n];
    }
};
```



下面这种方法思路也很巧妙，反其道而行之，相当于先计算了字符串s1和s2的最大相同子序列，在这道题中就是最大相同子序列的ASCII码值，然后用s1和s2的所有字符之和减去这个最大ASCII码值的两倍，就是要删除的字符的最小ASCII码值了。那么还是建立二维数组dp，其中dp[i][j]表示字符串s1的前i个字符和字符串s2点前j个字符中的最大相同子序列的ASCII值。然后我们遍历所有的位置，当对应位置的字符相等时，s1[i-1] == s2[j-1]，那么我们直接赋值为上一个状态的dp值加上这个相同的字符，即dp[i-1][j-1] + s1[i-1]，注意这里跟解法一不同之处，因为dp的定义不同，所以写法不同。如果s1[i-1] != s2[j-1]，那么就有两种情况，我们可以删除s[i-1]的字符，即dp[i-1][j]，或者删除s[j-1]的字符，即dp[i][j-1]，取二者中最大值赋给dp[i][j]。最后分别算出s1和s2的累加值，减去两倍的dp最大值即可，参见代码如下：



解法三：

```
class Solution {
public:
    int minimumDeleteSum(string s1, string s2) {
        int m = s1.size(), n = s2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1] + s1[i - 1];
                else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        int sum1 = accumulate(s1.begin(), s1.end(), 0);
        int sum2 = accumulate(s2.begin(), s2.end(), 0);
        return sum1 + sum2 - 2 * dp[m][n];
    }
};
```



类似题目：

[Edit Distance](http://www.cnblogs.com/grandyang/p/4344107.html)

[Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/4938187.html)


[Delete Operation for Two Strings](http://www.cnblogs.com/grandyang/p/7144045.html)



参考资料：

[https://discuss.leetcode.com/topic/107995/concise-dp-solution](https://discuss.leetcode.com/topic/107995/concise-dp-solution)

[https://discuss.leetcode.com/topic/107980/c-dp-with-explanation](https://discuss.leetcode.com/topic/107980/c-dp-with-explanation/2)

[https://discuss.leetcode.com/topic/108029/lcs-variation-solution-python-c](https://discuss.leetcode.com/topic/108029/lcs-variation-solution-python-c/2)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













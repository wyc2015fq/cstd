# [LeetCode] Delete Operation for Two Strings 两个字符串的删除操作 - Grandyang - 博客园







# [[LeetCode] Delete Operation for Two Strings 两个字符串的删除操作](https://www.cnblogs.com/grandyang/p/7144045.html)







Given two words *word1* and *word2*, find the minimum number of steps required to make *word1* and *word2* the same, where in each step you can delete one character in either string.

Example 1:
Input: "sea", "eat"
Output: 2
Explanation: You need one step to make "sea" to "ea" and another step to make "eat" to "ea".



Note:
- The length of given words won't exceed 500.
- Characters in given words can only be lower-case letters.



这道题给了我们两个单词，问我们最少需要多少步可以让两个单词相等，每一步我们可以在任意一个单词中删掉一个字符。那么我们分析怎么能让步数最少呢，是不是知道两个单词最长的相同子序列的长度，并乘以2，被两个单词的长度之和减，就是最少步数了。其实这道题就转换成求Longest Common Subsequence最长相同子序列的问题，令博主意外的是，LeetCode中竟然没有这道题，这与包含万物的LeetCode的作风不符啊。不过没事，有这道题也行啊，对于这种玩字符串，并且是求极值的问题，十有八九都是用dp来解的，曾经有网友问博主，如何确定什么时候用greedy，什么时候用dp？其实博主也不不太清楚，感觉dp要更tricky一些，而且出现的概率大，所以博主一般会先考虑dp，如果实在想不出递推公式，那么就想想greedy能做不。如果有大神知道更好的区分方法，请一定留言告知博主啊，多谢！那么决定了用dp来做，就定义一个二维的dp数组，其中dp[i][j]表示word1的前i个字符和word2的前j个字符组成的两个单词的最长公共子序列的长度。下面来看递推式dp[i][j]怎么求，首先来考虑dp[i][j]和dp[i-1][j-1]之间的关系，我们可以发现，如果当前的两个字符相等，那么dp[i][j] = dp[i-1][j-1] + 1，这不难理解吧，因为最长相同子序列又多了一个相同的字符，所以长度加1。由于我们dp数组的大小定义的是(n1+1) x (n2+1)，所以我们比较的是word1[i-1]和word2[j-1]。那么我们想如果这两个字符不相等呢，难道我们直接将dp[i-1][j-1]赋值给dp[i][j]吗，当然不是，我们还要错位相比嘛，比如就拿题目中的例子来说，"sea"和"eat"，当我们比较第一个字符，发现's'和'e'不相等，下一步就要错位比较啊，比较sea中第一个's'和eat中的'a'，sea中的'e'跟eat中的第一个'e'相比，这样我们的dp[i][j]就要取dp[i-1][j]跟dp[i][j-1]中的较大值了，最后我们求出了最大共同子序列的长度，就能直接算出最小步数了，参见代码如下：



解法一：

```
class Solution {
public:
    int minDistance(string word1, string word2) {
        int n1 = word1.size(), n2 = word2.size();
        vector<vector<int>> dp(n1 + 1, vector<int>(n2 + 1, 0));
        for (int i = 1; i <= n1; ++i) {
            for (int j = 1; j <= n2; ++j) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return n1 + n2 - 2 * dp[n1][n2];
    }
};
```



下面这种方法也是用的dp，但是和上面的dp思路不太一样，这种算法是跟之前那道[Edit Distance](http://www.cnblogs.com/grandyang/p/4344107.html)相同的思路。那道题问我们一个单词通过多少步修改可以得到另一个单词，其实word2删除一个字符，和跟在word1对应的地方加上那个要删除的字符，达到的效果是一样的，并不影响最终的步骤数，所以这道题完全可以按照那道题的解法来做，一点都不需要变动，定义一个二维的dp数组，其中dp[i][j]表示word1的前i个字符和word2的前j个字符组成的两个单词，能使其变相同的最小的步数，讲解可以参看那篇帖子，参见代码入下：



解法二：

```
class Solution {
public:
    int minDistance(string word1, string word2) {
        int n1 = word1.size(), n2 = word2.size();
        vector<vector<int>> dp(n1 + 1, vector<int>(n2 + 1, 0));
        for (int i = 0; i <= n1; ++i) dp[i][0] = i;
        for (int j = 0; j <= n2; ++j) dp[0][j] = j;
        for (int i = 1; i <= n1; ++i) {
            for (int j = 1; j <= n2; ++j) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[n1][n2];
    }
};
```



下面这种方法是解法二的递归写法，用的优化的dfs的方法，用memo数组来保存中间计算结果，以避免大量的重复计算，参见代码如下：



解法三：

```
class Solution {
public:
    int minDistance(string word1, string word2) {
        int n1 = word1.size(), n2 = word2.size();
        vector<vector<int>> memo(n1 + 1, vector<int>(n2 + 1, 0));
        return helper(word1, word2, 0, 0, memo);
    }
    int helper(string word1, string word2, int p1, int p2, vector<vector<int>>& memo) {
        if (memo[p1][p2] != 0) return memo[p1][p2];
        int n1 = word1.size(), n2 = word2.size();
        if (p1 == n1 || p2 == n2) return n1 - p1 + n2 - p2;
        if (word1[p1] == word2[p2]) {
            memo[p1][p2] = helper(word1, word2, p1 + 1, p2 + 1, memo);
        } else {
            memo[p1][p2] = 1 + min(helper(word1, word2, p1 + 1, p2, memo), helper(word1, word2, p1, p2 + 1, memo));
        }
        return memo[p1][p2];
    }
};
```



类似题目：

[Edit Distance](http://www.cnblogs.com/grandyang/p/4344107.html)

[Minimum ASCII Delete Sum for Two Strings](http://www.cnblogs.com/grandyang/p/7752002.html)



参考资料：

[https://discuss.leetcode.com/topic/89285/java-dp-solution-longest-common-subsequence](https://discuss.leetcode.com/topic/89285/java-dp-solution-longest-common-subsequence)

[https://discuss.leetcode.com/topic/89308/dynamic-programming-and-memoization-solution](https://discuss.leetcode.com/topic/89308/dynamic-programming-and-memoization-solution)

[https://discuss.leetcode.com/topic/89433/two-pointers-recursive-java-solution-with-memoization](https://discuss.leetcode.com/topic/89433/two-pointers-recursive-java-solution-with-memoization)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













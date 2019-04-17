# [LeetCode] Palindromic Substrings 回文子字符串 - Grandyang - 博客园







# [[LeetCode] Palindromic Substrings 回文子字符串](https://www.cnblogs.com/grandyang/p/7404777.html)







Given a string, your task is to count how many palindromic substrings in this string.

The substrings with different start indexes or end indexes are counted as different substrings even they consist of same characters.

Example 1:
Input: "abc"
Output: 3
Explanation: Three palindromic strings: "a", "b", "c".



Example 2:
Input: "aaa"
Output: 6
Explanation: Six palindromic strings: "a", "a", "a", "aa", "aa", "aaa".



Note:
- The input string length won't exceed 1000.



这道题给了我们一个字符串，让我们计算有多少个回文子字符串。博主看到这个题，下意识的想着应该是用DP来做，哼哼哧哧写了半天，修修补补，终于通过了，但是博主写的DP不是最简便的方法，略显复杂，这里就不贴了。还是直接讲解大神们的解法好了。其实这道题也可以用递归来做，而且思路非常的简单粗暴。就是以字符串中的每一个字符都当作回文串中间的位置，然后向两边扩散，每当成功匹配两个左右两个字符，结果res自增1，然后再比较下一对。注意回文字符串有奇数和偶数两种形式，如果是奇数长度，那么i位置就是中间那个字符的位置，所以我们左右两遍都从i开始遍历；如果是偶数长度的，那么i是最中间两个字符的左边那个，右边那个就是i+1，这样就能cover所有的情况啦，而且都是不同的回文子字符串，参见代码如下：



解法一：

```
class Solution {
public:
    int countSubstrings(string s) {
        if (s.empty()) return 0;
        int n = s.size(), res = 0;
        for (int i = 0; i < n; ++i) {
            helper(s, i, i, res);
            helper(s, i, i + 1, res);
        }
        return res;
    }
    void helper(string s, int i, int j, int& res) {
        while (i >= 0 && j < s.size() && s[i] == s[j]) {
            --i; ++j; ++res;
        }
    }
};
```



在刚开始的时候博主提到了自己写的DP的方法比较复杂，为什么呢，因为博主的dp[i][j]定义的是范围[i, j]之间的子字符串的个数，这样我们其实还需要一个二维数组来记录子字符串[i, j]是否是回文串，那么我们直接就将dp[i][j]定义成子字符串[i, j]是否是回文串就行了，然后我们i从n-1往0遍历，j从i往n-1遍历，然后我们看s[i]和s[j]是否相等，这时候我们需要留意一下，有了s[i]和s[j]相等这个条件后，i和j的位置关系很重要，如果i和j相等了，那么dp[i][j]肯定是true；如果i和j是相邻的，那么dp[i][j]也是true；如果i和j中间只有一个字符，那么dp[i][j]还是true；如果中间有多余一个字符存在，那么我们需要看dp[i+1][j-1]是否为true，若为true，那么dp[i][j]就是true。赋值dp[i][j]后，如果其为true，结果res自增1，参见代码如下：



解法二：

```
class Solution {
public:
    int countSubstrings(string s) {
        int n = s.size(), res = 0;
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        for (int i = n - 1; i >= 0; --i) {
            for (int j = i; j < n; ++j) {
                dp[i][j] = (s[i] == s[j]) && (j - i <= 2 || dp[i + 1][j - 1]);
                if (dp[i][j]) ++res;
            }
        }
        return res;
    }
};
```



类似题目：

[Longest Palindromic Subsequence](http://www.cnblogs.com/grandyang/p/6493182.html)

[Longest Palindromic Substring](http://www.cnblogs.com/grandyang/p/4464476.html)



参考资料：

[https://discuss.leetcode.com/topic/96819/java-solution-8-lines-extendpalindrome](https://discuss.leetcode.com/topic/96819/java-solution-8-lines-extendpalindrome)

[https://discuss.leetcode.com/topic/96884/very-simple-java-solution-with-detail-explanation](https://discuss.leetcode.com/topic/96884/very-simple-java-solution-with-detail-explanation)

[https://discuss.leetcode.com/topic/96821/java-dp-solution-based-on-longest-palindromic-substring](https://discuss.leetcode.com/topic/96821/java-dp-solution-based-on-longest-palindromic-substring)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













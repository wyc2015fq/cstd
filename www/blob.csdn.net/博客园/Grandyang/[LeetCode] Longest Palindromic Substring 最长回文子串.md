# [LeetCode] Longest Palindromic Substring 最长回文子串 - Grandyang - 博客园







# [[LeetCode] Longest Palindromic Substring 最长回文子串](https://www.cnblogs.com/grandyang/p/4464476.html)








Given a string s, find the longest palindromic substring in s. You may assume that the maximum length of s is 1000.

Example 1:
Input: "babad"
Output: "bab"
Note: "aba" is also a valid answer.

Example 2:
Input: "cbbd"
Output: "bb"


这道题让我们求[最长回文子串](http://en.wikipedia.org/wiki/Longest_palindromic_substring)，首先说下什么是回文串，就是正读反读都一样的字符串，比如 "bob", "level", "noon" 等等。那么最长回文子串就是在一个字符串中的那个最长的回文子串。LeetCode 中关于回文串的题共有五道，除了这道，其他的四道为 [Palindrome Number](http://www.cnblogs.com/grandyang/p/4125510.html)，[Validate Palindrome](http://www.cnblogs.com/grandyang/p/4030114.html)，[Palindrome Partitioning](http://www.cnblogs.com/grandyang/p/4270008.html)，[Palindrome Partitioning II](http://www.cnblogs.com/grandyang/p/4271456.html)，我们知道传统的验证回文串的方法就是两个两个的对称验证是否相等，那么对于找回文字串的问题，就要以每一个字符为中心，像两边扩散来寻找回文串，这个算法的时间复杂度是 O(n*n)，可以通过 OJ，就是要注意奇偶情况，由于回文串的长度可奇可偶，比如 "bob" 是奇数形式的回文，"noon" 就是偶数形式的回文，两种形式的回文都要搜索，对于奇数形式的，我们就从遍历到的位置为中心，向两边进行扩散，对于偶数情况，我们就把当前位置和下一个位置当作偶数行回文的最中间两个字符，然后向两边进行搜索，参见代码如下：



解法一：

```
class Solution {
public:
    string longestPalindrome(string s) {
        if (s.size() < 2) return s;
        int n = s.size(), maxLen = 0, start = 0;
        for (int i = 0; i < n - 1; ++i) {
            searchPalindrome(s, i, i, start, maxLen);
            searchPalindrome(s, i, i + 1, start, maxLen);
        }
        return s.substr(start, maxLen);
    }
    void searchPalindrome(string s, int left, int right, int& start, int& maxLen) {
        while (left >= 0 && right < s.size() && s[left] == s[right]) {
            --left; ++right;
        }
        if (maxLen < right - left - 1) {
            start = left + 1;
            maxLen = right - left - 1;
        }
    }
};
```



我们也可以不使用子函数，直接在一个函数中搞定，我们还是要定义两个变量 start 和 maxLen，分别表示最长回文子串的起点跟长度，在遍历s中的字符的时候，我们首先判断剩余的字符数是否小于等于 maxLen 的一半，是的话说明 maxLen 无法再变长了，直接break掉。否则就要继续判断，我们用两个变量left和right分别指向当前位置，然后我们先要做的是向右遍历跳过重复项，这个操作很必要，比如对于 noon，i在第一个o的位置，如果我们以o为最中心往两边扩散，是无法得到长度为4的回文串的，只有先跳过重复，此时left指向第一个o，right指向第二个o，然后再向两边扩散。而对于 bob，i在第一个o的位置时，无法向右跳过重复，此时 left 和 right 同时指向o，再向两边扩散也是正确的，所以可以同时处理奇数和偶数的回文串，之后的操作就是更新 maxLen 和 start 了，跟上面的操作一样，参见代码如下： 



解法二：

```
class Solution {
public:
    string longestPalindrome(string s) {
        if (s.size() < 2) return s;
        int n = s.size(), maxLen = 0, start = 0;
        for (int i = 0; i < n;) {
            if (n - i <= maxLen / 2) break;
            int left = i, right = i;
            while (right < n - 1 && s[right + 1] == s[right]) ++right;
            i = right + 1;
            while (right < n - 1 && left > 0 && s[right + 1] == s[left - 1]) {
                ++right; --left;
            }
            if (maxLen < right - left + 1) {
                maxLen = right - left + 1;
                start = left;
            }
        }
        return s.substr(start, maxLen);
    }
};
```



此题还可以用动态规划 Dynamic Programming 来解，根 [Palindrome Partitioning II](http://www.cnblogs.com/grandyang/p/4271456.html) 的解法很类似，我们维护一个二维数组 dp，其中 dp[i][j] 表示字符串区间 [i, j] 是否为回文串，当 i = j 时，只有一个字符，肯定是回文串，如果 i = j + 1，说明是相邻字符，此时需要判断 s[i] 是否等于 s[j]，如果i和j不相邻，即 i - j >= 2 时，除了判断 s[i] 和 s[j] 相等之外，dp[j + 1][i - 1] 若为真，就是回文串，通过以上分析，可以写出递推式如下：

dp[i, j] = 1                                               if i == j

           = s[i] == s[j]                                if j = i + 1

           = s[i] == s[j] && dp[i + 1][j - 1]    if j > i + 1      

这里有个有趣的现象就是如果我把下面的代码中的二维数组由 int 改为 vector<vector<int>> 后，就会超时，这说明 int 型的二维数组访问执行速度完爆 std 的 vector 啊，所以以后尽可能的还是用最原始的数据类型吧。



解法三：

```
class Solution {
public:
    string longestPalindrome(string s) {
        if (s.empty()) return "";
        int dp[s.size()][s.size()] = {0}, left = 0, right = 0, len = 0;
        for (int i = 0; i < s.size(); ++i) {
            dp[i][i] = 1;
            for (int j = 0; j < i; ++j) {
                dp[j][i] = (s[i] == s[j] && (i - j < 2 || dp[j + 1][i - 1]));
                if (dp[j][i] && len < i - j + 1) {
                    len = i - j + 1;
                    left = j;
                    right = i;
                }
            }
        }
        return s.substr(left, right - left + 1);
    }
};
```



最后要来的就是大名鼎鼎的马拉车算法 Manacher's Algorithm，这个算法的神奇之处在于将时间复杂度提升到了O(n)这种逆天的地步，而算法本身也设计的很巧妙，很值得我们掌握，参见我另一篇专门介绍马拉车算法的博客 [Manacher's Algorithm 马拉车算法](http://www.cnblogs.com/grandyang/p/4475985.html)，代码实现如下：



解法四：

```
class Solution {
public:
    string longestPalindrome(string s) {
        string t ="$#";
        for (int i = 0; i < s.size(); ++i) {
            t += s[i];
            t += '#';
        }
        int p[t.size()] = {0}, id = 0, mx = 0, resId = 0, resMx = 0;
        for (int i = 1; i < t.size(); ++i) {
            p[i] = mx > i ? min(p[2 * id - i], mx - i) : 1;
            while (t[i + p[i]] == t[i - p[i]]) ++p[i];
            if (mx < i + p[i]) {
                mx = i + p[i];
                id = i;
            }
            if (resMx < p[i]) {
                resMx = p[i];
                resId = i;
            }
        }
        return s.substr((resId - resMx) / 2, resMx - 1);
    }
};
```



类似题目：

[Shortest Palindrome](http://www.cnblogs.com/grandyang/p/4523624.html)

[Palindrome Permutation](http://www.cnblogs.com/grandyang/p/5223238.html)

[Palindrome Pairs](http://www.cnblogs.com/grandyang/p/5272039.html)

[Longest Palindromic Subsequence](http://www.cnblogs.com/grandyang/p/6493182.html)

[Palindromic Substrings](http://www.cnblogs.com/grandyang/p/7404777.html)



参考资料：

[https://leetcode.com/problems/longest-palindromic-substring/](https://leetcode.com/problems/longest-palindromic-substring/description/)

[https://leetcode.com/problems/longest-palindromic-substring/discuss/2928/Very-simple-clean-java-solution](https://leetcode.com/problems/longest-palindromic-substring/discuss/2928/Very-simple-clean-java-solution)

[https://leetcode.com/problems/longest-palindromic-substring/discuss/2923/Simple-C%2B%2B-solution-(8ms-13-lines)](https://leetcode.com/problems/longest-palindromic-substring/discuss/2923/Simple-C%2B%2B-solution-(8ms-13-lines))

[https://leetcode.com/problems/longest-palindromic-substring/discuss/2921/Share-my-Java-solution-using-dynamic-programming](https://leetcode.com/problems/longest-palindromic-substring/discuss/2921/Share-my-Java-solution-using-dynamic-programming)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













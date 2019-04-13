
# leetcode小白解题记录——第五题 - lin_tuer的博客 - CSDN博客


2016年09月08日 11:26:12[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：218


5. Longest Palindromic Substring

Given
 a stringS,
 find the longest palindromic substring inS.
 You may assume that the maximum length ofSis
 1000, and there exists one unique longest palindromic substring.

解决方案：
》1.
 穷竭搜索
穷举所有可能的子串，判断子串是否为回文，使用一变量记录最大回文长度，若新的回文超过之前的最大回文长度则更新标记变量并记录当前回文的起止索引，最后返回最长回文子串。

```java
Java
public class Solution {
    /**
     * @param s input string
     * @return the longest palindromic substring
     */
    public String longestPalindrome(String s) {
        String result = new String();
        if (s == null || s.isEmpty()) return result;
        int n = s.length();
        int longest = 0, left = 0, right = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j <= n; j++) {
                String substr = s.substring(i, j);
                if (isPalindrome(substr) && substr.length() > longest) {
                    longest = substr.length();
                    left = i;
                    right = j;
                }
            }
        }
        result = s.substring(left, right);
        return result;
    }
    private boolean isPalindrome(String s) {
        if (s == null || s.isEmpty()) return false;
        int n = s.length();
        for (int i = 0; i < n; i++) {
            if (s.charAt(i) != s.charAt(n - i - 1)) return false;
        }
        return true;
    }
}
```

》2.
 动态规划：由内而外一层一层扩展开来
首先初始化，i>=j时，flag[i][j]=true，这是因为s[i][i]是单字符的回文，当i>j时，为true，是因为有可能出现flag[2][1]这种情况，比如bcaa，当计算s从2到3的时候，s[2]==s[3]，这时就要计算s[2+1] ?= s[3-1]，总的来说，当i>j时置为true，就是为了考虑j=i+1这种情况。
接着比较s[i] ?= s[j]，如果成立，那么flag[i][j] = flag[i+1][j-1]，否则直接flag[i][j]=false

```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int len = s.length(), max = 1, ss = 0, tt = 0;
        bool flag[len][len];
        for (int i = 0; i < len; i++)
            for (int j = 0; j < len; j++)
                if (i >= j)
                    flag[i][j] = true;
                else flag[i][j] = false;
        for (int j = 1; j < len; j++)
            for (int i = 0; i < j; i++)
            {
                if (s[i] == s[j])
                {
                    flag[i][j] = flag[i+1][j-1];
                    if (flag[i][j] == true && j - i + 1 > max)
                    {
                        max = j - i + 1;
                        ss = i;
                        tt = j;
                    }
                }
                else flag[i][j] = false;
            }
        return s.substr(ss, max);
    }
};
```

参考网站：感谢！http://www.cnblogs.com/dollarzhaole/archive/2013/07/22/3207364.html
http://www.kancloud.cn/kancloud/data-structure-and-algorithm-notes/72941




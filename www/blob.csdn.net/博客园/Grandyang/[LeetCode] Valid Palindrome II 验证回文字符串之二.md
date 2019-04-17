# [LeetCode] Valid Palindrome II 验证回文字符串之二 - Grandyang - 博客园







# [[LeetCode] Valid Palindrome II 验证回文字符串之二](https://www.cnblogs.com/grandyang/p/7618468.html)







Given a non-empty string `s`, you may delete **at most** one character. Judge whether you can make it a palindrome.

**Example 1:**

**Input:** "aba"
**Output:** True

**Example 2:**

**Input:** "abca"
**Output:** True
**Explanation:** You could delete the character 'c'.

**Note:**

- The string will only contain lowercase characters a-z.
The maximum length of the string is 50000.



这道题是之前那道[Valid Palindrome](http://www.cnblogs.com/grandyang/p/4030114.html)的拓展，还是让我们验证回复字符串，但是区别是这道题的字符串中只含有小写字母，而且这道题允许删除一个字符，那么当遇到不匹配的时候，我们到底是删除左边的字符，还是右边的字符呢，我们的做法是两种情况都要算一遍，只要有一种能返回true，那么结果就返回true。我们可以写一个子函数来判断字符串中的某一个范围内的子字符串是否为回文串，参见代码如下：



解法一：

```
class Solution {
public:
    bool validPalindrome(string s) {
        int left = 0, right = s.size() - 1;
        while (left < right) {
            if (s[left] != s[right]) return isValid(s, left, right - 1) || isValid(s, left + 1, right);
            ++left; --right;
        }
        return true;
    }
    bool isValid(string s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) return false;
            ++left; --right;
        }
        return true;
    }
};
```



下面这种写法跟上面的解法思路一样，只不过没有写额外的函数，还是要遍历两种情况，参见代码如下：



解法二：

```
class Solution {
public:
    bool validPalindrome(string s) {
        int left = 0, right = s.size() - 1;
        while (left < right) {
            if (s[left] == s[right]) {
                ++left; --right;
            } else {
                int l = left, r = right - 1;
                while (l < r) {
                    if (s[l] != s[r]) break;
                    ++l; --r;
                    if (l >= r) return true;
                }
                ++left;
                while (left < right) {
                    if (s[left] != s[right]) return false;
                    ++left; --right;
                }
            }
        }
        return true;
    }
};
```



类似题目：

[Valid Palindrome](http://www.cnblogs.com/grandyang/p/4030114.html)



参考资料：

[https://discuss.leetcode.com/topic/103939/java-o-n-time-o-1-space](https://discuss.leetcode.com/topic/103939/java-o-n-time-o-1-space)

[https://discuss.leetcode.com/topic/103911/two-solutions-optimized-and-recursive-java-and-c](https://discuss.leetcode.com/topic/103911/two-solutions-optimized-and-recursive-java-and-c)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













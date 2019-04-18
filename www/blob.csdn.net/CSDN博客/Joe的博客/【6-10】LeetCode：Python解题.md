# 【6-10】LeetCode：Python解题 - Joe的博客 - CSDN博客





2017年12月09日 11:47:01[Joe-Han](https://me.csdn.net/u010089444)阅读数：822








## **6. ZigZag Conversion**【Medium】

The string **“PAYPALISHIRING”** is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

```
P   A   H   N
A P L S I I G
Y   I   R
```

And then read line by line: **“PAHNAPLSIIGYIR”**

Write the code that will take a string and make this conversion given a number of rows:

Write the code that will take a string and make this conversion given a number of rows:
`string convert(string text, int nRows);`
**convert(“PAYPALISHIRING”, 3)** should return **“PAHNAPLSIIGYIR”**.

**Solution:**

```python
class Solution(object):
    def convert(self, s, numRows):
        """
        :type s: str
        :type numRows: int
        :rtype: str
        """
        if numRows == 1 or numRows >= len(s):
            return s
        ss = ['']*numRows
        index = step =  0
        for i in s:
            ss[index] += i
            if index == 0:
                step = 1
            elif index == numRows - 1:
                step = -1
            index += step
        return ''.join(ss)
```

## **7. Reverse Integer**【Easy】

Reverse digits of an integer.

**Example1:** x = 123, return 321

**Example2:** x = -123, return -321

==Note:== 

The input is assumed to be a 32-bit signed integer. Your function should **return 0 when the reversed integer overflows.**

**Solution:**

```python
class Solution(object):
    def reverse(self, x):
        """
        :type x: int
        :rtype: int
        """
        if x > -10 and x < 10:
            return x
        result = int(str(abs(x))[::-1])

        if result > 2147483647:
            return 0
        else:
            if x < 0:
                return -result
            else:
                return result
```

## **8. String to Integer (atoi)**【Medium】

Implement atoi to convert a string to an integer.

**Hint**: Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.

**Notes**: It is intended for this problem to be specified vaguely (ie, no given input specs). You are responsible to gather all the input requirements up front.

**Requirements for atoi:**

The function first discards as many whitespace characters as necessary until the first non-whitespace character is found. Then, starting from this character, takes an optional initial plus or minus sign followed by as many numerical digits as possible, and interprets them as a numerical value.

The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.

If the first sequence of non-whitespace characters in str is not a valid integral number, or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed.

If no valid conversion could be performed, a zero value is returned. If the correct value is out of the range of representable values, INT_MAX (2147483647) or INT_MIN (-2147483648) is returned.

**Solution:**

```python
class Solution(object):
    def myAtoi(self, str):
        """
        :type str: str
        :rtype: int
        """
        str = str.strip()
        str = re.findall('^[+\-]?\d+', str)

        try:
            res = int(''.join(str))
            MAX = 2147483647
            MIN = -2147483648
            if res > MAX:
                return MAX
            if res < MIN: 
                return MIN
            return res
        except: 
            return 0
```

## **9. Palindrome Number**【Easy】

Determine whether an integer is a palindrome. Do this without extra space.

**Solution:**

```python
class Solution(object):
    def isPalindrome(self, x):
        """
        :type x: int
        :rtype: bool
        """
        if str(x) == str(x)[::-1]:
            return True
        else:
            return False
```

## **10. Regular Expression Matching**【Hard】

Implement regular expression matching with support for ‘.’ and ‘*’.

```
'.' Matches any single character.
'*' Matches zero or more of the preceding element.

The matching should cover the entire input string (not partial).

The function prototype should be:
bool isMatch(const char *s, const char *p)

Some examples:
isMatch("aa","a") ? false
isMatch("aa","aa") ? true
isMatch("aaa","aa") ? false
isMatch("aa", "a*") ? true
isMatch("aa", ".*") ? true
isMatch("ab", ".*") ? true
isMatch("aab", "c*a*b") ? true
```

**Solution:**

```python
class Solution(object):
    def isMatch(self, s, p):
        """
        :type s: str
        :type p: str
        :rtype: bool
        """
        lens, lenp = len(s), len(p)
        c = [[False for i in range(lenp+1)] for j in range(lens+1)]
        c[0][0] = True
        for i in range(2,lenp+1):
            c[0][i] = c[0][i-2] and p[i-1] == '*'

        for i in range(1, lens+1):
            for j in range(1, lenp+1):
                if p[j-1] != '*':
                    if p[j-1] == s[i-1] or p[j-1] == '.':
                        c[i][j] = c[i-1][j-1]
                else:
                    c[i][j] = c[i][j-2] or c[i][j-1]
                    if s[i-1] == p[j-2] or p[j-2] == '.':
                        c[i][j] |= c[i-1][j]

        return c[-1][-1]
```

**Discussion:**

使用动态规划的思路：c[i][j]表示s[:i]与p[:j]的匹配情况，具体思路见：[https://discuss.leetcode.com/topic/22948/my-dp-approach-in-python-with-comments-and-unittest](https://discuss.leetcode.com/topic/22948/my-dp-approach-in-python-with-comments-and-unittest)




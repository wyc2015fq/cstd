# leetcode_10. Regular Expression Matching ? 待解决 - wjheha的博客 - CSDN博客
2017年11月02日 21:37:00[wjheha](https://me.csdn.net/wjheha)阅读数：66
Implement regular expression matching with support for ‘.’ and ‘*’.
‘.’ Matches any single character. 
‘*’ Matches zero or more of the preceding element.
The matching should cover the entire input string (not partial).
The function prototype should be: 
bool isMatch(const char *s, const char *p)
Some examples: 
isMatch(“aa”,”a”) → false 
isMatch(“aa”,”aa”) → true 
isMatch(“aaa”,”aa”) → false 
isMatch(“aa”, “a*”) → true 
isMatch(“aa”, “.*”) → true 
isMatch(“ab”, “.*”) → true 
isMatch(“aab”, “c*a*b”) → true
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/regular-expression-matching/description/](https://leetcode.com/problems/regular-expression-matching/description/)
***思路***：

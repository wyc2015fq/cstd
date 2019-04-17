# [LeetCode] Longest Uncommon Subsequence I 最长非共同子序列之一 - Grandyang - 博客园







# [[LeetCode] Longest Uncommon Subsequence I 最长非共同子序列之一](https://www.cnblogs.com/grandyang/p/6666839.html)







Given a group of two strings, you need to find the longest uncommon subsequence of this group of two strings. The longest uncommon subsequence is defined as the longest subsequence of one of these strings and this subsequence should not be any subsequence of the other strings.

A subsequence is a sequence that can be derived from one sequence by deleting some characters without changing the order of the remaining elements. Trivially, any string is a subsequence of itself and an empty string is a subsequence of any string.

The input will be two strings, and the output needs to be the length of the longest uncommon subsequence. If the longest uncommon subsequence doesn't exist, return -1.

Example 1:
Input: "aba", "cdc"
Output: 3
Explanation: The longest uncommon subsequence is "aba" (or "cdc"), 
because "aba" is a subsequence of "aba", 
but not a subsequence of any other strings in the group of two strings. 



Note:
- Both strings' lengths will not exceed 100.
- Only letters from a ~ z will appear in input strings.



这道题是在4月1号出的，有人说这是愚人节最好的礼物，哈哈～这道题只是为了后面那道题做铺垫，不过这题确实简单啊，两个字符串的情况很少，如果两个字符串相等，那么一定没有非共同子序列，反之，如果两个字符串不等，那么较长的那个字符串就是最长非共同子序列，参见代码如下：



```
class Solution {
public:
    int findLUSlength(string a, string b) {
        return a == b ? -1 : max(a.size(), b.size());
    }
};
```




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














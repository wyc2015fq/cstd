# leetcode_438. Find All Anagrams in a String - wjheha的博客 - CSDN博客
2018年02月08日 16:26:37[wjheha](https://me.csdn.net/wjheha)阅读数：57
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Given a string s and a non-empty string p, find all the start indices of p’s anagrams in s.
Strings consists of lowercase English letters only and the length of both strings s and p will not be larger than 20,100.
The order of output does not matter.
Example 1:
Input: 
s: “cbaebabacd” p: “abc”
Output: 
[0, 6]
Explanation: 
The substring with start index = 0 is “cba”, which is an anagram of “abc”. 
The substring with start index = 6 is “bac”, which is an anagram of “abc”. 
Example 2:
Input: 
s: “abab” p: “ab”
Output: 
[0, 1, 2]
Explanation: 
The substring with start index = 0 is “ab”, which is an anagram of “ab”. 
The substring with start index = 1 is “ba”, which is an anagram of “ab”. 
The substring with start index = 2 is “ab”, which is an anagram of “ab”.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/find-all-anagrams-in-a-string/description/](https://leetcode.com/problems/find-all-anagrams-in-a-string/description/)
**思路**：

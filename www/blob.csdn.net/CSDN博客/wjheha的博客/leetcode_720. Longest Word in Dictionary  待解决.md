# leetcode_720. Longest Word in Dictionary ? 待解决 - wjheha的博客 - CSDN博客
2017年11月24日 10:00:09[wjheha](https://me.csdn.net/wjheha)阅读数：158
Given a list of strings words representing an English Dictionary, find the longest word in words that can be built one character at a time by other words in words. If there is more than one possible answer, return the longest word with the smallest lexicographical order.
If there is no answer, return the empty string. 
Example 1: 
Input:  
words = [“w”,”wo”,”wor”,”worl”, “world”] 
Output: “world” 
Explanation:  
The word “world” can be built one character at a time by “w”, “wo”, “wor”, and “worl”. 
Example 2: 
Input:  
words = [“a”, “banana”, “app”, “appl”, “ap”, “apply”, “apple”] 
Output: “apple” 
Explanation:  
Both “apply” and “apple” can be built from other words in the dictionary. However, “apple” is lexicographically smaller than “apply”. 
Note:
All the strings in the input will only contain lowercase letters. 
The length of words will be in the range [1, 1000]. 
The length of words[I] will be in the range [1, 30].
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/longest-word-in-dictionary/description/](https://leetcode.com/problems/longest-word-in-dictionary/description/)
***思路***：

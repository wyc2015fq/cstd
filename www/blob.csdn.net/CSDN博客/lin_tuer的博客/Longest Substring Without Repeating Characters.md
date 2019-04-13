
# Longest Substring Without Repeating Characters - lin_tuer的博客 - CSDN博客


2018年09月19日 22:23:02[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：91


# 最长无重复字符的子串
Given a string, find the length of the longest substring without repeating characters.
Example 1:
Input: “abcabcbb”
Output: 3
Explanation: The answer is “abc”, with the length of 3.
Example 2:
Input: “bbbbb”
Output: 1
Explanation: The answer is “b”, with the length of 1.
Example 3:
Input: “pwwkew”
Output: 3
Explanation: The answer is “wke”, with the length of 3.
– map 的使用
# code
`package leetcode;
import java.util.HashMap;
import java.util.Map;
public class LongestSubstringWithoutDup {
    public int lengthOfLongestSubstring(String s){
        if(s == null || s.length() == 0)
            return 0;
        Map<Character, Integer> map = new HashMap<Character, Integer>();
        int res = 0;
        for (int i = 0, j = 0; j < s.length(); j++){
            // i 保存当前无重复子串的起始下标
            // j 保存当前判断的位置
            if(map.containsKey(s.charAt(j))){
                i = Math.max(map.get(s.charAt(j)), i);
            }
            res = Math.max(res, j - i + 1);
            map.put(s.charAt(j),j + 1);
        }
        return res;
    }
}`


# leetcode_3.Longest Substring Without Repeating Characters - wjheha的博客 - CSDN博客
2017年10月14日 22:25:36[wjheha](https://me.csdn.net/wjheha)阅读数：71标签：[string																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
#### 题目：3. Longest Substring Without Repeating Characters
Given a string, find the length of the longest substring without repeating characters.
Examples:
Given “abcabcbb”, the answer is “abc”, which the length is 3.
Given “bbbbb”, the answer is “b”, with the length of 1.
Given “pwwkew”, the answer is “wke”, with the length of 3. Note that the answer must be a substring, “pwke” is a subsequence and not a substring.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/longest-substring-without-repeating-characters/description/](https://leetcode.com/problems/longest-substring-without-repeating-characters/description/)
#### 思路：这个题目是一个典型的使用窗口法解决，维护一个最长无重复子串的起始位置begin，一个hash表，一个记录长度的maxLength变量。对一个字符串进行遍历，把遍历的字符依次存到一个HashMap中，HashMap同时记录了字符的位置，当遇到重复的字符时，就将第一个重复字符前的字符从hash表中移除，同时更新begin值。最后比较hash表和maxLength的大小，返回maxLength。
```
package dailyAAlgorithm;
import java.util.HashMap;
public class LongestSubstringWithoutRepeatingCharacters_3 {
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        String s = "abcabcbc";
        int length = LongestSubstring(s);
        System.out.println(length);
    }
    private static int LongestSubstring(String s) {
        // TODO Auto-generated method stub
        HashMap<Character,Integer>  h = new HashMap<Character,Integer>();
        int begin = 0;   //最长字符串的起始位置
        int maxLength = 0;
        for(int i = 0; i < s.length(); i++) {
            if(!h.containsKey(s.charAt(i))) {   //不重复就进行存储
                h.put(s.charAt(i), i);
            } else {
                for(int j = begin; j < h.get(s.charAt(i)); j++) {   //对h移除重复字符前的多余元素
                    h.remove(s.charAt(j));
                }
                begin = h.get(s.charAt(i)) + 1;   //对起始位置进行记录
                h.put(s.charAt(i), i);  //给h中重复的字符赋予新的值，相当于擦出了重复的字符
            }
            if(h.size() > maxLength) {  //始终存储最长的无重复子串
                maxLength = h.size();
            }
        }
        return maxLength;
    }
}
```

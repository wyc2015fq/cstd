# Leetcode316——Remove Duplicate Letters - westbrook1998的博客 - CSDN博客





2018年02月19日 19:07:07[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：78








## 题目：

> 
Given a string which contains only lowercase letters, remove duplicate letters so that every letter appear once and only once. You must make sure your result is the smallest in lexicographical order among all possible results.

Example: 

  Given “bcabc” 

  Return “abc”  
Given “cbacdcbc” 

  Return “acdb”  

## 题解：

```
public class Solution {
    public static String removeDuplicateLetters(String s) {

        int[] cnt = new int[26];
        int pos = 0;
        for (int i = 0; i < s.length(); i++)
            cnt[s.charAt(i) - 'a']++;                 //记录每个字母出现的次数
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) < s.charAt(pos))
                pos = i;                              //遍历找出最小字符
            if (--cnt[s.charAt(i) - 'a'] == 0)        //如果该字符只有一个，直接跳出循环，但不是输出他，而是输出此时最小字符
                break;
            //如果此时i字符并不是最小，但只有一个，也跳出，因为他的前面有比他小的字符，而他只有一个，
            //如果此时不跳出、输出（输出他前面的字符），后面再输出，将会导致不按字典的字符排序。
        }
        return s.length() == 0 ? "" : s.charAt(pos) + removeDuplicateLetters(s.substring(pos + 1).replaceAll("" + s.charAt(pos), "")); //这个替换相当于删除pos后面之后出现的charAt(pos)字符
        //递归返回                    //这里是返回charAt(pos)而不是i
    }

    public static void main(String[] args) {
        String s = "cbacdcbc";
        System.out.println(removeDuplicateLetters(s));
        //输出acdb
    }
}
```

随机了一道难度为hard的题目。参考了网上的一些答案，感觉使用递归很巧妙






# [LeetCode] Valid Anagram 验证变位词 - Grandyang - 博客园







# [[LeetCode] Valid Anagram 验证变位词](https://www.cnblogs.com/grandyang/p/4694988.html)







Given two strings *s* and *t*, write a function to determine if *t* is an anagram of *s*.

For example,
*s* = "anagram", *t* = "nagaram", return true.
*s* = "rat", *t* = "car", return false.


**Note:**

You may assume the string contains only lowercase alphabets.



这不算一道难题，核心点就在于使用哈希表映射，我们还是用一个数组来代替哈希表，使用类似方法的题目有[Minimum Window Substring 最小窗口子串](http://www.cnblogs.com/grandyang/p/4340948.html)，[Isomorphic Strings 同构字符串](http://www.cnblogs.com/grandyang/p/4465779.html)，[Longest Substring Without Repeating Characters 最长无重复子串](http://www.cnblogs.com/grandyang/p/4480780.html) 和 [1.1 Unique Characters of a String 字符串中不同的字符](http://www.cnblogs.com/grandyang/p/4648044.html)。我们先判断两个字符串长度是否相同，不相同直接返回false。然后把s中所有的字符出现个数统计起来，存入一个大小为26的数组中，因为题目中限定了输入字符串为小写字母组成。然后我们再来统计t字符串，如果发现不匹配则返回false。 参见代码如下：



```
class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.size() != t.size()) return false;
        int m[26] = {0};
        for (int i = 0; i < s.size(); ++i) ++m[s[i] - 'a'];
        for (int i = 0; i < t.size(); ++i) {
            if (--m[t[i] - 'a'] < 0) return false;
        }
        return true;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













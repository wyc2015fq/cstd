# [CareerCup] 1.1 Unique Characters of a String 字符串中不同的字符 - Grandyang - 博客园







# [[CareerCup] 1.1 Unique Characters of a String 字符串中不同的字符](https://www.cnblogs.com/grandyang/p/4648044.html)







1.1 Implement an algorithm to determine if a string has all unique characters. What if you cannot use additional data structure?



这道题让我们判断一个字符串中是否有重复的字符，要求不用特殊的数据结构，这里应该是指哈希表之类的不让用。像普通的整型数组应该还是能用的，这道题的小技巧就是用整型数组来代替哈希表，在之前[Bitwise AND of Numbers Range 数字范围位相与](http://www.cnblogs.com/grandyang/p/4431646.html)的解法二中也用到过这种方法。由于ASCII表里的基本表共有128个字符，也就是可以用键盘表示出来的，整个表共有256个字符，所以我们只要用一个大小为256的整型数组就可以包含所有的字符，我们遍历输入字符串，对每一个字符都存入到相应位置，并赋值1，如果遇到已经为1的，说明之前出现过该字符，返回false，如果遍历完s，则返回true，代码如下:



```
class Solution {
public:
    bool isUniqueChar(string s) {
        if (s.size() > 128) return false;
        int m[256] = {0};
        for (int i = 0; i < s.size(); ++i) {
            if (m[s[i]] > 0) return false;
            m[s[i]] = 1;
        }
        return true;
    }
};
```



书上还给了另一种解法，是用位操作 Bit Manipulation，但是这种解法只有当输入字符串是由小写字母组成的才成立，因为小写字母只有26个，不超过整型int的32位，对于每个字母，我们将对应位置设为1，然后看之前是否是1，是的话返回false，不是的话设为1。跟上面的方法核心是一样的，只不过空间上省了很多，但是也对输入做了更为严格的限制，代码如下：



```
// Only works when s consists of lower-case letters a-z
class Solution {
public:
    bool isUniqueChar(string s) {
        int m = 0;
        for (int i = 0; i < s.size(); ++i) {
            int d = s[i] - 'a';
            if (m & (1 << d) > 0) return false;
            m |= (1 << d);
        }
        return true;
    }
};
```















# [CareerCup] 1.8 String Rotation 字符串的旋转 - Grandyang - 博客园







# [[CareerCup] 1.8 String Rotation 字符串的旋转](https://www.cnblogs.com/grandyang/p/4653373.html)







1.8 Assume you have a method isSubstring which checks if one word is a substring of another. Given two strings, s1 and s2, write code to check if s2 is a rotation of si using only one call to isSubstring (e.g.,"waterbottle"is a rotation of "erbottlewat").



这道题给定两个字符串，让我们判断其中一个是否是另一个的旋转字符串，并给了我们一个例子来说明旋转字符串，比如waterbottle是erbottlewat的旋转字符串。而且还给我们了一个isSubstring函数可以调用，这个函数是用来判断一个字符串是否是另一个字符串的子字符串，不过规定了我们只能调用一次。这题要用到一个小技巧，就是加入我们将s1重复两次，变成s1s1，那么s2如果是s1s1的子字符串，那么它们就互为旋转字符串，就拿题目中的梯子来分析：

若令 x = wat    y = erbottle

则 s1 = xy    s2 = yx

若令 s1s1 = xyxy

则 s2 一定是 s1s1的子字符串



```
class Solution {
public:
    bool isRotation(string s1, string s2) {
        if (s1.size() != s2.size() || s1.empty()) return false;
        string s1s1 = s1 + s1;
        return isSubstring(s1s1, s2);
    }
};
```















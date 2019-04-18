# LeetCode  Excel Sheet Column Number 表列数 - xcw0754 - 博客园
# [LeetCode  Excel Sheet Column Number 表列数](https://www.cnblogs.com/xcw0754/p/4415993.html)
题意：天啊！我竟然看不懂题意，还去翻别人的代码才懂！给定一个字符串，求该字符串二十六进制的总值。
思路：'A'~'Z'就是1到26，"AA"=26+1=27，"BA"=26*2+1=53，"CBA"=26*26*3+26*2+1。相当于321（十进制），那么就是10*10*3+10*2+1，说第3条式子，C在第3位，在第1位时就是1~26的值了，也就是3，在第2位时就是将3*26了，再第3位时就是将3*26*26了；同理B就是2*26啦，A就是1啦。说到底，就是将一个二十六进制的数转换成十进制来表示，
```
1 class Solution {
2 public:
3     int titleToNumber(string s) {
4         int sum=0, i;
5         for(i=0;i<s.size();i++)
6             sum = sum*26 + (s[i]-'A'+1);
7         return sum;
8     }
9 };
Excel Sheet Column Number
```


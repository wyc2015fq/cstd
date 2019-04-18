# LeetCode Excel Sheet Column Title （输出excel表的列名称） - xcw0754 - 博客园
# [LeetCode Excel Sheet Column Title （输出excel表的列名称）](https://www.cnblogs.com/xcw0754/p/4634600.html)
题意：给一个数字n，输出excel表的列名称。
思路：其实观察可知道，是个26进制的标记而已。那就模拟一下，每次计算一位时就先左移1位，再进行计算。
```
1 class Solution {
 2 public:
 3     string convertToTitle(int n) {
 4         string ans="";
 5         while(n)    //26进制，但是输出时不同而已，是从1开始，而不是0。每次计算将其先左移1位即可。
 6         {
 7             ans+=(n-1)%26+'A';
 8             n=(n-1-(n-1)%26)/26;
 9         }
10         reverse(ans.begin(),ans.end());
11         return  ans;
12     }
13 };
AC代码
```


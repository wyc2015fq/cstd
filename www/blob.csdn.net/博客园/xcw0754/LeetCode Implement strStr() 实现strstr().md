# LeetCode Implement strStr()  实现strstr() - xcw0754 - 博客园
# [LeetCode Implement strStr()  实现strstr()](https://www.cnblogs.com/xcw0754/p/4427224.html)
如题
思路：暴力就行了。1ms的暴力！！！别的牛人写出来的，我学而抄之~
![](https://images0.cnblogs.com/blog2015/641737/201504/142251081355171.png)
```
1 int strStr(char* haystack, char* needle) {
 2         if (!haystack || !needle) return -1;
 3         for (int i = 0; ; ++i) {
 4             for (int j = 0; ; ++j) {
 5                 if (needle[j] == 0) return i;
 6                 if (haystack[i + j] == 0) return -1;
 7                 if (haystack[i + j] != needle[j]) break;
 8             }
 9         }
10 }
strStr()
```


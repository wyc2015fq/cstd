# LeetCode Factorial Trailing Zeroes (阶乘后缀零) - xcw0754 - 博客园
# [LeetCode Factorial Trailing Zeroes (阶乘后缀零)](https://www.cnblogs.com/xcw0754/p/4623222.html)
题意：如标题
思路：其他文章已经写过，参考其他。
```
1 class Solution {
2 public:
3     int trailingZeroes(int n) {
4         return n/5<5? n/5: n/5+trailingZeroes(n/5);
5     }
6 };
AC代码
```


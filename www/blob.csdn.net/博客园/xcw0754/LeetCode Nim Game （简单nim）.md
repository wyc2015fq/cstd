# LeetCode Nim Game （简单nim） - xcw0754 - 博客园
# [LeetCode Nim Game （简单nim）](https://www.cnblogs.com/xcw0754/p/4916542.html)
题意：
　　有一堆石子，里面有n个石头，每次可以从中取出1~3个，两人轮流取，最后一个石子被谁取走即为赢家。你先取，问最后谁赢？
思路：
　　n%4>0则先手赢，因为每次总是可以给对方留4个石子的倍数，而对方最多只能取到3个，剩下的给先手来取，所以先手赢。
C++:
```
1 class Solution {
2 public:
3     bool canWinNim(int n) {
4         return n%4>0;
5     }
6 };
AC代码
```
python:
```
1 class Solution(object):
2     def canWinNim(self, n):
3         """
4         :type n: int
5         :rtype: bool
6         """
7         return bool(n&3) 
AC代码
```


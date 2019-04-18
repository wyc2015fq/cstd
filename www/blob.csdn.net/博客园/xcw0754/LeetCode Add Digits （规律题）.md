# LeetCode Add Digits （规律题） - xcw0754 - 博客园
# [LeetCode Add Digits （规律题）](https://www.cnblogs.com/xcw0754/p/4916722.html)
题意：
　　将一个整数num变成它的所有十进制位的和，重复操作，直到num的位数为1，返回num。
思路：
　　注意到答案的范围是在区间[0,9]的自然数，而仅当num=0才可能答案为0。
　　规律在于随着所给自然数num的递增，结果也是在1~9内循环递增的，那么结果为(num-1)%9+1。
C++： 
```
1 class Solution {
2 public:
3     int addDigits(int num) {
4         if(!num)    return 0;
5         else    return (num-1)%9+1;
6     }
7 };
AC代码
```
python：
```
1 class Solution(object):
2     def addDigits(self, num):
3         """
4         :type num: int
5         :rtype: int
6         """
7         return 0 if not num else (num-1)%9+1
AC代码
```


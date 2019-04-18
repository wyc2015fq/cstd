# LeetCode Power of Two (2的幂) - xcw0754 - 博客园
# [LeetCode Power of Two (2的幂)](https://www.cnblogs.com/xcw0754/p/4637741.html)
题意：判断1个数n是否刚好是2的幂，幂大于0。
思路：注意会给负数，奇数。对于每个数判断31次即可。
```
1 class Solution {
 2 public:
 3     bool isPowerOfTwo(int n) {
 4         if(n<1||(n&1)==1&&n>1)   return false;
 5         unsigned int t=1;
 6         while(t<n) //注意爆int
 7             t<<=1;
 8         if(t==n)    return true;
 9         return false;
10     }
11 };
AC代码
```
一行代码，更巧的写法：
　　如果一个数n是2的某次幂，那么他应该是100000这样的，那么n-1会是0111111这样的，两者相与n&(n-1)必定为0。
```
1 class Solution {
2 public:
3     bool isPowerOfTwo(int n) {
4         return n>0 && !(n&(n-1)) ;//100和011的&应该是0才对
5     }
6 };
AC代码
```


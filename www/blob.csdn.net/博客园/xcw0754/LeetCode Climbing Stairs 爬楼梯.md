# LeetCode Climbing Stairs 爬楼梯 - xcw0754 - 博客园
# [LeetCode Climbing Stairs 爬楼梯](https://www.cnblogs.com/xcw0754/p/4119384.html)
题意：
　　一个人在爬楼梯，此楼梯共有n个台阶，每次可以跨1步或者两步。问有多少种方案走完n个台阶？
思路：
　　当n=1时，返回1；
　　当n=2时，返回2。
　　当n>3时，要返回的是n-1和n-2所要返回的数之和。比如n=3，那么就返回1+2的值，n=4，返回3+2。类推下去。
　　其实这是“斐波那契数列”，通常方法就是递归/迭代。
C++
　　递归
```
1 class Solution {
 2 public:
 3     int climbStairs(int n) {
 4         if(n==1) return 1;
 5         if(n==2) return 2;
 6         int a=1,b=2,c=0,i;
 7         for( i=0;i<n-2 ;i++ ){
 8             c=a+b;
 9             a=b;
10             b=c;
11         }
12         return c;
13     }
14 };
AC代码
```
python3 
　　迭代
```
1 class Solution(object):
 2     def climbStairs(self, n):
 3         """
 4         :type n: int
 5         :rtype: int
 6         """
 7         p=[0,1]
 8         while n:
 9             n-=1
10             p[0],p[1]=p[1],p[0]+p[1]
11         return p[1]
AC代码
```


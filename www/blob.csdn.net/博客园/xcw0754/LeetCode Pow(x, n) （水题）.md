# LeetCode Pow(x, n)  （水题） - xcw0754 - 博客园
# [LeetCode Pow(x, n)  （水题）](https://www.cnblogs.com/xcw0754/p/4976252.html)
题意：
　　求浮点型x的n次幂结果。
思路：
　　logN直接求，注意n可能为负数！！！当n=-2147483648的时候，千万别直接n=-n，这样的结果是多少？其他求法大同小异。
```
1 class Solution {
 2 public:
 3     double myPow(double x, int n) {
 4         if(n==0)    return 1;
 5         int m=n>0?1:-1;
 6         double ans=1;
 7         long long b=(long long)n*m;
 8         while(b)
 9         {
10             if(b&1) ans*=x;
11             x*=x;
12             b>>=1;
13         }
14         if( m==-1 ) ans=1/ans;
15         return ans;
16     }
17 };
AC代码
```


# LeetCode Count Primes  求素数个数（埃拉托色尼筛选法） - xcw0754 - 博客园
# [LeetCode Count Primes  求素数个数（埃拉托色尼筛选法）](https://www.cnblogs.com/xcw0754/p/4603263.html)
![](https://images0.cnblogs.com/blog2015/641737/201506/262126205498304.png)
题意：给一个数n，返回小于n的素数个数。
思路：设数字 k =from 2 to sqrt(n)，那么对于每个k，从k2开始，在[2,n)范围内只要是k的倍数的都删掉（也就是说[k,k2)是不用理的，若能被筛掉早就被筛了，保留下来的就是素数）。最后统计一下[2,n)内有多少个还存在的，都是素数。
　　要注意，如果k已经被筛掉了，那么不用再用它来删别人了，因为已经被筛掉，那么现在比k2大的且是k的倍数，都已经被干掉了。
```
1 class Solution {
 2 public:
 3     int countPrimes(int n) {
 4         bool* isPrime =new bool[n] ;
 5         
 6         memset(isPrime,1,n);
 7         
 8         for(int i=2; i*i<n; i++)
 9         {
10             if(!isPrime[i])    continue;
11             for(int j=i*i; j<n; j+=i)    isPrime[j]=0;
12         }
13         int cnt=0;
14         for(int i=2; i<n; i++)    if(isPrime[i])    cnt++;
15         return cnt;
16     }
17 };
AC代码
```


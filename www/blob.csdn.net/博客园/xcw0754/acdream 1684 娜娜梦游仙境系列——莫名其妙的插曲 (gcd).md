# acdream 1684 娜娜梦游仙境系列——莫名其妙的插曲 (gcd) - xcw0754 - 博客园
# [acdream 1684 娜娜梦游仙境系列——莫名其妙的插曲 (gcd)](https://www.cnblogs.com/xcw0754/p/4660227.html)
题意：一开始有一个集合，集合里有n个不同的数，然后Alice(娜娜)与Bob轮流进行操作，每人都可以任意选择两个数a,b，不妨设a>b，不过要求a-b不在集合中，把a-b放入集合（集合元素个数只增不减）。如果轮到某人，无法进行任何操作，则该人输掉游戏。当Alice（娜娜）与Bob都沿着最优策略进行，娜娜先手，最终谁会获胜？
思路：减来减去的，跟最大公约数GCD差不多。此题没有什么最优的策略，都是平等的。用的也不是博弈知识。
　　最后不能操作的局面一定是｛1g, 2g,3g......xg｝，3g表示3*g，g表示最大公约数。这样的局面就不能操作了，谁遇谁输。经过多少步会产生这样的局面？原序列最大元素big一定不会从集合消失，所以等于xg，那么将这个“死局面”每个元素除以g变成｛1，2，3.....x｝，所以最后局面中有x个元素，而给的序列是n个元素，因每次操作增加一个数，所以功走了x-n步，奇数步为先手赢。
　　总结，这题需要求的就是gcd，big/gcd后再减去n，再判奇偶定结果。
```
1 /*
 2 * this code is made by xcw0754
 3 * Problem: 1684
 4 * Verdict: Accepted
 5 * Submission Date: 2015-07-19 23:45:13
 6 * Time: 0MS
 7 * Memory: 1680KB
 8 */
 9 #include <bits/stdc++.h>
10 using namespace std;
11 const int N=1050;
12 int a[N];
13  
14 int cal(int n)
15 {
16     sort(a,a+n);
17     int tmp=a[0];
18     for(int i=1; i<=n; i++)
19         tmp=__gcd(tmp,a[i]);
20     return tmp;
21 }
22 int main(void)
23 {
24     //freopen("e://input.txt", "r", stdin);
25     int t, n;
26     cin>>t;
27     while(t--)
28     {
29         scanf("%d",&n);
30         int big=0;
31         for(int i=0; i<n; i++)
32         {
33             scanf("%d",&a[i]);
34             big=max(big,a[i]);
35         }
36         int gcd=cal(n);//最大公约数
37         int ans=big/gcd-n;
38         if(ans&1)   puts("Win");
39         else    puts("Lose");
40     }
41     return 0;
42 }
AC代码
```


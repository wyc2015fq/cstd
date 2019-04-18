# POJ 1651 Multiplication Puzzle  （区间DP，经典） - xcw0754 - 博客园
# [POJ 1651 Multiplication Puzzle  （区间DP，经典）](https://www.cnblogs.com/xcw0754/p/4847377.html)
题意：
　　给出一个序列，共n个正整数，要求将区间[2,n-1]全部删去，只剩下a[1]和a[n]，也就是一共需要删除n-2个数字，但是每次只能删除一个数字，且会获得该数字与其旁边两个数字的积的分数，问最少可以获得多少分数？
思路：
　　类似于矩阵连乘的问题，用区间DP来做。
　　假设已知区间[i,k-1]和[k+1,j]各自完成删除所获得的最少分数，那么a[k]是区间a[i,j]内唯一剩下的一个数，那么删除该数字就会获得a[k]*a[i-1]*a[i+1]的分数了。在枚举k的时候要保证[i,j]的任一子区间都已经完成了计算。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x7f7f7f7f
12 #define LL unsigned long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=110;
16 
17 int a[N];
18 LL dp[N][N];
19 
20 int cal(int n)
21 {
22     memset(dp, 0, sizeof(dp));
23     for(int i=1; i<=n; i++)
24     for(int j=i; j<=n; j++)
25         dp[i][j]=INF;
26 
27     for(int j=2; j<n; j++)
28     for(int i=j; i>1; i--)
29     for(int k=i; k<=j; k++)     //枚举区间a[i->j]最后剩下的数字是a[k]
30         dp[i][j]=min(dp[i][j], dp[i][k-1]+dp[k+1][j]+a[k]*a[i-1]*a[j+1]);
31 
32     return dp[2][n-1];
33 }
34 
35 
36 int main()
37 {
38     freopen("input.txt", "r", stdin);
39     int n;
40     while(~scanf("%d",&n))
41     {
42         for(int i=1; i<=n; i++)    scanf("%d",&a[i]);
43         printf("%d\n", cal(n));
44     }
45     return 0;
46 
47 }
AC代码
```


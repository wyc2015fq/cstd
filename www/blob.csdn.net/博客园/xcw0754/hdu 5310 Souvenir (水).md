# hdu 5310 Souvenir (水) - xcw0754 - 博客园
# [hdu 5310 Souvenir (水)](https://www.cnblogs.com/xcw0754/p/4676856.html)
题意：今天是BestCoder一周年纪念日. 比赛管理员Soda想要给每个参赛者准备一个纪念品. 商店里纪念品的单价是元, 同时也可以花元购买纪念品套装, 一个套装里有个纪念品.今天总共有个参赛者, Soda想要知道最少需要花多少钱才可以给每个人都准备一个纪念品.
思路：3种可能之一，要么不买套装，要么全买套装，要么留下一点点单独买其他买套装。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstring>
 4 #include <cstdio>
 5 #include <cmath>
 6 #include <algorithm>
 7 #include <vector>
 8 #include <map>
 9 #include <set>
10 #include <stack>
11 #include <queue>
12 #define LL long long
13 #define pii pair<int,int>
14 #define INF 0x7f7f7f7f
15 using namespace std;
16 const int N=100;
17 
18 int main()
19 {
20     //freopen("input.txt", "r", stdin);
21     int n, m, t, p, q;
22     cin>>t;
23     while(t--)
24     {
25         scanf("%d%d%d%d",&n,&m,&p,&q);
26         int tmp=n/m*q + (n%m)*p;
27         int tmp2=min((n/m+1)*q, tmp);
28         int ans=min(n*p,tmp2);
29         printf("%d\n", ans);
30 
31     }
32     return 0;
33 }
AC代码
```


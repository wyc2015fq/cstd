# HDU  Senior's Gun （水题） - xcw0754 - 博客园
# [HDU  Senior's Gun （水题）](https://www.cnblogs.com/xcw0754/p/4641370.html)
题意：
　　给n把枪，m个怪兽，每把枪可消灭1怪兽，并获得能量=枪的攻击力-怪兽的防御力。求如何射杀能获得最多能量？（不必杀光）
思路：
　　用最大攻击力的枪杀防御力最小的怪兽明显可获得最大能量。如果每把枪都去射杀刚好1点能量都拿不到的怪物，那简直等于把枪全丢掉。
```
1 //#pragma comment(linker,"/STACK:102400000,102400000")
 2 #include <iostream>
 3 #include <stdio.h>
 4 #include <string.h>
 5 #include <vector>
 6 #include <stack>
 7 #include <algorithm>
 8 #include <map>
 9 #include <bits/stdc++.h>
10 #define LL long long
11 #define pii pair<int,int>
12 #define INF 0x7f7f7f7f
13 using namespace std;
14 const int N=100000+5;
15 int a[N], b[N];
16 
17 int main()
18 {
19     //freopen("input.txt", "r", stdin);
20     int t, n, m;
21     cin>>t;
22     while(t--)
23     {
24         scanf("%d%d",&n, &m);
25 
26         for(int i=0; i<n; i++)  scanf("%d",&a[i]);
27         for(int i=0; i<m; i++)  scanf("%d",&b[i]);
28 
29         sort(a, a+n);
30         sort(b, b+m);
31         int bb=0, aa=n-1;
32         LL ans=0;
33         while(aa>=0 && bb<m && a[aa]>b[bb])
34         {
35             ans+=a[aa]-b[bb];
36             bb++;
37             aa--;
38         }
39         printf("%lld\n",ans);
40     }
41 
42 
43     return 0;
44 }
AC代码
```


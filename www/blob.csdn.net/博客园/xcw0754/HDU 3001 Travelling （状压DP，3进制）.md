# HDU 3001 Travelling （状压DP，3进制） - xcw0754 - 博客园
# [HDU 3001 Travelling （状压DP，3进制）](https://www.cnblogs.com/xcw0754/p/4859232.html)
题意：
　　给出n<=10个点，有m条边的无向图。问：可以从任意点出发，至多经过同一个点2次，遍历所有点的最小费用？
思路：
　　本题就是要卡你的内存，由于至多可经过同一个点2次，所以只能用3进制来表示，3进制可以先将表打出来。在走的时候注意只能走2次，其他的和普通的TSP状压DP是一样的。注意点：重边，自环等等，老梗了。
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
11 #define INF 0x3f3f3f3f
12 #define LL long long
13 #define ULL unsigned long long
14 using namespace std;
15 const double PI  = acos(-1.0);
16 const int N=11;
17 int g[N][N], dp[60000][N], up[N], bit[N];
18 
19 int decode(int s)//将状态s解码
20 {
21     int cnt=0;
22     for(int i=1; s; i++)
23     {
24         bit[i]=s%3;
25         s/=3;
26         if(bit[i])  cnt++;
27     }
28     return cnt;
29 }
30 
31 int cal(int n)
32 {
33     memset(dp,0x3f,sizeof(dp));
34     int ans=INF;
35     for(int i=1; i<=n; i++)    dp[up[i-1]][i]=0;//先将每个起点设为0，并不影响结果
36     for(int s=1; s<=up[n]; s++)
37     {
38         int cnt=decode(s);//cnt表示已经遍历的点数
39         for(int i=1; i<=n; i++)   //枚举中间点
40         {
41             if( bit[i]>0 )    //确保已经遍历过i点
42             {
43                 for(int j=1; j<=n; j++)           //枚举终点
44                 {
45                     if( bit[j]==2 )    continue;   //已经走过了2次
46                     bool flag=cnt==n?true:false;//为了更新答案
47                     if( bit[j]==0&&cnt+1==n )   flag=true;
48                     int &q=dp[s+up[j-1]][j];
49                     if( q>dp[s][i]+g[i][j] )    q=dp[s][i]+g[i][j];
50                     if(flag)                    ans=min(ans, q);
51                 }
52             }
53         }
54     }
55     return ans;
56 }
57 
58 void init() //3进制先打表
59 {
60     int a=3;
61     up[0]=1;
62     for(int i=1; i<N; i++)
63     {
64         up[i]=a;
65         a*=3;
66     }
67 }
68 
69 
70 int main()
71 {
72     //freopen("input.txt","r",stdin);
73     init();
74     int n, m, a, b, c;
75     while(~scanf("%d%d",&n,&m)) //每个点至多可走2次
76     {
77         memset(g,0x3f,sizeof(g));
78         for(int i=1; i<=n; i++) g[i][i]=0;
79         for(int i=0; i<m; i++)
80         {
81             scanf("%d%d%d",&a,&b,&c);
82             g[a][b]=g[b][a]=min(g[a][b],c);
83         }
84         if(n==1)
85         {
86             puts("0");
87             continue;
88         }
89         int ans=cal(n);
90         printf("%d\n", ans==INF?-1:ans);
91     }
92     return 0;
93 }
AC代码
```


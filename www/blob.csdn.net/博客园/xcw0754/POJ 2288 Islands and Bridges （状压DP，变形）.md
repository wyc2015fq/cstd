# POJ 2288 Islands and Bridges （状压DP，变形） - xcw0754 - 博客园
# [POJ 2288 Islands and Bridges （状压DP，变形）](https://www.cnblogs.com/xcw0754/p/4859819.html)
题意：
　　给一个无向图，n个点m条边，每个点有点权，要求找到一条哈密顿路径，使得该路径的f(path)值最大。输出f值，若有多条最大f值的路径，输出路径数量。
　　f值由如下3点累加而来：
　　（1）所有点权之和。
　　（2）路径上相邻两点的点权之积。
　　（3）路径上如果有连续的3个点是一个三角形（即3点成环），则累加三点的点权之积。
思路：
　　根据f值的计算方式，第一项基本是不会变的，其他两项与路径有关。由于需要计算第3点，所以状态还需要记录每个状态的最后两个点（有序的），来判断是否为三角形。那么状态表示为[哪些点浏览过][倒数第2个点][末节点]，如果是在刚开始，只有1个点，那么第二维就是0就行了。路径数是根据DP的过程来计算的，开一个和状态数一样的数组保存即可。
　　注意点：路径统计要小心，图可能不连通，可能只有1个点，路径数可能爆LL？
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <cmath>
  6 #include <deque>
  7 #include <map>
  8 #include <algorithm>
  9 #include <vector>
 10 #include <iostream>
 11 #define pii pair<int,int>
 12 #define INF 0x3f3f3f3f
 13 #define LL long long
 14 #define ULL unsigned long long
 15 using namespace std;
 16 const double PI  = acos(-1.0);
 17 const int N=14;
 18 int n, w[N];
 19 LL cnt[1<<13][N][N], dp[1<<13][N][N],ans, ans2;
 20 bool g[N][N];
 21 
 22 void fuck(int s,int i,int k)
 23 {
 24     for(int j=1; j<=n; j++) //枚举终点
 25     {
 26         if( (s&(1<<j-1)) || !g[i][j] )      continue;//已经走过or无路径
 27 
 28         LL c=cnt[s][k][i];
 29         LL v=dp[s][k][i]+w[i]*w[j];
 30         if( g[k][j] )  v+=w[k]*w[i]*w[j];    //三角形出现
 31 
 32         LL &cc=cnt[s|(1<<j-1)][i][j];
 33         LL &dd=dp[s|(1<<j-1)][i][j];
 34         if( dd<=v )
 35         {
 36             if(dd==v)    cc+=c;
 37             else         cc=c;
 38             dd=v;
 39         }
 40     }
 41 }
 42 void cal()
 43 {
 44     memset(dp,-1,sizeof(dp));
 45     memset(cnt,0,sizeof(cnt));
 46     ans=-10086;
 47     ans2=-1;
 48     for(int i=1; i<=n; i++)
 49     {
 50         dp[1<<i-1][0][i]=0;
 51         cnt[1<<i-1][0][i]=1;
 52     }
 53     for(int s=1; s<(1<<n); s++)
 54     {
 55         for(int i=1; i<=n; i++)   //枚举k->i结尾
 56         {
 57             if( ( s&(1<<i-1) )==0 )   continue;
 58             for(int k=0; k<=n; k++)
 59             {
 60                 if( k>0&&(s&(1<<k-1))==0 || k==i ) continue;
 61                 if( dp[s][k][i]<0 )    continue;
 62                 fuck(s,i,k);
 63             }
 64         }
 65     }
 66     for(int i=1; i<=n; i++)   //枚举k->i结尾
 67     {
 68         for(int k=1; k<=n; k++)
 69         {
 70             if(i==k || dp[(1<<n)-1][k][i]<0 ) continue;
 71             if( ans==dp[(1<<n)-1][k][i] )
 72                 ans2+=cnt[(1<<n)-1][k][i];
 73             else if( ans<dp[(1<<n)-1][k][i] )
 74             {
 75                 ans=dp[(1<<n)-1][k][i];
 76                 ans2=cnt[(1<<n)-1][k][i];
 77             }
 78         }
 79     }
 80 }
 81 
 82 
 83 
 84 int main()
 85 {
 86     //freopen("input.txt","r",stdin);
 87     int m, a, b, t, sum;cin>>t;
 88     while(t--)
 89     {
 90         sum=0;
 91         scanf("%d%d",&n,&m);
 92         memset(g,0,sizeof(g));
 93         for(int i=1; i<=n; i++)
 94         {
 95             scanf("%d",&w[i]);
 96             sum+=w[i];
 97         }
 98         for(int i=0; i<m; i++)
 99         {
100             scanf("%d%d",&a,&b);
101             g[a][b]=g[b][a]=true;
102         }
103         if(n==1)    printf("%d 1\n", w[1]);
104         else
105         {
106             cal();
107             if(ans<0)    printf("0 0\n");
108             else printf("%lld %lld\n", ans+sum, ans2/2);
109         }
110     }
111     return 0;
112 }
AC代码
```


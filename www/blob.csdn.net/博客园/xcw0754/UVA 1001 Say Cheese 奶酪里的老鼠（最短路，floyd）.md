# UVA 1001 Say Cheese 奶酪里的老鼠（最短路，floyd） - xcw0754 - 博客园
# [UVA 1001 Say Cheese 奶酪里的老鼠（最短路，floyd）](https://www.cnblogs.com/xcw0754/p/4658569.html)
题意：一只母老鼠想要找到她的公老鼠玩具（cqww？），而玩具就丢在一个广阔的3维空间（其实可以想象成平面）上某个点，而母老鼠在另一个点，她可以直接走到达玩具的位置，但是耗时是所走过的欧几里得距离*10s。还有一种方法，就是靠钻洞，洞是球的，而在洞内怎么走都是不耗时间的。求母老鼠找到她的玩具所耗时？
思路：先要看清楚题意先！尽可能要找到洞，如果洞的半径越大，那么就可以越省时。如果老鼠和玩具都在同个洞上，那么不耗时即可找到。
　　其实就是求单源最短路，只是计算两点间的长度时要考虑到半径的。而且得注意两洞相连的情况，那么在两洞之间切换不需要时间。输出注意要用round(double)。
下面是dijkstra实现，比较快：
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define pdi pair<double,int>
 5 #define INF 0x7f7f7f7f
 6 using namespace std;
 7 const int N=110;
 8 int x[N], y[N], z[N], r[N];
 9 double dist[N][N];
10 double sdist[N];
11 int vis[N];
12 
13 LL dijkstra(int n)
14 {
15     memset(vis,0,sizeof(vis));
16     for(int i=0; i<=n; i++) sdist[i]=1e29;
17     priority_queue<pdi,vector<pdi>,greater<pdi> >   que;
18     que.push(make_pair(0.0, 0));
19     sdist[0]=0.0;
20     while(!que.empty())
21     {
22         int x=que.top().second;que.pop();
23         if(vis[x]) continue;
24         vis[x]=1;
25         for(int i=0; i<=n; i++)
26         {
27             if( sdist[i]>sdist[x]+dist[x][i] )
28             {
29                 sdist[i]=sdist[x]+dist[x][i];
30                 que.push(make_pair(sdist[i],i));
31             }
32         }
33     }
34     return round(sdist[n]*10)+0.5;
35 }
36 
37 
38 
39 int main()
40 {
41     freopen("input.txt", "r", stdin);
42     int n;
43     int  j=0;
44     while(scanf("%d", &n), n>=0)
45     {
46         memset(r,0,sizeof(r));
47         for(int i=1; i<=n; i++) scanf("%d %d %d %d",&x[i], &y[i], &z[i], &r[i]);
48 
49         scanf("%d %d %d",&x[0],&y[0],&z[0]);        //起点
50         scanf("%d %d %d",&x[n+1],&y[n+1],&z[n+1]);  //终点
51 
52         //求两两之间的距离
53         for(int i=0; i<=n+1; i++)
54         {
55             for(int j=0; j<=n+1; j++)
56             {
57                 dist[i][j]=0.0;
58                 if(j!=i)
59                 {
60                     dist[i][j]=sqrt( pow(x[i]-x[j],2)+pow(y[i]-y[j],2)+pow(z[i]-z[j],2) )-r[i]-r[j];
61                     if(dist[i][j]<1e-7)  dist[i][j]=0;
62                 }
63             }
64         }
65 
66         printf("Cheese %d: Travel time = %lld sec\n", ++j, dijkstra(n+1));
67 
68     }
69     return 0;
70 }
AC代码
```
下面是floyd实现，比较短：
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=110;
 7 int x[N], y[N], z[N], r[N];
 8 double dist[N][N];
 9 
10 int main()
11 {
12     freopen("input.txt", "r", stdin);
13     int n;
14     int  j=0;
15     while(scanf("%d", &n), n>=0)
16     {
17         memset(r,0,sizeof(r));
18         for(int i=1; i<=n; i++) scanf("%d %d %d %d",&x[i], &y[i], &z[i], &r[i]);
19 
20         scanf("%d %d %d",&x[0],&y[0],&z[0]);        //起点
21         scanf("%d %d %d",&x[n+1],&y[n+1],&z[n+1]);  //终点
22 
23         //求两两之间的距离
24         for(int i=0; i<=n+1; i++)
25         {
26             for(int j=0; j<=n+1; j++)
27             {
28                 dist[i][j]=0.0;
29                 if(j!=i)
30                 {
31                     dist[i][j]=sqrt( pow(x[i]-x[j],2)+pow(y[i]-y[j],2)+pow(z[i]-z[j],2) )-r[i]-r[j];
32                     if(dist[i][j]<1e-7)  dist[i][j]=0;
33                 }
34             }
35         }
36         //floyd
37         for(int k=0; k<=n+1; k++)
38             for(int i=0; i<=n+1; i++)
39                 for(int j=0; j<=n+1; j++)
40                     dist[i][j]=min(dist[i][j],dist[i][k]+dist[k][j]);
41         printf("Cheese %d: Travel time = %lld sec\n", ++j, (LL)(round(dist[0][n+1]*10)+0.5) );
42 
43     }
44     return 0;
45 }
AC代码
```


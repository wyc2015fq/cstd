# HDU 2544 最短路 （最短路，spfa） - xcw0754 - 博客园
# [HDU 2544 最短路 （最短路，spfa）](https://www.cnblogs.com/xcw0754/p/4611515.html)
题意：中文题目
思路：spfa+SLF优化。[关于SPFA的详情请戳我](http://www.nocow.cn/index.php/SPFA%E7%AE%97%E6%B3%95)
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=105, INF=0x7f7f7f7f;
 4 int n, m;
 5 bool vis[N];    //标记是否在队列中
 6 int dest[N];    //路径长度
 7 int mapp[N][N];
 8 
 9 
10 
11 int spfa(int s, int e) //求最短路径
12 {
13     memset(dest,0x7f,sizeof(dest));
14     dest[s]=0;
15     vis[s]=1;
16     deque<int> que;
17     que.push_back(s);
18     while(!que.empty())
19     {
20         int tmp=que.front();
21         que.pop_front();
22         vis[tmp]=0;
23         for(int i=1; i<=n; i++)
24         {
25             if(mapp[tmp][i] && dest[tmp]+mapp[tmp][i]<dest[i] )
26             {
27                 dest[i]=dest[tmp]+mapp[tmp][i];
28                 if(!vis[i]) //一旦被更新，必须进队列
29                 {
30                     vis[i]=1;
31                     if(que.empty())    que.push_back(i);        //SLF优化，路短的优先放前面
32                     else
33                     {
34                         if(dest[que.front()]>dest[i])
35                             que.push_front(i);
36                         else
37                             que.push_back(i);
38                     }
39                 }
40             }
41         }
42     }
43     return dest[e];
44 }
45 
46 
47 
48 
49 int main()
50 {
51     //freopen("input.txt", "r", stdin);
52     int a, b, c;
53     while(scanf("%d%d", &n, &m), n+m)
54     {
55         memset(vis,0,sizeof(vis));
56         memset(mapp,0,sizeof(mapp));
57 
58         for(int i=0; i<m; i++)
59         {
60             scanf("%d%d%d",&a,&b,&c);
61             mapp[a][b]=mapp[b][a]=c;
62         }
63         printf("%d\n",spfa(1,n));
64     }
65 
66     return 0;
67 }
AC代码
```
Dijkstra
```
1 #include <bits/stdc++.h>
 2 #define   LL long long
 3 using namespace std;
 4 const int N=100+5;
 5 const int INF=0x7f7f7f7f;
 6 
 7 int g[N][N];
 8 int vis[N];
 9 int dist[N];
10 
11 int dijkstra(int n)
12 {
13     memset(vis,0,sizeof(vis));
14     memset(dist,0x7f,sizeof(dist));
15     dist[1]=0;
16     for(int i=1; i<=n; i++)    //每次用1个点来更新到其他点的距离
17     {
18         int flag, small=INF;
19         for(int j=1; j<=n; j++)//找距离最小的点
20         {
21             if(!vis[j]&&dist[j]<=small)
22             {
23                 small=dist[j];
24                 flag=j;
25             }
26         }
27         vis[flag]=1;//标记，不需要再用其更新别人。
28         for(int j=1; j<=n; j++)//有边相连就得更新
29         {
30             if(g[j][flag])
31                 dist[j]=min(dist[flag]+g[flag][j], dist[j]);
32         }
33     }
34     return dist[n];
35 }
36 
37 
38 
39 int main()
40 {
41     freopen("input.txt", "r", stdin);
42     int n, m, a, b, w;
43     while(scanf("%d%d",&n, &m), n+m)
44     {
45         memset(g,0,sizeof(g));
46         for(int i=0; i<m; i++)
47         {
48             scanf("%d%d%d",&a,&b,&w);
49             g[a][b]=g[b][a]=w;
50         }
51         cout<<dijkstra(n)<<endl;
52     }
53     return 0;
54 }
AC代码
```


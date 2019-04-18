# HDU 1233 还是畅通工程(最小生成树，prim) - xcw0754 - 博客园
# [HDU 1233 还是畅通工程(最小生成树，prim)](https://www.cnblogs.com/xcw0754/p/4608334.html)
题意：中文题目
思路：prim实现，因为有n*(n-1)/2条边，已经是饱和的边了，prim比较合适。
（1）将点1置为浏览过，点1可以到达其他每个点，所以用low[i]数组记录下目前到达i点的最小长度。
（2）在low数组中找出到达未浏览过的点且距离最近的，置为浏览过，记该店为pos。
（3）将pos点可达的所有点，来更新low数组，使得从已浏览过的点到达i点的距离最短为low[i]。
（4）返回到2继续执行，直到所有的点都浏览过。
在第2步时就可以顺便记录下最小路径长度了。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=105;
 4 int v[N][N];    //权
 5 int vis[N];
 6 int low[N];     //到每个点的最小权
 7 
 8 
 9 int prim(int n)//普里姆算法
10 {
11     memset(vis,0,sizeof(vis));
12     int pos=vis[1]=1;   //从点1开始
13     for(int i=1; i<=n; i++) if(v[1][i]>0)   low[i]=v[1][i]; //目前到每个点的最小权
14     int ans=0;
15     for(int i=1; i<n; i++)  //搞定另外n-1个点
16     {
17         int big=LONG_MAX;
18         for(int j=1; j<=n; j++) //找权最小的边，及对应的点
19         {
20             if(!vis[j] && low[j]<big )
21             {
22                 pos=j;
23                 big=low[j];
24             }
25         }
26         ans+=big;//统计路径长
27         vis[pos]=1;
28         for( int j=1; j<=n; j++ )//更新到每个点的权值
29             if(!vis[j])    low[j]=min(low[j],v[pos][j]);
30     }
31     return ans;
32 }
33 
34 
35 
36 
37 int main()
38 {
39     freopen("input.txt", "r", stdin);
40     int n, a, b, t;
41     while(scanf("%d",&n),n>0)
42     {
43         int up=n*(n-1)/2;//超稠密图
44         for(int i=0; i<up; i++)
45         {
46             scanf("%d%d%d",&a,&b,&t);
47             v[a][b]=v[b][a]=t;
48         }
49         printf("%d\n",prim(n));
50     }
51     return 0;
52 }
AC代码
```


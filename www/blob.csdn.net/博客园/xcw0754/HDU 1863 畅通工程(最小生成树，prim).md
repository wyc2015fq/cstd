# HDU 1863 畅通工程(最小生成树，prim) - xcw0754 - 博客园
# [HDU 1863 畅通工程(最小生成树，prim)](https://www.cnblogs.com/xcw0754/p/4608439.html)
题意：
　　给出图的边和点数，要求最小生成树的代价，注：有些点之间是不可达的，也就是可能有多个连通图。比如4个点，2条边:1-2，3-4。
思路：
　　如果不能连通所有的点，就输出‘?’。之前以为每个点只要有边连着就一定能生成树，其实还可以是每个点虽然有边可达，但是给的其实是两个图，比如上例。其他按照常规Prim。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=105;
 4 const int mod=0x7f7f7f7f;
 5 int v[N][N];    //权
 6 int vis[N];
 7 int low[N];     //到每个点的最小权
 8 
 9 
10 int prim(int n)
11 {
12     int pos=vis[1]=1;   //从点1开始
13     for(int i=2; i<=n; i++)   low[i]=v[1][i]; //目前到每个点的最小权
14     int ans=0;
15     for(int i=1; i<n; i++)  //搞定另外n-1个点
16     {
17         int big=mod;
18         for(int j=2; j<=n; j++) //找权最小的边
19         {
20             if(!vis[j] && low[j]<big )  //未浏览过，目前可达，权小
21             {
22                 pos=j;
23                 big=low[j];
24             }
25         }
26         if(big==mod)    return 0;   //无法到达。
27         ans+=big;
28         vis[pos]++; //浏览过
29         for( int j=2; j<=n; j++ )   //更新到每个点的权值
30             if(!vis[j] && v[pos][j]<mod && low[j]>v[pos][j] ) low[j]=v[pos][j];    //未浏览过，有路可达,更短
31     }
32     return ans;
33 }
34 
35 
36 
37 
38 int main()
39 {
40     freopen("input.txt", "r", stdin);
41     int n, m, a, b, t;
42     while(scanf("%d%d", &n, &m), n)
43     {
44 
45         memset(cnt,0,sizeof(cnt));
46         memset(vis,0,sizeof(vis));
47         memset(v,0x7f,sizeof(v));   //置为不可达
48 
49         for(int i=0; i<n; i++)
50         {
51             scanf("%d%d%d",&a,&b,&t);
52             v[a][b]=v[b][a]=t;
53         }
54         int ans=prim(m);
55         if(ans)    printf("%d\n",ans);
56         else  printf("?\n");
57     }
58     return 0;
59 }
AC代码
```


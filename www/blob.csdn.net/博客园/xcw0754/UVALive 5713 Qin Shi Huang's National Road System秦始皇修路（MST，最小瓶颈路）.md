# UVALive 5713 Qin Shi Huang's National Road System秦始皇修路（MST，最小瓶颈路） - xcw0754 - 博客园
# [UVALive 5713 Qin Shi Huang's National Road System秦始皇修路（MST，最小瓶颈路）](https://www.cnblogs.com/xcw0754/p/4713144.html)
题意：
　　秦始皇要在n个城市之间修路，而徐福声可以用法术位秦始皇免费修1条路，每个城市还有人口数，现要求徐福声所修之路的两城市的人口数之和A尽量大，而使n个城市互通需要修的路长B尽量短，从而使得A/B最大。问A/B最大是多少？（1000个城市）
思路：
　　老徐可免费修得1条路，那么剩下最多也只需要修n-2条路了，这n-2条路要尽量挑短的，而老徐的那条无所谓长短，只要两城人口尽量多即可。这是没有什么贪心策略的，因为老徐所修之路会影响MST的权值之和的大小。穷举所有城市对要O(n*n)，再求次MST需要O(n*n)，不可行。
　　换个思路，如果能先求得MST，然后穷举要老徐所要修的路，那么在加上老徐的路之后，必然会有个环的出现，这个环中有一条边是不需要的，当然不是老徐那条。这只需要在原MST中求这个环的最小瓶颈路就行了，将其删掉，加上老徐的路，构成新的MST了，进行求值。穷举老徐所要修的路也要O(n*n)，那么求瓶颈路就只能用O(1)了。这可以预处理出任意城市对之间的最小瓶颈路，O(n*n)而已。
　　任意点对的最小瓶颈路的求法：对原图求最小生成树，只留下树边，树中任意点对之间的路径就是该点对的最小瓶颈路。接着对树图进行DFS，在DFS过程中，顺便求出任意点对的最小瓶颈路，考虑求当前节点x到其他点的最小瓶颈路，设其父亲far，那么x可以通过far到达前面已经访问过的节点，为maxcost[已访问过的节点][far]与cost[far][x]其中的大者。按此思路，在DFS过程中可以求出任意点对的最小瓶颈路。
```
1 #include <bits/stdc++.h>
  2 #include <cstdio>
  3 #include <vector>
  4 #include <iostream>
  5 #include <algorithm>
  6 #define INF 0x7f7f7f7f
  7 #define pii pair<int,int>
  8 #define LL long long
  9 using namespace std;
 10 const int N=1001000;
 11 int a[N], b[N], seq[N];     //求MST用的
 12 int x[N], y[N], p[N];       //所给的坐标及人口数
 13 int pre[N], vis[N], used[N];    //求任意点对最小瓶颈路用的
 14 double w[N], maxcost[1001][1001]; //两点间的最小瓶颈maxcost
 15 vector<int>  vect[N];    //建树时用
 16 vector<int> dfn;         //记录访问过的节点
 17 
 18 int cmp(int a,int b){return w[a]<w[b];}
 19 int find(int x){return pre[x]==x? x: pre[x]=find(pre[x]);}  //并查集
 20 double dis(int a,int b){return sqrt((x[a]-x[b])*(x[a]-x[b])+(y[a]-y[b])*(y[a]-y[b]));}
 21 
 22 
 23 
 24 
 25 void DFS(int x)
 26 {
 27     dfn.push_back(x);   //访问过
 28     vis[x]=1;
 29     for(int i=0; i<vect[x].size(); i++)
 30     {
 31         int t=vect[x][i];
 32         if(!vis[t] )
 33         {
 34             for(int j=0; j<dfn.size(); j++) //对于所有已经访问过的节点
 35             {
 36                 int from=dfn[j];
 37                 maxcost[t][from]=maxcost[from][t]=max(maxcost[from][x], dis(x, t) );//通过x连到t
 38             }
 39             DFS(t);
 40         }
 41     }
 42 }
 43 
 44 void init(int n)    //一堆初始化。
 45 {
 46     dfn.clear();
 47     for(int i=1; i<=n; i++) vect[i].clear(),pre[i]=i;
 48     for(int i=1; i<=n; i++)
 49         for(int j=1; j<=n; j++)
 50             maxcost[i][j]=0.0;
 51     memset(used, 0, sizeof(used));
 52     memset(vis,  0, sizeof(vis));
 53 }
 54 double cal(int n, int m)
 55 {
 56     init(n);
 57     double sum=0.0;   //MST
 58     for(int i=0; i<m; i++)      //kruscal求最小生成树
 59     {
 60         int u=find(a[seq[i]]);
 61         int v=find(b[seq[i]]);
 62         if( u!=v )
 63         {
 64             pre[u]=v;               //不是同个连通块，则连接。
 65             vect[a[seq[i]]].push_back( b[seq[i]] ); //顺便建图，方便建树
 66             vect[b[seq[i]]].push_back( a[seq[i]] );
 67             used[seq[i]]=1;
 68             sum+=w[seq[i]];
 69         }
 70     }
 71 
 72     DFS(1);                   //求任意点对间的最小瓶颈路
 73     double ans=0.0;
 74     for(int i=0; i<m; i++)    //穷举徐福声将要建的边。
 75     {
 76         double A=p[a[i]]+p[b[i]], B;
 77 
 78         if(used[i])     B=sum-w[i];            //树上的边
 79         else            B=sum-maxcost[a[i]][b[i]];
 80         ans=max( A/B, ans );
 81     }
 82     return ans;
 83 }
 84 
 85 
 86 int main()
 87 {
 88     freopen("input.txt", "r", stdin);
 89     int t, n;
 90     cin>>t;
 91     while(t--)
 92     {
 93         cin>>n;
 94         for(int i=1; i<=n; i++)    scanf("%d%d%d",&x[i],&y[i],&p[i]);
 95         int cnt=0;
 96         for(int i=1; i<=n; i++)     //求两点间的距离,共n*(n-1)/2条边
 97         {
 98             for(int j=i+1; j<=n; j++)
 99             {
100                 a[cnt]=i;
101                 b[cnt]=j;
102                 w[cnt]=dis(i,j);
103                 seq[cnt]=cnt;   //千万不要用seq[cnt]=cnt++;或者seq[cnt++]=cnt。
104                 cnt++;
105             }
106         }
107         sort(seq, seq+cnt, cmp);        //按边长排序
108         printf("%.2f\n", cal(n, cnt));
109     }
110 
111     return 0;
112 }
AC代码
```


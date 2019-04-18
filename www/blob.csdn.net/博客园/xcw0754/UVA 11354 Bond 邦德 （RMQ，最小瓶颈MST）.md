# UVA 11354 Bond 邦德 （RMQ，最小瓶颈MST） - xcw0754 - 博客园
# [UVA 11354 Bond 邦德 （RMQ，最小瓶颈MST）](https://www.cnblogs.com/xcw0754/p/4714946.html)
题意：
　　n个城市，m条路，每条路有个危险值，要使得从s走到t的危险值最小。回答q个询问，每个询问有s和t，要求输出从s到t最小的危险值。（5万个点，10万条边）
思路：
　　其实要求的是任意点对之间的最小瓶颈路的权值。
　　先对图求一次MST，那么所有的瓶颈路都在上面。但是q<=5万，即使预先求出所有点对，也需要O(n*n)，太大了。如果对于每个询问才来找答案，这又更慢了。所以得优化。
　　优化方案（1）：先求出生成树，对于每次询问的两个点，求一次LCA，两个点到LCA所经过的边其中一条边就是答案。很不幸，如果树很稀疏，树枝又很长，那么每次求LCA相当于O(n)的复杂度。
　　优化方案（2）：鉴于方案（1），要解决的是树枝很长的情况，由于要求的是区间最值，那么类似于RMQ问题了，只是在树上而不是序列中而已。假如将某个点p到其LCA之间的一条链作为一个区间的话，那么这段区间的最值，就是p到(p+LCA)/2与(p+LCA)/2到LCA两个区间的最值再取最值。这又变成了类似于ST算法了。
　　具体做法可以这样：
　　（1）预处理每个节点p到p+1,p+2...直到树根的这段区间的最值，那么得到m[p,p+1]，m[p,p+2]，m[p,p+4]...。大小是以2的幂为单位的。若某个点到树根的距离并不是刚好为2的整数次幂，那么这段放弃掉，因为总有点到树根的距离是恰好满足的，那么必要时，找这个区间即可。
　　（2）在查询时，两节点分别到他们的LCA所经过的边不断更新答案。
　　倍增算法：此代码用于优化求LCA的速度，以及从p到LCA这段区间的最值。保证在任何树的情况下都是O(logn)。
```
1 void preprocess(int n)//预处理2的幂次大小的区间内的最值
 2 {
 3     for(int i=1; i<=n; i++) //初始化，及先记录到父亲的距离
 4     {
 5         anc[i][0]=far[i];
 6         maxcost[i][0]=cost[i][far[i]]; 
 7         for(int j=1; (1<<j)<n; j++)  
 8             anc[i][j]=-1;
 9     }
10     for(int j=1; (1<<j)<n; j++) 
11     {
12         for(int i=1; i<=n; i++ )
13         {
14             int e=anc[i][j-1];
15             if(e>0)
16             {
17                 anc[i][j]=anc[e][j-1];      //将2的j次幂，分成两个2的j-1次来组成。
18                 maxcost[i][j]=max(maxcost[i][j-1],maxcost[e][j-1]);
19             }
20         }
21     }
22 }
23 
24 
25 
26 int query(int p,int q)  //先找LCA，再求答案
27 {
28     int tmp, log, i;
29     if(level[p]<level[q])   swap(p,q);//保持p更深一些
30 
31     for(log=1; (1<<log)<=level[p]; log++);//统计最深要几层
32     --log;
33     int ans=-1;
34     for(int i=log; i>=0; i--)//将p提到与q同层
35     {
36         if(level[p]-(1<<i)>=level[q])//这一步关键，只要多于level[q]的部分都会被抹去
37         {
38             ans=max(ans, maxcost[p][i]);
39             p=anc[p][i];
40         }
41     }
42 
43     if(p==q)    return ans;//LCA为q
44 
45     for(int i=log; i>=0; i--)//同时往LCA方向更新，直到成为LCA的孩子
46     {
47         if(anc[p][i]>0 && anc[p][i]!=anc[q][i] )
48         {
49             ans=max(ans, maxcost[p][i]);
50             p=anc[p][i];
51             ans=max(ans, maxcost[q][i]);
52             q=anc[q][i];
53         }
54     }
55     ans=max(ans, cost[p][far[p]]);//还有一层，别忘了
56     ans=max(ans, cost[q][far[q]]);
57 
58     return ans;
59 }
主要优化代码注释
```
```
1 #include <bits/stdc++.h>
  2 #define INF 0x7f7f7f7f
  3 #define pii pair<int,int>
  4 #define LL long long
  5 using namespace std;
  6 const int N=100010;
  7 int a[N], b[N], w[N], seq[N];     //求MST用的
  8 int pre[N], level[N], far[N], maxcost[N][100], anc[N][100];
  9 int vis[N];
 10 vector<int>  vect[N];    //建树时用
 11 map<int,int> cost[N];
 12 
 13 int cmp(int a,int b){return w[a]<w[b];}
 14 int find(int x){return pre[x]==x? x: pre[x]=find(pre[x]);}  //并查集
 15 
 16 
 17 
 18 void DFS(int x,int flo)
 19 {
 20     vis[x]=1;
 21     level[x]=flo;  //记录在哪一层
 22     for(int i=0; i<vect[x].size(); i++)
 23     {
 24         int t=vect[x][i];
 25         if(!vis[t])
 26         {
 27             far[t]=x;  //用孩子索引父亲
 28             DFS(t, flo+1);
 29         }
 30     }
 31 }
 32 
 33 void preprocess(int n)
 34 {
 35     for(int i=1; i<=n; i++) //对于每个点
 36     {
 37         anc[i][0]=far[i];
 38         maxcost[i][0]=cost[i][far[i]]; //与父亲的距离。
 39         for(int j=1; (1<<j)<n; j++)    //初始化祖先为-1
 40             anc[i][j]=-1;
 41     }
 42     for(int j=1; (1<<j)<n; j++)     //仅需到n-1即可。
 43     {
 44         for(int i=1; i<=n; i++ )
 45         {
 46             int e=anc[i][j-1];
 47             if(e>0)
 48             {
 49                 anc[i][j]=anc[e][j-1];      //将2的j次幂，分成两个2的j-1次来组成。
 50                 maxcost[i][j]=max(maxcost[i][j-1],maxcost[e][j-1]);
 51             }
 52         }
 53     }
 54 }
 55 
 56 
 57 int cal(int n,int m)
 58 {
 59     memset(maxcost, 0, sizeof(maxcost));
 60     memset(far, 0,   sizeof(far));
 61     memset(level, 0,  sizeof(level));
 62     memset(anc, 0x8f, sizeof(anc));
 63     //kruscal
 64     for(int i=0; i<=n; i++)
 65     {
 66         pre[i]=i;
 67         cost[i].clear();
 68         vect[i].clear();
 69     }
 70     for(int i=0; i<m; i++)
 71     {
 72         int u=find( a[seq[i]] );
 73         int v=find( b[seq[i]] );
 74         if(u!=v)
 75         {
 76             pre[u]=v;
 77             vect[a[seq[i]]].push_back(b[seq[i]]);
 78             vect[b[seq[i]]].push_back(a[seq[i]]);
 79             cost[a[seq[i]]][b[seq[i]]]=w[seq[i]];   //因为目前还不知道谁是父亲
 80             cost[b[seq[i]]][a[seq[i]]]=w[seq[i]];
 81         }
 82     }
 83     //dfs
 84     memset(vis, 0, sizeof(vis));
 85     DFS(1, 0);
 86     preprocess(n);
 87     return 0;
 88 }
 89 
 90 
 91 
 92 int query(int p,int q)  //先找LCA，再求答案
 93 {
 94     int tmp, log, i;
 95     if(level[p]<level[q])   swap(p,q);
 96 
 97     for(log=1; (1<<log)<=level[p]; log++);
 98     --log;
 99     int ans=-1;
100     for(int i=log; i>=0; i--)
101     {
102         if(level[p]-(1<<i)>=level[q])
103         {
104             ans=max(ans, maxcost[p][i]);
105             p=anc[p][i];
106         }
107     }
108 
109     if(p==q)    return ans;
110 
111     for(int i=log; i>=0; i--)
112     {
113         if(anc[p][i]>0 && anc[p][i]!=anc[q][i] )
114         {
115             ans=max(ans, maxcost[p][i]);
116             p=anc[p][i];
117             ans=max(ans, maxcost[q][i]);
118             q=anc[q][i];
119         }
120     }
121     ans=max(ans, cost[p][far[p]]);
122     ans=max(ans, cost[q][far[q]]);
123     return ans;
124 }
125 
126 int main()
127 {
128     freopen("input.txt", "r", stdin);
129     int n, m, q, x, y, k=0;
130 
131     while(~scanf("%d%d",&n,&m))
132     {
133         if(k!=0)  printf("\n");//格式
134         k++;
135         for(int i=0; i<m; i++)
136         {
137             seq[i]=i;   //边号
138             scanf("%d%d%d", &a[i], &b[i], &w[i]);
139         }
140         sort(seq, seq+m, cmp);
141         cal(n, m);
142         scanf("%d",&q);
143         for(int i=0; i<q; i++)
144         {
145             scanf("%d%d",&x,&y);
146             printf("%d\n", query(x,y));
147         }
148     }
149     return 0;
150 }
AC代码
```


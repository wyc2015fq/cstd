# UVA 11090 Going in Cycle!! 环平均权值（bellman-ford，spfa，二分） - xcw0754 - 博客园
# [UVA 11090 Going in Cycle!! 环平均权值（bellman-ford，spfa，二分）](https://www.cnblogs.com/xcw0754/p/4680654.html)
题意：
　　给定一个n个点m条边的带权有向图，求平均权值最小的回路的平均权值？
思路：
　　首先，图中得有环的存在才有解，其次再解决这个最小平均权值为多少。一般这种就是二分猜平均权值了，因为环在哪也难以找出来，还有可能是一条边属于多个环。对于每个猜到的平均值，如果对应环的存在，那么这个环的每条边的权减去这个平均值之后，用spfa算法就能判断其是否有环的存在即可。
　　假设环上各边权值为：w1+w2+...+wk。
　　式子：w1+w2+...+wk<k*even   相当于   (w1-even)+(w2-even)+...(wk-even)< 0。即更新完边权后应i该是有环存在的。
　　对于猜测的平均权值mid，如果不能找到环，则说明mid应该更大。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=60606;
 7 vector<int> vect[N];
 8 struct node
 9 {
10     int from,to;
11     double cost;
12     node(){};
13     node(int from,int to,int cost):from(from),to(to),cost(cost){};
14 }edge[N];
15 int edge_cnt;
16 int big, small;
17 void add_node(int from,int to,double cost)
18 {
19     edge[edge_cnt]=node(from, to, cost);
20     vect[from].push_back(edge_cnt++);
21 }
22 
23 int inq[N], cnt[N];
24 double dist[N];
25 bool spfa(int n, double q)
26 {
27     memset(inq,0,sizeof(inq));
28     memset(cnt,0,sizeof(cnt));
29     deque<int> que;
30     for(int i=1; i<=n; i++)    dist[i]=0.0, inq[i]=1, que.push_back(i); //因为是判断负环的，所以dist初始化为0即可。
31     while(!que.empty())
32     {
33         int x=que.front();que.pop_front();
34         inq[x]=0;
35         for(int i=0; i<vect[x].size(); i++)
36         {
37             node e=edge[vect[x][i]];
38             if(dist[e.to]>dist[x]+e.cost-q  )
39             {
40                 dist[e.to]=dist[x]+e.cost-q ;
41                 if(!inq[e.to])
42                 {
43                     inq[e.to]=1;
44                     que.push_back(e.to);
45                     if(++cnt[e.to]>n)
46                         return true;
47                 }
48             }
49         }
50     }
51     return false;
52 }
53 
54 double cal(int n)
55 {
56     double l=small, r=big, ans=0.0;
57     while(r-l>1e-3)
58     {
59         double mid=(l+r)/2;
60         if( spfa(n, mid) )  r=mid;    //有负环
61         else               l=mid;
62     }
63     return l;
64 }
65 
66 int main()
67 {
68    // freopen("input.txt", "r", stdin);
69     int n, m, t, a, b, c, j=0;
70     cin>>t;
71     while(t--)
72     {
73         scanf("%d%d",&n,&m);
74         edge_cnt=0;
75         for(int i=0; i<=n; i++)    vect[i].clear();
76         memset(edge,0,sizeof(edge));
77         big=0;
78         small=INF;
79 
80         for(int i=0; i<m; i++)
81         {
82             scanf("%d%d%d",&a,&b,&c);
83             add_node(a,b,c);
84             small=min(small,c);
85             big=max(big, c);
86         }
87         if( !spfa(n, big+1) ) printf("Case #%d: No cycle found.\n", ++j);
88         else    printf("Case #%d: %.2f\n", ++j, cal(n));
89     }
90     return 0;
91 }
AC代码
```


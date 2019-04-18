# HDU 3549 Flow Problem 流问题（最大流，入门） - xcw0754 - 博客园
# [HDU 3549 Flow Problem 流问题（最大流，入门）](https://www.cnblogs.com/xcw0754/p/4641026.html)
题意：给个赤裸的最大流问题。
思路：EK+BFS解决。跟HDU1532几乎一样的。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=16;
 7 int cap[N][N];
 8 int flow[N][N];
 9 
10 int a[N];
11 int path[N];
12 
13 vector<int> vect[N];
14 
15 int BFS(int n)
16 {
17     deque<int> que;
18     que.push_back(1);
19     a[1]=INF;
20     while( !que.empty() )
21     {
22         int x=que.front();
23         que.pop_front();
24         for(int i=0; i<vect[x].size(); i++)
25         {
26             int t=vect[x][i];
27             if(!a[t] && cap[x][t]>flow[x][t])
28             {
29                 path[t]=x;
30                 a[t]=min(a[x],cap[x][t]-flow[x][t]);
31                 que.push_back(t);
32             }
33         }
34         if(a[n])    return a[n];
35     }
36     return 0;   //没有增广路了
37 }
38 
39 
40 
41 
42 int cal(int n)
43 {
44     int ans_flow=0;
45     while(true)
46     {
47         memset(a, 0, sizeof(a));
48         memset(path, 0, sizeof(path));
49 
50         int tmp=BFS(n);
51         if(!tmp)    return ans_flow;
52         ans_flow+=tmp;
53 
54         int ed=n;
55         while(ed!=1)
56         {
57             int from=path[ed];
58             flow[from][ed]+=tmp;
59             flow[ed][from]-=tmp;
60             ed=from;
61         }
62     }
63 
64 }
65 
66 
67 int main()
68 {
69     freopen("input.txt", "r", stdin);
70     int t, n, m, st, ed, ca, j=0;
71     cin>>t;
72     while(t--)
73     {
74         scanf("%d%d",&n,&m);
75         for(int i=0; i<=n; i++) vect[i].clear();
76         memset(cap, 0, sizeof(cap));
77         memset(flow, 0, sizeof(flow));
78 
79         for(int i=0; i<m; i++)
80         {
81             scanf("%d%d%d", &st, &ed, &ca);
82             vect[st].push_back(ed);
83             vect[ed].push_back(st);
84             cap[st][ed]+=ca;
85         }
86 
87         printf("Case %d: %d\n",++j, cal(n));
88     }
89     return 0;
90 }
AC代码
```


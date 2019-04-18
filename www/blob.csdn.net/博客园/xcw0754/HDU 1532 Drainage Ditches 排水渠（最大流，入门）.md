# HDU 1532 Drainage Ditches 排水渠（最大流，入门） - xcw0754 - 博客园
# [HDU 1532 Drainage Ditches 排水渠（最大流，入门）](https://www.cnblogs.com/xcw0754/p/4640995.html)
题意：
　　给出一个有向图，以及边上的容量上限，求最大流。（有重边，要将容量上限叠加）
思路：
　　用最简单的EK+BFS解决。每次搜到一条到达终点的路径，就立刻退出，更新ans，然后再回头修改图中的当前flow状况（这就得靠记录路径了）。当当前图没有到达终点的路径图，流已经饱和，可以结束程序了。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=200+5;
 7 
 8 vector<int> vect[N];
 9 int c[N][N];    //容量
10 int flow[N][N]; //流量
11 
12 int a[N];       //临时流量
13 int path[N];    //得记录用的是哪条边，好更新flow和cap
14 
15 
16 int BFS(int m)
17 {
18     deque<int> que;
19     que.push_back(1);
20     a[1]=INF;       //先置为无穷大
21 
22     while(!que.empty())
23     {
24         int x=que.front();
25         que.pop_front();
26         for(int i=0; i<vect[x].size(); i++)
27         {
28             int t=vect[x][i];
29             if(!a[t] && c[x][t]>flow[x][t] )  //未遍历过，且容>流
30             {
31                 path[t]=x;                       //只需要记得到达t的是哪个点
32                 a[t]=min(a[x], c[x][t]-flow[x][t]);     //要么全部流给你，要么取能流过的上限
33                 que.push_back(t);
34             }
35         }
36         if(a[m])    return a[m];  //只要有路径能够更新到终点m，立刻退出。
37     }
38     return 0;
39 }
40 
41 int cal(int m)
42 {
43     int ans_flow=0;
44     while(true)                     //求最大流
45     {
46         memset(a,0,sizeof(a));
47         memset(path,0,sizeof(path));
48 
49         int tmp=BFS(m);
50         if(!tmp)    return ans_flow;    //找不到增广路了
51         ans_flow+=tmp;
52 
53         int ed=m;
54         while(ed!=1)        //根据路径调整一下流及上限
55         {
56             int from=path[ed];
57             flow[from][ed]+=tmp;        //正向边加流量
58             flow[ed][from]-=tmp;        //反向边减流量，相当于cap-flow一样大于0。
59             ed=from;
60         }
61     }
62 }
63 
64 int main()
65 {
66     freopen("input.txt", "r", stdin);
67     int n, m, st, ed, ca;
68     while(~scanf("%d%d",&n,&m))
69     {
70 
71         for(int i=0; i<=m; i++) vect[i].clear();
72         memset(c, 0, sizeof(c));
73         memset(flow, 0, sizeof(flow));
74 
75         for(int i=0; i<n; i++)
76         {
77             scanf("%d %d %d", &st, &ed, &ca);
78             vect[st].push_back(ed);             //邻接表
79             vect[ed].push_back(st);             //反向边，容量是0的。
80             c[st][ed]+=ca;                      //坑在这
81         }
82         cout<<cal(m)<<endl;
83 
84     }
85     return 0;
86 }
AC代码
```


# HDU 3592 World Exhibition （差分约束，spfa，水） - xcw0754 - 博客园
# [HDU 3592 World Exhibition （差分约束，spfa，水）](https://www.cnblogs.com/xcw0754/p/4871162.html)
题意：
　　有n个人在排队，按照前后顺序编号为1~n，现在对其中某两人的距离进行约束，有上限和下限，表示dis[a,b]<=c或者dis[a,b]>=c，问第1个人与第n个人的距离最多可能为多少？（若INF则输出-2，若冲突则输出-1，否则输出距离）
思路：
　　建图时都将约束转成a-b<=c的标准形式，然后建一条b->a的边，权为c。然后求最短路，注意最短路跑出来的结果却是最远的合法距离，而不是最短距离。本题无需添加辅助边，只要到达不了n，则距离为INF，输出-2，若有负环，那肯定是冲突了，为-1。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <set>
 7 #include <deque>
 8 #include <map>
 9 #include <algorithm>
10 #include <vector>
11 #include <iostream>
12 #define pii pair<int,int>
13 #define back que[rear-1]
14 #define INF 0x3f3f3f3f
15 #define LL long long
16 #define ULL unsigned long long
17 using namespace std;
18 const double PI  = acos(-1.0);
19 const int N=1010;
20 struct node
21 {
22     int from,to,dis,next;
23     node(){};
24     node(int from,int to,int dis,int next):from(from),to(to),dis(dis),next(next){};
25 }edge[N*N];
26 
27 int edge_cnt, head[N];
28 int inq[N], cnt[N], dist[N], n;
29 
30 void add_node(int from,int to,int dis)
31 {
32     edge[edge_cnt]=node(from,to,dis,head[from]);
33     head[from]=edge_cnt++;
34 }
35 
36 int spfa(int st,int ed)
37 {
38     memset(cnt,0,sizeof(cnt));//入队次数
39     memset(inq,0,sizeof(inq));//是否在队中
40     memset(dist,0x3f,sizeof(dist));//距离
41     deque<int> que(1,st);
42     inq[st]=1;
43     dist[st]=0;
44     while(!que.empty())
45     {
46         int t=que.front();que.pop_front();
47         inq[t]=0;node e;
48         for(int i=head[t]; i!=-1; i=e.next)
49         {
50             e=edge[i];
51             if( dist[e.to]>dist[t]+e.dis )
52             {
53                 dist[e.to]=dist[t]+e.dis;
54                 if(!inq[e.to])  //没有在队列中
55                 {
56                     if(++cnt[e.to]>n)   //入队次数过多
57                         return -1;
58                     inq[e.to]=1;//下面是优化，可删
59                     if(!que.empty() && dist[e.to]<dist[que.front()])
60                         que.push_front(e.to);
61                     else    que.push_back(e.to);
62                 }
63             }
64 
65         }
66     }
67     return dist[ed]==INF?-2:dist[ed];
68 }
69 
70 void init()
71 {
72     edge_cnt=0;
73     //for(int i=0; i<=n; i++)   head[i]=-1;
74     memset(head,-1,sizeof(head));
75 }
76 
77 int main()
78 {
79     freopen("input.txt", "r", stdin);
80     int x, y, a, b, c, t;cin>>t;
81     while(t--)
82     {
83         init();
84         scanf("%d%d%d",&n,&x,&y);
85         for(int i=1; i<=x; i++) //最多
86         {
87             scanf("%d%d%d",&a,&b,&c);
88             add_node(a,b,c);
89         }
90         for(int i=1; i<=y; i++) //最小
91         {
92             scanf("%d%d%d",&a,&b,&c);
93             add_node(b,a,-c);
94         }
95         printf("%d\n", spfa(1,n));
96     }
97     return 0;
98 }
AC代码
```


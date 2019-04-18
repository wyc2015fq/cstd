# POJ 3259 Wormholes 虫洞（负权最短路，负环） - xcw0754 - 博客园
# [POJ 3259 Wormholes 虫洞（负权最短路，负环）](https://www.cnblogs.com/xcw0754/p/4662672.html)
题意：
　　给一个混合图，求判断是否有负环的存在，若有，输出YES，否则NO。有重边。
思路：
　　这是spfa的功能范围。一个点入队列超过n次就是有负环了。因为是混合图，所以当你跑一次spfa时发现没有负环，但是负环仍可能存在，因为有向边！
　　但是单源最短路也有起点啊，难道穷举起点？不用，负环是必须有某些边是带负权的，那么我们只要穷举负权边的起点就行了，因为单单跑一次spfa不能保证能遍历所有点，但是如果穷举负权边起点还没有找到负环，那么负环不可能存在（剩下的都是正权，怎么可能有负环）。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <vector>
 6 #include <deque>
 7 #define LL long long
 8 #define pii pair<int,int>
 9 #define INF 0x7f7f7f7f
10 using namespace std;
11 const int N=550;
12 int n, m, edge_cnt;
13 vector<int> vect[N];
14 struct node
15 {
16     int from, to, val;
17     node(){};
18     node(int f,int t,int v):from(f),to(t),val(v){};
19 }edge[600000];
20 
21 
22 void add_node(int from,int to,int val)
23 {
24     edge[edge_cnt]=node(from,to,val);
25     vect[from].push_back(edge_cnt++);
26 }
27 
28 int dis[N], inq[N], cnt[N];
29 int spfa(int s)//模板
30 {
31     memset(inq,0,sizeof(inq));
32     memset(cnt,0,sizeof(cnt));
33     memset(dis,0x7f,sizeof(dis));
34     deque<int> que(1,s);
35     inq[s]=1;
36     dis[s]=0;
37 
38     while(!que.empty())
39     {
40         int x=que.front();
41         que.pop_front();
42         inq[x]=0;
43         for(int i=0; i<vect[x].size(); i++)
44         {
45             node e=edge[vect[x][i]];
46             if(dis[e.to]>dis[x]+e.val)
47             {
48                 dis[e.to]=dis[x]+e.val;
49                 if(!inq[e.to])
50                 {
51                     if(++cnt[e.to]>n)   return false;
52                     inq[e.to]=1;
53                     que.push_back(e.to);
54                 }
55             }
56         }
57     }
58     return true;
59 }
60 
61 int main()
62 {
63     freopen("input.txt", "r", stdin);
64     int a, b, c, t, w;
65     cin>>t;
66     while(t--)
67     {
68         scanf("%d%d%d",&n,&m,&w);
69         edge_cnt=0;
70         memset(edge,0,sizeof(edge));
71         for(int i=0; i<=n; i++) vect[i].clear();
72 
73         for(int i=0; i<m; i++)  //无向边
74         {
75             scanf("%d%d%d",&a,&b,&c);
76             add_node(a,b,c);
77             add_node(b,a,c);
78         }
79         vector<int> ver;
80         for(int i=0; i<w; i++)  //有向
81         {
82             scanf("%d%d%d",&a,&b,&c);
83             add_node(a,b,-c);
84             ver.push_back(a);
85         }
86         int i;
87         for(i=0; i<ver.size(); i++)
88         {
89             int q=ver[i];
90             if(!spfa(q))
91             {
92                 puts("YES");
93                 break;
94             }
95         }
96         if(i==ver.size())   puts("NO");
97     }
98     return 0;
99 }
AC代码
```


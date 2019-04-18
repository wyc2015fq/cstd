# HDU 2196 Computer （树形DP） - xcw0754 - 博客园
# [HDU 2196 Computer （树形DP）](https://www.cnblogs.com/xcw0754/p/4806948.html)
题意：给一棵树，问每个点到其他点的最远距离是多少？
思路：
　　按两次深搜，第一次求每个点的后代中最远的距离以及次远距离，第二次就可以从上往下逐个更新答案了。当一棵树形成了，那么每个点只有两个方向可能会有最远距离，一个是经过其父亲，一个是经过其某个孩子。当在第二次深搜时往下推时要注意，经过父亲到达本节点的，可能是从本节点的某个兄弟那里来的。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=10010;
 7 int  ans[N], first[N][2], second[N][2], edge_cnt;
 8 
 9 struct node
10 {
11     int from,to;
12     int len;
13     node(){};
14     node(int from,int to,int len):from(from),to(to),len(len){};
15 }edge[N*2];
16 vector<int> tree[N];
17 
18 void add_edge(int from,int to,int len)
19 {
20     edge[edge_cnt]=node(from,to,len);
21     tree[from].push_back(edge_cnt++);
22 }
23 
24 bool vis[N];
25 int DFS1(int t)    //从下往上
26 {
27     vis[t]=1;
28     for(int i=0; i<tree[t].size(); i++)
29     {
30         node e=edge[tree[t][i]];
31         if(!vis[e.to])
32         {
33             int sum=DFS1(e.to)+e.len;
34             if(sum>second[t][0])
35             {
36                 second[t][0]=sum;
37                 second[t][1]=e.to;
38                 if(second[t][0]>first[t][0])    swap(second[t],first[t]);
39             }
40         }
41     }
42     return first[t][0];
43 }
44 
45 void DFS2(int t,int up)     //从上往下
46 {
47     vis[t]=1;
48     ans[t]=max(up, first[t][0]);
49     for(int i=0; i<tree[t].size(); i++)
50     {
51         node e=edge[tree[t][i]];
52         if(!vis[e.to])
53         {
54             if(first[t][1]==e.to)    DFS2(e.to, e.len+max(up, second[t][0]));
55             else                     DFS2(e.to, e.len+max(up, first[t][0]));
56         }
57     }
58 }
59 
60 int main()
61 {
62     //freopen("input.txt", "r", stdin);
63     int n, a, b;
64     while(cin>>n)
65     {
66         memset(second, 0, sizeof(second));
67         memset(first, 0, sizeof(first));
68         for(int i=0; i<=n; i++) tree[i].clear();
69         edge_cnt=0;
70         for(int i=2; i<=n; i++) //1号是主机
71         {
72             scanf("%d%d",&a,&b);
73             add_edge(i,a,b);
74             add_edge(a,i,b);
75         }
76         memset(vis,0,sizeof(vis));
77         DFS1(1);
78         memset(vis,0,sizeof(vis));
79         DFS2(1,0);
80         for(int i=1; i<=n; i++) printf("%d\n", ans[i]);
81     }
82     return 0;
83 }
AC代码
```


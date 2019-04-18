# HDU 3666 THE MATRIX PROBLEM （差分约束，最短路） - xcw0754 - 博客园
# [HDU 3666 THE MATRIX PROBLEM （差分约束，最短路）](https://www.cnblogs.com/xcw0754/p/4694156.html)
题意：
　　给一个n*m矩阵，每个格子上有一个数字a[i][j]，给定L和U，问：是否有这样两个序列{a1...an}和{b1...bn}，满足 L<=a[i][j]*ai/bj<=U 。若存在输出yes，否则no。
思路：
　　能够得到的是一堆不等式，那么可以用最短路来解决差分约束系统。但是a[i][j]*ai/bj<=U是除的，得提前变成减的才行。可以用对数log来解决，先不管a[i][j]，logai-logbj<=U不就行了？可以得到：
　　（1）logai - logbj<=U/a[i][j]       （注：这里已经是浮点型了）
　　（2）logbj - logai<=-L/a[i][j]
根据这个来建图即可，值得注意的是，点是log(x)，而不是x，只有n+m个点。
但是起点呢？不妨以随便以一个点r作为起点，置cost[r]=0，其他INF，来进行最短路，我们只需要对其判断是否有负环即可。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <cstring>
 4 #include <map>
 5 #include <iostream>
 6 #include <deque>
 7 #include <vector>
 8 #define INF 0x7f7f7f7f
 9 #define pii pair<int,int>
10 #define LL unsigned long long
11 using namespace std;
12 const int N=810;
13 struct node
14 {
15     int from,to;
16     double cost;
17     node(){};
18     node(int from,int to,double cost):from(from),to(to),cost(cost){};
19 }edge[N*N];
20 vector<int> vect[N];
21 int edge_cnt;
22 
23 void add_node(int from,int to,double cost)
24 {
25     edge[edge_cnt]=node(from,to,cost);
26     vect[from].push_back(edge_cnt++);
27 }
28 
29 
30 double cost[N];
31 int cnt[N];
32 bool inq[N];
33 int spfa(int up)
34 {
35     memset(inq,  0, sizeof(inq));
36     memset(cost, 0x7f, sizeof(cost));
37     cost[1]=0;
38     deque<int> que(1,1);//随便以1作为起点
39 
40     while(!que.empty())
41     {
42         int x=que.front();que.pop_front();
43         inq[x]=0;
44         for(int i=0; i<vect[x].size(); i++)
45         {
46             node e=edge[vect[x][i]];
47             if(cost[e.to]>cost[x]+e.cost)
48             {
49                 cost[e.to]=cost[x]+e.cost;
50                 if(!inq[e.to])
51                 {
52                     inq[e.to]=1;
53                     if(++cnt[e.to]>up)  return false;
54                     if(!que.empty()&&cost[e.to]<cost[que.front()])
55                         que.push_front(e.to);//一个优化
56                     else
57                         que.push_back(e.to);
58                 }
59             }
60         }
61     }
62     return true;
63 }
64 
65 int main()
66 {
67     freopen("input.txt", "r", stdin);
68     int n, m, a, b, c, L, U;
69     double g;
70 
71     while(~scanf("%d %d %d %d", &n, &m, &L, &U))
72     {
73         edge_cnt=0;
74         memset(edge,0,sizeof(edge));
75         for(int i=0; i<=n+m; i++)  vect[i].clear();
76 
77         //a1~an编号为1~n，b1~bn编号为n+1~n+m
78         for(int i=1; i<=n; i++)
79         {
80             for(int j=1; j<=m; j++)
81             {
82                 scanf("%lf",&g);
83                 add_node(i, n+j, -L/g);
84                 add_node(n+j, i, U/g);
85             }
86         }
87         if( spfa(n+m) )    puts("YES");
88         else    puts("NO");
89     }
90     return 0;
91 }
AC代码
```


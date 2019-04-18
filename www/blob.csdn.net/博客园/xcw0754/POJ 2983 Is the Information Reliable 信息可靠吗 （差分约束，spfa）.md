# POJ 2983 Is the Information Reliable? 信息可靠吗 （差分约束，spfa） - xcw0754 - 博客园
# [POJ 2983 Is the Information Reliable? 信息可靠吗 （差分约束，spfa）](https://www.cnblogs.com/xcw0754/p/4695662.html)
题意：有n个站排成一列，针对每个站的位置与距离关系，现有多个约束条件，约束条件分两种：（1）确定的。明确说明站a距离站b多少个单位距离。（2）不确定的。只知道a在b的左边至少1个单位距离。  根据已知条件，问有没有冲突？不冲突则输出reliable。
思路：
　　第2种条件比较好确定，如果知道如何用最短路解差分约束的话。
　　问题在第1种，明确地说明了距离，怎么办？拆成两条式子，比如 dis(a,b)=c，那么可以写成 b-a>=c ，b-a<=c 这样，只要满足这两个条件，原来明确说明的距离也会成立的。这样就可以根据两条式子建图了。再用spfa解就可以了。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <vector>
 5 #include <cstring>
 6 #include <deque>
 7 #define INF 0x7f7f7f7f
 8 #define pii pair<int,int>
 9 #define LL unsigned long long
10 using namespace std;
11 const int N=1010;
12 struct node
13 {
14     int from, to, cost;
15     node(){};
16     node(int from,int to,int cost):from(from),to(to),cost(cost){};
17 }edge[N*N];
18 int edge_cnt;
19 vector<int> vect[N];
20 
21 void add_node(int from,int to,int cost)
22 {
23     edge[edge_cnt]=node(from, to, cost);
24     vect[from].push_back(edge_cnt++);
25 }
26 
27 int inq[N], cost[N], cnt[N];
28 
29 bool spfa(int n)
30 {
31     memset(inq, 1, sizeof(inq));
32     memset(cost, 0, sizeof(cost));
33     memset(cnt, 0, sizeof(cnt));
34     deque<int> que;
35     for(int i=1; i<=n; i++) que.push_back(i);
36 
37     while(!que.empty())
38     {
39         int x=que.front();
40         que.pop_front();
41         inq[x]=0;
42         for(int i=0; i<vect[x].size(); i++)
43         {
44             node e=edge[vect[x][i]];
45             if(cost[e.to]>cost[e.from]+e.cost)
46             {
47                 cost[e.to]=cost[e.from]+e.cost;
48                 if(!inq[e.to])
49                 {
50                     inq[e.to]=1;
51                     if(++cnt[e.to]>n)   return false;
52                     if(!que.empty()&& cost[e.to]<cost[que.front()])
53                         que.push_front(e.to);
54                     else
55                         que.push_back(e.to);
56                 }
57             }
58         }
59     }
60     return true;
61 }
62 
63 int main()
64 {
65     freopen("input.txt", "r", stdin);
66     int t, a, b, d, n, m;
67     char c;
68     while(~scanf("%d%d", &n, &m))
69     {
70         edge_cnt=0;
71         for(int i=0; i<=n; i++) vect[i].clear();
72         memset(edge,0,sizeof(edge));
73 
74         for(int i=0; i<m; i++)
75         {
76             while(scanf("%c", &c), !isalpha(c) );
77             if(c=='P')//确定的，要拆
78             {
79                 scanf("%d %d %d", &a, &b, &d);
80                 add_node(a,b,d);
81                 add_node(b,a,-d);
82             }
83             else
84             {
85                 scanf("%d %d", &a, &b);
86                 add_node(b,a,-1);
87             }
88         }
89         if(spfa(n)) puts("Reliable");
90         else    puts("Unreliable");
91     }
92     return 0;
93 }
AC代码
```


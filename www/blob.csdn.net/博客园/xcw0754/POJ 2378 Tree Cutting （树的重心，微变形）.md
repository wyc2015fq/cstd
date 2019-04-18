# POJ 2378 Tree Cutting  （树的重心，微变形） - xcw0754 - 博客园
# [POJ 2378 Tree Cutting  （树的重心，微变形）](https://www.cnblogs.com/xcw0754/p/4837440.html)
题意：
　　给定一棵树，n个节点，若删除点v使得剩下的连通快最大都不超过n/2，则称这样的点满足要求。求所有这样的点，若没有这样的点，输出NONE。
思路：
　　只需要拿“求树的重心”的代码改一行就OK了。因为依然是在判别最大连通块的点数。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <deque>
 8 #include <algorithm>
 9 #include <vector>
10 #include <iostream>
11 #define pii pair<int,int>
12 #define max(x,y) ((x)>(y)?(x):(y))
13 #define min(x,y) ((x)<(y)?(x):(y))
14 #define INF 0x7f7f7f7f
15 #define LL  long long
16 using namespace std;
17 const double PI  = acos(-1.0);
18 const int N=10010;
19 int n,  edge_cnt, head[N];
20 
21 struct node
22 {
23     int from, to, next;
24     node(){};
25     node(int from,int to,int next):from(from),to(to),next(next){};
26 }edge[N*2];
27 
28 void add_node(int from,int to)
29 {
30     edge[edge_cnt]=node(from,to,head[from]);
31     head[from]=edge_cnt++;
32 }
33 deque<int> que;
34 
35 int DFS(int t,int far)
36 {
37     node e;
38     int big=0, sum=0;
39     for(int i=head[t]; i!=-1; i=e.next)
40     {
41         e=edge[i];
42         if(e.to==far)   continue;
43 
44         int tmp=DFS(e.to, t);
45         big=max(big, tmp);
46         sum+=tmp;
47     }
48     big=max(big, n-sum-1);
49 
50     if(big<=n/2)    que.push_back(t);//只改这处地方
51     return sum+1;
52 }
53 
54 int main()
55 {
56     //freopen("input.txt", "r", stdin);
57     int a, b;
58     while(~scanf("%d",&n))
59     {
60         edge_cnt=0;
61         memset(head,-1,sizeof(head));
62 
63         for(int i=1; i<n; i++)
64         {
65             scanf("%d%d",&a,&b);
66             add_node(a,b);
67             add_node(b,a);
68         }
69         DFS(1,-1);
70         sort(que.begin(),que.end());
71         if(que.empty()) puts("NONE");
72         else
73         {
74             while(!que.empty())
75             {
76                 printf("%d\n",que.front());
77                 que.pop_front();
78             }
79         }
80     }
81     return 0;
82 }
AC代码
```


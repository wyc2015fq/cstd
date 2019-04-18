# POJ 3140 Contestants Division （树形DP，简单） - xcw0754 - 博客园
# [POJ 3140 Contestants Division （树形DP，简单）](https://www.cnblogs.com/xcw0754/p/4837659.html)
题意：
　　有n个城市，构成一棵树，每个城市有v个人，要求断开树上的一条边，使得两个连通分量中的人数之差最小。问差的绝对值。（注意本题的M是没有用的，因为所给的必定是一棵树，边数M必定是n-1）
思路：
　　考虑当前节点t，当断开t与父亲的边时，“子树t中的人数”与“剩下的人数”之差的绝对值若最小，则为答案。
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
14 #define abs(x) ((x)<0?-(x):(x))
15 #define INF 2147483647
16 #define LL  long long
17 using namespace std;
18 const double PI  = acos(-1.0);
19 const int N=1000010;
20 int edge_cnt, head[N], w[N];
21 LL ans, sum;
22 struct node
23 {
24     int from, to, next;
25     node(){};
26     node(int from,int to,int next):from(from),to(to),next(next){};
27 }edge[N*2];
28 
29 void add_node(int from,int to)
30 {
31     edge[edge_cnt]=node(from,to,head[from]);
32     head[from]=edge_cnt++;
33 }
34 
35 LL DFS(int t,int far)  //枚举删除t头上的边
36 {
37     node e;
38     LL cnt=0;
39     for(int i=head[t]; i!=-1; i=e.next)
40     {
41         e=edge[i];
42         if(e.to==far)   continue;
43         cnt+=DFS(e.to, t);
44     }
45     cnt+=w[t];  //本子树的人数
46     ans=min(ans, abs( 2*cnt-sum ));
47     return cnt;
48 }
49 
50 
51 int main()
52 {
53     //freopen("input.txt", "r", stdin);
54     int a, b, n, m, Case=0;
55     while(scanf("%d%d",&n,&m), n+m)
56     {
57         sum=edge_cnt=0;
58         memset(head,-1,sizeof(head));
59 
60         for(int i=1; i<=n; i++)
61         {
62             scanf("%d",&w[i]);
63             sum+=w[i];
64         }
65         ans=sum;
66         for(int i=0; i<m; i++)
67         {
68             scanf("%d%d",&a,&b);
69             add_node(a,b);
70             add_node(b,a);
71         }
72         DFS(1,-1);
73         printf("Case %d: %lld\n", ++Case, ans);
74     }
75     return 0;
76 }
AC代码
```


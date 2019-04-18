# POJ 1861 Network （MST） - xcw0754 - 博客园
# [POJ 1861 Network （MST）](https://www.cnblogs.com/xcw0754/p/4712777.html)
题意：求解最小生成树，以及最小瓶颈生成树上的瓶颈边。
思路：只是求最小生成树即可。瓶颈边就是生成树上权值最大的那条边。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <vector>
 4 #include <iostream>
 5 #include <algorithm>
 6 #define INF 0x7f7f7f7f
 7 #define pii pair<int,int>
 8 #define LL long long
 9 using namespace std;
10 const int N=16100;
11 
12 int seq[N], a[N], b[N], w[N], pre[N];
13 int cmp(int a,int b)
14 {
15     return w[a]<w[b];
16 }
17 
18 int find(int x)
19 {
20     return pre[x]==x? x: pre[x]=find(pre[x]);
21 }
22 
23 
24 vector<int> edge;
25 int cal(int n, int m)
26 {
27     edge.clear();
28     int ans=0;
29     for(int i=0; i<=n; i++)     pre[i]=i;
30     for(int i=0; i<m; i++)
31     {
32         int u=find(a[seq[i]]);
33         int v=find(b[seq[i]]);
34         if( u!=v )
35         {
36             pre[u]=v;   //不是同个连通块，则连接。
37             ans=max(ans, w[seq[i]]);
38             edge.push_back(seq[i]);
39         }
40     }
41     return ans;
42 }
43 
44 int main()
45 {
46     freopen("input.txt", "r", stdin);
47     int t, n, m;
48 
49     while(cin>>n>>m)
50     {
51         for(int i=0; i<m; i++)
52         {
53             seq[i]=i;
54             scanf("%d%d%d", &a[i], &b[i], &w[i]);
55         }
56         sort(seq,seq+m,cmp);
57         cout<<cal(n, m)<<endl;
58         cout<<n-1<<endl;
59         for(int i=0; i<edge.size(); i++)
60         {
61             int q=edge[i];
62             printf("%d %d\n", a[q], b[q] );
63         }
64 
65     }
66     return 0;
67 }
AC代码
```


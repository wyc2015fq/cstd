# POJ 2395 Out of Hay 草荒 （MST，Kruscal，最小瓶颈树） - xcw0754 - 博客园
# [POJ 2395 Out of Hay 草荒 （MST，Kruscal，最小瓶颈树）](https://www.cnblogs.com/xcw0754/p/4712737.html)
题意：Bessie要从牧场1到达各大牧场去，他从不关心他要走多远，他只关心他的水袋够不够水，他可以在任意牧场补给水，问他走完各大牧场，最多的一次需要多少带多少单位的水？
思路：其实就是要让所带的水尽量少，即所选的每条路都要尽量短，即最小瓶颈生成树。其实也就是最小生成树。再求生成树上权值最大的边即可。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <iostream>
 4 #include <algorithm>
 5 #define INF 0x7f7f7f7f
 6 #define pii pair<int,int>
 7 #define LL long long
 8 using namespace std;
 9 const int N=10100;
10 
11 int seq[N], a[N], b[N], w[N], pre[N];
12 int cmp(int a,int b)
13 {
14     return w[a]<w[b];
15 }
16 
17 int find(int x)
18 {
19     return pre[x]==x? x: pre[x]=find(pre[x]);
20 }
21 
22 int cal(int n, int m)
23 {
24     int ans=0;
25     for(int i=0; i<=n; i++)     pre[i]=i;
26     for(int i=0; i<m; i++)
27     {
28         int u=find(a[seq[i]]);
29         int v=find(b[seq[i]]);
30         if( u!=v )
31         {
32             pre[u]=v;   //不是同个连通块，则连接。
33             ans=max(ans, w[seq[i]]);
34         }
35     }
36 
37     return ans;
38 }
39 
40 
41 
42 int main()
43 {
44     freopen("input.txt", "r", stdin);
45     int t, n, m;
46 
47     while(cin>>n>>m)
48     {
49         for(int i=0; i<m; i++)
50         {
51             seq[i]=i;
52             scanf("%d%d%d", &a[i], &b[i], &w[i]);
53         }
54         sort(seq,seq+m,cmp);
55         cout<<cal(n, m)<<endl;
56     }
57     return 0;
58 }
AC代码
```


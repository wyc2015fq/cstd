# UVA 1395 Slim Span (最小生成树，MST，kruscal) - xcw0754 - 博客园
# [UVA 1395 Slim Span (最小生成树，MST，kruscal)](https://www.cnblogs.com/xcw0754/p/4620897.html)
题意：给一个图，找一棵生成树，其满足：最大权-最小权=最小。简单图，不一定连通，权值可能全相同。
思路：点数量不大。根据kruscal每次挑选的是最小权值的边，那么苗条度一定也是最小。但是生成树有多棵，苗条度自然也有多个，穷举下所有生成树，就知道了结果了。根据“只要起始边不同，生成树必定不同”来穷举起始边。
　　又发现一可能的坑！！我以为LONG_MAX就是int的正最大值，也就是2147483647=2^31-1，在我的机器上也许如此，在OJ上不一定了，用LONG_MAX转int会不同，得注意。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=100+5;
 4 const int INF=0x7f7f7f7f;
 5 int g[N][N];
 6 int n, m;
 7 int pre[N];
 8 vector< pair<int,int> > vect;
 9 inline int cmp(pair<int,int> a,pair<int,int> b)
10 {
11     return g[a.first][a.second]<g[b.first][b.second]? true: false;
12 }
13 
14 int find(int x)
15 {
16     return pre[x]==x? x: pre[x]=find(pre[x]);
17 }
18 void joint(int a,int b)
19 {
20     a=find(a);
21     b=find(b);
22     if(a!=b)    pre[a]=b;
23 }
24 
25 
26 int kruscal(int i)
27 {
28     int q=vect[i].first;
29     int p=vect[i].second;
30     int cnt=0;
31     for(int i=0; i<=n; i++) pre[i]=i;
32 
33     for(int j=i; j<m; j++)
34     {
35         int a=vect[j].first;
36         int b=vect[j].second;
37         if(find(a)!=find(b))
38         {
39             cnt++;
40             if(cnt==n-1)    return g[a][b]-g[q][p];
41             joint(a,b);
42         }
43     }
44     return INF;
45 }
46 
47 int cal()
48 {
49     if(m<n-1 ||  kruscal(0)==INF)   return -1;//不连通
50 
51     sort(vect.begin(), vect.end(), cmp);
52     int ans=INF;
53     for(int i=0; i<m; i++)
54     {
55         int q=kruscal(i);
56         if(q==INF)    continue;
57         ans=min(ans,q);
58     }
59     return ans;
60 }
61 
62 int main()
63 {
64     //freopen("input.txt", "r", stdin);
65     int a, b, w;
66     while(scanf("%d%d",&n,&m), n+m )
67     {
68         memset(g,0,sizeof(g));
69         vect.clear();
70         for(int i=0; i<m; i++)
71         {
72             scanf("%d%d%d",&a,&b,&w);
73             g[a][b]=g[b][a]=w;
74             vect.push_back(make_pair(b,a));
75         }
76         cout<<cal()<<endl;
77     }
78     return 0;
79 }
AC代码
```


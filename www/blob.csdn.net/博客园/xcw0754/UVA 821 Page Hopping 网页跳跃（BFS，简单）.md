# UVA 821 Page Hopping 网页跳跃（BFS，简单） - xcw0754 - 博客园
# [UVA 821 Page Hopping 网页跳跃（BFS，简单）](https://www.cnblogs.com/xcw0754/p/4657208.html)
题意：
　　给一个无权有向图，可认为边的长度为1，求两点间的平均长度（即所有点对的长度取平均），保留3位小数。保证任意点对都可达。
思路：
　　简单题。直接穷举每个点，进行BFS求该点到其他点的距离。累加后除去边数即可。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=110;
 7 int g[N][N];
 8 int dis[N];
 9 
10 map<int,int> mapp;
11 int BFS(int s,int n)
12 {
13     memset(dis,0,sizeof(dis));
14     deque<int> que(1,s);
15     dis[s]=INF;
16     int len=0;
17     while(!que.empty())
18     {
19         len++;
20         int siz=que.size();
21         for(int i=0; i<siz; i++)
22         {
23             int x=que.front();
24             que.pop_front();
25             for(int j=1; j<=n; j++)
26             {
27                 if(!dis[j] && g[x][j])
28                 {
29                     dis[j]=len;
30                     que.push_back(j);
31                 }
32             }
33         }
34     }
35     int ans=0;
36     for(int i=1; i<=n; i++)    if(dis[i]<INF)  ans+=dis[i];
37     return ans;
38 }
39 
40 double cal(int n)
41 {
42     int ans=0;
43     for(int i=1; i<=n; i++)
44         ans+=BFS(i, n);
45     return (double)ans/(n*(n-1));
46 }
47 
48 int main()
49 {
50     freopen("input.txt", "r", stdin);
51 
52     int a, b, j=0;
53     while(1)
54     {
55         mapp.clear();
56         memset(g,0,sizeof(g));
57         int cnt=0;
58         while(scanf("%d%d",&a,&b),a+b)
59         {
60             if(!mapp[a])    mapp[a]=++cnt;
61             if(!mapp[b])    mapp[b]=++cnt;
62             g[mapp[a]][mapp[b]]=1;
63         }
64         if(!cnt)    return 0;
65         printf("Case %d: average length between pages = %.3f clicks\n", ++j, cal(cnt));
66 
67     }
68     return 0;
69 }
AC代码
```


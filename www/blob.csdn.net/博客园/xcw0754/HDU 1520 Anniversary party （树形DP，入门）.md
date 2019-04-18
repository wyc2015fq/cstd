# HDU 1520 Anniversary party （树形DP，入门） - xcw0754 - 博客园
# [HDU 1520 Anniversary party （树形DP，入门）](https://www.cnblogs.com/xcw0754/p/4806509.html)
题意：给一棵树，每个节点都有权值，要求选择部分节点出来，使得权值之和最大，但是每对（父亲，儿子）中最多只能挑一个。
思路：
　　比较入门的题，每个节点可以选也可以不选。若当前节点选的话，孩子必须全部不选；若当前节点不选，则孩子可以选也可以不选。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=7010;
 7 int w[N], dp[N][2], pre[N];
 8 vector<int> tree[N];
 9 
10 void DFS(int t)
11 {
12     for(int i=0; i<tree[t].size(); i++)    DFS(tree[t][i]);
13     int sum[2]={0,0};
14     for(int i=0; i<tree[t].size(); i++)
15     {
16         sum[0]+=max(dp[tree[t][i]][0], dp[tree[t][i]][1]); //点t尝试不取，则孩子可以取也可以不取，取大者即可。
17         sum[1]+=max(dp[tree[t][i]][0], 0);    //点t尝试取，则孩子必须全部不取
18     }
19     dp[t][0]=sum[0];
20     dp[t][1]=sum[1]+w[t];
21 }
22 
23 
24 int main()
25 {
26     //freopen("input.txt", "r", stdin);
27     int n, a, b, root;
28     while(cin>>n)
29     {
30         memset(pre,0,sizeof(pre));
31         memset(dp,0,sizeof(dp));
32         for(int i=0; i<=n; i++) tree[i].clear();
33 
34         for(int i=1; i<=n; i++) scanf("%d",&w[i]);
35         while(scanf("%d%d",&a,&b),a+b)
36         {
37             tree[b].push_back(a);
38             pre[a]=b;
39         }
40         for(int i=1; i<=n; i++) if(pre[i]==0)   root=i;
41         DFS(root);
42         printf("%d\n", max(dp[root][0],dp[root][1]) );
43     }
44     return 0;
45 }
AC代码
```


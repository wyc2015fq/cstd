# POJ 1655 Balancing Act （树的重心，常规） - xcw0754 - 博客园
# [POJ 1655 Balancing Act （树的重心，常规）](https://www.cnblogs.com/xcw0754/p/4808212.html)
题意：求树的重心，若有多个重心，则输出编号较小者，及其子树中节点最多的数量。
思路：
　　树的重心：指的是一个点v，在删除点v后，其子树的节点数分别为:u1,u2....，设max(u)为其中的最大值，点v的max(u)是所有点里面最小的，称v为树的重心。
　　如何求任一重心？按树形来看，max(v)可以由其父亲贡献，也可以由其任一孩子贡献。孩子比较好解决，不就是深搜一遍，然后回溯时统计下数量就行了？而父亲的怎么办？可以知道，点v到其父亲这一叉就是n-sum(v)了，sum(v)指的是以v为根的子树的节点数。那么一次DFS就可以知道答案了，复杂度O(n)。
```
1 //#include <bits/stdc++.h>
 2 #include <vector>
 3 #include <iostream>
 4 #include <cstdio>
 5 #include <cstring>
 6 #define pii pair<int,int>
 7 #define INF 0x3f3f3f3f
 8 #define LL long long
 9 using namespace std;
10 const int N=20100;
11 int n, vis[N], cnt[N];
12 vector<int> vect[N];
13 int DFS(int x)      //深搜求删除任一点后，其某一子树的节点数量达到的最大值。
14 {
15     vis[x]=1;
16     int big=0,sum=0;
17     for(int i=0; i<vect[x].size(); i++)
18     {
19         if(!vis[vect[x][i]])
20         {
21             int t=DFS(vect[x][i]);
22             big=max(t,big);
23             sum+=t;
24         }
25     }
26     cnt[x]=max(big, n-sum-1);
27     return sum+1;
28 }
29 
30 int main()
31 {
32     //freopen("input.txt", "r", stdin);
33     int t,a,b;cin>>t;
34     while(t--)
35     {
36         scanf("%d",&n);
37         memset(vis,0,sizeof(vis));
38         memset(cnt,0,sizeof(cnt));
39         for(int i=0; i<=n; i++) vect[i].clear();
40         for(int i=1; i<n; i++)
41         {
42             scanf("%d%d",&a,&b);
43             vect[a].push_back(b);
44             vect[b].push_back(a);
45         }
46         DFS(1);
47         int big=INF, pos;
48         for(int i=1; i<=n; i++)
49         {
50             if(cnt[i]<big)
51             {
52                 big=cnt[i];
53                 pos=i;
54             }
55         }
56         printf("%d %d\n", pos, big);
57     }
58     return 0;
59 }
AC代码
```


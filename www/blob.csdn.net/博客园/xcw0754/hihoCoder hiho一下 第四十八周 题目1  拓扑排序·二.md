# hihoCoder hiho一下 第四十八周 题目1 : 拓扑排序·二 - xcw0754 - 博客园
# [hihoCoder hiho一下 第四十八周 题目1 : 拓扑排序·二](https://www.cnblogs.com/xcw0754/p/4544665.html)
题意：
　　给定一个拓扑图，其中部分结点含有1个病毒，每个结点只要收到病毒就会立即往出边所能到达的点传播，病毒数可叠加，求所有结点的病毒数总和。
思路：
　　根据拓扑的特点，每个入度为0的点肯定不会再被传播病毒，而且会将自己的所有病毒向与其相连的结点传播。那么可以从入度味为0的点着手，逐个删除入度为0的结点，在删除的过程中，更新与其相连的结点的病毒数（即将病毒数累加到该结点），到最后所有结点都没了，各个结点所累积的病毒数的和就是答案。
```
1 #include<bits/stdc++.h>
 2 using namespace std;
 3 const int N=100100;
 4 const int mod=142857;
 5 
 6 bool init[N];   //初始是否有病毒
 7 int cnt[N];     //各点的入度
 8 vector< vector<int> >   vect;   //邻接表
 9 int ans[N]; //各个节点的病毒数，不包括其自身初始的那个病毒
10 
11 
12 int cal(int n, int k)
13 {
14     vector<int> a;
15     for(int i=1; i<=n; i++)    if(!cnt[i]) a.push_back(i);  //先装进去初始时入度为0的点
16 
17     vector<int> b;
18     while(!a.empty())
19     {
20         b.clear();
21         for(int i=0; i<a.size(); i++)   //对于每个入度为0的点，更新与其相连的每个点的病毒数
22         {
23             for(int j=0; j<vect[a[i]].size(); j++)
24             {
25                 int tmp=vect[a[i]][j];
26                 cnt[tmp]--; //入度减少1
27                 ans[tmp]=(ans[tmp]+ans[a[i]]+init[a[i]])%mod;   //更新病毒数，包括初始那个病毒
28                 if(!cnt[tmp])    b.push_back(tmp);  //又一个入度为0的
29             }
30         }
31         a.clear();
32         if(!b.empty())
33             a.insert(a.end(), b.begin(), b.end());
34     }
35     int sum=0;
36     for(int i=1; i<=n; i++) //统计
37         sum=(sum+ans[i])%mod;
38     sum+=k; //还有初始的病毒
39     return sum%mod;
40 }
41 
42 
43 int main()
44 {
45     //freopen("e://input.txt","r",stdin);
46     int k, n, m, a, b;
47     cin>>n>>m>>k;
48     vector<int> tmp;
49     for(int i=0; i<=n; i++)    vect.push_back(tmp); //初始化
50 
51     for(int i=0; i<k; i++)  //k个病毒
52     {
53         scanf("%d",&a);
54         init[a]=true;
55     }
56     for(int i=0; i<m; i++)  //m个边
57     {
58         scanf("%d%d",&a,&b);
59         vect[a].push_back(b);
60         cnt[b]++;   //入度数
61     }
62     printf("%d\n",cal(n,k));
63     return 0;
64 }
AC代码
```


# ZOJ 3396 Conference Call(3点最小生成树) - xcw0754 - 博客园
# [ZOJ 3396 Conference Call(3点最小生成树)](https://www.cnblogs.com/xcw0754/p/4559682.html)
![](http://acm.zju.edu.cn/onlinejudge/showImage.do?name=conference-call-sample.jpg)
题意：给出一组含m个点的无向图，再给出n个点，这n个点分别以一条边连接到这个无向图中的某个点。对于每个询问，求出3点连通的最小代价。有可能3个点是不能互通的。如图，最小代价就是红色的边的权之和。
思路：先对m个点的无向图进行求两两之间最短路径，用floyd。接下来对于每个询问，穷举m个点，求3个点分别到该点的距离之和，求最小即可。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #define INB 0x7f7f7f7f
 6 using namespace std;
 7 const int N=10005;
 8 int tsf[N];         //电话i接到的中转站
 9 int gph[505][505];  //邻接矩阵表示图
10 
11 int nexte[505][505];    //floyd用的辅助矩阵
12 int dis[505][505];
13 
14 
15 int cal(int a,int b, int c,int m)
16 {
17     int small=INB;
18     for(int k=1; k<=m; k++)
19     {
20         if(dis[a][k]==INB||dis[b][k]==INB||dis[c][k]==INB)
21             continue;
22         if(dis[a][k]+dis[b][k]+dis[c][k]<small)
23             small=dis[a][k]+dis[b][k]+dis[c][k];
24     }
25     if(small>=INB)  return 0;
26     return small;
27 }
28 
29 void floyd(int m)
30 {
31     memset(nexte,0x80,sizeof(nexte));   //初始化为负
32     memset(dis,0x7f,sizeof(dis));   //初始化为无穷大
33 
34     //初始化 next为可到达的
35     for(int i=1; i<=m; i++)
36     {
37         for(int j=1; j<=m; j++)
38         {
39             if(i==j)    dis[i][i]=0;    //到自己
40             if(gph[i][j]>0)       //如果i到j有边
41             {
42                 nexte[i][j]=j;          //这是路径矩阵
43                 dis[i][j]=gph[i][j];    //无向图
44             }
45         }
46     }
47 
48     for(int k=1; k<=m; k++)
49     {
50         for(int i=1; i<=m; i++)
51         {
52             for(int j=1; j<=m; j++)
53             {
54 
55                 if(dis[i][k]>=INB || dis[k][j]>=INB)  continue; //两段其中一段已经不可达
56                 if(dis[i][j]>dis[i][k]+ dis[k][j])  //此路更短
57                 {
58                     dis[i][j]=dis[i][k]+ dis[k][j];
59                     nexte[i][j]=nexte[i][k];     //修改path
60                 }
61             }
62         }
63     }
64 }
65 
66 int main()
67 {
68     //freopen("e://input.txt", "r", stdin);
69     int n, m, l, j=0;
70     int a, b, v, q;
71     while(cin>>n>>m>>l)
72     {
73         memset(gph,0,sizeof(gph));      //初始化
74         for(int i=0; i<n; i++)          //输入电话所连接的中转站
75             scanf("%d",&tsf[i+1]);
76         for(int i=0; i<l; i++)          //输入中转站图
77         {
78             scanf("%d%d%d",&a,&b,&v);
79             gph[a][b]=v;
80             gph[b][a]=v;
81         }
82         floyd(m);       //佛洛依德开始
83         scanf("%d",&q);
84         printf("Case #%d\n",++j);
85         for(int i=0; i<q; i++)      //处理每个询问
86         {
87             scanf("%d%d%d",&a,&b,&v);
88             int ans=cal(tsf[a],tsf[b],tsf[v],m);
89             if(ans)
90                 printf("Line %d: The minimum cost for this line is %d.\n",i+1,ans);
91             else
92                 printf("Line %d: Impossible to connect!\n",i+1);
93         }
94     }
95     return 0;
96 }
AC代码
```


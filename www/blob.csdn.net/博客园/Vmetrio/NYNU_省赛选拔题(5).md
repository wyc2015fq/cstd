# NYNU_省赛选拔题(5) - Vmetrio - 博客园







# [NYNU_省赛选拔题(5)](https://www.cnblogs.com/wangmengmeng/p/5271859.html)





## 题目描述

P 的一家要出去旅游，买了当地的地图，发现各地分别由各个景点，若 P 想使家人分队去景点，尽快到达各个景点(必须所有景点)，并且最终所有家人都到达 M 所在的景点.   
你用程序告诉 P 最少需要多少天才能完成这次旅游。假设 P 的家人足够多，各分队速度一样.

## 输入

首先是一个正整数T，接下来是T组测试数据，每组数据第一行是两个整数n,m(2=<n<=1000,1=<m<=10000)，分别表示景点数量和总边数，山寨编号0，1，2，3…n-1
接下来m行，每行三个整数i,j,k(0=<i,j<n,k<=10^4)，分别表示景点i和景点j之间有一条路，在这条路上需要k天，接下来一行两个整数s,t(0<=s,t<=n-1)，分别表示P所在的起点和M所在终点的编号

## 输出

对每组数据输出一个整数，表示P到达所有山寨并汇聚到M所在景点所需要的最少天数，每个输出独占一行

## 样例输入
2
5 6
0 1 1
1 2 3
3 1 2
4 0 3
3 2 3
3 4 1
4 3
5 5
1 0 2
1 2 4
1 3 3
4 2 1
3 4 2
4 2
## 样例输出
7
13


求出所有点到起点，终点最短路权值，然后相加取最大，dijkstra算法


```
1 # include <iostream>
 2 # include <cstdio>
 3 # include <cstring>
 4 # include <algorithm>
 5 # include <cmath>
 6 using namespace std;
 7 #define INF 0x3f3f3f3f
 8 #define Max 1001
 9 int cost[Max][Max];
10 int d[Max];
11 bool used[Max];
12 void dijkstra(int n,int s)    
13 {
14     memset(d,INF,sizeof(d));
15     memset(used,false,sizeof(used));
16     d[s]=0;
17     while(true)
18     {
19         int v=-1;
20         for(int u=0;u<n;u++)
21             if(!used[u]&&(v==-1||d[u]<d[v]))
22                 v=u;
23         if(v==-1)
24             break;
25         used[v]=true;
26         for(int u=0;u<n;u++)
27         {
28             d[u]=min(d[u],d[v]+cost[u][v]);
29         }
30     }
31 }
32 
33 int main()
34 {
35     int T,m,n,k,i,j;
36     int from,to,Cost,f,t;
37     cin>>T;
38     while(T--)
39     {
40         memset(cost,INF,sizeof(cost));
41         cin>>n>>m;
42         for(i=0;i<m;i++)
43         {
44             cin>>from>>to>>Cost;
45             cost[from][to]=cost[to][from]=Cost;
46         }
47         cin>>f>>t;
48         dijkstra(n,f);
49         int Time[Max],tim=0;
50         memset(Time,0,sizeof(Time));
51         for(i=0;i<n;i++)
52             Time[i]=d[i];
53         dijkstra(n,t);
54         for(i=0;i<n;i++)
55         {
56             Time[i]+=d[i];
57             tim=max(tim,Time[i]);
58         }
59         cout<<tim<<endl;
60 
61     }
62 }
```















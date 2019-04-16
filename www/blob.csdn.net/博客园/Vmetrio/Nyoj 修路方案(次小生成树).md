# Nyoj  修路方案(次小生成树) - Vmetrio - 博客园







# [Nyoj  修路方案(次小生成树)](https://www.cnblogs.com/wangmengmeng/p/5363859.html)




- 描述

南将军率领着许多部队，它们分别驻扎在N个不同的城市里，这些城市分别编号1~N，由于交通不太便利，南将军准备修路。

现在已经知道哪些城市之间可以修路，如果修路，花费是多少。

现在，军师小工已经找到了一种修路的方案，能够使各个城市都联通起来，而且花费最少。

但是，南将军说，这个修路方案所拼成的图案很不吉利，想让小工计算一下是否存在另外一种方案花费和刚才的方案一样，现在你来帮小工写一个程序算一下吧。


- 输入
第一行输入一个整数T(1<T<20)，表示测试数据的组数
每组测试数据的第一行是两个整数V,E，(3<V<500,10<E<200000)分别表示城市的个数和城市之间路的条数。数据保证所有的城市都有路相连。
随后的E行，每行有三个数字A B L，表示A号城市与B号城市之间修路花费为L。- 输出
对于每组测试数据输出Yes或No（如果存在两种以上的最小花费方案则输出Yes,如果最小花费的方案只有一种，则输出No)- 样例输入
2
3 3
1 2 1
2 3 2
3 1 3
4 4
1 2 2
2 3 2
3 4 2
4 1 2- 样例输出
No
Yes- 来源
[POJ题目改编](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=POJ%E9%A2%98%E7%9B%AE%E6%94%B9%E7%BC%96)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)




题意:次小生成树



WA代码:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


  1  
  2 #include <vector>
  3 #include <map>
  4 #include <set>
  5 #include <algorithm>
  6 #include <iostream>
  7 #include <cstdio>
  8 #include <cmath>
  9 #include <cstdlib>
 10 #include <string>
 11 #include <cstring>
 12 #include <queue>
 13 using namespace std;
 14 
 15 #define MAX 200020
 16 #define INF 0x3f3f3f3f
 17 
 18 struct edge
 19 {
 20     int x,y,cost;
 21     int flag;
 22 };
 23 
 24 edge e[MAX];
 25 int f[MAX];
 26 int dis[MAX];
 27 int v,n,m,ans;
 28 
 29 bool cmp(edge a,edge b)
 30 {
 31     return a.cost<b.cost;
 32 }
 33 
 34 void init(int n)
 35 {
 36     for(int i=0;i<=n;i++)
 37         f[i]=i;
 38 }
 39 
 40 int find(int x)
 41 {
 42     if(x!=f[x])
 43         f[x]=find(f[x]);
 44     return f[x];
 45 }
 46 
 47 void Union(int x,int y)
 48 {
 49     x=find(x);
 50     y=find(y);
 51     f[y]=x;
 52 }
 53 
 54 int same(int x,int y)
 55 {
 56     return find(x)==find(y);
 57 }
 58 
 59 int kruskal_1(int n,int m)
 60 {
 61     int ans=0;
 62     for(int i=0;i<m;i++)
 63     {
 64         if(!same(e[i].x,e[i].y))
 65         {
 66             e[i].flag=1;
 67             Union(e[i].x,e[i].y);
 68             ans+=e[i].cost;
 69         }
 70     }
 71     return ans;
 72 }
 73 
 74 
 75 int kruskal_2(int n,int m)
 76 {
 77     int ans=0,cnt=1;
 78     for(int i=0;i<m;i++)
 79     {
 80         if (i == n)//除去这条边之后再求一次最小生成树
 81            continue;
 82         if(!same(e[i].x,e[i].y))
 83         {
 84             Union(e[i].x,e[i].y);
 85             ans+=e[i].cost;
 86             cnt++;
 87         }
 88     }
 89     if(cnt!=n)
 90         return -1;
 91     else
 92         return ans;
 93 }
 94 
 95 int main()
 96 {
 97     int t;
 98     scanf("%d",&t);
 99     while(t--){
100         init(n);
101         memset(e,0,sizeof(e));
102         int v=0;
103         scanf("%d%d",&n,&m);
104         int count;
105         for(int i=0; i<m; i++){
106             scanf("%d%d%d",&e[i].x,&e[i].y,&e[i].cost);
107             e[i].flag=0;
108         }
109         sort(e,e+m,cmp);
110         int flag=0,prim_2;
111         int prim_1=0;
112 
113         prim_1=kruskal_1(n,m);
114         /*for(int i=0; i<m; i++){
115             cout<<e[i].x<<e[i].y<<e[i].cost<<e[i].flag<<endl;
116         }*/
117 
118         for(int i=0; i<m; i++){
119             if(e[i].flag==0)
120                 continue;
121             prim_2=0;
122             init(n);
123             prim_2=kruskal_2(i,m);
124             if(prim_2==prim_1){
125                 flag=1;
126                 break;
127             }
128         }
129 
130         if(prim_1==prim_2){
131             flag=1;
132         }
133 
134         if(flag)
135             printf("Yes\n");
136         else
137             printf("No\n");
138     }
139 }        


View Code
```



AC代码:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1  
 2 #include<stdio.h>
 3 #include<string.h>
 4 #include<algorithm>
 5 using namespace std;
 6 struct node
 7 {
 8     int u,v,val;
 9     int flog;
10 }edge[200000+10];
11 int pre[1000],m,n,minn;
12 void init()
13 {
14     for(int i=0;i<1000;i++)
15     pre[i]=i;
16 }
17 int cmp(node s1,node s2)
18 {
19     return s1.val<s2.val;
20 }
21 int find(int x)
22 {
23     return pre[x]==x?x:pre[x]=find(pre[x]);
24 } 
25 int F(int w)
26 {
27     int sum=0;
28     for(int i=0;i<m;i++)
29     {
30         if(i!=w)
31         {
32             int fx=find(edge[i].u);
33             int fy=find(edge[i].v);
34             if(fx!=fy)
35             {
36                 pre[fx]=fy;
37                 sum+=edge[i].val;
38             }
39         }
40     }
41     int s=find(1);//判断全部的点是不是已经全部连进去 
42     for(int i=2;i<=n;i++)
43     if(pre[i]!=s)
44     return -1;
45     return sum;
46 }
47 int main()
48 {
49     int t;
50     scanf("%d",&t);
51     while(t--)
52     {
53         init();
54         scanf("%d%d",&n,&m);
55         for(int i=0;i<m;i++)
56         scanf("%d%d%d",&edge[i].u,&edge[i].v,&edge[i].val),edge[i].flog=0;
57         sort(edge,edge+m,cmp);
58         minn=0;
59         for(int i=0;i<m;i++)//找到最小生成树 
60         {
61             int fx=find(edge[i].u);
62             int fy=find(edge[i].v);
63             if(fx!=fy)
64             {
65                 pre[fx]=fy;
66                 edge[i].flog=1;//标记这条边在最小生成树中已经用过 
67                 minn+=edge[i].val;
68             }
69         }
70         int flag=0;
71         for(int i=0;i<m;i++)
72         {
73             if(edge[i].flog)//每次排除一条边 
74             {
75                 init();
76                 if(F(i)==minn)
77                 {
78                     flag=1;
79                     break;
80                 }
81             }
82             if(flag) break;
83         }
84         if(flag) printf("Yes\n");
85         else printf("No\n");
86     }
87     return 0;
88 }        


View Code
```















# Nyoj  三国志(dijkstra+01背包) - Vmetrio - 博客园







# [Nyoj  三国志(dijkstra+01背包)](https://www.cnblogs.com/wangmengmeng/p/5364390.html)





描述

《三国志》是一款很经典的经营策略类游戏。我们的小白同学是这款游戏的忠实玩家。现在他把游戏简化一下，地图上只有他一方势力，现在他只有一个城池，而他周边有一些无人占的空城，但是这些空城中有很多不同数量的同种财宝。我们的小白同学虎视眈眈的看着这些城池中的财宝。

按照游戏的规则，他只要指派一名武将攻占这座城池，里面的财宝就归他所有了。不过一量攻占这座城池，我们的武将就要留守，不能撤回。因为我们的小白手下有无数的武将，所以他不在乎这些。

从小白的城池派出的武将，每走一公理的距离就要消耗一石的粮食，而他手上的粮食是有限的。现在小白统计出了地图上城池间的道路，这些道路都是双向的，他想请你帮忙计算出他能得到 的最多的财宝数量。我们用城池的编号代表城池，规定小白所在的城池为0号城池，其他的城池从1号开始计数。


- 输入
本题包含多组数据：
首先，是一个整数T(1<=T<=20)，代表数据的组数
然后，下面是T组测试数据。对于每组数据包含三行：
第一行：三个数字S,N,M
（1<=S<=1000000,1<=N<=100,1<=M<=10000)
S代表他手中的粮食（石），N代表城池个数，M代表道路条数。
第二行：包含M个三元组行 Ai,Bi,Ci(1<=A,B<=N,1<=C<=100)。
代表Ai,Bi两城池间的道路长度为Ci(公里）。
第三行：包含N个元素，Vi代表第i个城池中的财宝数量。(1<=V<=100)- 输出
每组输出各占一行，输出仅一个整数，表示小白能得到的最大财富值。- 样例输入
2
10 1 1
0 1 3
2
5 2 3
0 1 2 0 2 4 1 2 1
2 3- 样例输出
2
5- 来源
[郑州大学校赛题目](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E9%83%91%E5%B7%9E%E5%A4%A7%E5%AD%A6%E6%A0%A1%E8%B5%9B%E9%A2%98%E7%9B%AE)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)


题解:确定最短路,把最短路做为容量,01背包.....



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
14 #define INF 0x3f3f3f3f
15 #define MAX 111
16 
17 int s,n,m;
18 int mp[MAX][MAX],dis[MAX],vis[MAX];
19 int dp[1000010],v[MAX];
20 
21 void dijkstra()
22 {
23     memset(vis,0,sizeof(vis));
24     for(int i=0; i<=n; i++)
25         dis[i]=mp[0][i];
26     vis[0]=1;
27     dis[0]=0;
28     while(true){
29         int min=INF;
30         int k=0;
31         for(int i=0; i<=n; i++){
32             if(!vis[i] && dis[i]<min){
33                 min=dis[i];
34                 k=i;
35             }
36         }
37         if(min==INF)
38             break;
39         vis[k]=1;
40         for(int i=0; i<=n; i++){
41             if(!vis[i] && dis[i]>dis[k]+mp[k][i])
42                 dis[i]=dis[k]+mp[k][i];
43         }
44     }
45 }
46 
47 void init()
48 {
49     memset(mp,INF,sizeof(mp));
50     memset(dis,INF,sizeof(dis));
51     memset(dp,0,sizeof(dp));
52     memset(v,0,sizeof(v));
53 }
54 
55 int main()
56 {
57     int t,a,b,c;
58     scanf("%d",&t);
59     while(t--){
60         init();
61         scanf("%d%d%d",&s,&n,&m);
62         for(int i=0; i<m; i++){
63             scanf("%d%d%d",&a,&b,&c);
64             mp[a][b]=c;
65             mp[b][a]=c;
66         }
67         for(int j=1; j<=n; j++)
68             scanf("%d",&v[j]);
69         dijkstra();
70 
71         /*for(int i=0;i<=n;i++)
72             printf("%d ",dis[i]);
73         printf("\n");*/
74 
75         for(int i=1; i<=n; i++){
76             for(int j=s; j>=dis[i]; j--){
77                 if(dp[j]<dp[j-dis[i]]+v[i])
78                     dp[j]=dp[j-dis[i]]+v[i];
79             }
80         }
81         printf("%d\n",dp[s]);
82     }
83 }        


View Code
```



AC代码:

```
1 #include <vector>
 2 #include <map>
 3 #include <set>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <cstdio>
 7 #include <cmath>
 8 #include <cstdlib>
 9 #include <string>
10 #include <cstring>
11 #include <queue>
12 using namespace std;
13 #define INF 0x3f3f3f3f
14 #define MAX 111
15 
16 int s,n,m;
17 int mp[MAX][MAX],dis[MAX],vis[MAX];
18 int dp[1000010],v[MAX];
19 
20 void dijkstra()
21 {
22     memset(vis,0,sizeof(vis));
23     for(int i=0; i<=n; i++)
24         dis[i]=mp[0][i];
25     vis[0]=1;
26     dis[0]=0;
27     while(true){
28         int min=INF;
29         int k=0;
30         for(int i=0; i<=n; i++){
31             if(!vis[i] && dis[i]<min){
32                 min=dis[i];
33                 k=i;
34             }
35         }
36         if(min==INF)
37             break;
38         vis[k]=1;
39         for(int i=0; i<=n; i++){
40             if(!vis[i] && dis[i]>dis[k]+mp[k][i])
41                 dis[i]=dis[k]+mp[k][i];
42         }
43     }
44 }
45 
46 void init()
47 {
48     memset(mp,INF,sizeof(mp));
49     memset(dis,INF,sizeof(dis));
50     memset(dp,0,sizeof(dp));
51     memset(v,0,sizeof(v));
52 }
53 
54 int main()
55 {
56     int t,a,b,c;
57     scanf("%d",&t);
58     while(t--){
59         init();
60         scanf("%d%d%d",&s,&n,&m);
61         for(int i=0; i<m; i++){
62             scanf("%d%d%d",&a,&b,&c);
63             if(mp[a][b] > c)
64             mp[a][b]=mp[b][a]=c;
65         }
66         for(int j=1; j<=n; j++)
67             scanf("%d",&v[j]);
68         dijkstra();
69 
70         /*for(int i=0;i<=n;i++)
71             printf("%d ",dis[i]);
72         printf("\n");*/
73 
74         for(int i=1; i<=n; i++){
75             for(int j=s; j>=dis[i]; j--){
76                 if(dp[j]<dp[j-dis[i]]+v[i])
77                     dp[j]=dp[j-dis[i]]+v[i];
78             }
79         }
80         printf("%d\n",dp[s]);
81     }
82 }
```















# Nyoj  城市平乱(图论) - Vmetrio - 博客园







# [Nyoj  城市平乱(图论)](https://www.cnblogs.com/wangmengmeng/p/5299715.html)





描述

南将军统领着N个部队，这N个部队分别驻扎在N个不同的城市。

他在用这N个部队维护着M个城市的治安，这M个城市分别编号从1到M。

现在，小工军师告诉南将军，第K号城市发生了暴乱，南将军从各个部队都派遣了一个分队沿最近路去往暴乱城市平乱。

现在已知在任意两个城市之间的路行军所需的时间，你作为南将军麾下最厉害的程序员，请你编写一个程序来告诉南将军第一个分队到达叛乱城市所需的时间。

![](http://acm.nyist.edu.cn/JudgeOnline/admin/kind/attached/20101122174626_27048.png)

注意，两个城市之间可能不只一条路。


- 输入
第一行输入一个整数T，表示测试数据的组数。(T<20)
每组测试数据的第一行是四个整数N,M,P,Q(1<=N<=100,N<=M<=1000,M-1<=P<=100000)其中N表示部队数，M表示城市数，P表示城市之间的路的条数,Q表示发生暴乱的城市编号。
随后的一行是N个整数，表示部队所在城市的编号。
再之后的P行，每行有三个正整数，a,b,t(1<=a,b<=M,1<=t<=100)，表示a,b之间的路如果行军需要用时为t

数据保证暴乱的城市是可达的。- 输出
对于每组测试数据，输出第一支部队到达叛乱城市时的时间。每组输出占一行- 样例输入
1
3 8 9 8
1 2 3
1 2 1
2 3 2
1 4 2
2 5 3
3 6 2
4 7 1
5 7 3
5 8 2
6 8 2 - 样例输出
4- 来源
[《世界大学生程序设计竞赛高级教程·第一册》改编](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E3%80%8A%E4%B8%96%E7%95%8C%E5%A4%A7%E5%AD%A6%E7%94%9F%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E7%AB%9E%E8%B5%9B%E9%AB%98%E7%BA%A7%E6%95%99%E7%A8%8B%C2%B7%E7%AC%AC%E4%B8%80%E5%86%8C%E3%80%8B%E6%94%B9%E7%BC%96)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)


思想:  把终点作为起点,去找起点......get啦.....



代码:

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
14 #define MAX 1000000 
15 
16 int n,m,p,q;
17 int num[111];
18 int mp[1111][1111];
19 int vis[1111];
20 int dis[1111];
21 
22 void dijkstra(int s)
23 {
24     for(int i=1; i<=m; i++)  
25         dis[i] = mp[s][i];  
26 
27     dis[s]=0;
28     vis[s]=1;
29 
30     for(int i=1; i<=m; i++){
31         int min=INF;
32         int k=0;
33         for(int j=1; j<=m; j++){
34             if(!vis[j] && min>dis[j]){
35                 min=dis[j];
36                 k=j;
37             }
38         }
39         if(min==INF)
40             break;
41         vis[k]=1;
42         for(int j=1; j<=m; j++){
43             if(!vis[j] && dis[j]>min+mp[k][j])
44                 dis[j]=min+mp[k][j];
45         }
46     }
47 }
48 
49 int main()
50 {
51     int t;
52     scanf("%d",&t);
53     while(t--){
54         scanf("%d%d%d%d",&n,&m,&p,&q);
55         memset(mp,INF,sizeof(mp));
56         for(int i=0; i<n; i++){
57             scanf("%d",&num[i]);
58         }
59         int a,b,c;
60         for(int i=0; i<p; i++){
61              scanf("%d %d %d", &a, &b, &c);  
62             if(mp[a][b] > c)  
63                 mp[a][b] = mp[b][a] = c;  
64         }
65         dijkstra(q);
66         int min = INF;
67         for(int i=0; i<n; i++){
68             if(dis[num[i]] < min)
69                 min=dis[num[i]];
70         }
71         printf("%d\n",min);
72         memset(vis,0,sizeof(vis));
73     }
74 }
```















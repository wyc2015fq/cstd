# NSOJ   一个人的旅行(图论) - Vmetrio - 博客园







# [NSOJ   一个人的旅行(图论)](https://www.cnblogs.com/wangmengmeng/p/5308017.html)





虽然草儿是个路痴（就是在杭电待了一年多，居然还会在校园里迷路的人，汗~),但是草儿仍然很喜欢旅行，因为在旅途中 会遇见很多人（白马王子，^0^），很多事，还能丰富自己的阅历，还可以看美丽的风景……草儿想去很多地方，她想要去东京铁塔看夜景，去威尼斯看电影，去阳明山上看海芋，去纽约纯粹看雪景，去巴黎喝咖啡写信，去北京探望孟姜女……眼看寒假就快到了，这么一大段时间，可不能浪费啊，一定要给自己好好的放个假，可是也不能荒废了训练啊，所以草儿决定在要在最短的时间去一个自己想去的地方！因为草儿的家在一个小镇上，没有火车经过，所以她只能去邻近的城市坐火车（好可怜啊~）。




### Input

输入数据有多组，每组的第一行是三个整数T，S和D，表示有T条路，和草儿家相邻的城市的有S个，草儿想去的地方有D个；
接着有T行，每行有三个整数a，b，time,表示a,b城市之间的车程是time小时；(1=<(a,b)<=1000;a,b 之间可能有多条路)
接着的第T+1行有S个数，表示和草儿家相连的城市；
接着的第T+2行有D个数，表示草儿想去地方。




### Output

输出草儿能去某个喜欢的城市的最短时间。




### Sample Input
6 2 3
1 3 5
1 4 7
2 8 12
3 8 4
4 9 12
9 10 2
1 2
8 9 10
### Sample Output
9




多个起点,多个终点,找到最短的起点,终点....枚举遍历即可.....

自己手残,找了好久的bug  变量写串了....orz



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
14 #define MAX 1010
15 
16 int s,d;
17 int dis[MAX],vis[MAX];
18 int mp[MAX][MAX],x[MAX],y[MAX];
19 
20 void dijkstra(int s)
21 {
22     int p;
23     memset(vis,0,sizeof(vis));
24     memset(dis,INF,sizeof(dis));
25     for(int i=1; i<=MAX; i++)  
26         dis[i]=mp[s][i];  
27     dis[s]=0;
28     vis[s]=1;
29     for(int i=1; i<MAX; i++){
30         int min=INF;
31         for(int j=1; j<MAX; j++){
32             if(!vis[j] && min>dis[j]){
33                 min=dis[j];
34                 p=j;
35             }
36         }
37         if(min==INF)
38             break;
39         vis[p]=1;
40         for(int j=1; j<MAX; j++){
41             if(!vis[j] && dis[j]>min+mp[p][j])
42                 dis[j]=min+mp[p][j];
43         }
44     }
45 }
46 
47 int main()
48 {
49     int t,a,b,c;
50     while(~scanf("%d%d%d",&t,&s,&d)){
51         memset(mp,INF,sizeof(mp));
52         for(int i=0; i<t; i++){
53             scanf("%d%d%d",&a,&b,&c);
54             if(mp[a][b]>c)
55                  mp[a][b]=mp[b][a]=c;
56         }
57         for(int i=0; i<s; i++)
58             scanf("%d",&x[i]);
59         for(int i=0; i<d; i++)
60             scanf("%d",&y[i]);
61         int ans=INF;
62         for(int i=0; i<s; i++){
63             dijkstra(x[i]);
64             for(int j=0; j<d; j++){
65                 if(ans>dis[y[j]])
66                     ans=dis[y[j]];
67             }
68         }
69         printf("%d\n",ans);
70     }
71 }
```















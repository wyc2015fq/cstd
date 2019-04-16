# Nyoj  布线问题(并查集&&图论) - Vmetrio - 博客园







# [Nyoj  布线问题(并查集&&图论)](https://www.cnblogs.com/wangmengmeng/p/5300966.html)





描述南阳理工学院要进行用电线路改造，现在校长要求设计师设计出一种布线方式，该布线方式需要满足以下条件：
1、把所有的楼都供上电。
2、所用电线花费最少


- 输入
第一行是一个整数n表示有n组测试数据。(n<5)
每组测试数据的第一行是两个整数v,e.
v表示学校里楼的总个数(v<=500)
随后的e行里，每行有三个整数a,b,c表示a与b之间如果建铺设线路花费为c(c<=100)。（哪两栋楼间如果没有指明花费，则表示这两栋楼直接连通需要费用太大或者不可能连通）
随后的1行里，有v个整数,其中第i个数表示从第i号楼接线到外界供电设施所需要的费用。( 0<e<v*(v-1)/2 )
（楼的编号从1开始），由于安全问题，只能选择一个楼连接到外界供电设备。
数据保证至少存在一种方案满足要求。- 输出
每组测试数据输出一个正整数,表示铺设满足校长要求的线路的最小花费。- 样例输入
1
4 6
1 2 10
2 3 10
3 1 10
1 4 1
2 4 1
3 4 1
1 3 5 6- 样例输出
4- 来源
[[张云聪]原创](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%5B%E5%BC%A0%E4%BA%91%E8%81%AA%5D%E5%8E%9F%E5%88%9B)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)


自己定义的排序函数没有写在前边  Runtime Error多次......

排序之后,用并查集即可,,,Prim也可以处理....



并查集代码:

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
16 int v;
17 int dis[555];
18 struct node{
19     int a,b,cost;
20 }c[555555];
21 
22 bool cmp(node x,node y)
23 {
24     return x.cost<y.cost;
25 }
26 
27 void init()
28 {
29     for(int i=1; i<=v; i++){
30         dis[i]=i;
31     }
32 }
33 
34 int find(int x)
35 {
36     if(dis[x]!=x)
37         dis[x]=find(dis[x]);
38     return dis[x];
39 }
40 
41 int main()
42 {
43     int n,e;
44     scanf("%d",&n);
45     while(n--){
46         int min=INF,m,sum=0;
47         scanf("%d%d",&v,&e);
48         init();
49         for(int i=0; i<e; i++){
50             scanf("%d%d%d",&c[i].a,&c[i].b,&c[i].cost);
51         }
52         for(int i=0; i<v; i++){
53             scanf("%d",&m);
54             if(min>m)
55                 min=m;
56         }
57         sort(c,c+e,cmp);
58         for(int i=0; i<e; i++){
59             int x=find(c[i].a);
60             int y=find(c[i].b);
61             if(x!=y){
62                 dis[x]=y;
63                 sum+=c[i].cost;
64             }
65         }
66         printf("%d\n",sum+min);
67     }
68     return 0;
69 }
```





Prim代码:

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
16 int dis[555],vis[555],mp[555][555];
17 int v;
18 
19 int Prim()
20 {
21     fill(dis,dis+v,INF);
22     memset(vis,0,sizeof(vis));
23     int n=1,ans=0;
24     for(int i=1; i<=v; i++){
25         if(i!=n)
26             dis[i]=mp[n][i];
27     }
28     vis[1]=1;
29     while(true){
30         int min=INF;
31         for(int j=1; j<=v; j++){
32             if(!vis[j] && min>dis[j]){
33                 min=dis[j];
34                 n=j;
35             }
36         }
37         if(min==INF)
38             break;
39         ans+=min;
40         vis[n]=1;
41         for(int i=1; i<=v; i++){
42             if(!vis[i] && dis[i]>mp[n][i])
43                 dis[i]=mp[n][i];
44         }
45     }
46     return ans;
47 }
48 
49 int main()
50 {
51     int n,e,i,x,y,cost;
52     scanf("%d",&n);
53     while(n--){
54         int sum=0,min=INF,m;
55         memset(mp,INF,sizeof(mp));
56         scanf("%d%d",&v,&e);
57         for(i=0; i<e; i++){
58             scanf("%d%d%d",&x,&y,&cost);
59             mp[x][y]=mp[y][x]=cost;
60         }
61         for(i=0; i<v; i++){
62             scanf("%d",&m);
63             if(min>m)
64                 min=m;
65         }
66         sum=min+Prim();
67         printf("%d\n",sum);
68     }
69 }
```















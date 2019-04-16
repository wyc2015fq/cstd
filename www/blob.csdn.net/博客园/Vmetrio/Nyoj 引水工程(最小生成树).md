# Nyoj  引水工程(最小生成树) - Vmetrio - 博客园







# [Nyoj  引水工程(最小生成树)](https://www.cnblogs.com/wangmengmeng/p/5331209.html)





描述

南水北调工程是优化水资源配置、促进区域协调发展的基础性工程，是新中国成立以来投资额最大、涉及面最广的战略性工程，事关中华民族长远发展。“[南水北调工程](http://baike.baidu.com/view/51108.htm)”，旨在缓解中国[华北](http://baike.baidu.com/subview/144248/5062655.htm)和[西北](http://baike.baidu.com/subview/393189/11037469.htm)地区水资源短缺的国家战略性工程。就是把中国长江流域丰盈的水资源抽调一部分送到华北和西北地区。我国南涝北旱，南水北调工程通过跨流域的[水资源](http://baike.baidu.com/view/16491.htm)合理配置，促进南北方经济、社会与人口、资源、环境的协调发展。

整个工程分东线、中线、西线三条调水线。东线工程位于东部，因地势低需抽水北送至[华北地区](http://baike.baidu.com/view/417518.htm)。中线工程从[汉水](http://baike.baidu.com/view/18683.htm)与其最大支流[丹江](http://baike.baidu.com/subview/74729/11988878.htm)交汇处的[丹江口水库](http://baike.baidu.com/view/188128.htm)引水，自流供水给[黄淮海平原](http://baike.baidu.com/view/416642.htm)大部分地区，20多座大中城市；西线工程在[青藏高原](http://baike.baidu.com/subview/4979/5043819.htm)上，由[长江](http://baike.baidu.com/subview/4185/4998436.htm)上游向黄河上游补水。

现在有N个区域需要建设水资源工程，它们可以自建水库解决缺水问题，也可以从已有水源的地区建立管道引水过来。当然，这些建设都需要大量投资。

你能不能给出一个优化水资源配置方案，在保证每个区域都能用上水的前提下，使得整个引水工程费用最低。


- 输入
第一行： K 表示有多少组测试数据。
接下来对每组测试数据：
第1行: N 表示有N个区域（ 1<=N<=300 ）
第2 行： W1 W2 …. WN Wi表示第i个区域自建水库需要的费用
再有N行： Pi1 Pi2 …. Pin Pij表示建立第i个区域与第j个区域引水管道的费用- 输出
对于每组测试数据，输出占一行，即建立整个引水工程的最小费用。- 样例输入
1
5
5 4 4 3 6
0 2 2 2 2
2 0 3 3 3
2 3 0 4 5
2 3 4 0 1
2 3 5 1 0
- 样例输出
10- 来源
[第八届河南省程序设计大赛](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E7%AC%AC%E5%85%AB%E5%B1%8A%E6%B2%B3%E5%8D%97%E7%9C%81%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E5%A4%A7%E8%B5%9B)- 上传者
[hnu_acm](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=hnu_acm)




题解:    找到建立水库的最小值,以此建立最小生成树.......蒟蒻啊   ....自己刚开始的思路想错了.....



WA代码:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


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
14 #define MAX 100010
15 
16 int n,ans,minn;
17 int quyu[333],vis[333],dis[333];
18 int mp[333][333];
19 
20 void prim()
21 {
22     memset(vis,0,sizeof(vis));
23     memset(dis,INF,sizeof(dis)); 
24     dis[1]=0;
25     ans=0;
26     dis[0]=INF;
27     while(true){
28         int m=0;
29         for(int i=1; i<=n; i++){
30             if(!vis[i] && dis[i]<dis[m])
31                 m=i;
32         }
33         if(m==0)
34             break;
35         vis[m]=1;
36         ans+=dis[m];
37         for(int i=1; i<=n; i++)
38             dis[i]=min(dis[i],mp[m][i]);
39     }
40 }
41 
42 void init()
43 {
44     memset(mp,INF,sizeof(mp));
45     memset(quyu,0,sizeof(quyu));
46     minn=INF;
47 }
48 
49 int main()
50 {
51     int t;
52     scanf("%d",&t);
53     while(t--){
54         init();
55         scanf("%d",&n);
56         for(int i=0; i<n; i++){
57             scanf("%d",&quyu[i]);
58             if(minn>quyu[i])
59                 minn=quyu[i];
60         }
61         for(int i=1; i<=n; i++)
62             for(int j=1; j<=n; j++){
63                 scanf("%d",&mp[i][j]);
64             }
65         prim();
66         printf("%d\n",ans+minn);
67     }
68 }        


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
14 #define MAX 100010
15 
16 int n,ans,minn;
17 int quyu[333],vis[333],dis[333];
18 int mp[333][333];
19 
20 void prim()
21 {
22     memset(vis,0,sizeof(vis));
23     memset(dis,INF,sizeof(dis));
24     for(int i=0;i<=n;i++)
25         dis[i]=mp[0][i]; 
26     ans=0;
27     //dis[0]=INF;
28     while(true){
29         int m=0;
30         for(int i=1; i<=n; i++){
31             if(!vis[i] && dis[i]<dis[m])
32                 m=i;
33         }
34         if(m==0)
35             break;
36         vis[m]=1;
37         ans+=dis[m];
38         for(int i=1; i<=n; i++)
39             dis[i]=min(dis[i],mp[m][i]);
40     }
41 }
42 
43 void init()
44 {
45     memset(mp,INF,sizeof(mp));
46     memset(quyu,0,sizeof(quyu));
47     minn=INF;
48 }
49 
50 int main()
51 {
52     int t;
53     scanf("%d",&t);
54     while(t--){
55         init();
56         scanf("%d",&n);
57         for(int i=1; i<=n; i++){
58             scanf("%d",&mp[0][i]);
59         }
60         for(int i=1; i<=n; i++)
61             for(int j=1; j<=n; j++){
62                 scanf("%d",&mp[i][j]);
63             }
64         prim();
65         printf("%d\n",ans);
66     }
67 }
```



别人的克鲁斯卡尔A的:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <string.h>
 4 #define Maxsize 310
 5 #define INF 0x3f3f3f3f
 6 int n;
 7 int p[Maxsize][Maxsize];
 8 int par[Maxsize];
 9 int rank[Maxsize];
10 void init()
11 {
12     memset(p,0x3f,sizeof(p));
13     for(int i=0;i<=n;i++)
14     {
15         par[i]=i;
16         rank[i]=0;
17     }
18 }
19 int find(int x)
20 {
21     if(par[x]==x)
22         return x;
23     else return par[x]=find(par[x]);
24 }
25 void unite(int x,int y)
26 {
27     x=find(x);
28     y=find(y);
29     if(x==y)
30         return;
31     if(rank[x]<rank[y])
32         par[x]=y;
33     else
34     { 
35         par[y]=x;
36         if(rank[x]==rank[y])
37             rank[x]++;
38     }
39 }
40 bool same(int x,int y)
41 {
42     return find(x)==find(y);
43 }
44 int FindTree()
45 {
46     int res=0;
47     int a,b;
48     for(;;)
49     {
50         int min=INF;
51         for(int i=0;i<=n;i++)
52             for(int j=i+1;j<=n;j++)
53             {
54                 if(min>p[i][j]&&(!same(i,j)))
55                 {
56                     min=p[i][j];
57                     a=i;b=j;
58                 }
59             }
60         if(min==INF)
61             break;
62     //    printf("%d\n",min);
63         res+=p[a][b];
64         unite(a,b);
65     }
66     return res;
67 }
68 int main()
69 {
70     int K;
71     int i,j;
72     scanf("%d",&K);
73     while(K--)
74     {
75         scanf("%d",&n);
76         init();
77         for(i=1;i<=n;i++)
78         {
79             scanf("%d",&p[0][i]);
80             p[i][0]=p[0][i];
81         }
82         for(i=1;i<=n;i++)
83             for(j=1;j<=n;j++)
84                 scanf("%d",&p[i][j]);
85         printf("%d\n",FindTree());
86     }
87     return 0;
88 }


View Code
```















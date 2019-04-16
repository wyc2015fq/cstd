# NSOJ   Constructing Roads(图论) - Vmetrio - 博客园







# [NSOJ   Constructing Roads(图论)](https://www.cnblogs.com/wangmengmeng/p/5307995.html)





There are N villages, which are numbered from 1 to N, and you should build some roads such that every two villages can connect to each other. We say two village A and B are connected, if and only if there is a road between A and B, or there exists a village C such that there is a road between A and C, and C and B are connected. 

We know that there are already some roads between some villages and your job is the build some roads such that all the villages are connect and the length of all the roads built is minimum.




### Input

The first line is an integer N (3 <= N <= 100), which is the number of villages. Then come N lines, the i-th of which contains N integers, and the j-th of these N integers is the distance (the distance should be an integer within [1, 1000]) between village i and village j.

Then there is an integer Q (0 <= Q <= N * (N + 1) / 2). Then come Q lines, each line contains two integers a and b (1 <= a < b <= N), which means the road between village a and village b has been built.




### Output

You should output a line contains an integer, which is the length of all the roads to be built such that all the villages are connected, and this value is minimum. 




### Sample Input
3
0 990 692
990 0 179
692 179 0
1
1 2
### Sample Output
179


最小生成树....先建图,之后再处理相同地点....只需要遍历一半就行....



时间超限:

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
14 #define MAX 1000000 
15 
16 int n,ans;
17 int dis[111],vis[111],mp[111][111];
18 
19 void prim()
20 {
21     memset(vis,0,sizeof(vis));
22     memset(dis,INF,sizeof(dis)); 
23     dis[1]=0;ans=0;dis[0]=INF;
24     while(true){
25         int m=0;
26         for(int i=1; i<=n; i++){
27             if(!vis[i] && (dis[i]<dis[m]))
28                 m=i;
29         }
30         if(m==0)
31             break;
32         vis[m]=1;
33         ans+=dis[m];
34         for(int i=1; i<=n; i++)
35             dis[i]=min(dis[i],mp[m][i]);
36     }
37 }
38 
39 int main()
40 {
41     int x,a,b;
42     while(scanf("%d",&n)){
43         if(n==1)
44             break;
45         for(int i=1; i<=n; i++){
46             for(int j=1; j<=n; j++){
47                 scanf("%d",&mp[i][j]);
48             }
49         }
50         scanf("%d",&x);
51         while(x--){
52             scanf("%d%d",&a,&b);
53             mp[a][b]=mp[b][a]=0;
54         }
55         prim();
56         printf("%d\n",ans);
57     }
58 
59 }


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
14 #define MAX 1000000 
15 
16 int n,ans;
17 int dis[111],vis[111],mp[111][111];
18 
19 void prim()
20 {
21     memset(vis,0,sizeof(vis));
22     memset(dis,INF,sizeof(dis)); 
23     dis[1]=0;
24     ans=0;
25     dis[0]=INF;
26     while(true){
27         int m=0;
28         for(int i=1; i<=n; i++){
29             if(!vis[i] && dis[i]<dis[m])
30                 m=i;
31         }
32         if(m==0)
33             break;
34         vis[m]=1;
35         ans+=dis[m];
36         for(int i=1; i<=n; i++)
37             dis[i]=min(dis[i],mp[m][i]);
38     }
39 }
40 
41 int main()
42 {
43     int x,a,b;
44     while(scanf("%d",&n)==1){
45         for(int i=1; i<=n; i++){
46             for(int j=1; j<=n; j++){
47                 scanf("%d",&mp[i][j]);
48             }
49         }
50         scanf("%d",&x);
51         while(x--){
52             scanf("%d%d",&a,&b);
53             mp[a][b]=mp[b][a]=0;
54         }
55         prim();
56         printf("%d\n",ans);
57     }
58 
59 }
```















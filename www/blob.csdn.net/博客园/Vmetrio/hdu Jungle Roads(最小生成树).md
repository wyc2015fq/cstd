# hdu  Jungle Roads(最小生成树) - Vmetrio - 博客园







# [hdu  Jungle Roads(最小生成树)](https://www.cnblogs.com/wangmengmeng/p/5289149.html)





Problem Description

![](http://acm.hdu.edu.cn/data/images/1301-1.gif)
The Head Elder of the tropical island of Lagrishan has a problem. A burst of foreign aid money was spent on extra roads between villages some years ago. But the jungle overtakes roads relentlessly, so the large road network is too expensive to maintain. The Council of Elders must choose to stop maintaining some roads. The map above on the left shows all the roads in use now and the cost in aacms per month to maintain them. Of course there needs to be some way to get between all the villages on maintained roads, even if the route is not as short as before. The Chief Elder would like to tell the Council of Elders what would be the smallest amount they could spend in aacms per month to maintain roads that would connect all the villages. The villages are labeled A through I in the maps above. The map on the right shows the roads that could be maintained most cheaply, for 216 aacms per month. Your task is to write a program that will solve such problems. 

The input consists of one to 100 data sets, followed by a final line containing only 0. Each data set starts with a line containing only a number n, which is the number of villages, 1 < n < 27, and the villages are labeled with the first n letters of the alphabet, capitalized. Each data set is completed with n-1 lines that start with village labels in alphabetical order. There is no line for the last village. Each line for a village starts with the village label followed by a number, k, of roads from this village to villages with labels later in the alphabet. If k is greater than 0, the line continues with data for each of the k roads. The data for each road is the village label for the other end of the road followed by the monthly maintenance cost in aacms for the road. Maintenance costs will be positive integers less than 100. All data fields in the row are separated by single blanks. The road network will always allow travel between all the villages. The network will never have more than 75 roads. No village will have more than 15 roads going to other villages (before or after in the alphabet). In the sample input below, the first data set goes with the map above. 

The output is one integer per line for each data set: the minimum cost in aacms per month to maintain a road system that connect all the villages. Caution: A brute force solution that examines every possible set of roads will not finish within the one minute time limit. 





Sample Input



Problem Description

![](http://acm.hdu.edu.cn/data/images/1301-1.gif)
The Head Elder of the tropical island of Lagrishan has a problem. A burst of foreign aid money was spent on extra roads between villages some years ago. But the jungle overtakes roads relentlessly, so the large road network is too expensive to maintain. The Council of Elders must choose to stop maintaining some roads. The map above on the left shows all the roads in use now and the cost in aacms per month to maintain them. Of course there needs to be some way to get between all the villages on maintained roads, even if the route is not as short as before. The Chief Elder would like to tell the Council of Elders what would be the smallest amount they could spend in aacms per month to maintain roads that would connect all the villages. The villages are labeled A through I in the maps above. The map on the right shows the roads that could be maintained most cheaply, for 216 aacms per month. Your task is to write a program that will solve such problems. 

The input consists of one to 100 data sets, followed by a final line containing only 0. Each data set starts with a line containing only a number n, which is the number of villages, 1 < n < 27, and the villages are labeled with the first n letters of the alphabet, capitalized. Each data set is completed with n-1 lines that start with village labels in alphabetical order. There is no line for the last village. Each line for a village starts with the village label followed by a number, k, of roads from this village to villages with labels later in the alphabet. If k is greater than 0, the line continues with data for each of the k roads. The data for each road is the village label for the other end of the road followed by the monthly maintenance cost in aacms for the road. Maintenance costs will be positive integers less than 100. All data fields in the row are separated by single blanks. The road network will always allow travel between all the villages. The network will never have more than 75 roads. No village will have more than 15 roads going to other villages (before or after in the alphabet). In the sample input below, the first data set goes with the map above. 

The output is one integer per line for each data set: the minimum cost in aacms per month to maintain a road system that connect all the villages. Caution: A brute force solution that examines every possible set of roads will not finish within the one minute time limit. 




Sample Input


9
A 2 B 12 I 25
B 3 C 10 H 40 I 8
C 2 D 18 G 55
D 1 E 44
E 2 F 60 G 38
F 0
G 1 H 35
H 1 I 35
3
A 2 B 10 C 40
B 1 C 20
0



Sample Output


216
30





Source

[Mid-Central USA 2002](http://acm.hdu.edu.cn/search.php?field=problem&key=Mid-Central+USA+2002&source=1&searchmode=source)









Sample Output


216
30






Source

[Mid-Central USA 2002](http://acm.hdu.edu.cn/search.php?field=problem&key=Mid-Central+USA+2002&source=1&searchmode=source)





Prim算法   促进理解了最小生成树的构建  找边



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
13 
14 #define INF 0x3f3f3f3f;
15 int cost[27][27],mincost[27],ans,cnt;
16 int n,m;
17 bool vis[27];
18 
19 void prim()
20 {
21     memset(vis,false,sizeof(vis));
22     memset(mincost,0x3f,sizeof(mincost));
23     ans=cnt=mincost[1]=0;
24     while(true){
25         m=0;
26         for(int i=1; i<=n; i++){
27             if(!vis[i] && mincost[i]<mincost[m])
28                 m=i;
29         }
30         if(m==0)
31             break;
32         vis[m]=true;
33         ans+=mincost[m];
34         for(int i=1; i<=n; i++){
35             mincost[i]=min(mincost[i],cost[m][i]);
36         }
37     }
38 }
39 
40 int main()
41 {
42     int t,s1,s2;
43     char a[3],b[3];
44     while(~scanf("%d",&n),n){
45         memset(cost,0x3f,sizeof(cost));
46         for(int i=1; i<n; i++){
47             scanf("%s %d",a,&t);
48             s1=a[0]-'A'+1;
49             for(int j=0; j<t; j++){
50                 scanf("%s%d",b,&s2);
51                 cost[s1][b[0]-'A'+1]=cost[b[0]-'A'+1][s1]=s2;
52             }
53         }
54         prim();
55         printf("%d\n",ans);
56     }
57 }
```















# Nyoj  Fire Station - Vmetrio - 博客园







# [Nyoj  Fire Station](https://www.cnblogs.com/wangmengmeng/p/5367819.html)





描述A city is served by a number of fire stations. Some residents have complained that the distance from their houses to the nearest station is too far, so a new station is to be built. You are to choose the location of the fire station so as to reduce the distance to the nearest station from the houses of the disgruntled residents. 

The city has up to 500 intersections, connected by road segments of various lengths. No more than 20 road segments intersect at a given intersection. The location of houses and firestations alike are considered to be at intersections (the travel distance from the intersection to the actual building can be discounted). Furthermore, we assume that there is at least one house associated with every intersection. There may be more than one firestation per intersection.




- 输入
The first line of input contains two positive integers: f,the number of existing fire stations (f <= 100) and i, the number of intersections (i <= 500). The intersections are numbered from 1 to i consecutively. f lines follow; each contains the intersection number at which an existing fire station is found. A number of lines follow, each containing three positive integers: the number of an intersection, the number of a different intersection, and the length of the road segment connecting the intersections. All road segments are two-way (at least as far as fire engines are concerned), and there will exist a route between any pair of intersections.

Subsequent test cases are separated with a single blank line.

The number of test cases are less than 200.- 输出
You are to output a single integer for each test case: the lowest intersection number at which a new fire station should be built so as to minimize the maximum distance from any intersection to the nearest fire station.- 样例输入
1 6
2
1 2 10
2 3 10
3 4 10
4 5 10
5 6 10
6 1 10
- 样例输出
5- 来源
[University of Waterloo Local Contest 199](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=University+of+Waterloo+Local+Contest+199)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)


无奈...自己的程序POJ可以过....理工的过不去...POJ(可以用Floyd...水过)



标程代码:

```
1  
 2 #include <iostream>
 3 #include <stdio.h>
 4 #include <string.h>
 5 #include <queue>
 6 using namespace std;
 7 #define maxN 510
 8 #define MAX 0x0fffffff
 9 int max(int x,int y){return x>y?x:y;}
10 struct point{
11     int v,nex,w;
12 }po[maxN*maxN];
13 int num,N,M,head[maxN],disY[maxN],disX[maxN],key[maxN],flag[maxN];
14 bool vis[maxN];
15 void insert(int u,int v,int w)
16 {
17     po[num].v=v;
18     po[num].w=w;
19     po[num].nex=head[u];
20     head[u]=num++;
21 }
22 void spfa(int soure,int* dis)
23 {
24     memset(vis,false,sizeof(vis));
25     queue<int>q;
26     q.push(soure);
27     vis[soure]=true;dis[soure]=0;
28     while(!q.empty())
29     {
30         int u=q.front();q.pop();vis[u]=false;
31         for(int i=head[u];i!=-1;i=po[i].nex)
32             if(dis[po[i].v]>dis[u]+po[i].w){
33                 dis[po[i].v]=dis[u]+po[i].w;
34                 if(!vis[po[i].v]){
35                     q.push(po[i].v);
36                     vis[po[i].v]=true;
37                 }
38             }
39     }
40 }
41 int main()
42 {
43     //freopen("1.txt","r",stdin);
44     char s[30];
45     while(~scanf("%d%d",&M,&N))
46     {
47         getchar();
48         int i,j,u,v,w,k=1,minn=MAX;
49         num=0;
50         memset(head,-1,sizeof(head));
51         memset(flag,0,sizeof(flag));
52         for(i=1;i<=M;i++){scanf("%d",&key[i]);getchar();}
53         if(N==1){printf("1\n");continue;}
54         while(gets(s)!=NULL&&strlen(s)){sscanf(s,"%d%d%d",&u,&v,&w);insert(u,v,w);insert(v,u,w);}
55         for(i=0;i<=N;i++)disX[i]=MAX;
56         for(i=1;i<=M;i++)spfa(key[i],disX);               
57         for(j=1;j<=N;j++){
58             int maxx=0;                           
59             if(disX[j]==0)continue;
60             for(i=1;i<=N;i++)disY[i]=disX[i];
61             spfa(j,disY);
62             for(i=1;i<=N;i++)maxx=max(maxx,disY[i]);
63             if(minn>maxx||maxx==0){k=j;minn=maxx;}                
64         }
65         printf("%d\n",k);
66     }
67 }
```





POJ:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 #include<cstdio>
 2 #include<iostream>
 3 #include<algorithm>
 4 #include<cstring>
 5 using namespace std;
 6 #define maxn 501
 7 #define INF 0x3f3f3f3f
 8 int n,m,cost[maxn][maxn],dis[maxn],vis[maxn];
 9 int main()
10 {
11     while(~scanf("%d%d",&m,&n))
12     {
13         int u,v,c;
14         for(int i=1;i<=n;i++)
15         {
16             dis[i]=INF;vis[i]=0;
17             for(int j=1;j<=n;j++)
18                 cost[i][j]=i==j?0:INF;
19         }
20         for(int i=1;i<=m;i++)
21         {
22             scanf("%d",&u);
23             dis[u]=0;
24             vis[u]=1;
25         }
26         while(~scanf("%d%d%d",&u,&v,&c))
27             cost[u][v]=cost[v][u]=c;
28         for(int k=1;k<=n;k++)
29             for(int i=1;i<=n;i++)   
30                 for(int j=1;j<=n;j++)   
31                     cost[i][j]=min(cost[i][j],cost[i][k]+cost[k][j]);
32         for(int i=1;i<=n;i++)
33             if(vis[i])
34                 for(int j=1;j<=n;j++)
35                     dis[j]=min(dis[j],cost[i][j]);
36         int ans=INF,pos;
37         for(int i=1;i<=n;i++)
38         {
39             int temp=-1;
40             for(int j=1;j<=n;j++)
41                 temp=max(temp,min(dis[j],cost[i][j]));
42             if(ans>temp)ans=temp,pos=i;
43         }
44         printf("%d\n",pos);
45     }
46     return 0;
47 }


View Code
```















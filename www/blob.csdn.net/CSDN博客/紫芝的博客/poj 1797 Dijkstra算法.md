# poj 1797 Dijkstra算法 - 紫芝的博客 - CSDN博客





2018年04月24日 21:24:37[紫芝](https://me.csdn.net/qq_40507857)阅读数：25








# 题意：
给定n个顶点，以及m条边，每条边包括：起点，终点，权重
现在要从顶点出发到达顶点n，求路径中能够承受的最大权重 


# 思路：
本题要求出从顶点1到n的所有的可行路径中各边权值的最小值的最大值

max（min(可行路径)），最短路径的变形

```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std ;
#define N 1005
#define inf 1000005
int edge[N][N];
int dis[N];//从1到其余点的可行路径上边权值最小值得最大值 
bool vis[N];//标记是否已经求出dis 
int n,m;
void Dijkstra()
{	
	memset(vis,0,sizeof(vis));
	dis[1]=0;
	vis[1]=1;
	for(int i=2;i<=n;i++)
	dis[i]=edge[1][i];
	int maxint,ind;

	for(int i=1;i<n;i++)//求出每个顶点的dis值 
	{
		maxint=-inf,ind=1;
		for(int j=1;j<=n;j++)//求出最大的dis值
		if(!vis[j]&&dis[j]>maxint)
		{
			maxint=dis[j];
			ind=j;
		 } 
		vis[ind]=1;//标记已经求出
		 for(int j=1;j<=n;j++)//扩展各顶点 
		  if(!vis[j]&&(min(dis[ind],edge[ind][j]))>dis[j])
		   dis[j]=min(dis[ind],edge[ind][j]);
		}	
	 printf("%d\n\n",dis[n]);
}
int main()
{
	ios::sync_with_stdio(0);
	int t,kase=1;
	cin>>t;
	while(t--){
		
		cin>>n>>m;
		//初始化为负无穷 
		for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
		edge[i][j]=-inf;
	
		for(int i=0;i<m;i++)
		{
			int a,b,d;
			cin>>a>>b>>d;
			edge[a][b]=edge[b][a]=d;
		}
		printf("Scenario #%d:\n",kase++);
		Dijkstra(); 
	}
	return 0;
}
```





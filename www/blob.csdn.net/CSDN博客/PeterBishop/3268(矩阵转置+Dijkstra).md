# 3268(矩阵转置+Dijkstra) - PeterBishop - CSDN博客





2019年01月21日 17:11:58[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：13
个人分类：[POJ](https://blog.csdn.net/qq_40061421/article/category/7589386)









求两次最短路，第一次求x到其余各点的最短路，第二次求各点到x的最短路。前者易于解决，直接应用Dijkstra或其他最短路算法即可，后者要先将邻接矩阵转置再执行最短路算法。

为什么进行矩阵转置？比如u（u ！= x）到x的最短路为<u,v1>,<v1,v2>,<v2,v3>,...,<vi, x>，这条路径在转置邻接矩阵后变成<x,vi>,...,<v3,v2>,<v2, v1>,<v1,u>.于是乎，在转置邻接矩阵后，执行最短路算法求出x到u的最短路<x,vi>,...,<v3,v2>,<v2, v1>,<v1,u>即可得到转置前u到x的最短路。

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
#define INF 0x3f3f3f3f
#define maxn 1010
int map[maxn][maxn],n;
int way[maxn],dis[maxn];
 
void dijkstra(int x)
{
	int visit[maxn],i,j,min,next=x;
	memset(visit,0,sizeof(visit));
	for(i=1;i<=n;++i)
		dis[i]=map[x][i];
	visit[x]=1;
	for(i=2;i<=n;++i)
	{
		min=INF;
		for(j=1;j<=n;++j)
		{
			if(!visit[j]&&dis[j]<min)
			{
				min=dis[j];
				next=j;
			}
		}
		visit[next]=1;
		for(j=1;j<=n;++j)
		{
			if(!visit[j]&&dis[j]>dis[next]+map[next][j])
				dis[j]=dis[next]+map[next][j];
		}
	}
}
 
int main()
{
	int m,x,i,j,a,b,t;
	while(scanf("%d%d%d",&n,&m,&x)!=EOF)
	{
		for(i=1;i<=n;++i)
		{
			for(j=1;j<=n;++j)
			{
				if(i!=j)
					map[i][j]=INF;
				else
					map[i][j]=0;
			}
		}
		while(m--)
		{
			scanf("%d%d%d",&a,&b,&t);
			if(t<map[a][b])
				map[a][b]=t;
		}
		dijkstra(x);
		for(i=1;i<=n;++i)
			way[i]=dis[i];
		int ans=0;
		for(i=1;i<=n;++i)//置换矩阵的值，也就是交换map[i][j]与map[j][i]的值 
		{
			for(j=i+1;j<=n;++j)
			{
				int cnt;
				cnt=map[j][i];
				map[j][i]=map[i][j];
				map[i][j]=cnt;
			}
		}
		dijkstra(x);
		for(i=1;i<=n;++i)
		{
			if(i!=x)
				ans=max(ans,way[i]+dis[i]);
		} 
		printf("%d\n",ans);
	}
	return 0;
}
```






# POJ2516 Minimum Cost  最小费用最大流 - 紫芝的博客 - CSDN博客





2018年10月06日 15:15:37[紫芝](https://me.csdn.net/qq_40507857)阅读数：26








### [POJ2516-Minimum Cost](https://blog.csdn.net/lyy289065406/article/details/6742534)

Dearboy, a goods victualer, now comes to a big problem, and he needs your help. In his sale area there are N shopkeepers (marked from 1 to N) which stocks goods from him.Dearboy has M supply places (marked from 1 to M), each provides K different kinds of goods (marked from 1 to K). Once shopkeepers order goods, Dearboy should arrange which supply place provide how much amount of goods to shopkeepers to cut down the total cost of transport. 


It's known that the cost to transport one unit goods for different kinds from different supply places to different shopkeepers may be different. Given each supply places' storage of K kinds of goods, N shopkeepers' order of K kinds of goods and the cost to transport goods for different kinds from different supply places to different shopkeepers, you should tell how to arrange the goods supply to minimize the total cost of transport.

### Input

The input consists of multiple test cases. The first line of each test case contains three integers N, M, K (0 < N, M, K < 50), which are described above. The next N lines give the shopkeepers' orders, with each line containing K integers (there integers are belong to [0, 3]), which represents the amount of goods each shopkeeper needs. The next M lines give the supply places' storage, with each line containing K integers (there integers are also belong to [0, 3]), which represents the amount of goods stored in that supply place. 


Then come K integer matrices (each with the size N * M), the integer (this integer is belong to (0, 100)) at the i-th row, j-th column in the k-th matrix represents the cost to transport one unit of k-th goods from the j-th supply place to the i-th shopkeeper. 


The input is terminated with three "0"s. This test case should not be processed.

### Output

For each test case, if Dearboy can satisfy all the needs of all the shopkeepers, print in one line an integer, which is the minimum cost; otherwise just output "-1".

### Sample Input

```
1 3 3   
1 1 1
0 1 1
1 2 2
1 0 1
1 2 3
1 1 1
2 1 1

1 1 1
3
2
20

0 0 0
```

### Sample Output

```
4
-1
```

**题意**：

**给出n个客户对k种商品的需求量，又给出m个仓库对k种物品的存货量，以及对k种物品从i仓库到j客户的一个物品的运费价格，判断是否可以满足客户需求；如果满足，求出最小的运费；如果不满足，输出 -1.**

![](http://hi.csdn.net/attachment/201109/2/0_1314935618nv9Y.gif)

**思路**：

** 简单的最小费用最大流，注意建图细节即可**

**  首先必须判断m个仓库是否有足够的k种商品给n个客户,如果不足,那么明显就是不行的. **

下面假设仓库的商品足够的话:

** 对于每一种商品我们都算出满足满足顾客需求量的最小运费即可.**

**所以我们对K种商品分开处理如下,假设当前处理第x种商品,建图如下:**

       源点s编号0, m个仓库编号1到m, n个顾客编号m+1到m+n, 汇点编号m+n+1.
-        从源点s到每个仓库i有边(s, i, 仓库i对商品x的存货量, 0)
-        从每个仓库i到顾客j有边(i, j, INF, 仓库i到顾客j的单位X商品的运费)
-        从每个顾客j到汇点t有边(j, t, 顾客j对X商品的需求量, 0)

** 然后求最小费用最大流即可求出N个顾客对第X种商品的最小运费，最后将K种物品的最小费用加起来就是总费用**

**下图以第一种商品为例，介绍如何建图，最小费用为2，最大流量为1**![](https://img-blog.csdn.net/2018100615480842?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-6
using namespace std;
typedef long long ll;
#define MAXN 1003
#define MAXM 40004
//最小费用最大流
struct Edge{
    int to,next;
    int flow,cost,cap;
}edge[MAXM];
int tol,head[MAXN];
void init()
{
    tol=0;
    memset(head,-1,sizeof head);
}
void AddEdge(int u,int v,int cap,int cost){
    edge[tol].to=v;
    edge[tol].cap=cap;
    edge[tol].cost=cost;
    edge[tol].flow=0;
    edge[tol].next=head[u];
    head[u]=tol++;

    edge[tol].to=u;
    edge[tol].cap=0;
    edge[tol].cost=-cost;
    edge[tol].flow=0;
    edge[tol].next=head[v];
    head[v]=tol++;
}

bool inq[MAXN];//标记是否点是否在队列
int dis[MAXN];//最短距离
int pre[MAXN];//记录路径
int q[MAXN*10];//队列
//单位费用可能是负值，所以用SPFA
bool spfa(int st,int en)
{
    memset(inq,0,sizeof inq);
    memset(dis,INF,sizeof dis);
    memset(pre,-1,sizeof pre);

    int rear=0,front=0;
    dis[st]=0;
    inq[st]=true;
    q[front++]=st;
    while(rear<front){
        int u=q[rear++];
        inq[u]=false;

        for(int e=head[u];e!=-1;e=edge[e].next){
            int v=edge[e].to;
            if(edge[e].cap>edge[e].flow&&dis[v]>dis[u]+edge[e].cost){
                dis[v]=dis[u]+edge[e].cost;
                pre[v]=e;//表示边e-->v,e就是v的前驱
                if(!inq[v])
                    inq[v]=true,q[front++]=v;
            }
        }
    }
    return pre[en]!=-1;
}
void MCMF(int st,int en,int &cost,int &flow)
{
    //如果能找到从源点到汇点的最短路，说明还没有达到最小费用最大流
    while(spfa(st,en)){
        int Min=INF;//最小残余流量
        //沿着当前路径返回
        for(int i=pre[en];i!=-1;i=pre[edge[i^1].to]){
            int rem=edge[i].cap-edge[i].flow;
            Min=Min>rem?rem:Min;
        }
        for(int i=pre[en];i!=-1;i=pre[edge[i^1].to]){
            edge[i].flow+=Min;//正向边添加残余流量
            edge[i^1].flow-=Min;//反向边减少残余流量
            cost+=Min*edge[i].cost;
        }
        flow+=Min;
    }
}

int need[60][60];        //i顾客对j商品的需求量
int have[60][60];        //i仓库对j商品的供应量
int cost[60][60][60];    //仓库j到i顾客对x商品的运费
int main()
{
	int n,m,k;
	while (scanf("%d%d%d",&n,&m,&k)!=EOF && n)
	{
        int goods[60];
		int flag = 1;
		memset(goods,0,sizeof(goods));
		for (int i = 1;i<=n;i++)
			for (int j = 1;j<=k;j++)
			{
				scanf("%d",&need[i][j]);
				goods[j]+=need[i][j];
			}

		for (int i = 1;i<=m;i++)
			for (int j = 1;j<=k;j++)
			{
				scanf("%d",&have[i][j]);
				goods[j]-=have[i][j];
			}

		for (int h = 1;h<=k;h++)
			for (int i = 1;i<=n;i++)
				for (int j = 1;j<=m;j++)
					scanf("%d",&cost[h][i][j]);

		for (int i = 1;i<=k;i++)
			if (goods[i]>0)
			{
				flag = 0;
				break;
			}
		if (!flag)
		{
			printf("-1\n");
			continue;
		}

		int mins = 0;
        for(int g = 1;g<=k;g++)
		{
            init();
			for (int i = 1;i<=m;i++)
				AddEdge(0,i,have[i][g],0);
			for (int i = 1;i<=n;i++)
				AddEdge(m+i,m+n+1,need[i][g],0);
			for (int i = 1;i<=m;i++)
				for (int j = 1;j<=n;j++)
                AddEdge(i,j+m,INF,cost[g][j][i]);
            int cost=0,flow=0;
            MCMF(0,n+m+1,cost,flow);
			mins+=cost;
		}
		printf("%d\n",mins);
	}
	return 0;
}
```






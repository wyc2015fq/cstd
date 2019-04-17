# 网络流 最大流—最小割 之SAP算法 详解 - 在思索中前行！ - CSDN博客





2017年03月24日 16:40:21[_Tham](https://me.csdn.net/txl16211)阅读数：3262










首先引入几个新名词：

1、距离标号：

所谓距离标号 ，就是**某个点到汇点的最少的弧的数量**（即边权值为1时某个点到汇点的最短路径长度）。

设点i的标号为level[i]，那么如果将满足level[i]=level[j]+1的弧(i,j)叫做允许弧 ，且**增广时只走允许弧**。

2、断层（本算法的Gap优化思想）：

**gap[i]数组表示距离标号为i的点有多少个**，如果到某一点没有符合距离标号的允许弧，那么**需要修改距离标号来找到增广路； **如果重标号使得gap数组中原标号数目变为0，则算法结束。

**SAP算法框架：**

**1、初始化；**

**2、不断沿着可行弧找增广路。可行弧的定义为{( i , j ) , level[i]==level[j]+1};**

**3、当前节点遍历完以后，为了保证下次再来的时候有路可走，重新标号当前距离，level[i]=min(level[j]+1)**

**该算法最重要的就是gap常数优化了。**




题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=1532](http://acm.hdu.edu.cn/showproblem.php?pid=1532)



题目大意：

      就是由于下大雨的时候约翰的农场就会被雨水给淹没，无奈下约翰不得不修建水沟，而且是网络水沟，并且聪明的约翰还控制了水的流速，本题就是让你求出最大流速，无疑要运用到求最大流了。题中N为水沟数，M为水沟的顶点，接下来Si,Ei,Ci分别是水沟的起点，终点以及其容量。求源点1到终点M的最大流速。注意重边
题大意：给出边数N,点数M，每条边都是单向的，问从1点到M的最大流是多少。

EK算法模板



```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

int u,v,dist,a[205][205],flow[205],pre[205],q[205],n,m;


int bfs(int s,int des)
{
    memset(q,0,sizeof(q));
    memset(pre,0,sizeof(pre));
    int h=0,t=0;
    q[h++]=s; flow[s]=0x7fffffff;
    pre[s]=s; flow[des]=0;
    while (h!=t)
    {
        int u=q[t];
        if (u==des) break;
        for (int v=1;v<=m;v++) if (a[u][v]>0 && !pre[v])
        {
            q[h++]=v; pre[v]=u;
            flow[v]=min(flow[u],a[u][v]);
        }
        t++;
    }
    return flow[des];
}

int maxflow(int s,int des)
{
    int t,ans=0;
    while(t=bfs(s,des),t!=0)
    {
        for (int i=des;i!=s;i=pre[i])
        {
            a[pre[i]][i]-=t;
            a[i][pre[i]]+=t;
        }
        ans+=t;
    }
    return ans;
}

int main()
{
    while (scanf("%d%d",&n,&m)!=EOF)
    {
        memset(a,0,sizeof(a));
        for (int i=1;i<=n;i++)
        {
            scanf("%d%d%d",&u,&v,&dist);
            a[u][v]+=dist;
        }
        int ans=maxflow(1,m);
        printf("%d\n",ans);
    }
    return 0;
}
```





**SAP算法分析**





EK的，最经典，也是最白痴的[算法](http://lib.csdn.net/base/datastructure)……（不过有些时候正经很好用呢~）

昨天研究了一下神奇的SAP算法，今天又拿他做了一些网络流的题目，发现确实优化效果极其明显！

简单总结一下：

首先我么先回顾一下EK（这个不会的可以看namiheike写的EK的详解，地址：http://www.oibh.org/bbs/thread-29333-1-1.html）。

EK的思想就是每一次都用一个BFS来找到一条增广路，所以说我们就会发现他的**复杂度是：O(V*E^2)**。所以说我们找到的不一定就是最优的。




** EK详细讲解及优化分析**

求最大流有一种经典的算法，就是每次找增广路时用BFS找，保证找到的增广路是弧数最少的，也就是所谓的Edmonds-Karp算法。可以证明的是在使用最短路增广时增广过程不超过V*E次，每次BFS的时间都是O(E)，所以Edmonds-Karp的时间复杂度就是O(V*E^2)。

如果能让每次寻找增广路时的时间复杂度降下来，那么就能提高算法效率了，使用**距离标号的最短增广路算法**就是这样的。所谓距离标号，就是某个点到汇点的最少的弧的数量（另外一种距离标号是从源点到该点的最少的弧的数量，本质上没什么区别）。

设点i的标号为D[i]，那么如果将**满足D[i]=D[j]+1的弧(i,j)叫做允许弧，且增广时只走允许弧，那么就可以达到“怎么走都是最短路”的效果**。每个点的初始标号可以在一开始用一次从汇点沿所有反向边的BFS求出，实践中可以初始设全部点的距离标号为0，问题就是如何在增广过程中维护这个距离标号。




维护距离标号的方法是这样的：当找增广路过程中发现某点出发没有允许弧时，将这个点的距离标号设为由它出发的所有弧的终点的距离标号的最小值加一。

这种维护距离标号的方法的正确性我就不证了。由于距离标号的存在，由于“怎么走都是最短路”，所以就可以采用DFS找增广路，用一个栈保存当前路径的弧即可。当某个点的距离标号被改变时，栈中指向它的那条弧肯定已经不是允许弧了，所以就让它出栈，并继续用栈顶的弧的端点增广。

**为了使每次找增广路的时间变成均摊O(V)**，还有一个重要的优化是对于**每个点保存“当前弧”**：初始时当前弧是邻接表的第一条弧；在邻接表中查找时从当前弧开始查找，找到了一条允许弧，就把这条弧设为当前弧；改变距离标号时，把当前弧重新设为邻接表的第一条弧，还有一种在常数上有所优化的写法是改变距离标号时把当前弧设为那条提供了最小标号的弧。当前弧的写法之所以正确就在于任何时候我们都能保证在邻接表中当前弧的前面肯定不存在允许弧。

**优化2：**

还有一个常数优化是在每次找到路径并增广完毕之后不要将路径中所有的顶点退栈，而是只将瓶颈边以及之后的边退栈，这是借鉴了Dinic算法的思想。注意任何时候待增广的“当前点”都应该是栈顶的点的终点。这的确只是一个常数优化，由于当前边结构的存在，我们肯定可以在O(n)的时间内复原路径中瓶颈边之前的所有边。




**优化小结：**

1.邻接表优化：

如果顶点多的话，往往N^2存不下，这时候就要存边：

存每条边的出发点，终止点和价值，然后排序一下，再记录每个出发点的位置。以后要调用从出发点出发的边时候，只需要从记录的位置开始找即可（其实可以用链表）。优点是时间加快空间节省，缺点是编程复杂度将变大，所以在题目允许的情况下，建议使用邻接矩阵。

2.GAP优化：

如果一次重标号时，出现距离断层，则可以证明ST无可行流，此时则可以直接退出算法。

3.当前弧优化：

为了使每次找增广路的时间变成均摊O(V)，还有一个重要的优化是对于每个点保存“当前弧”：初始时当前弧是邻接表的第一条弧；在邻接表中查找时从当前弧开始查找，找到了一条允许弧，就把这条弧设为当前弧；改变距离标号时，把当前弧重新设为邻接表的第一条弧。

学过之后又看了算法速度的比较，发现如果写好的话SAP的速度不会输给HLPP。

SAP实现代码如下：




```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
#define MAXN 222
#define inf 100000000+1000
int map[MAXN][MAXN];//存图
int pre[MAXN];//记录当前点的前驱
int level[MAXN];//记录距离标号
int gap[MAXN];//gap常数优化
int NV,NE;

//入口参数vs源点，vt汇点
int SAP(int vs,int vt)
{
	memset(pre,-1,sizeof(pre));
	memset(level,0,sizeof(level));
	memset(gap,0,sizeof(gap));
	gap[0]=vt;
	int v,u=pre[vs]=vs,maxflow=0,aug=inf;
	
	while(level[vs]<vt)
	{
		//寻找可行弧
		for(v=1;v<=vt;v++)
		{
		    if(map[u][v]>0&&level[u]==level[v]+1){
		         break;
		    }
		}
		if(v<=vt)
		{
		   pre[v]=u;
	       u=v;
		   if(v==vt)
		   {
		   	     int neck=0;
		         aug=inf;
		         //寻找当前找到的一条路径上的最大流 , (瓶颈边)
		         for(int i=v;i!=vs;i=pre[i])
				 {
		             if(aug>map[pre[i]][i])
					 {
					 	aug=map[pre[i]][i];
						neck=i;
					 }
		         }
		         maxflow+=aug;
		         //更新残留网络
		         for(int i=v;i!=vs;i=pre[i]){
		             map[pre[i]][i]-=aug;
		             map[i][pre[i]]+=aug;
		        }
			    u=vs;		  //从源点开始继续搜
		//		u=neck; 	  // Dnic 多路增广优化，下次增广时，从瓶颈边(后面)开始
		    }
		}
		else
		{
		    //找不到可行弧
		    int minlevel=vt;
		    //寻找与当前点相连接的点中最小的距离标号
		    for(v=1;v<=vt;v++){
		         if(map[u][v]>0&&minlevel>level[v]){
		             minlevel=level[v];
		         }
		    }
		    gap[level[u]]--;//(更新gap数组）当前标号的数目减1；
		    if(gap[level[u]]==0)break;//出现断层
		    level[u]=minlevel+1;
		    gap[level[u]]++;
		    u=pre[u];
		}
	}
	return maxflow;
}

int main()
{
	int n,m,u,v,cap;
	while(~scanf("%d%d",&m,&n))
	{
		memset(map,0,sizeof(map));
		for(int i=1;i<=m;i++)
		{
		    scanf("%d%d%d",&u,&v,&cap);
		    map[u][v]+=cap;
		}
			printf("%d\n",SAP(1,n));
	}
	return 0;
}
```











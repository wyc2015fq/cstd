# hdu  4738  Caocao's Bridges（Tarjan求无向图的桥） - HJ - CSDN博客
2017年01月18日 02:17:20[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：396
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[连通分量](https://blog.csdn.net/feizaoSYUACM/article/category/6308002)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Caocao was defeated by Zhuge Liang and Zhou Yu in the battle of Chibi. But he wouldn’t give up. Caocao’s army still was not good at water battles, so he came up with another idea. He built many islands in the Changjiang river, and based on those islands, Caocao’s army could easily attack Zhou Yu’s troop. Caocao also built bridges connecting islands. If all islands were connected by bridges, Caocao’s army could be deployed very conveniently among those islands. Zhou Yu couldn’t stand with that, so he wanted to destroy some Caocao’s bridges so one or more islands would be seperated from other islands. But Zhou Yu had only one bomb which was left by Zhuge Liang, so he could only destroy one bridge. Zhou Yu must send someone carrying the bomb to destroy the bridge. There might be guards on bridges. The soldier number of the bombing team couldn’t be less than the guard number of a bridge, or the mission would fail. Please figure out as least how many soldiers Zhou Yu have to sent to complete the island seperating mission.
Input 
There are no more than 12 test cases.
In each test case:
The first line contains two integers, N and M, meaning that there are N islands and M bridges. All the islands are numbered from 1 to N. ( 2 <= N <= 1000, 0 < M <= N2 )
Next M lines describes M bridges. Each line contains three integers U,V and W, meaning that there is a bridge connecting island U and island V, and there are W guards on that bridge. ( U ≠ V and 0 <= W <= 10,000 )
The input ends with N = 0 and M = 0.
Output 
For each test case, print the minimum soldier number Zhou Yu had to send to complete the mission. If Zhou Yu couldn’t succeed any way, print -1 instead.
Sample Input
3 3 
1 2 7 
2 3 4 
3 1 4 
3 2 
1 2 7 
2 3 4 
0 0
Sample Output
-1 
4
Source 
2013 ACM/ICPC Asia Regional Hangzhou Online 
**题目大意：**
　　曹操在长江上建立了一些点，点之间有一些边连着。如果这些点构成的无向图变成了连通图，那么曹操就无敌了。刘备为了防止曹操变得无敌，就打算去摧毁连接曹操的点的桥。但是诸葛亮把所有炸弹都带走了，只留下一枚给刘备。所以刘备只能炸一条桥。
　　题目给出n，m。表示有n个点，m条桥。
　　接下来的m行每行给出a，b，c，表示a点和b点之间有一条桥，而且曹操派了c个人去守卫这条桥。
　　现在问刘备最少派多少人去炸桥。
　　如果无法使曹操的点成为多个连通图，则输出-1.
**思路：**
　　就是用tarjan算法算出桥的数量，再比较哪一个的值最小。
**注意：**
　　①. 有重边，所以tarjan算法要处理重边。有两种处理方法，一种是先把所有的边存下，发现两点有重边的时候就只给这两个点连一条权值为无穷大的边。或者是在tarjan算法里处理重边，即使之求u或u的子树能够追溯到的最早的栈中节点的次序号时可访问父节点的次序号。
　　②. 如果无向图图本身已经有两个连通图了，就无需派人去炸桥，这时候输出0。
　　③. 如果求出来的最小权值桥的守卫人数为0时，也需要派出一个人去炸桥。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
const int INF=0xffffff; 
const int maxn=1007;
struct node{
    int to,cost,next;
}edge[maxn*maxn];
int head[maxn],dfn[maxn],low[maxn];
int n,m,sum,child,ans,num;
void add(int u,int v,int w)
{
    edge[num].to=v;edge[num].cost=w;edge[num].next=head[u];
    head[u]=num++;
    edge[num].to=u;edge[num].cost=w;edge[num].next=head[v];
    head[v]=num++;
}
void tarjan(int u,int fa)
{
    child++;
    low[u]=dfn[u]=sum++;
    for(int i=head[u];i!=-1;i=edge[i].next)
    {
        int v=edge[i].to;
        if(i==(fa^1))
          continue;
        if(!dfn[v])
        {
            tarjan(v,i);
            low[u]=min(low[u],low[v]);
            if(dfn[u]<low[v])
              ans=min(ans,edge[i].cost);
        }
        else
          low[u]=min(low[u],low[v]);
    }
}
int main()
{
    int a,b,c;
    while(cin>>n>>m&&(n+m))
    {
        memset(head,-1,sizeof(head));
        memset(dfn,0,sizeof(dfn));
        memset(low,0,sizeof(low));
        ans=INF;child=0;sum=1;
        while(m--)
        {
            cin>>a>>b>>c;
            add(a,b,c);
        }
        tarjan(1,-1);
        if(child<n)
          printf("0\n");    
        else
          printf("%d\n",ans?(ans==INF?-1:ans):1);
    }
    return 0;
}
```

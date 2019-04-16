# POJ2135 Farm Tour 最小费用最大流 - 紫芝的博客 - CSDN博客





2018年08月24日 19:02:42[紫芝](https://me.csdn.net/qq_40507857)阅读数：84








# Farm Tour
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 19741||**Accepted:** 7670|

Description

When FJ's friends visit him on the farm, he likes to show them around. His farm comprises N (1 <= N <= 1000) fields numbered 1..N, the first of which contains his house and the Nth of which contains the big barn. A total M (1 <= M <= 10000) paths that connect the fields in various ways. Each path connects two different fields and has a nonzero length smaller than 35,000. 


To show off his farm in the best way, he walks a tour that starts at his house, potentially travels through some fields, and ends at the barn. Later, he returns (potentially through some fields) back to his house again. 


He wants his tour to be as short as possible, however he doesn't want to walk on any given path more than once. Calculate the shortest tour possible. FJ is sure that some tour exists for any given farm.

Input

* Line 1: Two space-separated integers: N and M. 


* Lines 2..M+1: Three space-separated integers that define a path: The starting field, the end field, and the path's length. 

Output

A single line containing the length of the shortest tour. 

Sample Input

4 5
1 2 1
2 3 1
3 4 1
1 3 2
2 4 2
Sample Output

6

Source

[USACO 2003 February Green](http://poj.org/searchproblem?field=source&key=USACO+2003+February+Green)

### 最小费用最大流

反复用SPFA算法做源点到汇点的最短路进行增广，边权值为边上的单位费用，反向边的单位费用是负的。

直到无法增广，即为找到最小费用最大流

成立的原因：每次增广时，每增加一个流量，费用都是最小的

## 题意：

有n个景点，要从1号景点走到n号景点，再从n 号景点走到1号（回来的路不能重复，不一定走完所有 景点，只要求从1到n即可），给你一些景点之间的路的 长度（双向），问你最短需要走多少路才能回来？

## 分析：

最小费用就是路径长度的总和，最大流就是来回的两条路，由于去和回来可以看成两条从1到n的不同的路径，所以问题转化为求从1到n的两条不同的路。

建立模型：

假设a b之间有长度为c的路。按照最小费用流建图： ab之间费用为c，容量是1。 ba之间费用为-c，容量是0.

建立一个超级源点，连接1号景点，无费用，容量为2 （表示可以有两条路）

同理，建立一个超级汇点，连接n号景点，无费用， 容量为2.

这样，如果求的的最大流是2，就表示了有两条从1到 n的不同的路。

（因为中间的点边容量只能是1，只能用 一次） 这样求的的最小费用就是最短路径长度。

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;
#define INF 0x3f3f3f3f
const int V=1e4+10,E=1e5+10;

int pre[V];//记录前驱点
int head[V],cnt=0;//前向星
int dist[V];//记录s至i的最小距离（费用）
int n,m;//n个景点，m条边
int vis[V];
struct node
{
    int x,y;
    int cap;//容量
    int cost;//费用
    int next;//存储下一条边的编号
    int to;//存储该边指向的顶点
}edge[E];

void init()
{
    cnt=0;
    memset(head,-1,sizeof head);
}

//建边u->v，cap为容量，cost为费用
void add_edge(int u,int v,int cap,int cost)
{
    //正向边
    edge[cnt].x=u;
    edge[cnt].y=v;
    edge[cnt].cost=cost;
    edge[cnt].cap=cap;

    edge[cnt].next=head[u];//next指向前一条边
    //edge[cnt].to=v;//这条边的终点是v
    head[u]=cnt++;//更新head

    //反向边
    edge[cnt].x=v;
    edge[cnt].y=u;
    edge[cnt].cost=-cost;
    edge[cnt].cap=0;

    edge[cnt].next=head[v];//next指向前一条边
    //edge[cnt].to=v;//这条边的终点是v
    head[v]=cnt++;//更新head
}

int spfa(int start,int end)
{
    queue<int>q;
    memset(dist,INF,sizeof dist);
    memset(vis,0,sizeof vis);
    memset(pre,-1,sizeof pre);
    /*
    for(int i=0;i<=end+2;i++)
    {
        dist[i]=INF;
        vis[i]=0;
        pre[i]=-1;
    }
    */
    vis[start]=1;
    dist[start]=0;
    q.push(start);
    while(!q.empty()){
        int u=q.front();
        q.pop();
        vis[u]=0;
        for(int i=head[u];i!=-1;i=edge[i].next){
            if(edge[i].cap>0){
                int v=edge[i].y;
                if(dist[v]>dist[u]+edge[i].cost)
                {
                   dist[v]=dist[u]+edge[i].cost;
                   pre[v]=i;//记录前驱
                   if(!vis[v]){
                    q.push(v);
                    vis[v]=1;
                   }
                }
            }
        }
    }
    return dist[end]!=INF;
}
int MCMF(int begin,int end)
{
    int ans=0,flow,flow_sum=0;
    while(spfa(begin,end)){
        flow=INF;
        for(int i=pre[end];i!=-1;i=pre[edge[i].x])
            flow=min(flow,edge[i].cap);
        for(int i=pre[end];i!=-1;i=pre[edge[i].x]){
            edge[i].cap-=flow;
            edge[i^1].cap+=flow;//i^1就变成了反向边
        }
        flow_sum+=flow;
        ans+=dist[end];//这题最大流只有1或者0，一般化的是flow*dist[end]
    }
    return ans;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
            init();
        add_edge(0,1,2,0);  //建立一个超级源点
        add_edge(n,n+1,2,0);//建立一个超级汇点
        for(int i=1;i<=m;i++)
        {
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            //u,v之间有长度为w的路，按最小费用流建图
            add_edge(u,v,1,w);//u，v之间费用为w，容量为1
            add_edge(v,u,1,w);//v,u之间费用为w，容量为1
        }
    printf("%d\n",MCMF(0,n+1));
    }
    return 0;
}
```






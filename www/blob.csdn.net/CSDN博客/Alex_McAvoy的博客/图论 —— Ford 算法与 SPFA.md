# 图论 —— Ford 算法与 SPFA - Alex_McAvoy的博客 - CSDN博客





2019年03月18日 19:29:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：87








# 【Ford 算法】

## 1.概述

Bellman-Ford算法适用于计算单源最短路径，即：只能计算起点只有一个的情况。

其最大特点是可以处理存在负边权的情况，但无法处理存在负权回路的情况。

其时间复杂度为：O(N*E)，其中，N 是顶点数，E 是边数。

## 2.算法描述

设起点为 s，dis[v] 表示从 s 到 v 的最短路径，u[i] 和 v[i] 分别表示第 i 条边的起点和终点，w[j] 是连接 u、v 的边 j 的长度。

初始化：

dis[s]=0，dis[v]=0x3f3f3f3f（v≠s），即：初始化为一极大值

算法主体：

```cpp
void Bellman_Ford()
{
    for(int i=0;i<n;i++) 
        dis[i]=INF;
    dis[0]=0;

    for(int i=1;i<=n-1;i++)//枚举除终点外的所有点
        for(int j=1;j<=m;j++)//枚举所有边
        {
            int x=u[j];//边j的起点
            int y=v[j];//边j的终点
            if(dis[x]<INF)//松弛
                dis[y]=min(dis[y],dis[x]+w[j]);
        }
}
```

算法结束：dis[v] 即为 s 到 v 最短距离

## 3.算法分析

一开始认为起点是白点（dis[1]=0），每一次都枚举所有的边，必然会有一些边，连接着白点和蓝点。因此每次都能用所有的白点去修改所有的蓝点，每次循环也必然会有至少一个蓝点变成白点。

以下图为例

![](https://img-blog.csdn.net/20180515121105257)

令起点为白点，即：dis[1]=0，dis[2、3、4、5]=∞

![](https://img-blog.csdn.net/20180515121250525)

遍历所有边，将与白点相连的蓝点变为白点，即：dis[1]=0，dis[2]=2，dis[3]=1，dis[4]=2，dis[5] =∞

![](https://img-blog.csdn.net/20180515121657441)

继续向下遍历，修改蓝点为白点，即：dis[1]=0，dis[2]=2，dis[3]=1，dis[4]=2，dis[5] =4

![](https://img-blog.csdn.net/20180515121754163)

## 4.模版

### 1）简单版

```cpp
#define INF 0x3f3f3f3f
int n,m;//点数,边数，编号都从0开始
int w[N];//w[i]表示第i条边的权值(距离)
int u[N],v[N];//u[i]和v[i]分别表示第i条边的起点和终点
int dis[N];//单源最短路径
 
//计算以s为源点的单源最短距离
void Bellman_Ford(int s)
{
    for(int i=0;i<n;i++) 
        dis[i]=INF;
    dis[s]=0;

    for(int i=1;i<=n-1;i++)//枚举除终点外的所有点
        for(int j=1;j<=m;j++)//枚举所有边
        {
            int x=u[j];//边j的起点
            int y=v[j];//边j的终点
            if(dis[x]<INF)//松弛
                dis[y]=min(dis[y],dis[x]+w[j]);
        }
}
```

### 2）负环判断版

```cpp
int n,w,m;
int dis[N];
int k;
struct Node{
    int start;
    int endd;
    int value;
}edge[N];
void add(int u,int v,int w)
{
    edge[k].start=u;
    edge[k].endd=v;
    edge[k].value=w;
    k++;
}
bool Ford(int s)
{
    memset(dis,INF,sizeof(dis));
    dis[s]=0;
 
    for(int i=0;i<n;i++)
    {
        bool flag=true;
        for(int j=0;j<k;j++)
        {
            int u=edge[j].start;
            int v=edge[j].endd;
            int w=edge[j].value;
            if(dis[u]+w<dis[v])
            {
                flag=false;
                dis[v]=dis[u]+w;
                if(i==n-1)
                    return true;
            }
        }
 
        if(flag)
            return false;
    }
    return false;
}
```

# 【SPFA】

## 1.概述

SPFA 是 Ford 算法的队列实现，其与 Ford 算法最大的不同是，其可以判断负环。

其时间复杂度可达：O(k*E)，其中，E 是边数，k 是常数，平均值是 2。

## 2.基本思想

初始时将起点加入队列，每次从队列中取出一个元素，并对所有与它相邻的点进行修改，若相邻的点修改成功，则将其入队，直到队列为空时算法结束。

## 3.模版

### 1）简单版

```cpp
#define INF 0x3f3f3f3f
int n,m;             //点数,边数，编号都从0开始
int w[N];            //w[i]表示第i条边的权值(距离)
int first[N],next[N];//first[i]和next[i]分别表示第i个点的前驱点与后继点
int u[N],v[N];       //u[i]和v[i]分别表示第i条边的起点和终点
int dis[N];          //单源最短路径
bool vis[N];         //vis[i]标记是否在队列中

//Ford算法队列实现,求从s点到其他点的单源最短路径
void SPFA(int s)
{
    memset(vis,false,sizeof(vis));
    memset(dis,INF,sizeof(dis));
    dis[s]=0;

    queue<int> q;
    q.push(s);
    while(!q.empty())
    {
        int x=q.front();
        q.pop();

        vis[x]=false;//释放对v的标记，可以重新入队

        for(int e=first[x];e!=-1;e=next[e])//对于与队首相连的每一条边
            if(dis[v[e]]> dis[x]+w[e])//松弛操作
            {
                dis[v[e]] = dis[x]+w[e];

                //不在队列，则加入队列
                if(!vis[v[e]])
                {
                    vis[v[e]]=true;
                    q.push(v[e]);
                }
            }
    }
}
```

### 2）求单源最短路时判断有无负环

```cpp
#include<cstdio>
#include<cstring>
#include<queue>
#include<vector>
#include<algorithm>
using namespace std;
#define INF 0x3f3f3f3ff
 
struct Edge{
    int from;
    int to;
    int dis;
    Edge(int f,int t,int d):from(f),to(t),dis(d){}
};
int head[N],next[N],num;
int dis[N];
bool vis[N];
int outque[N];//进队次数
void init(){
    num=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int from,int to,int w){
    edge[num]=Edge(from,to,w);
    next[num]=head[from];
    head[from]=num++;
}

//计算以s为源点的最短路径
//如果图中存在s能到达的负圈，那么返回true
bool SPFA(int s){
    memset(vis,false,sizeof(vis));
    memset(outque,0,sizeof(outque));
    memset(dis,INF,sizeof(dis));
    dis[s]=0;

    queue<int> Q;
    Q.push(0);
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        vis[x]=false;

        outque[x]++;
        if(outque[x]>n-1)
            return true;

        for(int i=head[x];i!=-1;i=next[i]){
            Edge &e=edge[i];
            if(dis[e.to]>dis[x]+e.w){
                dis[e.to]=dis[x]+e.w;
                if(!vis[e.to]){
                    vis[e.to]=true;
                    Q.push(e.to);
                }
            }
        }
    }
    return false;
}
```

### 3）两遍 SPFA 求任意两点最短路

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>
#define INF 0x3f3f3f3f
#define N 5001
using namespace std;
int n,m;
vector<int> G[N];
int dis[N][N];
bool vis[N];
void SPFA(){
    for(int i=1;i<=n;i++){
        memset(vis,false,sizeof(vis));
        vis[i]=true;
        queue<int> Q;
        Q.push(i);
        while(!Q.empty()){
            int x=Q.front();
            Q.pop();
            for(int j=0;j<G[x].size();j++){
                int y=G[x][j];
                if(vis[y])
                    continue;

                vis[y]=true;
                dis[i][y]=dis[i][x]+1;
                Q.push(y);
            }
        }
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
        for(int i=1;i<=m;i++)
            G[i].clear();

        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
        }

        SPFA();

        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                printf("%d\n",dis[i][j]);
    }
    return 0;
}
```

# 【例题】
- Wormholes（POJ-3259）**(Ford 判负环)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81193649)
- Currency Exchange（POJ-1860）**(Ford 求递增环)**：[点击这里](https://blog.csdn.net/u011815404/article/details/83958151)
- zz's Mysterious Present（HDU-2145）**(SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/83958150)
- 热浪（信息学奥赛一本通-T1379）**(SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80796181)
- 分糖果（信息学奥赛一本通-T1380）**(SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80796180)
- 城市路（信息学奥赛一本通-T1381）**(SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80796183)
- 最短路（信息学奥赛一本通-T1382）**(SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80796184)
- 香甜的黄油（信息学奥赛一本通-T1345）**(SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80752633)
- Skiing（POJ - 3037）**(图的构建+SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81207207)
- In Action（HDU-3339）**(01背包+SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80399982)
- Telephone Linse（POJ-3662）**(二分+SPFA)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80501483)
- Destroying Roads（CF-302B）**(SPFA 求任意两点最短路)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86510559)




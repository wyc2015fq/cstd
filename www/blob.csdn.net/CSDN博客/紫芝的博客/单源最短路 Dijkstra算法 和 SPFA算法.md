# 单源最短路  Dijkstra算法 和 SPFA算法 - 紫芝的博客 - CSDN博客





2018年08月14日 17:38:51[紫芝](https://me.csdn.net/qq_40507857)阅读数：369








# 单源最短路

**•从一个点出发，到达其他顶点的最短路径的长度。**

**•基本操作：松弛**

**•d[u]+map[u, v]< d[v]这样的边(u,v)称为紧的(tense),可以对它进行松弛(relax): **

**•d[v] = d[u]+w, pred[v] = u**

**•最开始给每一个点一个很大的值，dis[v]=inf，从d[s]=0开始，不断的对可以松弛的点进行松弛，不能松弛的时候就已经求出了最短路**

![](https://img-blog.csdn.net/2018081416112365?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Dijkstra算法

•Dijkstra(迪杰斯特拉)算法是典型的单源最短路径算法，用于计算一个节点到其他所有节点的最短路径。主要特点是以起始点为中心向外层层扩展，直到扩展到终点为止

•可以证明，具有最小的d[i]（临时最短路）值的（还没加入最短路）点在此以后无法松弛

•所以每次找最近的点进行松弛操作

**注意该算法要求图中不存在负权边**

•Dijkstra算法也适用于无向图。但不适用于有负权边的图。

•d[1,2] = 2 ,但用Dijkstra算法求得 d[1,2] = 3

![](https://img-blog.csdn.net/20180814181055840?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**算法步骤：**

1、在开始之前，认为所有的点都没有进行过计算，dis[i]全部赋值为极大值(dis[i]表示各点当前到源点的最短距离）

2、源点的 dis[start] 值明显为0

3、计算与 start 相邻的所有点的dis值 —— dis[v] = map[s][v]

4、还没算出最短路的点中dis[]最小的一个点u， 其最短路就是当前的dis[u]

5、对于与u相连的所有点v，若dis[u]+map[u][v] 比当前的dis[v]小, 更新dis[v]

6、重复4,5直到源点到所有点的最短路都已求出

```cpp
const int inf = 0x3f3f3f3f; //需将road及dis初始化为正无穷inf
int n,m;
int dis[maxn];  //储存各个点到源点的最短距离,dis[s]为0
int road[maxn][maxn];   //两点之间直接距离关系
bool vis[maxn];     //判断源点到该点的距离是否为最短距离
void dijkstra(int s)
{
    memset(vis, false, sizeof(vis));//标记是否求出最短路径
    for(int i = 1; i <= n; i++)
        dis[i] = road[s][i];//初始化起点到每一个点的距离
     vis[s] = true;//标记起点到这一点的最小距离已经求出
     dis[s]=0;
    for(int u = 1; u<n; u++)
    {
        int minD = inf,k = -1;
        for(int i = 1; i<= n; i++)
        {
            if(!vis[i]&&dis[i]<minD)
            {
                k = i;//记录下标
                minD = dis[i];//记录最小值
            }
        }
        vis[k] = true;//标记已经访问过
        //松弛操作
        for(int i = 1; i<= n; i++)
        {
            if(!vis[i]&&dis[k]+road[k][i]<dis[i])
            {
                dis[i]=dis[k]+road[k][i];
            }//if
        }//for
    }
}
```

**堆优化的Dijkstra算法，使用优先队列来实现小根堆**

```cpp
#include<cstdio>
#include<queue>
#include<algorithm>
#include<cstring>
using namespace std;
#define inf 0x3f3f3f3f
const int maxn=1007;
int dis[maxn];
bool vis[maxn];
int m,n;
struct qnode{
    int v,c;
    qnode(int _v=0,int _c=0):v(_c),c(_c){}
    bool operator<(const qnode&r)const {return c>r.c;}//小根堆
};

struct Edge{
    int v;
    int cost;
    Edge(int _v=0,int _cost=0):v(_v),cost(_cost){}//构造方法
};

vector<Edge> E[maxn];
void dijkstra(int start)
{
    memset(vis,0,sizeof (vis));
    memset(dis,inf,sizeof (dis));
    priority_queue<qnode> que;
    qnode s;
    s.c=0;
    s.v=start;
    que.push(s);
    dis[start]=0;//起点的最短距离为0
    while(!que.empty()){
        qnode tmp=que.top();
        que.pop();
        int u=tmp.v;
        if(vis[u])  continue;
        vis[u]=true;
        for(int i=0;i<E[u].size();i++){
            int v=E[u][i].v;
            int cost=E[u][i].cost;
            if(!vis[v]&&dis[v]>dis[u]+cost){
                dis[v]=dis[u]+cost;
                qnode next;
                next.v=v;
                next.c=dis[v];
                que.push(next);
            }
        }
    }
}
//建边
void addedge(int u,int v,int w)
{
    E[u].push_back(Edge(v,w));
    E[v].push_back(Edge(u,w));
}

int main()
{
    while(scanf("%d%d",&m,&n)!=EOF){
       // printf("%d %d\n",m,n);
        for(int i=0;i<m;i++)
        {
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            addedge(u,v,w);
        }

        dijkstra(n);
        printf("%d\n",dis[1]);
    }
}
```

# **SPFA算法**

•Dijkstra算法每一次松弛的时候bellmanford都要枚举所有的点，而其实有很多点都是不需要枚举的，所以有很多的无效枚举，于是效率显得略低

•**SPFA算法****（Shortest Path Faster Algorithm）**每次松弛的时候只需要枚举与上次被松弛的点相连的点就可以了

### SPFA算法的实现

**先将源点入队，****然后重复从队首取出一个顶点，****对所有该点指向的顶点进行松弛，****如果松弛成功并且该不在队列中，****直到队空，即可找出源点到所有顶点的最短距离**。**时间复杂度为O(KM)。M为边数，k是每个点平均入队次数。**

```cpp
#define N 105
#define inf 0x3f3f3f3f
int res[N];//存储源点到顶点的最短距离的值
int g[N][N];//存储两点之间的直接距离关系
int cnt[N];//每个点入队的次数，判断是否出现负环
int que[N*N];//队列
bool in_que[N];//标记一个点是否在队列中
int front;//队首位置
int rear;//队尾位置
void spfa(int n,int start)
{
    memset(res,inf,sizeof(res));
    memset(in_que,0,sizeof(in_que));
    rear=front=0;
    que[++rear]=start;//起点入队
    in_que[start]=true;//起点在队列中
    res[start]=0;
    while(front<rear){
        int cur=que[++front];
        in_que[cur]=0;//出队
        int i;
        for(i=1;i<=n;i++){
            if(res[cur]+g[cur][i]<res[i]){
                res[i]=res[cur]+g[cur][i];
                if(!in_que[i]){
                    que[++rear]=i;
                    in_que[i]=true;//入队
                }
            }
        }
    }
}
```

**STL 队列,实现简单，浪费时间**

```cpp
bool vis[maxn];//标记点是否在队列
double mp[maxn][maxn];
double dis[maxn];

void spfa(int from,int to)
{
    memset(vis,0,sizeof vis);
    //memset(dis.inf,sizeof dis);
    for(int i=0;i<maxn;i++)
        dis[i]=inf;

    queue<int> q;
    q.push(from);
    vis[from]=true;
    dis[from]=0;
    while(!q.empty()){
        int cur=q.front();
        q.pop();
        vis[cur]=false;
        for(int i=0;i<n;i++)
        {
            if(dis[i]>dis[cur]+mp[cur][i])
            {
                dis[i]=dis[cur]+mp[cur][i];
                if(!vis[i]){
                    vis[i]=true;
                    q.push(i);
                }
            }
        }
    }
}
```

**使用堆栈实现的SPFA，比队列要快****（POJ1847，学好英语）**

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
//#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-6
using namespace std;
typedef long long ll;
const int mod=1e9+7;
const int maxn=1000;
int n;
int cnt;
int head[maxn];
struct Edge
{
    int to;
    int next;
    int val;
}edge[maxn];
//建边：from->to,边权为val
void addEdge(int from,int to,int val)
{
    edge[cnt].to=to;
    edge[cnt].val=val;
    edge[cnt].next=head[from];
    head[from]=cnt++;
}

bool inq[maxn];//标记是否在队列中
int dis[maxn];//到起点的最短距离
int q[maxn];//队列

void spfa(int start)
{
    memset(inq,0,sizeof inq);
    memset(dis,INF,sizeof dis);

    int top=0;
    q[++top]=start;
    //queue<int>q;
    //q.push(start);

    inq[start]=true;
    dis[start]=0;

    while(top){
        //int cur=q.front();
        //q.pop();
        int cur=q[top--];
        inq[cur]=false;
        for(int i=head[cur];i!=-1;i=edge[i].next){
            int to=edge[i].to;
            if(dis[to]>dis[cur]+edge[i].val){
                dis[to]=dis[cur]+edge[i].val;
                if(!inq[to])
                    inq[to]=1,q[++top]=to;//入队
            }
        }
    }
}
int main()
{
    int s,t;
    while(scanf("%d%d%d",&n,&s,&t)!=EOF){
    memset(head,-1,sizeof head);
    cnt=0;

    for(int i=1;i<=n;i++){
        int sum;
        scanf("%d",&sum);
        for(int j=0;j<sum;j++)
        {
            int x;
            scanf("%d",&x);
            if(j==0)
              addEdge(i,x,0);
            else
                addEdge(i,x,1);
        }
    }
    spfa(s);
    if(dis[t]==INF)
        printf("-1\n");
    else
    printf("%d\n",dis[t]);
    }
    return 0;
}
```






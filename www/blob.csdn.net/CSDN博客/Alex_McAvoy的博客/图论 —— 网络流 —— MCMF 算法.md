# 图论 —— 网络流 —— MCMF 算法 - Alex_McAvoy的博客 - CSDN博客





2019年03月30日 21:51:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：55








# 【概述】

EK 算法是每次用广搜寻找一条最短的增广路，然后沿其增广，而 MCMF 算法是在 EK 算法的基础上，每次用 SPFA 计算图的距离标号，然后沿着可行边进行增广，即将 EK 算法中的 bfs 替换为 SPFA 求最短路，边权为该边的单位流量花费，即改变遍历的优先级来实现。

# 【基本思想】

1.每次查找是否存在从源点到汇点可增广的路径（源点到汇点的最短路且路径上的所有边均不能满流），并用 pre[i] 记录路径上到达点 i 的边的编号。

2.若存在这样的路径，则从汇点沿着 pre 数组向前找，找到该路径上可以增广的流量 Min（所有边中剩余流量最小的），再从汇点遍历一次，正向边增加流量 Min，反向边减少 Min，总费用累加 Min * edge[i].cost，总流量累加 Min

# 【实现】

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
#define N 10001
using namespace std;

struct Edge{
    int from,to;
    int cap,flow;
    int cost;//费用
    int next;//指向下一条边的编号
    Edge(){}
    Edge(int from,int to,int cap,int flow,int cost):from(from),to(to),cap(cap),flow(flow),cost(cost){}
};

int n,m;
int S,T;
vector<Edge> edge;
vector<int> G[N];
int dis[N];//单源最短路径
bool vis[N];//SPFA中用于判断是否在队列
int pre[N];//记录从S到i的最小费用路径上的最后一条弧编号

void init(){
    for(int i=0;i<n;i++)
        G[i].clear();
    edge.clear();
}
void addEdge(int from,int to,int cap,int cost){
    edge.push_back(Edge(from,to,cap,0,cost));
    edge.push_back(Edge(to,from,0,0,-cost));
    int m=edge.size();
    G[from].push_back(m-2);
    G[to].push_back(m-1);
}
bool SPFA(){
    for(int i=0;i<n;i++)
        dis[i]=INF;
    memset(vis,false,sizeof(vis));
    memset(pre,-1,sizeof(pre));
    dis[S]=0;
    vis[S]=true;
    pre[S]=0;

    queue<int> Q;
    Q.push(S);
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        vis[x]=false;

        for(int y=0;y<G[x].size();y++){
            Edge &e=edge[G[x][y]];
            if(e.cap>e.flow&&dis[e.to]>dis[x]+e.cost){
                dis[e.to]=dis[x]+e.cost;
                pre[e.to]=G[x][y];

                if(!vis[e.to]){
                    Q.push(e.to);
                    vis[e.to]=true;
                }
            }
        }
    }
    return pre[T]!=-1;
}
void MCMF(int &flow,int &cost){
    while(SPFA()){//每次寻找花销最小的路径
        int minn=INF;
        for(int i=pre[T];i!=-1;i=pre[edge[i^1].to])//寻找最小增广流
            minn=min(minn,edge[i].cap-edge[i].flow);

        for(int i=pre[T];i!=-1;i=pre[edge[i^1].to]){
            edge[i].flow+=minn;
            edge[i^1].flow-=minn;
            cost+=edge[i].cost*minn;//增广流的花销
        }
        flow+=minn;//总流量增加
    }
}

int main(){
    scanf("%d%d",&n,&m);
    init();
    for(int i=0;i<m;i++){
        int x,y,c,w;
        scanf("%d%d%d%d",&x,&y,&c,&w);
        addEdge(x,y,c,w);
        addEdge(y,x,-c,0);
    }
    S=1,T=n;
    int cost=0;//最小费用
    int flow=0;//最大流
    MCMF(flow,cost);
    printf("minCost:%d\n",cost);
    printf("maxFlow:%d\n",flow);
    return 0;
}
```






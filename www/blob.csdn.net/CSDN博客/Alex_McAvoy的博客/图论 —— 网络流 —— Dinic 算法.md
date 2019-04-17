# 图论 —— 网络流 —— Dinic 算法 - Alex_McAvoy的博客 - CSDN博客





2019年03月30日 21:50:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：56
个人分类：[图论——网络流](https://blog.csdn.net/u011815404/article/category/8562971)









# 【概述】

Dinic 算法在 EK 算法的基础上进行了优化，其时间复杂度为 O(n*n*m)。

Dinic 在找增广路的时也是找最短增广路, 但与 EK 算法不同的是 Dinic 算法并不是每次 bfs 只找一个增广路，他会首先通过一次 bfs 为所有点添加一个标号，构成一个层次图，然后在层次图中寻找增广路进行更新。

# 【基本思想】

1.初始化容量网络与网络流

2.构造残量网络，根据残留网络通过 BFS 计算层次网络，若汇点不在层次图中（汇点层次为 -1），则结束

3.在层次网络中使用一次 DFS 进行增广，DFS 执行完毕，该阶段的增广也执行完毕

4.转步骤 2

# 【求最大流】

## 1.一般方式

使用邻接矩阵的形式，适用于点较少的情况。

```cpp
#include<cstdio>
#include<string>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#define INF 0x3f3f3f3f
#define N 1001
int n,m;//n个点m条边
int S,T;//源点、汇点
int level[N];//存储结点分层层数
struct Edge{
    int cap;
    int flow;
}edge[N][N];
bool bfs(){//构造层次网络
    memset(level,0,sizeof(level));

    queue<int> Q;
    Q.push(S);
    level[S]=1;
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        for(int y=1;y<=n;y++){
            if(!level[y]&&edge[x][y].cap>edge[x][y].flow){
                level[y]=level[x]+1;
                Q.push(y);
            }
        }
    }
    return level[T]!=0;
}

int dfs(int x,int cp){//计算可增加流量
    if(x==n)
        return cp;

    int flow=cp;//记录从x到t的最小残量
    for(int y=1;y<=n;y++){
        if(level[x]+1==level[y]){
            if(edge[x][y].cap>edge[x][y].flow){
                int minn=min(flow,edge[x][y].cap-edge[x][y].flow);
                int newFlow=dfs(y,minn);
                edge[x][y].flow+=newFlow;
                edge[y][x].flow-=newFlow;
                flow-=newFlow;
            }
        }

        if(flow==0)
            break;
    }

    return cp-flow;
}
int dinic(){
    int flow=0;
    int tf=0;
    while(bfs()){
        while(tf=dfs(1,INF)){
            flow+=tf;
        }
    }
    return flow;
}
int main(){
    scanf("%d%d",&n,&m);
    memset(edge,0,sizeof(edge));
    while(m--){
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);
        edge[x][y].cap+=w;//便于处理重边
    }
    S=1,T=n;
    printf("%d\n",dinic());
    return 0;
}
```

## 2.vector 邻接表实现

使用 vector 邻接表与结构体结合，适用于点较多的情况，但速度稍慢于邻接矩阵。

```cpp
#include<cstdio>
#include<cstring>
#include<queue>
#include<vector>
#define INF 1e9
#define N 1001
using namespace std;

struct Edge{
    int from,to;
    int cap,flow;
    Edge(){}
    Edge(int from,int to,int cap,int flow):from(from),to(to),cap(cap),flow(flow){}

};
int n,m;             //结点数,边数(含反向弧)
int S,T;             //源点、汇点
vector<Edge> edges;  //边表,edges[e]和edges[e^1]互为反向弧
vector<int> G[N];    //邻接表,G[i][j]表示结点i的第j条边在e数组中的序号
bool vis[N];         //BFS使用,标记一个节点是否被遍历过
int dis[N];          //dis[i]表从起点s到i点的距离(层次)
int cur[N];          //cur[i]表当前正访问i节点的第cur[i]条弧
void addEdge(int from,int to,int cap){
    edges.push_back( Edge(from,to,cap,0) );
    edges.push_back( Edge(to,from,0,0) );
    int m=edges.size();
    G[from].push_back(m-2);
    G[to].push_back(m-1);
}
bool BFS(){//构建层次网络
    memset(vis,0,sizeof(vis));
    dis[S]=0;
    vis[S]=true;

    queue<int> Q;//用来保存节点编号
    Q.push(S);
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        for(int y=0;y<G[x].size();y++){
            Edge& e=edges[G[x][y]];
            if(!vis[e.to] && e.cap>e.flow){
                vis[e.to]=true;
                dis[e.to]=dis[x]+1;
                Q.push(e.to);
            }
        }
    }
    return vis[T];
}

int DFS(int x,int cp){//cp表示从s到x目前为止所有弧的最小残量
    if(x==T || cp==0)
        return cp;

    int flow=0,newFlow;//flow用来记录从x到t的最小残量
    for(int &y=cur[x];y<G[x].size();y++){
        Edge &e=edges[G[x][y]];
        if(dis[x]+1==dis[e.to]){
            int minn=min(cp,e.cap-e.flow);
            newFlow=DFS(e.to,minn);
            if(newFlow>0){
                e.flow+=newFlow;
                edges[G[x][y]^1].flow-=newFlow;
                flow+=newFlow;
                cp-=newFlow;

                if(cp==0)
                    break;
            }
        }
    }
    return flow;
}
int Dinic(){
    int flow=0;
    while(BFS()){
        memset(cur,0,sizeof(cur));
        flow+=DFS(S,INF);
    }
    return flow;
}

int main(){
    int n,m;
    scanf("%d%d",&n,&m);//节点编号从1到n,边编号从0到m-1
    for(int i=1;i<=n;i++)
            G[i].clear();
    edges.clear();

    for(int i=0;i<m;i++){
        int u,v,w;
        scanf("%d%d%d",&u,&v,&w);
        addEdge(u,v,w);
    }

    S=1,T=n;
    printf("%d\n",Dinic());
    return 0;
}
```

## 3.邻接数组

使用邻接数组，速度快于 vector 邻接表，适用于点较多的情况。

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#include<set>
#define INF 0x3f3f3f3f
#define N 1001
using namespace std;
struct Edge{
    int cap,flow;
    int from,to;
}edge[N];
int n,m;
int S,T;
int head[N],cnt;
int level[N];
int cur[N];
void addEdge(int from,int to,int cap){//添边
    edge[cnt].to=to;
    edge[cnt].from=head[from];
    edge[cnt].cap=cap;
    head[from]=cnt++;

    edge[cnt].to=from;
    edge[cnt].from=head[to];
    head[to]=cnt++;
}
bool bfs(){//构建层次网络
    memset(level,0,sizeof(level));

    queue<int> Q;
    Q.push(S);
    level[S]=1;

    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        for(int y=head[x];y!=-1;y=edge[y].from){
            int to=edge[y].to;
            int cap=edge[y].cap,flow=edge[y].flow;
            if(!level[to]&&cap>flow){
                level[to]=level[x]+1;
                Q.push(to);
            }
        }
    }
    return level[T];

}
int dfs(int x,int cp){//计算可增加流量
    if(x==T||cp==0)
        return cp;

    int ans=0,newFlow;
    for(int &y=cur[x];y!=-1;y=edge[y].from){
        int to=edge[y].to;
        int cap=edge[y].cap,flow=edge[y].flow;

        int minn=min(cp,cap-flow);
        newFlow=dfs(to,minn);
        if( level[to]==level[x]+1 && newFlow>0 ){

            edge[y].flow+=newFlow;
            edge[y^1].flow-=newFlow;
            ans+=newFlow;
            cp-=newFlow;

            if(cp==0)
                break;
        }
    }
    return ans;
}
int dinic(){
    int flow=0;
    while(bfs()){
        for(int i=1;i<=n;i++)
            cur[i]=head[i];

        flow+=dfs(S,INF);
    }
    return flow;
}
int main(){
    cnt=0;
    memset(head,-1,sizeof(head));

    scanf("%d%d",&n,&m);
    while(m--){
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);
        addEdge(x,y,w);
    }

    S=1,T=n;
    printf("%d\n",dinic());

    return 0;
}
```

# 【求最小割】

使用邻接数组与 STL 中的 set 容器，在求最大流的过程中，将最小割求出。

其原理是最大流最小割定理：**网络的最大流的流量等于最小割的容量**

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#include<set>
#define INF 0x3f3f3f3f
#define N 1001
using namespace std;
struct Edge{
    int cap,flow;
    int from,to;
}edge[N];
int n,m;
int S,T;
int head[N],cnt;
int level[N];
int cur[N];
bool flag;
set<int> cutSet;
void addEdge(int from,int to,int cap){//添边
    edge[cnt].to=to;
    edge[cnt].from=head[from];
    edge[cnt].cap=cap;
    head[from]=cnt++;

    edge[cnt].to=from;
    edge[cnt].from=head[to];
    head[to]=cnt++;
}
bool bfs(){//构建层次网络
    memset(level,0,sizeof(level));

    queue<int> Q;
    Q.push(S);
    level[S]=1;

    if(flag)
        cutSet.insert(S);

    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        for(int y=head[x];y!=-1;y=edge[y].from){
            int to=edge[y].to;
            int cap=edge[y].cap,flow=edge[y].flow;
            if(!level[to]&&cap>flow){
                level[to]=level[x]+1;
                Q.push(to);

                if(flag)
                    cutSet.insert(to);
            }
        }
    }
    return level[T];

}
int dfs(int x,int cp){//计算可增加流量
    if(x==T||cp==0)
        return cp;

    int ans=0,newFlow;
    for(int &y=cur[x];y!=-1;y=edge[y].from){
        int to=edge[y].to;
        int cap=edge[y].cap,flow=edge[y].flow;

        int minn=min(cp,cap-flow);
        newFlow=dfs(to,minn);
        if( level[to]==level[x]+1 && newFlow>0 ){

            edge[y].flow+=newFlow;
            edge[y^1].flow-=newFlow;
            ans+=newFlow;
            cp-=newFlow;

            if(cp==0)
                break;
        }
    }
    return ans;
}
int dinic(){
    int flow=0;
    while(bfs()){
        for(int i=1;i<=n;i++)
            cur[i]=head[i];

        flow+=dfs(S,INF);
    }
    return flow;
}
int main(){
    cnt=0;
    memset(head,-1,sizeof(head));

    scanf("%d%d",&n,&m);
    while(m--){
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);
        addEdge(x,y,w);
    }

    S=1,T=n;
    flag=false;
    printf("Max Flow:%d\n",dinic());//最大流
    flag=true;
    bfs();
    printf("The Number of Min Cut:%d\n",cutSet.size());//最小割个数
    printf("The Min Cut Set:");

    set<int>::iterator it=cutSet.begin();
    for(;it!=cutSet.end();it++)//最小割
        printf("%d ",*it);
    printf("\n");
    return 0;
}
```




# Birthday - westbrook1998的博客 - CSDN博客





2018年10月06日 19:31:54[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：51








> 
链接：[https://www.nowcoder.com/acm/contest/206/A](https://www.nowcoder.com/acm/contest/206/A)

来源：牛客网

题目描述

恬恬的生日临近了。宇扬给她准备了一个蛋糕。

正如往常一样，宇扬在蛋糕上插了n支蜡烛，并把蛋糕分为m个区域。因为某种原因，他必须把第i根蜡烛插在第ai个区域或第bi个区域。区域之间是不相交的。宇扬在一个区域内同时摆放x支蜡烛就要花费x2的时间。宇扬布置蛋糕所用的总时间是他在每个区域花的时间的和。

宇扬想快些见到恬恬，你能告诉他布置蛋糕最少需要多少时间吗？

输入描述:

第一行包含两个整数n，m（1 ≤ n ≤ 50， 2≤ m≤ 50）。

接下来n行，每行两个整数ai,bi（1 ≤ ai, bi ≤ m）。

输出描述:

一个整数表示答案。

示例1

输入

复制

3 3

1 2

1 2

1 2

输出

复制

5

示例2

输入

复制

3 3

1 2

2 3

1 3

输出

复制

3
第一眼觉得是二分图匹配，多次匹配，抄了个模板改了改没过，赛后测了是95.4%，然后觉得也挺像网络流，就用最小费用最大流写了下过了

网络流的问题主要就是建图，这里加个源点和汇点，源点到蜡烛加一条流量1费用0的边，蜡烛到蛋糕也是加流量1费用0的边，然后主要是蛋糕和汇点这里的边，流量也是1，而这个费用，因为对一个蛋糕来说，一个蜡烛就是1时间，两个蜡烛是4，三个是9，这样的话每多一个蜡烛（也就是多一个从蜡烛流过来的流量，流向汇点），就应该多1 3 5 。。。99费用

然后跑一下最小费用最大流模板
代码：

```
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;
const int N=550;
const int INF=0x3f3f3f3f;
struct Edge{
    int from,to,cap,flow,cost;
};
int n,m;
vector<Edge> edges;
vector<int> G[N];
int a[N],p[N];
void init(){
    for(int i=0;i<N;i++){
        G[i].clear();
    }
    edges.clear();
}
void addEdge(int from,int to,int cap,int cost){
    edges.push_back(Edge{from,to,cap,0,cost});
    edges.push_back(Edge{to,from,0,0,-cost});
    m=edges.size();
    G[from].push_back(m-2);
    G[to].push_back(m-1);
}
int d[N];
bool vis[N];
int cost;
int flow;
void spfa(int s,int t){
    queue<int> q;
    memset(vis,false,sizeof(vis));
    for(int i=0;i<N;i++){
        d[i]=INF;
    }
    d[s]=0;
    q.push(s);
    vis[s]=true;
    p[s]=0;
    a[s]=INF;
    while(!q.empty()){
        int v=q.front();
        vis[v]=false;
        q.pop();
        for(int i=0;i<G[v].size();i++){
            Edge &e=edges[G[v][i]];
            if(e.cap>e.flow && d[e.to]>d[v]+e.cost){
                d[e.to]=d[v]+e.cost;
                p[e.to]=G[v][i];
                a[e.to]=min(a[v],e.cap-e.flow);
                if(!vis[e.to]){
                    q.push(e.to);
                    vis[e.to]=true;
                }
            }
        }
    }
}
void mcmf(int s,int t){
    flow=0;
    cost=0;
    while(true){
        spfa(s,t);
        if(d[t]==INF){
            break;
        }
        flow+=a[t];
        cost+=d[t]*a[t];
        for(int u=t;u!=s;u=edges[p[u]].from){
            edges[p[u]].flow+=a[t];
            edges[p[u]^1].flow-=a[t];
        }
    }
}
int nn,mm;
int mat[N][2];
int main(void){
    scanf("%d%d",&nn,&mm);
    int s=nn+mm+1;
    int t=nn+mm+2;
    for(int i=1;i<=nn;i++){
        addEdge(s,i,1,0);
    }
    for(int i=1;i<=mm;i++){
        for(int j=1;j<=50;j++){
            addEdge(nn+i,t,1,j*2-1);
        }
    }
    for(int i=1;i<=nn;i++){
        scanf("%d",&mat[i][0]);
        scanf("%d",&mat[i][1]);
        //printf("%d %d\n",i,nn+mat[i][0]);
        //printf("%d %d\n",i,nn+mat[i][1]);
        addEdge(i,nn+mat[i][0],1,0);
        addEdge(i,nn+mat[i][1],1,0);
    }
    mcmf(s,t);
    printf("%d\n",cost);
    return 0;
}
```







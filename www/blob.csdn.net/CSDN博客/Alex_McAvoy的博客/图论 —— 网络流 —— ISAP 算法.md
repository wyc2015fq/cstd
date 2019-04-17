# 图论 —— 网络流 —— ISAP 算法 - Alex_McAvoy的博客 - CSDN博客





2019年03月30日 21:51:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49








# 【概述】

ISAP（Improved Shortest Augment Path）算法其实是通过 dfs 中不断修改层次标号 level 的方式省去了每次的 bfs，所以称为improved。

EK 算法直接进行增广，而 Dinic 则是每次进行 bfs 求出层次图再 dfs 沿着层次图进行多路增广。

然而，Dinic 中每次进行 bfs 求层次图有些浪费，因为层次图的改动并不是很大，在这种思路下，因此考虑直接在每次 dfs 增广的时候修改层次图来优化求最短路的过程。

ISAP 的时间复杂度上界与 Dinic 相同，同样是 O(n*n*m)

# 【基本思想】

Dinic 算法中，需要每次搜索出层次图，而在 ISAP 中，只需要每次在 dfs 的过程中修改层次标号。

具体来说，用 level[x] 表示残余网络上 x 到汇点 t 的最短层次，每次沿着 level[x] = level[v] + 1 的路增广，如果点 x 的出边的点没有发现满足这个条件，那么说明当前的最短路已经过时，需要修改层次标号。

修改层次标号，就是让 x 可以至少有一个点能够增广，所以取所有 level[v] 中最小的那个加一即可，这样增广下去，当 level[s]>=n 时，结束算法，即：s 到 t 的距离大于等于 n 时，说明至少有一个点经过了两次，即不存在增广路。

# 【优化过程】
- 若一开始将层次标号都设为 0，那么 dfs 最多需要 O(n*n) 来将标号初始化，但可以最开始逆向 bfs 一次，在 O(n+m) 的范围内初始化所有层次标号
- 若层次标号出现了断层，那么显然不存在新的增广路。可以用一数组 gpa 来记录每种层次标号有多少个，若当前修改最后一个某种层次标号，那么就出现了前后断层，从而结束算法
- 增广过程中，若一个点的标号没有修改过，那么它已经遍历过的边不需要再遍历一次，因此存下每次遍历到哪条边，下一次从这条边开始遍历（可能到这里后流量用完但还未增广完）
- 最短路的修改具有连续性，即不需要每次求后继的标号最小值，而是直接给标号加一
- 同 Dinic，若流量用完，直接退出

# 【实现】

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
#define N 1001
using namespace std;
struct Edge{
    int from,to;
    int flow,cap;
    Edge(){}
    Edge(int from,int to,int cap,int flow):from(from),to(to),cap(cap),flow(flow){}
};
int n,m;
int S,T;
bool vis[N];//逆向bfs的标记数组
int level[N];//记录层次
int father[N];//标记父节点
int gap[N];//记录每组层次标号有几个
int cur[N];//当前正访问i节点的第cur[i]条弧
vector<Edge> edge;
vector<int> G[N];

void addEdge(int from,int to,int cap){//添边
    edge.push_back(Edge(from,to,cap,0));
    edge.push_back(Edge(to,from,0,0));
    int m=edge.size();
    G[from].push_back(m-2);
    G[to].push_back(m-1);
}
int augument(int x,int cp){
    while(x!=S){//寻找最小残量
        Edge &e=edge[father[x]];
        cp=min(cp,e.cap-e.flow);
        x=edge[father[x]].from;
    }

    x=T;
    while(x!=S){//增广
        edge[father[x]].flow+=cp;
        edge[father[x]^1].flow-=cp;
        x=edge[father[x]].from;
    }
    return cp;
}
void bfs(){//逆向bfs
    memset(vis,false,sizeof(vis));
    level[T]=0;
    vis[T]=1;

    queue<int> Q;
    Q.push(T);
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();

        for(int y=0;y<G[x].size();y++){
            Edge &e=edge[G[x][y]];
            if(!vis[e.from]&&e.cap>e.flow){
                vis[e.from]=true;
                level[e.from]=level[x]+1;
                Q.push(e.from);
            }
        }
    }
}
int ISAP(){//根据情况前进或后退，走到汇点时增广
    bfs();

    memset(cur,0,sizeof(cur));
    memset(gap,0,sizeof(gap));
    for(int i=0;i<n;i++)
        gap[level[i]]++;

    int x=S;
    int flow=0;
    while(level[S]<n){
        if(x==T){//走到汇点，进行增广
            flow+=augument(T,INF);
            x=S;//增广后回到源点
        }

        bool flag=false;
        for(int y=cur[x];y<G[x].size();y++){
            Edge &e=edge[G[x][y]];
            if(level[x]==level[e.to]+1&&e.cap>e.flow){
                flag=true;
                father[e.to]=G[x][y];//记录来时走的父边
                cur[x]=y;
                x=e.to;//前进
                break;
            }
        }

        if(!flag){//无法前进，后退
            int m=n-1;//若没有弧，则m+1=n，即level[i]=n
            for(int y=0;y<G[x].size();y++){
                Edge &e=edge[G[x][y]];
                if(e.cap>e.flow)
                    m=min(m,level[e.to]);
            }

            //gap优化
            if(--gap[level[x]]==0)//如果走不动了，且这个距离值原来只有一个，那么s-t不连通
                break;

            gap[level[x]=m+1]++;
            cur[x]=0;
            if(x!=S)//退一步，沿父边返回
                x=edge[father[x]].from;
        }
    }
    return flow;
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(level,0,sizeof(level));
        for(int i=0;i<n;i++)
            G[i].clear();
        edge.clear();

        for(int i=0;i<m;i++){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            addEdge(x,y,w);
        }
        S=1,T=n;
        printf("%d\n",ISAP());
    }
    return 0;
}
```






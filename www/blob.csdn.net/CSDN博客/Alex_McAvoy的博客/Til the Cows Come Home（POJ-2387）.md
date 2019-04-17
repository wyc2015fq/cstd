# Til the Cows Come Home（POJ-2387） - Alex_McAvoy的博客 - CSDN博客





2018年11月07日 22:51:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：31








> 
# Problem Description

Bessie is out in the field and wants to get back to the barn to get as much sleep as possible before Farmer John wakes her for the morning milking. Bessie needs her beauty sleep, so she wants to get back as quickly as possible. 

Farmer John's field has N (2 <= N <= 1000) landmarks in it, uniquely numbered 1..N. Landmark 1 is the barn; the apple tree grove in which Bessie stands all day is landmark N. Cows travel in the field using T (1 <= T <= 2000) bidirectional cow-trails of various lengths between the landmarks. Bessie is not confident of her navigation ability, so she always stays on a trail from its start to its end once she starts it. 

Given the trails between the landmarks, determine the minimum distance Bessie must walk to get back to the barn. It is guaranteed that some such route exists.

# **Input**

* Line 1: Two integers: T and N 

* Lines 2..T+1: Each line describes a trail as three space-separated integers. The first two integers are the landmarks between which the trail travels. The third integer is the length of the trail, range 1..100.

# Output

* Line 1: A single integer, the minimum distance that Bessie must travel to get from landmark N to landmark 1.

# Sample Input

**5 51 2 202 3 303 4 204 5 201 5 100**

# Sample Output

**90**


题意：给出一 n 个点 t 条边的无向图，求从 1 号点到 n 号点的最短距离

思路：可能存在重边，使用 Dijkstra 标准版模版直接处理即可

# **Source Program**

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
int n,m;
struct Edge{//边
    int from;//下一条边的编号
    int to;//边到达的点
    int dis;//边的长度
    Edge(int f,int t,int d){//构造函数
        from=f;
        to=t;
        dis=d;
    }
};

struct HeapNode{//Dijkstra用到的优先队列的结点
    int dis;//点到起点距离
    int u;//点的序号
    HeapNode(int a,int b){
        dis=a;
        u=b;
    }
    bool operator < (const HeapNode &rhs) const  {
        return dis > rhs.dis;
    }
};

struct Dijkstra{
    int n,m;//边数、点数，均从0开始
    vector<Edge> edges;//边列表
    vector<int> G[N];//每个结点出发的边的编号
    bool vis[N];//是否走过
    int dis[N];//起点s到各点的距离
    int p[N];//从起点s到i的最短路中的最后一条边的编号

    void init(int n) {//初始化
        this->n = n;
        for(int i=0;i<n;i++) //清空邻接表
            G[i].clear();
        edges.clear();//清空边列表
    }

    void AddEdge(int from,int to,int diss) {//添加边，若为无向图，调用两次
        edges.push_back(Edge(from,to,diss));
        m=edges.size();//边的个数
        G[from].push_back(m-1);//添加至边列表
    }

    int dijkstra() {//求s到所有点的距离

        memset(dis,INF,sizeof(dis));
        memset(vis,false,sizeof(vis));
        dis[0]=0;

        priority_queue<HeapNode> Q;//优先队列
        Q.push(HeapNode(0,0));
        while(!Q.empty()) {
            HeapNode x=Q.top();
            Q.pop();

            int u=x.u;
            if(vis[u])//若已被访问
                continue;

            vis[u]=true;//标记为已访问
            for(int i=0;i<G[u].size();i++) {//枚举所有与当前点相连的边
                Edge &e=edges[G[u][i]];
                if(dis[e.to] > dis[u]+e.dis) {//更新距离
                    dis[e.to] = dis[u]+e.dis;
                    Q.push(HeapNode(dis[e.to],e.to));
                }
            }
        }
        return dis[n-1];
    }
}DJ;

int main()
{
    while(scanf("%d%d",&m,&n)!=EOF&&(n+m))
    {
        DJ.init(n);//初始化
        for(int i=0;i<m;i++){
            int x,y,dis;
            scanf("%d%d%d",&x,&y,&dis);
            x--;y--;
            DJ.AddEdge(x,y,dis);
            DJ.AddEdge(y,x,dis);
        }

        int res=DJ.dijkstra();
        printf("%d\n",res);

    }
    return 0;
}
```






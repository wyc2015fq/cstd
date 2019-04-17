# 图论 —— Dijkstra 算法 - Alex_McAvoy的博客 - CSDN博客





2019年03月18日 19:36:24[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：110
个人分类：[图论——Dijkstra																[——————图论——————](https://blog.csdn.net/u011815404/article/category/7664328)](https://blog.csdn.net/u011815404/article/category/8312429)








# 【概述】

Dijkstra 算法是单源最短路径算法，即计算起点只有一个的情况到其他点的最短路径，其无法处理存在负边权的情况。

其时间复杂度是：O(N*N)，N 是顶点数

# 【算法描述】

设起点为 s，dis[v] 表示从 s 到 v 的最短路径，pre[v] 为 v 的前驱结点，vis[v] 用于记录 v 是否被访问过。

1.初始化：

dis[v]=0x3f3f3f3f（v≠s），vis[v]=false，即：从始点到各点的值初始化为一极大值，所有点均标记为未访问

dis[s]=0，pre[s]=0，即：始点到始点的距离为 0，且其没有前驱结点

2.算法主体：

```cpp
for(int i=1;i<=n;i++) {
    int min=INF;
    int u=0;
    
    for(int v=1;v<=n;v++) { //在没有被访问过的点中找一个顶点u，使得dis[u]是最小的
        if( vis[v]==false && dis[v]<min) {
            min=dis[v];
            u=v;
        }
    }
    if(u==0)
        break;
    
    vis[u]=true;//u标记为已确定的最短路径
    for(int v=1;j<=n;j++) { //枚举与u相连的每个未确定的最短路的顶点
        if(dis[u]+w[u][v]<dis[v]) {
            dis[j]=dis[u]+w[u][v]; //更新最短路径
            pre[v]=u;//记录前驱
        }
    }
}
```

3.算法结束：

dis[v] 即为 s 到 v 最短距离，pre[v] 即为 v 的前驱结点，用来输出路径。

# 【算法分析】

将点分为两类，一类是已确定最短路径的点，称为：白点，一类是未确定最短路径的点，称为：蓝点。

求一个点的最短路径，就是把这个点由蓝点变为白点，从起点到蓝点的最短路径上的中转点在这个时刻只能是白点。

Dijkstra 算法的思想，就是一开始将起点到终点的距离标记为 0，而后进行 n 次循环，每次找出一个到起点距离 dis[u] 最短的点 u ，将它从蓝点变为白点，随后枚举所有白点 Vi，如果以此白点为中转到达蓝点 Vi 的路径 dis[u]+w[u][vi] 更短的话，这将它作为 Vi 的更短路径（此时还不能确定是不是Vi的最短路径）。

以此类推，每找到一个白点，就尝试用它修改其他所有蓝点，中转点先于终点变成白点，故每一个终点一定能被它的最后一个中转点所修改，从而求得最短路径。

以下图为例

![](https://img-blog.csdn.net/20180515112759559)

算法开始时，作为起点的 dis[1]=0，其他的点 dis[i]=0x3f3f3f3f

第一轮循环找到 dis[1] 最小，将 1 变为白点，对所有蓝点进行修改，使得：dis[2]=2，dis[3]=4，dis[4]=7

![](https://img-blog.csdn.net/20180515112935129)

此时，dis[2]、dis[3]、dis[4] 被它的最后一个中转点 1 修改了最短路径。

第二轮循环找到 dis[2] 最小，将 2 变成白点，对所有蓝点进行修改，使得：dis[3]=3、dis[5]=4

![](https://img-blog.csdn.net/20180515113319994)

此时，dis[3]、dis[5] 被它的最后一个中转点 2 修改了最短路径。

第三轮循环找到 dis[3] 最小，将 3 变成白点，对所有蓝点进行修改，使得：dis[4]=4。

![](https://img-blog.csdn.net/20180515113540526)

此时，dis[4] 被它的最后一个中转点 3 修改了最短路径，但发现以 3 为中转不能修改 5，说明 3 不是 5 的最后一个中转点。

接下来两轮循环将 4、5 也变成白点。

![](https://img-blog.csdn.net/20180515113810932)

N轮循环结束，所有点的最短路径均可求出。

![](https://img-blog.csdn.net/20180515114145380)

# 【模版】

## 1.简化版

简化版的时间复杂度为 O(n*n)，其不可处理重边图

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
#define INF 1e8
#define N 100000
int n;//图节点数目,从1到n编号
int dis[N];//单源最短距离
int G[N][N];//G[i][j]表示i到j的有向边长
bool vis[N];//表示w[i]是否已经计算完
void dijkstra(int s)//进入此函数前，需要将所有边的距离保存在dis中
{
    memset(vis,0,sizeof(vis));
    for(int i=1;i<=n;i++) {
        if(i==s)
            dis[i]=0;
        else
            dis[i]=INF;
    }

    for(int i=1;i<=n;i++)
    {
       
        int x;//x标记当前最短w的点
        int min_dis=INF;//记录当前最小距离
 
        for(int y=1;y<=n;y++){
            if(!vis[y] && min_dis>=dis[y]){
                x=y;
                min_dis=dis[x];
            }
        }

        vis[x]=true;
 
        for(int y=1;y<=n;y++) 
            dis[y]=min(dis[y],dis[x]+G[x][y]);
    }
}
```

## 2.标准版

标准版的时间复杂度为 O(mlogn)，适用于稀疏图，可处理重边图

```cpp
/* 
输入
5 7
1 2 2
1 3 4
1 4 7
2 3 1
3 4 1
3 5 6
2 5 2
1
运行结果
1->1: 0
1->2: 2
1->3: 3
1->4: 4
1->5: 4
*/
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#define INF 0x3f3f3f3f
#define N 1001
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

    void dijkstra(int s) {//求s到所有点的距离

        memset(dis,INF,sizeof(dis));
        memset(vis,false,sizeof(vis));
        dis[0]=0;

        priority_queue<HeapNode> Q;//优先队列
        Q.push(HeapNode(0,s));
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
    }
}DJ;

int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m))
    {
        DJ.init(n);//初始化
        for(int i=0;i<m;i++) {
            int x,y,dis;
            scanf("%d%d%d",&x,&y,&dis);
            x--;y--;
            //无向图添边两次
            DJ.AddEdge(x,y,dis);
            DJ.AddEdge(y,x,dis);
        }

        int start;
        scanf("%d",&start);
        start--;
        DJ.dijkstra(start);//求start到各点的距离
        for(int i=0,j=0,s=++start;i<n;i++)//输出start到各点的距离
            printf("%d->%d: %d\n",s,++j,DJ.dis[i]);
    }
    return 0;
}
```

# 【例题】
- 一个人的旅行（HDU-2066）**(Dijkstra)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80399840)
- 畅通工程续（HDU-1874）**(Dijkstra)**：[点击这里](https://blog.csdn.net/u011815404/article/details/83833581)
- MPI Maelstrom（POJ-1502）**(Dijkstra)**：[点击这里](https://blog.csdn.net/u011815404/article/details/83833185)
- Til the Cows Come Home（POJ-2387）**(Dijkstra)**：[点击这里](https://blog.csdn.net/u011815404/article/details/83833406)
- Here We Go(relians) Again（HDU-2722）**(输入预处理+Dijkstra)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80399975)
- 最小花费（信息学奥赛一本通-T1344）**(输入预处理+Dijkstra)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80752632)
- A Walk Through the Forest（HDU-1142）**(dfs搜索路径+Dijkstra)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80397058)
- Silver Cow Party（POJ-3268）**(双向最短路)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80820561)
- The Battle of Guandu（HDU-5545）**(多源多汇最短路)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86561902)
- 炫酷路途（2019牛客寒假算法基础集训营 Day5-D）**(建图+边权计算)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86763089)





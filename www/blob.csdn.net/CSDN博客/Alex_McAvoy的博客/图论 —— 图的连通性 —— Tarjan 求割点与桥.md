# 图论 —— 图的连通性 —— Tarjan 求割点与桥 - Alex_McAvoy的博客 - CSDN博客





2019年01月30日 15:06:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：90








# 【概念】

## 1.割点

1）割点：删除某点后，整个图变为不连通的两个部分的点

2）割点集合：在一个无向图中删除该集合中的所有点，能使原图变成互不相连的连通块的点的集合

3）点连通度：最小割点集合点数

![](https://img-blog.csdn.net/20181021133850418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如上图，若去掉 0，则图被分成 12 和 34 两个连通分量；若去掉 3，则图被分成 012 和 4 两个连通分量。

故：0、3 是割点，两者是一个割点集，点连通度是 2

## 2.桥

1）桥（割边）：删除某边后，使得连通图变为不连通的图，一图中可能有多条割边

2）割边集合：所有割边的集合

3）边连通度：最小割边集合边数

![](https://img-blog.csdn.net/20181021134927333?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如上图，去掉 5-6，2-5 后不再连通，图被分为 1234、5、6 三个连通分量

故：5-6、2-5 是割边，两者组成的集是割边集，边连通度是 2

# 【原理】

## 1.求割点

对于一棵 DFS 搜索树：

1）根结点：当且仅当根节点至少有两个儿子时，其是割点

2）其他点：对于其他点 v，当仅有一个儿子 u 时，从 u 或 u 的后代出发，没有指向 v 祖先(不含 v)的边，则删除 v后，u 和 v 的父亲不连通时，v 是割点

## 2.求桥

对于边 (u,v)，若发现 v 和它的后代不存在一条连接 u 或其祖先的边，则删除 (u,v) 后 u 和 v 不连通，因此边 (u,v) 为桥

# 【过程】

## 1.求割点

1）对图进行 Tarjan 算法，得到 dfn 和 low 两个数组

2）每遍历一个新的 u 的儿子 v 都记录个数

3）low[u] 值更新后进行以下判断（前提：v 未被遍历）：

    ① u 为树根，且儿子个数大于 1

    ② u 不为树根，但 low[v]>=dfn[u]

    满足以上任意条件 u 便为割点，记录在一数组里，Tarjan 完成后再输出即可（中途输出会重复）

## 2.求桥

判断原则：若 low[v]>dfn[u]，则 (u,v) 为割边。

由于有的图有重边，因此不好通过上述判断原则来处理，故使用以下方法：

1）对图进行 Tarjan 算法，得到 dfn 和 low 两个数组

2）形参加上 father，作用为记录 u 的父亲节点，避免在遍历 v 时遇到重边重新更新 low[u]

3）在 v 已被遍历的位置加上判断：如果 v 点等于 father，那么就不更新 low[u] 值

4）Tarjan算法结束后，遍历所有节点u及其子节点v，如果 low[u]==low[v]，那么这条边就为割边

# 【实现】

## 1.先将数据保存下来，再求割点与桥

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#define N 201
using namespace std;
int n,m;
int low[N],dfn[N];
vector<int>G[N];
bool is_cut[N];//记录是否为割点
int father[N];
int tim;//时间戳
void Tarjan(int x,int pre)
{
    father[x]=pre;//记录每一个点的父亲
    dfn[x]=low[x]=tim++;

    for(int y=0;y<G[x].size();y++)
    {
        int temp=G[x][y];
        if(dfn[temp]==-1)
        {
            Tarjan(temp,x);
            low[x]=min(low[x],low[temp]);
        }
        else if(pre!=temp)//假如k是i的父亲的话，那么这就是无向边中的重边，有重边那么一定不是桥
            low[x]=min(low[x],dfn[temp]);//可能dfn[k]！=low[k]，所以不能用low[k]代替dfn[k],否则会上翻过头
    }
}

int main()
{
    scanf("%d%d",&n,&m);

    for(int i=1;i<=m;++i)
    {
        int x,y;
        scanf("%d%d",&x,&y);
        G[x].push_back(y);
        G[y].push_back(x);
    }

    tim=0;
    memset(low,-1,sizeof(low));
    memset(dfn,-1,sizeof(dfn));
    memset(father,0,sizeof(father));
    memset(is_cut,false,sizeof(is_cut));


    Tarjan(1,0);

    int root=0;
    for(int i=2;i<=n;i++)//统计根节点子树的个数，根节点的子树个数>=2,即为割点
    {
        int v=father[i];
        if(v==1)
            root++;
        else{
            if(low[i]>=dfn[v])//割点的条件
                is_cut[v]=true;
        }
    }
    if(root>1)
        is_cut[1]=true;


    //打印割点
    for(int i=1;i<=n;++i)
        if(is_cut[i])
            printf("%d ",i);
    printf("\n");


    for(int i=1;i<=n;++i){
        int v=father[i];
        if(v>0&&low[i]>dfn[v])//桥的条件
            printf("%d,%d\n",v,i);
    }

    return 0;
}
```

## 2.在 Tarjan 的过程中求桥

```cpp
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#include<vector>
#include<map>
#define INF 0x3f3f3f3f
#define N 100001
using namespace std;
struct Edge{
    int from,to,next;
    int cut;//记录该边是否为桥
    Edge(){}
    Edge(int from,int to,int next,int cut):from(from),to(to),next(next),cut(cut){}
}edge[N];
int n,m;
int head[N],edgeNum;
int dfn[N],low[N];
int block_cnt;
void addEdge(int from,int to){//添边
    edge[edgeNum]=Edge(from,to,head[from],false);
    head[from]=edgeNum++;
}
void tarjin(int x,int father){
    low[x]=dfn[x]=++block_cnt;
    for(int i=head[x];i!=-1;i=edge[i].next){
        int y=edge[i].to;
        if(!dfn[y]){
            tarjin(y,x);
            if(low[x]>low[y])
                low[x]=low[y];
            if(low[y]>dfn[x])
                edge[i].cut=true;
        }
        else if(y!=father)
            low[x]=min(low[x],dfn[y]);
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(dfn,0,sizeof(dfn));
        memset(head,-1,sizeof(head));
        edgeNum=0;
 
        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            addEdge(x,y);
            addEdge(y,x);
        }
 
        tarjin(1,1);   

        for(int i=0;i<edgeNum;i++)//打印桥
            if(edge[i].cut)
                printf("%d %d\n",edge[i].from,edge[i].to);
    }
    return 0;
}
```




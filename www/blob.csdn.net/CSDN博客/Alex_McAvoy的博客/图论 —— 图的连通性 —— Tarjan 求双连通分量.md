# 图论 —— 图的连通性 —— Tarjan 求双连通分量 - Alex_McAvoy的博客 - CSDN博客





2019年01月30日 15:10:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58








# 【概念】

1.双连通分量：对于一个无向图，其边/点连通度大于1，满足任意两点之间，能通过两条或两条以上没有任何重复边的路到达的图，即删掉任意边/点后，图仍是连通的

2.分类：

    1）点双连通图：点连通度大于 1 的图

    2）边双连通图：边连通度大于 1 的图

# 【原理】

## 1.求点双连通分量

求点双连通分量可以在求割点的同时用栈维护。

在搜索图时，每找到一条树枝边或后向边(非横叉边)，就把这条边加入栈中。如果遇到满足 dfn(u)<=low(v)，说明 u 是一个割点，同时把边从栈顶一个个取出，直到遇到了边 (u,v)，取出的这些边与其关联的点，就组成一个点双连通分支。

割点可以属于多个点双连通分支，其余点和每条边只属于且属于一个点双连通分支。

## 2.求边双连通分量

求边双连通分量时，需要先求出桥，然后把桥全部去掉，原图变成多个连通块，此时每个连通块就是一个边双连通分量。

桥不属于任何一个边双连通分量，其余的边和每个顶点都属于且只属于一个边双连通分量

# 【过程】

## 1.求点双连通分量

1）Tarjan 求割点

2）每找到一个割点，将它上面的所有点弹出栈，所得到的点集就是点双连通分量

## 2.求边双连通分量

1）Tarjan 找桥

2）删除桥

3）剩余各部分即为边双连通分量

# 【实现】

## 1.求点双连通分量

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<stack>
#include<vector>
#define N 20001
using namespace std;
int n,m;
vector<int> G[N];
vector<int> bcc[N];//包含i号点双连通分量的所有结点
int dfn[N];
bool iscut[N];//记录i结点是否是割点
int bccno[N];//记录第i个点属于第几号点双连通分量
int block_cnt;//时间戳
int bcc_cnt;//记录点双连通分量个数
struct Edge{
    int x;
    int y;
};
stack<Edge> S;
int Tarjan(int x,int father){
    int lowx=dfn[x]=++block_cnt;
    int child=0;//子节点数目
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];//当前结点的下一结点
        Edge e;
        e.x=x;
        e.y=y;
        if(dfn[y]==0){//若未被访问过
            S.push(e);
            child++;//未访问过的节点才能算是x的孩子

            int lowy=Tarjan(y,x);
            lowx=min(lowx,lowy);
            if(lowy>=dfn[x])
            {
                iscut[x]=true;//x点是割点
                bcc_cnt++;
                bcc[bcc_cnt].clear();

                while(true){
                    Edge temp=S.top();
                    S.pop();
                    if(bccno[temp.x]!=bcc_cnt)
                    {
                        bcc[bcc_cnt].push_back(temp.x);
                        bccno[temp.x]=bcc_cnt;
                    }
                    if(bccno[temp.y]!=bcc_cnt)
                    {
                        bcc[bcc_cnt].push_back(temp.y);
                        bccno[temp.y]=bcc_cnt;
                    }
                    if(temp.x==x && temp.y==y)
                        break;
                }
            }
        }
        else if(dfn[y]<dfn[x] && y!=father){//y!=father确保了(x,y)是从x到y的反向边
            S.push(e);
            lowx=min(lowx,dfn[y]);
        }
    }

    if(father<0 && child==1 )//x若是根且孩子数<=1,那x就不是割点
        iscut[x]=false;
    return lowx;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        for(int i=0;i<n;i++)
            G[i].clear();
        while(m--)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
        }

        bcc_cnt=0;
        block_cnt=0;
        memset(dfn,0,sizeof(dfn));
        memset(iscut,0,sizeof(iscut));
        memset(bccno,0,sizeof(bccno));
        for(int i=0;i<n;i++)
            if(!dfn[i])
                Tarjan(i,-1);

        printf("点-双连通分量一共%d个\n",bcc_cnt);
        for(int i=1;i<=bcc_cnt;i++)        {
            printf("第%d个点-双连通分量包含以下点:\n",i);
            sort(&bcc[i][0],&bcc[i][0]+bcc[i].size()); //对vector排序,使输出的点从小到大
            for(int j=0;j<bcc[i].size();j++)
                printf("%d ",bcc[i][j]);
            printf("\n");
        }
    }
}
```

## 3.求边双连通分量

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<stack>
#include<vector>
#define N 20001
using namespace std;
struct Edge{
    int x;
    int yl
}edge[N];
int n,m;
int dfn[N],low[N];
int bccno[N];
vector<int> G[N],bcc[N];
int sig,block_cnt;
bool g[N][N],isbridge[N];
void tarjan(int x,int father)
{
    dfn[x]=low[x]=++block_cnt;

    for(int i=0;i<G[x].size();i++)
    {
        int y=edge[G[x][i]].y;
        if(!dfn[y])
        {
            tarjan(y,x);
            low[x]=min(low[x],low[y]);
            if(low[y]>dfn[x]){
                isbridge[G[x][i]]=1;
                isbridge[G[x][i]^1]=1;
            }
        }
        else if(dfn[y]<dfn[x] && y!=father)
            low[x]=min(low[x], dfn[y]);
    }
}
void dfs(int x)
{
    dfn[x]=1;
    bccno[x]=sig;
    for(int i=0;i<G[x].size();i++)
    {
        int y=G[x][i];
        if(isbridge[y])
            continue;
        if(!dfn[edge[y].y])
            dfs(edge[y].y);
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        for(int i=0;i<n;i++)
            G[i].clear();
        while(m--)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
        }

        sig=0;
        block_cnt=0;
        memset(dfn,0,sizeof(dfn));
        memset(low,0,sizeof(low));
        memset(isbridge,0,sizeof(isbridge));
        memset(bccno,0,sizeof(bccno));
        memset(bcc,0,sizeof(bcc));

        for(int i=1;i<=n;i++)//先DFS找桥
            if(!dfn[i])
                tarjan(i, -1);

        memset(dfn,0,sizeof(dfn));
        for(int i=1;i<=n;i++)//再DFS找边双连通分量
            if(!dfn[i]){
                sig++;
                dfs(i);
            }
        printf("%d\n",sig);//边双连通分量个数
    }
}
```






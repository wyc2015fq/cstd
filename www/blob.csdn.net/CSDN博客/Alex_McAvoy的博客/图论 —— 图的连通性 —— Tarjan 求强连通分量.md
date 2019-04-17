# 图论 —— 图的连通性 —— Tarjan 求强连通分量 - Alex_McAvoy的博客 - CSDN博客





2019年01月30日 15:00:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54








# 【概述】

Tarjan 算法是基于对图深度优先搜索的算法，每个强连通分量为搜索树中的一棵子树。

搜索时，把当前搜索树中未处理的节点加入一个堆栈，回溯时可以判断栈顶到栈中的节点是否为一个强连通分量。

# 【基本思路】

定义 DFN(u) 为节点 u 搜索的次序编号（时间戳），即是第几个被搜索到的，Low(u) 为 u 或 u 的子树能够追溯到的最早的栈中节点的次序号。

每次找到一个新点 i，有：DFN(i)=low(i)

当点 u 与点 v 相连时，如果此时（时间为 DFN[u] 时）v不在栈中，u 的 low 值为两点的 low 值中较小的一个

即：low[u]=min(low[u],low[v])

当点 u 与点 v 相连时，如果此时（时间为 DFN[u] 时）v 在栈中，u 的 low 值为 u 的 low 值和 v 的 dfn 值中较小的一个

即：low[u]=min(low[u],dfn[v]) 

当 DFN(u)=Low(u) 时，以 u 为根的搜索子树上所有节点是一个强连通分量。

# 【流程】

以下图为例，共有三个强连通分量：1234、5、6

![](https://img-blog.csdn.net/20181020220717987?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从节点 1 开始 DFS，把遍历到的节点加入栈中，搜索到节点 u=6 时，DFN[6]=LOW[6]=4，找到了一个强连通分量 {6}

![](https://img-blog.csdn.net/20181020220726137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

返回节点 5，发现 DFN[5]=LOW[5]=3，退栈后 {5} 为一个强连通分量。

![](https://img-blog.csdn.net/20181020220733891?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

返回节点 3，继续搜索到节点 4，把 4 加入堆栈。发现节点 4 像节点 1 的后向边，节点 1 还在栈中，所以 LOW[4]=1。节点 6 已经出栈，不再访问 6，返回 3，(3,4) 为树枝边，所以 LOW[3]=LOW[4]=1。

![](https://img-blog.csdn.net/20181020220741984?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

继续回到节点 1，最后访问节点 2。访问边 (2,4)，4 还在栈中，所以 LOW[2]=4。返回 1 后，发现 DFN[1]=LOW[1]，把栈中节点全部取出，组成一个连通分量 {1,3,4,2}。

![](https://img-blog.csdn.net/20181020220749719?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

至此，算法结束。经过该算法，求出了图中全部的三个强连通分量{1,3,4,2}、{5}、{6}。

# 【时间复杂度】

通过上述流程分析，运行 Tarjan 算法的过程中，每个顶点都被访问了一次，且只进出了一次堆栈，每条边也只被访问了一次，所以该算法的时间复杂度为 O(N+M)。

# 【实现】

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
stack<int> S;
int dfn[N],low[N];
bool vis[N];//标记数组
int sccno[N];//记录结点i属于哪个强连通分量
int block_cnt;//时间戳
int sig;//记录强连通分量个数
void Tarjan(int x){
    vis[x]=true;
    dfn[x]=low[x]=++block_cnt;//每找到一个新点，纪录当前节点的时间戳
    S.push(x);//当前结点入栈

    for(int i=0;i<G[x].size();i++){//遍历整个栈
        int y=G[x][i];//当前结点的下一结点
        if(vis[y]==false){//若未被访问过
            Tarjan(y);
            low[x]=min(low[x],low[y]);
        }
        else if(!sccno[y])//若已被访问过，且不属于任何一个连通分量
            low[x]=min(low[x],dfn[y]);
    }

    if(dfn[x]==low[x]){//满足强连通分量要求
        sig++;//记录强连通分量个数

        while(true){//记录元素属于第几个强连通分量
            int temp=S.top();
            S.pop();
            sccno[temp]=sig;
            if(temp==x)
                break;
        }
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
        memset(vis,0,sizeof(vis));
        memset(dfn,0,sizeof(dfn));
        memset(low,0,sizeof(low));
        memset(sccno,0,sizeof(sccno));

        for(int i=0;i<n;i++)
            if(vis[i]==false)
                Tarjan(i);

        for(int i=0;i<n;i++)
            printf("%d号点属于%d分量\n",i,sccno[i]);
    }
}
```




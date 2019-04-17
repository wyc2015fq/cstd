# TWO NODES（HDU-4587） - Alex_McAvoy的博客 - CSDN博客





2018年10月28日 16:15:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：23
个人分类：[HDU																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Suppose that G is an undirected graph, and the value of stab is defined as follows:

![](https://img-blog.csdnimg.cn/20181028160824890.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_27,color_FFFFFF,t_70)

Among the expression,G-i, -j is the remainder after removing node i, node j and all edges that are directly relevant to the previous two nodes. cntCompent is the number of connected components of X independently.

Thus, given a certain undirected graph G, you are supposed to calculating the value of stab.

# Input

The input will contain the description of several graphs. For each graph, the description consist of an integer N for the number of nodes, an integer M for the number of edges, and M pairs of integers for edges (3<=N,M<=5000).

Please note that the endpoints of edge is marked in the range of [0,N-1], and input cases ends with EOF.

# Output

For each graph in the input, you should output the value of stab.

# Sample Input

4 5

0 1

1 2

2 3

3 0

0 2

# Sample Output

2


题意：给出一个 n 个点 m 条边的无向图，求从这个无向图中删除任意两点后，所能得到的独立连通分量的最大值

思路：容易想到从割点入手，因此第一个点先枚举所有点，第二个点通过 Tarjan 求割点的同时求出最大值，即原来用 iscut 表示是否为割点，现在用其来作为遍历时成为割点的次数，那么对于非根节点，删去后剩余个数为子树个数加父节点个数，即：iscut[i]+1，对于根节点，由于没有父节点，剩余个数为子节点个数，即：iscut[i]

最后通过 max(iscut[i]+sum) 即可得出最优解

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 20001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
int n,m;
vector<int> G[N];
int dfn[N],low[N];
int iscut[N];
int block_cnt;
int Tarjan(int x,int father,int forbid){//forbid为禁止访问的点
    int lowx=dfn[x]=++block_cnt;
    int child=0;

    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(y==father || y==forbid)
            continue;

        if(dfn[y]==0){
            child++;

            int lowy=Tarjan(y,x,forbid);
            lowx=min(lowx,lowy);

            if(lowy>=dfn[x])
                iscut[x]++;
        }
        else
            lowx=min(lowx,dfn[y]);
    }

    if(father<0)
        iscut[x]--;

    return low[x]=lowx;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=0;i<n;i++)
            G[i].clear();

        for(int i=0;i<m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
        }

        int res=-INF;
        for(int x=0;x<n;x++){//枚举所有点
            int sum=0;//删除点x后的连通分量
            block_cnt=0;
            memset(dfn,0,sizeof(dfn));
            memset(iscut,0,sizeof(iscut));

            for(int y=0;y<n;y++){
                if(x!=y && dfn[y]==0){
                    sum++;
                    Tarjan(y,-1,x);
                }
            }

            for(int y=0;y<n;y++)
                res=max(res,sum+iscut[y]);
        }
        printf("%d\n",res);
    }
    return 0;
}
```







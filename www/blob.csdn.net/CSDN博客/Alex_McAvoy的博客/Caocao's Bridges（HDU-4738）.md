# Caocao's Bridges（HDU-4738） - Alex_McAvoy的博客 - CSDN博客





2018年10月28日 16:17:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[HDU																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Caocao was defeated by Zhuge Liang and Zhou Yu in the battle of Chibi. But he wouldn't give up. Caocao's army still was not good at water battles, so he came up with another idea. He built many islands in the Changjiang river, and based on those islands, Caocao's army could easily attack Zhou Yu's troop. Caocao also built bridges connecting islands. If all islands were connected by bridges, Caocao's army could be deployed very conveniently among those islands. Zhou Yu couldn't stand with that, so he wanted to destroy some Caocao's bridges so one or more islands would be seperated from other islands. But Zhou Yu had only one bomb which was left by Zhuge Liang, so he could only destroy one bridge. Zhou Yu must send someone carrying the bomb to destroy the bridge. There might be guards on bridges. The soldier number of the bombing team couldn't be less than the guard number of a bridge, or the mission would fail. Please figure out as least how many soldiers Zhou Yu have to sent to complete the island seperating mission.

# Input

There are no more than 12 test cases.

In each test case:

The first line contains two integers, N and M, meaning that there are N islands and M bridges. All the islands are numbered from 1 to N. ( 2 <= N <= 1000, 0 < M <= N2 )

Next M lines describes M bridges. Each line contains three integers U,V and W, meaning that there is a bridge connecting island U and island V, and there are W guards on that bridge. ( U ≠ V and 0 <= W <= 10,000 )

The input ends with N = 0 and M = 0.

# Output

For each test case, print the minimum soldier number Zhou Yu had to send to complete the mission. If Zhou Yu couldn't succeed any way, print -1 instead.

# Sample Input

3 3

1 2 7

2 3 4

3 1 4

3 2

1 2 7

2 3 4

0 0

# Sample Output

-1

4


题意：给出 n 个点 m 条边的无向图，m 行数据中，前两个代表相连的点，后一个代表这条边的价值，现要取掉一条边使图变成不连通的，求要花费的最小价值，如果无法使图变为不连通图，则输出 -1

思路：实质是 Tarjan 算法求割边，但要输出边价值最小的割边，用 vector 做比较麻烦，因此考虑使用邻接表来做

要注意的是，如果无向图本身就已经是不连通的了，直接输出 0 即可，由于有重边的情况，因此要在 Tarjan 算法时处理重边，由于存在权值为 0 的情况，因此如果求出来的最小权值桥的权值为 0 时，也要排一个人去炸桥，此时输出 1

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
vector<int> G[N];
int n,m;
int dfn[N],low[N];
int head[N];
int edge_cnt;
int block_cnt;
int res;
struct Edge{
    int to;//下一个点
    int next;//下一跳边
    int id;//增边的顺序编号
    int cost;//边的权值
}edge[N];
void add(int x,int y,int cost,int id){
    edge[edge_cnt].to=y;
    edge[edge_cnt].cost=cost;
    edge[edge_cnt].id=id;
    edge[edge_cnt].next=head[x];
    head[x]=edge_cnt++;
}
void Tarjan(int x,int father){
    low[x]=dfn[x]=++block_cnt;

    for(int i=head[x];~i;i=edge[i].next){
        int y=edge[i].to;
        int id=edge[i].id;

        if(id==father)
            continue;

        if(dfn[y]==0){
            Tarjan(y,id);
            low[x]=min(low[x],low[y]);

            if(dfn[x]<low[y])
                res=min(res,edge[i].cost);
        }
        else
            low[x]=min(low[x],dfn[y]);
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        edge_cnt=0;
        memset(head,-1,sizeof(head));
        for(int i=1;i<=m;i++){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            add(x,y,w,i);
            add(y,x,w,i);

        }

        res=INF;
        block_cnt=0;
        memset(dfn,0,sizeof(dfn));

        int connection=0;
        for(int i=1;i<=n;i++){
            if(dfn[i]==0){
                connection++;
                Tarjan(i,0);
            }
        }

        if(res==0)
            res=1;
        if(res==INF)
            res=-1;
        if(connection>1)
            res=0;

        printf("%d\n",res);
    }
    return 0;
}
```







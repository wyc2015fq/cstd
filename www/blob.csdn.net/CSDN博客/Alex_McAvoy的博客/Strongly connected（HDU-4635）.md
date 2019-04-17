# Strongly connected（HDU-4635） - Alex_McAvoy的博客 - CSDN博客





2018年10月24日 21:18:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44
个人分类：[HDU																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Give a simple directed graph with N nodes and M edges. Please tell me the maximum number of the edges you can add that the graph is still a simple directed graph. Also, after you add these edges, this graph must NOT be strongly connected.

A simple directed graph is a directed graph having no multiple edges or graph loops.

A strongly connected digraph is a directed graph in which it is possible to reach any node starting from any other node by traversing edges in the direction(s) in which they point. 

# Input

The first line of date is an integer T, which is the number of the text cases.

Then T cases follow, each case starts of two numbers N and M, 1<=N<=100000, 1<=M<=100000, representing the number of nodes and the number of edges, then M lines follow. Each line contains two integers x and y, means that there is a edge from x to y.

# Output

For each case, you should output the maximum number of the edges you can add.

If the original graph is strongly connected, just output -1.

# Sample Input

3

3 3

1 2

2 3

3 1

3 3

1 2

2 3

1 3

6 6

1 2

2 3

3 1

4 5

5 6

6 4

# Sample Output

Case 1: -1

Case 2: 1

Case 3: 15


题意： 给一个 n 个点，m 条边的有向图，求最多添加多少条边能使得该图依然不是强连通，若该图初始已经强连通，则输出-1

思路：缩点

假设一非连通图由两个强连通子图构成，结点分别为 x、y 个，即：x+y=n，且图中只有从 x 的任一节点到 y 中任一节点的边，不存在从 y 到 x 的边，则非连通图的边数 F=x*(x-1)+y*(y-1)+x*y=n*(n-1)-x*y，若想让边数 F 最大，则必须让 x*y 最小，由于 x+y=n，因此当 x、y 的差值最大时，x*y 必然最小，从而 F 最大

依照题意，F-m 即为所求结果，根据原图求所有强连通分量，缩点得新的有向无环图，因此只有 入度=0 或 出度=0 所代表的点（分量），才有资格成为 x 或 y 部分，因此根据上述结论，找出最小值即可，注意数据范围要用 long long

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
stack<int> S;
int dfn[N],low[N];
bool vis[N];
int sccno[N];
int in[N],out[N];
int num[N];//分量的点数
int block_cnt;
int sig;
void Tarjan(int x){
    vis[x]=true;
    dfn[x]=low[x]=++block_cnt;
    S.push(x);

    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(vis[y]==false){
            Tarjan(y);
            low[x]=min(low[x],low[y]);
        }
        else if(!sccno[y])
            low[x]=min(low[x],dfn[y]);
    }

    if(dfn[x]==low[x]){
        sig++;
        num[sig]=0;
        while(true){
            int temp=S.top();
            S.pop();
            sccno[temp]=sig;
            num[sig]++;
            if(temp==x)
                break;
        }
    }
}
int shrink(){//缩点
    for(int i=1;i<=sig;i++){
        in[i]=0;
        out[i]=0;
    }

    for(int x=1;x<=n;x++){
        for(int i=0;i<G[x].size();i++){
            int y=G[x][i];
            if(sccno[x]!=sccno[y]){
                out[sccno[x]]++;
                in[sccno[y]]++;
            }
        }
    }
}
int main()
{
    int t;
    scanf("%d",&t);
    for(int Case=1;Case<=t;Case++){
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
            G[i].clear();
        for(int i=0;i<m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
        }

        sig=0;
        block_cnt=0;
        memset(vis,false,sizeof(vis));
        memset(dfn,0,sizeof(dfn));
        memset(low,0,sizeof(low));
        memset(sccno,0,sizeof(sccno));

        for(int i=1;i<=n;i++)
            if(vis[i]==false)
                Tarjan(i);

        shrink();
        LL res=0;
        int minn=INF;
        for(int i=1;i<=sig;i++)
            if(out[i]==0||in[i]==0)
                minn=min(minn,num[i]);
        res=(long long)n*n-n-(long long)minn*(n-minn)-m;

        printf("Case %d: ",Case);
        if(sig==1)
            printf("-1\n");
        else
            printf("%d\n",res);

    }
    return 0;
}
```







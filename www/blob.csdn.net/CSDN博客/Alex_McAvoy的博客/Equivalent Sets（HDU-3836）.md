# Equivalent Sets（HDU-3836） - Alex_McAvoy的博客 - CSDN博客





2018年10月24日 21:04:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30
个人分类：[HDU																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

To prove two sets A and B are equivalent, we can first prove A is a subset of B, and then prove B is a subset of A, so finally we got that these two sets are equivalent.

You are to prove N sets are equivalent, using the method above: in each step you can prove a set X is a subset of another set Y, and there are also some sets that are already proven to be subsets of some other sets.

Now you want to know the minimum steps needed to get the problem proved.

# Input

The input file contains multiple test cases, in each case, the first line contains two integers N <= 20000 and M <= 50000.

Next M lines, each line contains two integers X, Y, means set X in a subset of set Y.

# Output

For each case, output a single integer: the minimum steps needed.

# Sample Input

4 0

3 2

1 2

1 3

# Sample Output

4

2


题意：给你一个有向图，问在图中最少要加多少条边能使得该图变成一个强连通图

思路：缩点模板题

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
bool in[N],out[N];//记录入度、出度是否为0
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
        while(true){
            int temp=S.top();
            S.pop();
            sccno[temp]=sig;
            if(temp==x)
                break;
        }
    }
}
int shrink(){//缩点

    memset(in,false,sizeof(in));
    memset(out,false,sizeof(out));
    for(int i=1;i<=sig;i++){
        in[i]=true;
        out[i]=true;
    }

    for(int x=0;x<n;x++){
        for(int i=0;i<G[x].size();i++){
            int y=G[x][i];
            if(sccno[x]!=sccno[y]){//统计每个点出度、入度是否为0
                out[sccno[x]]=false;
                in[sccno[y]]=false;
            }
        }
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m))
    {
        for(int i=0;i<n;i++)
            G[i].clear();
        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            x--;
            y--;
            G[x].push_back(y);
        }

        sig=0;
        block_cnt=0;
        memset(vis,false,sizeof(vis));
        memset(dfn,0,sizeof(dfn));
        memset(low,0,sizeof(low));
        memset(sccno,0,sizeof(sccno));

        for(int i=0;i<n;i++)
            if(vis[i]==false)
                Tarjan(i);

        shrink();
        int a=0,b=0;
        for(int i=1;i<=sig;i++){//统计入度、出度为0的点的个数
            if(in[i])
                a++;
            if(out[i])
                b++;
        }
        int res=max(a,b);
        if(sig==1)//强连通分量为1时
            res=0;

        printf("%d\n",res);
    }
}
```





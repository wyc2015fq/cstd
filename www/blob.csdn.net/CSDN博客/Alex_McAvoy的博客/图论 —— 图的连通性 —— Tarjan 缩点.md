# 图论 —— 图的连通性 —— Tarjan 缩点 - Alex_McAvoy的博客 - CSDN博客





2019年01月30日 15:01:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：90








缩点常应用于给一个有向图，求在图中最少要加多少条边能使得该图变成一个强连通图

首先求出该图的各个强连通分量，然后把每个强连通分量看出一个点(即缩点)，最后得到了一个有向无环图(DAG)

对于一个DAG，需要添加 max(a,b) 条边才能使其强连通

其中 a 为 DAG 中出度为 0 的点总数，b 为 DAG 中入度为 0 的点总数

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
void shrink(){//缩点
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
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
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




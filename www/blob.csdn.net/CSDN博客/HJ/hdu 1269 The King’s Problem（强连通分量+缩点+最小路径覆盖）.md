# hdu 1269  The King’s Problem（强连通分量+缩点+最小路径覆盖） - HJ - CSDN博客
2017年10月19日 21:30:21[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：285
Problem Description 
In the Kingdom of Silence, the king has a new problem. There are N cities in the kingdom and there are M directional roads between the cities. That means that if there is a road from u to v, you can only go from city u to city v, but can’t go from city v to city u. In order to rule his kingdom more effectively, the king want to divide his kingdom into several states, and each city must belong to exactly one state. What’s more, for each pair of city (u, v), if there is one way to go from u to v and go from v to u, (u, v) have to belong to a same state. And the king must insure that in each state we can ether go from u to v or go from v to u between every pair of cities (u, v) without passing any city which belongs to other state. 
Now the king asks for your help, he wants to know the least number of states he have to divide the kingdom into.
Input 
The first line contains a single integer T, the number of test cases. And then followed T cases.
The first line for each case contains two integers n, m(0 < n <= 5000,0 <= m <= 100000), the number of cities and roads in the kingdom. The next m lines each contains two integers u and v (1 <= u, v <= n), indicating that there is a road going from city u to city v.
Output 
The output should contain T lines. For each test case you should just output an integer which is the least number of states the king have to divide into.
Sample Input
1 
3 2 
1 2 
1 3
Sample Output 
2
题目大意： 
有n个城市和m条有向通道。如果从某个城市一条道路一直走到没有路可走，那么在这条首路上的所有城市划分成一个国家，每个城市只能属于一个国家，一个环形上的城市当然也是属于同一国家。问这些城市最少分为多少个国家。
解题：因为有向图中存在环，而二分图是在有向无环图时才正确，所以先用强连通缩点重新构成有向无环图。再二分匹配。（最小路径覆盖）
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<stack>
#include<vector>
#define INF 0x3f3f3f3f
using namespace std;
const int N=10005;
vector<int> g[N];
stack<int> q;
int n,m,dfs_clock,scc_cnt;
int dfn[N],low[N],sccno[N],link[N];
bool vis[N],map[N][N];
void Init()         //初始化 
{
    memset(dfn,0,sizeof(dfn));
    memset(low,0,sizeof(low));
    memset(sccno,0,sizeof(sccno));
    memset(map,false,sizeof(map)); 
    dfs_clock=scc_cnt=0;
    for(int i=0;i<=n;i++)
      g[i].clear();
    while(!q.empty())
      q.pop();
}
void tarjan(int u)        //计算强连通分量个数 
{
    dfn[u]=low[u]=++dfs_clock;
    q.push(u);
    for(int i=0;i<g[u].size();i++)
    {
        int v=g[u][i];
        if(!dfn[v])
        {
            tarjan(v);
            low[u]=min(low[u],low[v]);
        }
        else if(!sccno[v])
          low[u]=min(low[u],dfn[v]);
    } 
    if(low[u]==dfn[u])
    {
        scc_cnt++;
        while(!q.empty())
        {
            int v=q.top();
            q.pop();
            sccno[v]=scc_cnt;
            if(v==u)
              break;
        } 
    }
} 
bool dfs(int u)     //dfs搜索增广路 
{
    for(int i=1;i<=n;i++)
    {
        if(!vis[i]&&map[u][i])
        {
            vis[i]=true;
            if(!link[i]||dfs(link[i]))
            {
                link[i]=u;
                return true;
            }
        }
    }
    return false;
}
int hungary()         //匈牙利二分匹配 
{
    int ans=0;
    memset(link,0,sizeof(link));
    for(int i=1;i<=n;i++)
    {
        memset(vis,false,sizeof(vis));
        if(dfs(i))
          ans++;
    }
    return scc_cnt-ans;
}
int main()
{
    int t,u,v;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        Init();
        while(m--)
        {
            scanf("%d%d",&u,&v);
            g[u].push_back(v);
        }
        for(int i=1;i<=n;i++)
          if(!dfn[i])
            tarjan(i);
        for(int u=1;u<=n;u++)          //强连通分量缩点 重新构造出一个DAG 
          for(int i=0;i<g[u].size();i++)
          {
              int v=g[u][i];
              if(sccno[u]!=sccno[v])
                map[sccno[u]][sccno[v]]=1;
          } 
        printf("%d\n",hungary());
    }
    return 0;
}
```

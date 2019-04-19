# hdu 1827 Summer Holiday（强连通分量+缩点） - HJ - CSDN博客
2017年10月19日 21:43:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：170
Problem Description 
To see a World in a Grain of Sand 
And a Heaven in a Wild Flower, 
Hold Infinity in the palm of your hand 
And Eternity in an hour. 
                  —— William Blake
听说lcy帮大家预定了新马泰7日游，Wiskey真是高兴的夜不能寐啊，他想着得快点把这消息告诉大家，虽然他手上有所有人的联系方式，但是一个一个联系过去实在太耗时间和电话费了。他知道其他人也有一些别人的联系方式，这样他可以通知其他人，再让其他人帮忙通知一下别人。你能帮Wiskey计算出至少要通知多少人，至少得花多少电话费就能让所有人都被通知到吗？
Input 
多组测试数组，以EOF结束。 
第一行两个整数N和M（1<=N<=1000, 1<=M<=2000），表示人数和联系对数。 
接下一行有N个整数，表示Wiskey联系第i个人的电话费用。 
接着有M行，每行有两个整数X，Y，表示X能联系到Y，但是不表示Y也能联系X。
Output 
输出最小联系人数和最小花费。 
每个CASE输出答案一行。
Sample Input
12 16 
2 2 2 2 2 2 2 2 2 2 2 2  
1 3 
3 2 
2 1 
3 4 
2 4 
3 5 
5 4 
4 6 
6 4 
7 4 
7 12 
7 8 
8 7 
8 9 
10 9 
11 10
Sample Output
3 6
解题思路： 
强连通缩点后求入度为零的点，在从入度为零的点（强连通分量）中选择花费最小的点，花费求和。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
#include<vector>
#include<stack>
#define INF 0x3f3f3f3f
using namespace std;
const int N=1005;
int n,m,scc_cnt,dfs_clock;
int dfn[N],low[N],sccno[N],in[N],w[N],scc_w[N];
vector<int> g[N];
stack<int> q;
void Init()
{
    memset(dfn,0,sizeof(dfn));
    memset(low,0,sizeof(low));
    memset(sccno,0,sizeof(sccno));
    memset(in,0,sizeof(in));
    scc_cnt=dfs_clock=0;
    for(int i=0;i<=n;i++)
      g[i].clear();
    while(!q.empty())
      q.pop();
}
void tarjan(int u)
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
        scc_w[++scc_cnt]=INF;
        while(!q.empty())
        {
            int x=q.top();
            q.pop();
            sccno[x]=scc_cnt;
            scc_w[scc_cnt]=min(scc_w[scc_cnt],w[x]); //更新最小花费 
            if(x==u)
              break;
        }
    }
}
int main()
{
    while(~scanf("%d%d",&n,&m))
    {
        int u,v,ans=0,sum=0;
        Init();
        for(int i=1;i<=n;i++)
          scanf("%d",&w[i]);
        while(m--)
        {
            scanf("%d%d",&u,&v);
            g[u].push_back(v);
        }
        for(int i=1;i<=n;i++)
          if(!dfn[i])
            tarjan(i);
        for(int u=1;u<=n;u++)      //缩点 
          for(int i=0;i<g[u].size();i++)
          {
              int v=g[u][i];
              if(sccno[u]!=sccno[v])
                in[sccno[v]]++;
          }
        for(int i=1;i<=scc_cnt;i++)
          if(!in[i])
          {
              ans++;
              sum+=scc_w[i];    //累加最小花费 
          }
        printf("%d %d\n",ans,sum);
    }
    return 0;
}
```

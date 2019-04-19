# POJ 1679 The Unique MST（判断MST的唯一性） - HJ - CSDN博客
2017年10月30日 15:35:53[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：193
个人分类：[====ACM====																[----------图论---------																[生成树																[---------OJ---------																[POJ](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6695873)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://poj.org/problem?id=1679](http://poj.org/problem?id=1679)
题目大意： 
就是判断这个图中是否存在多个MST
解题思路： 
实际上就是求次小生成树是否等于最小生成树。如何求最小生成树？找出MST中权值最大的边，用MST以外的任何一条边进行替换，重新求一次最小生成树，判断前后值是否相等。如果相等，代表MST不唯一，否则MST唯一。
Prim 版：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#define INF 0x3f3f3f3f
using namespace std;
const int N=105;
int n,d[N],g[N][N],pre[N];      //pre[i]记录MST中的顶点的相关联边的直接前驱 
int Max[N][N];   //Max[i][j]表示在最小生成树中从i到j的路径中的最大边权 
bool used[N][N],vis[N];        //used[i]记录加入到MST中的顶点  
int Prim()
{
    int ans=0;
    memset(Max,0,sizeof(Max));
    memset(used,false,sizeof(used));
    memset(vis,false,sizeof(vis));
    memset(pre,0,sizeof(pre));  
    for(int i=1;i<=n;i++)
      d[i]=g[0][i];
    vis[0]=true;
    pre[0]=-1;
    d[0]=0;
    for(int i=1;i<n;i++)
    {
        int mind=INF,mark=-1;
        for(int j=0;j<n;j++)
          if(!vis[j]&&mind>d[j])
            mind=d[mark=j];
        if(mind==INF)
          return -1;
        ans+=mind;
        vis[mark]=true;
        used[mark][pre[mark]]=used[pre[mark]][mark]=true;
        for(int j=0;j<n;j++)
        {
            if(vis[j])
              Max[j][mark]=Max[mark][j]=max(d[mark],Max[j][pre[mark]]);   //新加入点到MST各点路径最大值 
            if(!vis[j]&&d[j]>g[mark][j])
              d[j]=g[pre[j]=mark][j];    //记录直接前驱 
        } 
    }
    return ans;
}
int ans;
int smst()
{
    int mind=INF;
    for(int i=0;i<n;i++)
      for(int j=i+1;j<n;j++)
        if(g[i][j]!=INF&&!used[i][j])      //枚举所有不在生成树中的边 
          mind=min(mind,ans+g[i][j]-Max[i][j]);      //替换生成树里最大边权的边 更新答案 
    if(mind==INF)
      return -1;
    return mind;
} 
int main()
{
    int t,m,u,v,w;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        for(int i=0;i<=n;i++)
          for(int j=0;j<=n;j++)
            g[i][j]=(i==j)?0:INF;
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&w);
            u--,v--;
            g[u][v]=g[v][u]=w;
        }
        ans=Prim();
        if(ans==-1)
        {
            printf("Not Unique!\n");
            continue;
        }
        if(ans==smst())
          printf("Not Unique!\n");
        else
          printf("%d\n",ans);
    }
    return 0;
}
```
Kruskal 版：
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>
#define INF 0x3f3f3f3f
using namespace std;
const int N=10005;
int n,m,flag,fa[N];
struct Edge{
    int u,v,w,eq,used,del;   //eq判断是否有重边  uesd判断是否使用过  vis表示是否拆除 
}edges[15005]; 
int find(int x)
{
    return fa[x]==x?x:find(fa[x]);
}
int cmp(Edge a,Edge b)
{
    return a.w<b.w;
}
void Union(int x,int y)
{
    x=find(x);
    y=find(y);
    if(x!=y)
      fa[x]=y;
}
int Kruskal()
{
    for(int i=0;i<m;i++)
      fa[i]=i;
    int ans=0,num=0;
    for(int i=0;i<m;i++)
    {
        if(edges[i].del)       //判断该边是否拆除 
          continue;
        int u=edges[i].u,v=edges[i].v,w=edges[i].w;
        if(find(u)!=find(v))
        {
            ans+=w;
            if(!flag)
              edges[i].used=1;       //记录下使用过的边 
            num++;
            Union(u,v);
        } 
        if(num>=n-1)
          break;
    }
    return ans;
} 
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        for(int i=0;i<m;i++)
        {
            cin>>edges[i].u>>edges[i].v>>edges[i].w;
            edges[i].del=edges[i].used=edges[i].eq=0;
        }
        for(int i=0;i<m;i++)
          for(int j=0;j<m;j++)
          {
              if(i==j)
                continue;
              if(edges[i].w==edges[j].w)       //判断每条边是否有重边 
                edges[i].eq=1;            
          }
        sort(edges,edges+m,cmp);
        flag=false;
        int ans=Kruskal();     //求一次最小生成树 
        flag=true;
        bool blag=false;
        for(int i=0;i<m;i++)
        {
            if(edges[i].used==1&&edges[i].eq==1)    //依次去掉第一次使用过的且含有重边的边  
            {
                edges[i].del=1;
                if(ans==Kruskal())      //再求一次最小生成树 若结果与第一次结果一样 则最小生成树不唯一 
                {
                    blag=true;
                    break;
                }           
                edges[i].del=0;
            }
        } 
        if(blag)
          printf("Not Unique!\n");
        else
          printf("%d\n",ans);
    } 
    return 0;
}
```

# LCA问题的三种求法 - HJ - CSDN博客
2018年08月16日 18:40:24[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：148
个人分类：[====ACM====																[--------数据结构--------																[RMQ+LCA																[---------总结---------																[算法笔记](https://blog.csdn.net/feizaoSYUACM/article/category/6638369)](https://blog.csdn.net/feizaoSYUACM/article/category/6625811)](https://blog.csdn.net/feizaoSYUACM/article/category/7611705)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
先来看一道例题：
hdu 2586 How far away ？
[http://acm.hdu.edu.cn/showproblem.php?pid=2586](http://acm.hdu.edu.cn/showproblem.php?pid=2586)
题目描述的就是给你一棵n个节点的树，然后Q次询问，每次询问的内容是节点x和节点y的最近公共祖先（LCA）
接下来就来说一说LCA（最近公共祖先）
你需要准备的预备知识： 
①ST表处理RMQ问题； 
②并查集的思想以及实现； 
③DFS遍历整棵树，维护一些值；
求LCA的三种方法：
1、LCA转RMQ；
做法：
①首先按DFS序遍历整棵树，得到每个节点第一次出现的序号，每个节点的深度，以及每个节点所维护的值；
②对于每个询问(a,b)，判断第一次出现a与第一次出现b的大小，化成一个区间查询最小值；
③ST打表，O(1)回答每次区间最小值的查询即可；
模板如下：
```cpp
#include<iostream>
#include<cstring>
#include<vector>
#include<cstdio>
#include<cmath>
using namespace std;
const int maxn=1e5+10;
int dis[maxn],dep[maxn],first[maxn],pos[maxn];
int dp[maxn][25];
int cnt;
struct edge{
    int to,w;
};
vector<edge> G[maxn];
int min(int i,int j)  //因为需要得到最小值的序号，因此更新的时候返回序号即可 
{
    return dep[i]<dep[j]?i:j;
}
void Init_RMQ(int n)   //递推求ST表 
{
    for(int i=0;i<=n;i++)
      dp[i][0]=i;
    for(int j=1;(1<<j)<=n;j++)
      for(int i=1;i+(1<<j)-1<=n;i++)
        dp[i][j]=min(dp[i][j-1],dp[i+(1<<(j-1))][j-1]);
} 
int query(int l,int r)  //用ST表求RMQ 
{
    int k=log(r-l+1);
    return min(dp[l][k],dp[r-(1<<k)+1][k]);
}
int get_lca(int a,int b)  //比较两个节点谁先出现，确定RMQ的范围 
{
    return first[a]>first[b]?pos[query(first[b],first[a])]:pos[query(first[a],first[b])];
}
void dfs(int u,int fa,int deep)  //dfs遍历每个节点 
{
    dep[cnt]=deep,pos[cnt]=u,first[u]=cnt++;  //保存每个节点的深度，这个节点的值，第一次出现的序号 
    for(int i=0;i<G[u].size();i++)
    {
        int v=G[u][i].to;
        if(v==fa)
          continue;
        dis[v]=dis[u]+G[u][i].w;  //更新子节点距离 
        dfs(v,u,deep+1);  //继续遍历子节点 
        pos[cnt]=u; 
        dep[cnt++]=deep;
    }
}
int main()
{
    int t,a,b,c,n,m;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        cnt=0;
        for(int i=0;i<maxn;i++)
          G[i].clear();
        for(int i=0;i<n-1;i++)
        {
            scanf("%d%d%d",&a,&b,&c);
            G[a].push_back(edge{b,c});
            G[b].push_back(edge{a,c});
        }
        dfs(1,-1,1);
        Init_RMQ(2*n-1);
        while(m--)
        {
            scanf("%d%d",&a,&b);
            printf("%d\n",dis[a]+dis[b]-2*dis[get_lca(a,b)]);
        }
    }
    return 0; 
}
```
2、Tarjan算法求LCA；
做法：
①对于当前节点x，遍历所有子节点，当遍历结束之后，将其整棵子树合并到x，并保证合并之后的祖先为x； 
②处理有关于x的询问，对于询问(x,y),如果y节点已经遍历，则x与y的LCA就是y节点所在的集合的祖先。否则，将其推迟到遍历y的时候再处理。
其中合并和查询集合祖先用到的是并查集
模板如下：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring> 
#include<vector>
using namespace std;
const int maxn=1e5+10;
int n,m,t,x,y,z,vis[maxn],dis[maxn],fa[maxn],ans[maxn];
struct node{
    int v,w;
};
vector<node> a[maxn];    //保存节点信息 
vector<node> b[maxn];   //保存查询信息 
void Init()    //初始化 
{
    for(int i=1;i<=n;i++)
    {
        fa[i]=i;
        vis[i]=ans[i]=dis[i]=0;
        a[i].clear(),b[i].clear();
    }
}
int find(int x)   //查找祖先节点 
{
    return fa[x]==x?x:fa[x]=find(fa[x]);
} 
void Union(int x,int y)   //合并 
{
    fa[find(y)]=find(x);
}
void tarjan(int u)
{
    vis[u]=1;   //标记节点u已经被访问 
    for(int i=0;i<a[u].size();i++)   //遍历节点u 
    {
        int v=a[u][i].v,w=a[u][i].w;
        if(!vis[v])
        {
            dis[v]=dis[u]+w;   //更新距离 
            tarjan(v);    //继续遍历节点u的子节点v 
            Union(u,v);     //将节点v合并到节点u中 
        }
    } 
    for(int i=0;i<b[u].size();i++)   //搜索关于节点u的查询 
    {
        int v=b[u][i].v;
        if(vis[v])    //如果关于节点u查询的对立节点v已经被遍历过  
          ans[b[u][i].w]=dis[u]+dis[v]-2*dis[find(v)];   //则可以回答查询 
    }
}
int main()
{
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        Init();
        for(int i=1;i<n;i++)
        {
            scanf("%d%d%d",&x,&y,&z);
            a[x].push_back(node{y,z});
            a[y].push_back(node{x,z});
        }
        for(int i=1;i<=m;i++)
        {
            scanf("%d%d",&x,&y);
            b[x].push_back(node{y,i});
            b[y].push_back(node{x,i});
        }
        tarjan(1);
        for(int i=1;i<=m;i++)
          printf("%d\n",ans[i]);
    }
    return 0;
}
```
3、倍增求LCA；
做法：
①预处理遍历整棵树，保存每个节点的深度，并且递推求出它的二进制距离的祖先节点的维护值，这些值保存在f[i][j]中，其中f[i][j]表示的是对于节点i，与它相距2^j次方距离的祖先节点的值。
②对于每个询问（a,b），找到深度大的那个点（假设是a点），逆序遍历j，其中j表示能够往上跳的距离的log值，直到a和b两个点的深度相等；
③接下来a和b两个点一起往上跳，每次跳的高度和距离都必须相等，跳的方法同②，直到两个点跳到同一个点为止，那个点即为最近公共祖先；
模板如下：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring> 
using namespace std;
const int maxn=1e5+10;
int tot,head[maxn],dis[maxn],fa[maxn][25],dep[maxn];
struct node{
    int u,v,w,next;
}edge[maxn];
void add(int u,int v,int w)  //无向边，正反建两次 
{
    edge[++tot].next=head[u],edge[tot].v=v,edge[tot].w=w;
    head[u]=tot;
    edge[++tot].next=head[v],edge[tot].v=u,edge[tot].w=w;
    head[v]=tot;
}
void Init(int n)  //递推预处理fa数组 
{
    for(int i=1;i<=n;i++)
      for(int j=0;j<20;j++)
        fa[i][j+1]=fa[i][j]<0?-1:fa[fa[i][j]][j];
}
void dfs(int u,int pre,int deep,int d)   //dfs遍历整棵树的所有节点 
{
    dis[u]=d,fa[u][0]=pre,dep[u]=deep;
    for(int i=head[u];i;i=edge[i].next)
      if(edge[i].v!=pre)
        dfs(edge[i].v,u,deep+1,d+edge[i].w);
}
int query(int u,int v)   //倍增查询 
{
    if(dep[u]>dep[v]) swap(u,v);
    for(int i=0;i<=20;i++)
      if((dep[v]-dep[u])>>i&1)
        v=fa[v][i];
    if(u==v) return u;
    for(int i=20;i>=0;i--)
      if(fa[u][i]!=fa[v][i])
        u=fa[u][i],v=fa[v][i];
    return fa[u][0];
}
int main()
{
    int t,m,x,y,z,i,n;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        tot=0;
        memset(head,0,sizeof(head));
        for(int i=1;i<n;i++)
          scanf("%d%d%d",&x,&y,&z),add(x,y,z);
        dfs(1,-1,1,0);
        Init(n);
        while(m--)
        {
            scanf("%d%d",&x,&y);
            printf("%d\n",dis[x]+dis[y]-2*dis[query(x,y)]);
        }
    }
    return 0;
}
```

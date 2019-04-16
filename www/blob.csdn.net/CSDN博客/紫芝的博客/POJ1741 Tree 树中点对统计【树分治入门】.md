# POJ1741 Tree 树中点对统计【树分治入门】 - 紫芝的博客 - CSDN博客





2018年11月21日 20:09:18[紫芝](https://me.csdn.net/qq_40507857)阅读数：1117








**算法合集之《分治算法在树的路径问题中的应用》**
[论文下载地址](https://pan.baidu.com/s/11Isd5NMOK268hVipLCjRlA)

树被定义为没有圈的连通图，有几个性质- 在树中去掉一条边后，得到的图是不连通的
- 在树中添加一条边后，一定存在一条边
- 树的每一对顶点U和V之间有且只有一条路径

## 分治

分而治之，**将一个问题分割成一些规模较小的相互独立的子问题** ,通常在一个线性结构上分治，分至算法在树结构上的应用称为树分治算法
- **基于点的分治**

首先，选取一个点将无根树转化为有根树，再递归处理以根节点的儿子为根的子树

我们选取一个点，要求将其删除后，节点最多的树的节点个数最小，这个点被称为 `树的重心` .
**可以用树上的动态规划解决，时间复杂度为O(n),n为树的节点总数**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121193200832.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)
- **基于边的分治**

在树中选取一条边，将原树分为两棵不相交的树，递归处理

选取的边要满足所分离出来的两棵子树的节点个数尽量平均，这条边称为 `中心边`。
**可以用树上的动态规划解决，时间复杂度为O(n),n为树的节点总数**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121193414853.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)
**定理1**

存在一个点使得分出的子树的节点个数均不大于 N/2
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121195708656.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)
**定理2**

如果一棵树中，每个点的度均不大于D，那么存在一条边使得分出两棵子树的节点个数在 `[N/(D+1),N*(D+1)],(N>=2)`

## POJ1741

**题意**
 给定一个N个节点的带权树，定义`dist(u,v)`为u，v两点之间的最短路径长度，路径长度定义为路径上所有边的权和。对于两个不同的点a，b，如果满足`dist(a,b)<=k`,称为合法点对，求合法点对的个数

```cpp
#include<iostream>
#include<map>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#include<set>
#include<sstream>
#include<cstdio>
#include<cmath>
#include<climits>
using namespace std;
const int maxn=1e4+7;
const int inf=0x3f3f3f3f;
typedef long long ll;
const int mod=1e9+7;
int n,k,allnode;
int head[maxn*2];
int num;
int dp[maxn];
int size[maxn];
int Focus,M;
ll dist[maxn];
int deep[maxn];
bool vis[maxn];
ll ans;
struct Edge
{
    int u,v,w,next;
}edge[maxn<<2];
void addEdge(int u,int v,int w)
{
    edge[num].u=u;
    edge[num].v=v;
    edge[num].w=w;
    edge[num].next=head[u];
    head[u]=num++;
}
void init()
{
    memset(head,-1,sizeof(head));
    memset(dist,0,sizeof(dist));
    memset(vis,0,sizeof(vis));
    num=0;
}
void getFocus(int u,int pre)
{
    size[u]=1;
    dp[u]=0;
    for(int i=head[u];i!=-1;i=edge[i].next)
    {
        int v=edge[i].v;
        if(v==pre||vis[v]) continue;
        getFocus(v,u);
        size[u]+=size[v];
        dp[u]=max(dp[u],size[v]);
    }
    dp[u]=max(dp[u],allnode-size[u]);
    if(M>dp[u])
    {
        M=dp[u];
        Focus=u;
    }
}
void dfs(int u,int pre)
{
    deep[++deep[0]]=dist[u];
    for(int i=head[u];i!=-1;i=edge[i].next)
    {
        int v=edge[i].v,w=edge[i].w;
        if(v==pre||vis[v]) continue;
        dist[v]=dist[u]+w;
        dfs(v,u);
    }
}
int cal(int x,int now)
{
    dist[x]=now,deep[0]=0;
    dfs(x,0);
    sort(deep+1,deep+deep[0]+1);
    int ans=0;
    for(int l=1,r=deep[0];l<r;)
    {
        if(deep[l]+deep[r]<=k)
        {
            ans+=r-l;
            l++;
        }
        else r--;
    }
    return ans;
}
void solve(int x)
{
    vis[x]=1;
    ans+=cal(x,0);
    for(int i=head[x];i!=-1;i=edge[i].next)
    {
        int v=edge[i].v;
        if(vis[v]) continue;
        ans-=cal(v,edge[i].w);
        allnode=size[v];
        Focus=0,M=1e9;
        getFocus(v,x);
        solve(Focus);
    }
}
int main()
{
    #ifndef ONLINE_JUDGE
        freopen("in.txt","r",stdin);
        freopen("out.txt","w",stdout);
    #endif
    while(scanf("%d%d",&n,&k)!=EOF&&(n+k))
    {
        init();
        for(int i=1,u,v,w;i<n;i++)
        {
            scanf("%d%d%d",&u,&v,&w);
            addEdge(u,v,w);
            addEdge(v,u,w);
        }
        Focus=ans=0;
        allnode=n,M=1e9;
        getFocus(1,0);
        solve(Focus);
        printf("%lld\n",ans);
    }
    return 0;
}
```

## 聪聪可可 [HYSBZ - 2152](https://cn.vjudge.net/problem/HYSBZ-2152)

[链接](https://www.lydsy.com/JudgeOnline/problem.php?id=2152)

**题意**

这道题和POJ1741非常类似，这里只不过多了一步转化，要处理3的倍数，在建图和统计的时候都进行模3操作，这样最后两个点之间的距离只可能是0，1，2，然后就可以用他们组合为3的倍数，首先0和0组合，1和2组合，2和1组合【题中（1，2）和（2，1）不同】 分子可以由树分治求出来，分母必然是N*N

```cpp
#include<iostream>
#include<map>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#include<set>
#include<sstream>
#include<cstdio>
#include<cmath>
#include<climits>
using namespace std;
const int maxn=2e4+7;
const int inf=0x3f3f3f3f;
typedef long long ll;
const int mod=1e9+7;
int n,k,allnode;
int head[maxn*2];
int num;
int dp[maxn];
int size[maxn];
int Focus,M;
ll dist[maxn];
int deep[maxn];
bool vis[maxn];
int ans;
int cnt[4];
struct Edge
{
    int u,v,w,next;
}edge[maxn<<2];
void addEdge(int u,int v,int w)
{
    edge[num].u=u;
    edge[num].v=v;
    edge[num].w=w;
    edge[num].next=head[u];
    head[u]=num++;
}
void init()
{
    memset(head,-1,sizeof(head));
    memset(dist,0,sizeof(dist));
    memset(vis,0,sizeof(vis));
    num=0;
}
void getFocus(int u,int pre)
{
    size[u]=1;
    dp[u]=0;
    for(int i=head[u];i!=-1;i=edge[i].next)
    {
        int v=edge[i].v;
        if(v==pre||vis[v]) continue;
        getFocus(v,u);
        size[u]+=size[v];
        dp[u]=max(dp[u],size[v]);
    }
    dp[u]=max(dp[u],allnode-size[u]);
    if(M>dp[u])
    {
        M=dp[u];
        Focus=u;
    }
}
void dfs(int u,int pre)
{
    cnt[deep[u]]++;
    for(int i=head[u];i!=-1;i=edge[i].next)
    {
        int v=edge[i].v;
        if(v==pre||vis[v]) continue;
        deep[v]=(deep[u]+edge[i].w)%3;
        dfs(v,u);
    }
}
int cal(int x,int now)
{
    deep[x]=now;
    cnt[0]=cnt[1]=cnt[2]=0;
    dfs(x,0);
    return cnt[1]*cnt[2]*2+cnt[0]*cnt[0];
}
void solve(int x)
{
    ans+=cal(x,0);
    vis[x]=1;
    for(int i=head[x];i!=-1;i=edge[i].next)
    {
        int v=edge[i].v;
        if(vis[v]) continue;
        ans-=cal(v,edge[i].w);
        allnode=size[v],M=1e9,Focus=0;
        getFocus(v,x);
        solve(Focus);
    }   
}
int main()
{
    #ifndef ONLINE_JUDGE
        freopen("in.txt","r",stdin);
        freopen("out.txt","w",stdout);
    #endif
    scanf("%d",&n);
    init();
    for(int i=1,u,v,w;i<n;i++)
    {
        scanf("%d%d%d",&u,&v,&w);
        addEdge(u,v,w%3);
        addEdge(v,u,w%3);
    }
    allnode=n,Focus=0,M=1e9;
    getFocus(1,0);
    solve(Focus);
    int t=__gcd(ans,n*n);
    printf("%d/%d\n",ans/t,n*n/t);
    return 0;
}
```








# hdu  4751  Divide Groups（图的二染色） - HJ - CSDN博客
2017年10月16日 21:21:53[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：201
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[二分图最大匹配](https://blog.csdn.net/feizaoSYUACM/article/category/6308031)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=4751](http://acm.split.hdu.edu.cn/showproblem.php?pid=4751)
题目大意： 
判断是否为二分图？
解题思路： 
用DFS或者BFS二染色的方法判断即可
DFS递归写法：
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
const int maxn=105;
int n,cnt,head[maxn],g[maxn][maxn],color[maxn];
struct node{
    int to,next;
}edges[maxn*maxn];
void Init()        //初始化 
{
    cnt=0;
    memset(head,-1,sizeof(head));
    memset(color,-1,sizeof(color));   //-1表示未染色 0表示染成黑色 1表示染成白色 
}
void addedge(int u,int v)
{
    edges[cnt].to=v;
    edges[cnt].next=head[u];
    head[u]=cnt++;
} 
bool dfs(int u,int col)      //染色判断二分图  u表示节点 col表示节点u染的颜色值 
{
    color[u]=col;
    for(int i=head[u];~i;i=edges[i].next)
    {
        int v=edges[i].to;
        if(color[v]!=-1)     //如果v已经染了色 
        {
            if(color[v]==col)         //并且与u染色相同 则染色失败 
              return false;
            continue;
        }
        if(!dfs(v,!col))     //如果v没有染色 但是不能染与u相反的颜色 则染色失败 
          return false;
    }
    return true;      //最后才染色成功   
}
bool Judge()
{
    for(int i=1;i<=n;i++)
      if(color[i]==-1&&dfs(i,0)==false)   //如果存在一个点未染色 或者 并且这个点经过染色之后染色失败 
        return false;         //则原图不是二分图 
    return true;
}
int main()
{
    while(~scanf("%d",&n))
    {
        int t;
        memset(g,0,sizeof(g));
        for(int i=1;i<=n;i++)
          while(scanf("%d",&t)&&t)
            g[i][t]=1;
        Init();
        for(int i=1;i<=n;i++)
          for(int j=i+1;j<=n;j++)
            if(g[i][j]==0||g[j][i]==0)
            {
                addedge(i,j);
                addedge(j,i);
            }
        printf("%s\n",Judge()?"YES":"NO");
    }
    return 0;
}
```
BFS的非递归（队列写法）：
```cpp
#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
const int maxn=105;
int n,g[maxn][maxn],vis[maxn];
bool bfs(int x)
{
    queue<int>q;
    q.push(x);
    vis[x]=1;
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i=1;i<=n;i++)
        {
            if(g[u][i]||u==i)    //若 u与i不连通或者u与i是同一个节点 则不操作 
              continue;
            if(vis[i]==-1)   //如果i未染色 
            {
                vis[i]=1-vis[u];      //将i染成与u不相同的颜色 
                q.push(i);      //压入栈内 
            }
            else if(vis[i]==vis[u])       //如果i已经染色但是与u的染色值相同 则染色失败 
              return false;
        }
    } 
    return true;
}
int main()
{
    while(cin>>n)
    {
        int t,blag=0;
        memset(g,0,sizeof(g));
        memset(vis,-1,sizeof(vis));
        for(int i=1;i<=n;i++)
          while(cin>>t&&t)
            g[i][t]=1;
        for(int i=1;i<=n;i++)
          for(int j=1;j<=n;j++)
            if(g[i][j]==0)
              g[j][i]=0;
        for(int i=1;i<=n;i++)
          if(vis[i]==-1&&!bfs(i))
          {
              blag=1;
              break;
          }
        if(blag)
          cout<<"NO"<<endl;
        else
          cout<<"YES"<<endl;
    }
    return 0;
}
```

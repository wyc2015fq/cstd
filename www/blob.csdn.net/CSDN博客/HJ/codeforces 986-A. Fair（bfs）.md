# codeforces 986-A. Fair（bfs） - HJ - CSDN博客
2018年08月17日 09:08:39[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：96
个人分类：[====ACM====																[---------OJ---------																[codeforce																[-----------搜索-----------																[BFS](https://blog.csdn.net/feizaoSYUACM/article/category/7238039)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/986/A](http://codeforces.com/problemset/problem/986/A)
做法：
对于每一种k，bfs每个镇上要取得的最短路径，最后对于每个镇根据k的值sort排序取s个输出即可。
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#include<set>
#include<stack>
#include<map>
#include<queue>
#include<cstdio>
using namespace std;
#define mod 1000000007
#define INF 0x3f3f3f3f
typedef long long ll;
const int maxn=1e5+10;
vector<int> G[maxn];
int n,m,k,s,a[maxn],vis[maxn],dis[maxn][105];
queue<int> q;
void bfs()
{
    for(int j=1;j<=k;j++)
    {
        memset(vis,0,sizeof(vis));
        for(int i=1;i<=n;i++)
          if(a[i]==j)
            q.push(i),dis[i][j]=0,vis[i]=1;
        while(!q.empty())
        {
            int u=q.front();
            q.pop();
            for(int i=0;i<G[u].size();i++)
            {
                int v=G[u][i];
                if(vis[v]==0)   //该节点未访问 
                {
                    dis[v][j]=dis[u][j]+1;
                    vis[v]=1;   
                    q.push(v);
                }
            }
        }
    }
}
int main()
{
    int x,y;
    while(cin>>n>>m>>k>>s)
    {
        for(int i=0;i<=n;i++)
          G[i].clear();
        for(int i=1;i<=n;i++) 
          cin>>a[i];
        while(m--)
        {
            cin>>x>>y;
            G[x].push_back(y);
            G[y].push_back(x);
        }
        bfs();
        for(int i=1;i<=n;i++)
        {
            sort(dis[i]+1,dis[i]+1+k);
            int ans=0;
            for(int j=1;j<=s;j++)
              ans+=dis[i][j];
            cout<<ans<<" ";
        }
        cout<<endl;
    }
    return 0;
}
```

# hdu 1151 Air Raid （二分匹配） - HJ - CSDN博客
2017年10月14日 13:13:40[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：170
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1151](http://acm.split.hdu.edu.cn/showproblem.php?pid=1151)
题目大意： 
在一个城镇，有m个路口，和n条路，这些路都是单向的，而且路不会形成环，现在要弄一些伞兵去巡查这个城镇，伞兵只能沿着路的方向走，问最少需要多少伞兵才能把所有的路口搜一遍。这个题目就转换成求解有向无环图的最小路径覆盖问题了。
思路： 
**有向无环图的最小路径覆盖=该图的顶点数-该图的最大匹配**
```cpp
#include<iostream>
#include<cstring>
using namespace std;
const int maxn=125;
int n,m,link[maxn];
bool map[maxn][maxn],vis[maxn];
bool dfs(int u)
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
int hungary()
{
    int ans=0;
    memset(link,0,sizeof(link));        
    memset(vis,false,sizeof(vis));
    for(int i=1;i<=n;i++)
      if(dfs(i))
        ans++;
    return ans;
}
int main()
{
    int t,x,y;
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        memset(map,false,sizeof(map));
        while(m--)
        {
            cin>>x>>y;
            map[x][y]=true;
        } 
        int ans=hungary();
        cout<<n-ans<<endl;
    } 
    return 0;
}
```

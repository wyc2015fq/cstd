# hdu 1150  Machine Schedule（二分匹配） - HJ - CSDN博客
2017年10月14日 13:10:26[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：186
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1150](http://acm.split.hdu.edu.cn/showproblem.php?pid=1150)
题目大意： 
给你两台机器A和B，A机器有n种模式，B机器有m种模式，初始时都是0，现在给你k个任务，每个任务可以由机器A的x模式完成或者机器B的y模式完成，而每次改变机器的模式都要重启一次，问你最少的重启次数使得完成所有任务！
思路： 
 在这个二分图中找出最少的点使得所有的边都至少有一个端点被选中，而这就是最小点覆盖的模型，而 
**最小点覆盖=最大匹配**
 最大匹配用匈牙利算法求得。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
const int maxn=205;
int n,m,link[maxn],map[maxn][maxn];
bool vis[maxn];
bool dfs(int u)
{
    for(int i=1;i<n+m;i++)
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
    for(int i=1;i<n;i++)
    {
        memset(vis,false,sizeof(vis));
        if(dfs(i))
          ans++;
    }
    return ans;
}
int main()
{
    int k,a,b,c;
    while(cin>>n&&n)
    {
        cin>>m>>k;
        memset(map,0,sizeof(map));
        while(k--)
        {
            cin>>a>>b>>c;
            if(b==0||c==0)
              continue;
            map[b][c+n]=1;
        }
        int ans=hungary();
        cout<<ans<<endl;
    }
    return 0;
}
```

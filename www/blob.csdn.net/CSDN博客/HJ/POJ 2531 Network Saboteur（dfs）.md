# POJ 2531  Network Saboteur（dfs） - HJ - CSDN博客
2018年04月24日 19:58:54[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：91
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[POJ](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://poj.org/problem?id=2531](http://poj.org/problem?id=2531)
题目大意：有n个点，把所有点任意地选择放到两个集合里，在两个集合的每个点之间都会有权值，求可能形成的最大权值。
解题思路： 
典型的dfs题目，假设刚开始所有的点都在集合A内，每个点放在集合A内表示状态0，放在集合B内表示状态1，每次dfs搜索当前点是否从集合A挪到集合B结果更优（即根据挪动之后的差值是否大于零来判断即可）
```cpp
#include<iostream>
#include<cstring>
using namespace std;
typedef long long ll;
int n,a[25][25],vis[25];
ll ans;
void dfs(int cnt,ll res)
{ 
    vis[cnt]=1;
    ll sum=res;
    for(int i=1;i<=n;i++)  //从集合A挪到集合B中的值更新操作 
      sum=vis[i]?sum-a[cnt][i]:sum+a[cnt][i];
    ans=max(ans,sum);
    if(sum>res)        //如果第cnt个点从集合A挪到集合B中会增加res的值 则继续dfs 
    {
        for(int i=cnt+1;i<=n;i++)
          dfs(i,sum);
    }
    vis[cnt]=0;  //否则回溯 
} 
int main()
{
    while(cin>>n)
    {
        memset(vis,0,sizeof(0));
        for(int i=1;i<=n;i++)
          for(int j=1;j<=n;j++)
            cin>>a[i][j];
        ans=0;
        dfs(1,0);
        cout<<ans<<endl;
    }
    return 0;
}
```

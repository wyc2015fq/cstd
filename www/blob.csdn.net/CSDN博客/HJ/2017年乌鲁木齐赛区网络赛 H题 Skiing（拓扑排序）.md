# 2017年乌鲁木齐赛区网络赛 H题 Skiing（拓扑排序） - HJ - CSDN博客
2018年04月25日 17:31:15[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：65
个人分类：[====ACM====																[----------图论---------																[拓扑排序																[---------OJ---------																[计蒜客](https://blog.csdn.net/feizaoSYUACM/article/category/6307990)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238159)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[https://nanti.jisuanke.com/t/16957](https://nanti.jisuanke.com/t/16957)
题目大意：输入数据为一个DAG，请找出一条最长路，起点和终点可以任意选择。
解题思路：因为是一个DAG图，因此可以进行拓扑排序，每次排序过程中更新节点i的d[i]值，其中d[i]任意节点表示到节点 i 的最长路的值。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<queue> 
using namespace std;
const int maxn=1e5+10;
vector<pair<int,int> > vec[maxn];
queue<int> q;
int ans,n,m,u,v,w;
int Indeg[maxn],dp[maxn];
void topsort()
{
    for(int i=1;i<=n;i++)
      if(!Indeg[i])
        q.push(i);
    while(!q.empty())
    {
        int now=q.front();
        q.pop();
        for(int i=0;i<vec[now].size();i++)
        {
            if(--Indeg[vec[now][i].first]==0)
              q.push(vec[now][i].first);
            dp[vec[now][i].first]=max(dp[vec[now][i].first],dp[now]+vec[now][i].second);        
        }
    } 
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
          vec[i].clear();
        memset(Indeg,0,sizeof(Indeg));
        memset(dp,0,sizeof(dp));
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&w);
            vec[u].push_back(make_pair(v,w));
            Indeg[v]++; 
        }
        topsort();
        ans=0;
        for(int i=1;i<=n;i++)
          ans=max(ans,dp[i]);
        printf("%d\n",ans);
    }
    return 0;
}
```

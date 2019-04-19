# codeforces 919-D.Substring（拓扑排序+dp） - HJ - CSDN博客
2018年04月20日 18:46:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：89
个人分类：[====ACM====																[--------动态规划---------																[基础DP																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/919/D](http://codeforces.com/problemset/problem/919/D)
题目大意：给出n个点，m条边，n个点用长度为n的一个字符串表示，字符串中只有26个小写字母，问任意一条联通路径中出现相同字母次数最多的值是多少？如果有无穷大，输出-1,。
解题思路：用拓扑排序或者dfs跑一跑判断是否有环，d[i][j]表示前i个点联通的路径中字母j出现相同次数的最大值。
```cpp
#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
const int N=3e5+10;
int n,m,dp[N][30];
int cnt,deg[N],head[N];
char a[N];
struct node{
    int to,next;
}edge[N*2];
void add(int u,int v)
{
    edge[cnt].to=v;
    edge[cnt].next=head[u];
    head[u]=cnt++;
}
void topsort()
{
    queue<int> q;
    for(int i=1;i<=n;i++)
    {
        if(!deg[i])
        {
            q.push(i);
            dp[i][a[i-1]-'a']++; 
        } 
    }
    int cnt=0;
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        cnt++;
        for(int i=head[u];~i;i=edge[i].next)
        {
            int v=edge[i].to;
            deg[v]--;
            if(!deg[v])
              q.push(v);
            for(int j=0;j<26;j++)
              dp[v][j]=max(dp[v][j],dp[u][j]+(a[v-1]-'a'==j));
        }
    }
    if(cnt<n)
      cout<<-1<<endl;
    else
    {
        int ans=0;
        for(int i=1;i<=n;i++)
           for(int j=0;j<26;j++)
             ans=max(ans,dp[i][j]);
        cout<<ans<<endl;
    }
}
int main()
{
    while(cin>>n>>m>>a)
    {
        memset(head,-1,sizeof(head));
        memset(dp,0,sizeof(dp));
        memset(deg,0,sizeof(deg));
        cnt=0;
        int u,v;
        for(int i=0;i<m;i++)
        {
            cin>>u>>v;
            deg[v]++;
            add(u,v);
        } 
        topsort();
    }
    return 0;
}
```

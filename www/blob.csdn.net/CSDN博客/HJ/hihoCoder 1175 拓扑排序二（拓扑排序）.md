# hihoCoder  1175 拓扑排序二（拓扑排序） - HJ - CSDN博客
2018年04月25日 17:27:14[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：64
个人分类：[====ACM====																[----------图论---------																[拓扑排序																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238159)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[https://hihocoder.com/problemset/problem/1175](https://hihocoder.com/problemset/problem/1175)
简单题。只需要将每条边的前驱节点本身值赋给后继节点，最后计算所有点的值。
```cpp
#include<iostream>
#include<cstring>
#include<vector>
#include<queue>
using namespace std;
#define mod 142857
const int maxn=1e5+10; 
int n,m,k,ans,x,u,v;
int Indeg[maxn],vir[maxn];
vector<int> vec[maxn];
queue<int> q;
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
            if(--Indeg[vec[now][i]]==0)
              q.push(vec[now][i]);
            vir[vec[now][i]]=(vir[now]+vir[vec[now][i]])%mod;
        }
    }
}
int main()
{
    while(cin>>n>>m>>k)
    {
        for(int i=1;i<=n;i++)
          vec[i].clear();
        memset(Indeg,0,sizeof(Indeg));  
        memset(vir,0,sizeof(vir));
        while(k--)
        {
            cin>>x;
            vir[x]++;
        }
        for(int i=0;i<m;i++)
        {
            cin>>u>>v;
            vec[u].push_back(v);
            Indeg[v]++;
        }
        topsort();
        int ans=0;
        for(int i=1;i<=n;i++)
          ans=(ans+vir[i])%mod;
        cout<<ans<<endl;
    }
    return 0;
}
```

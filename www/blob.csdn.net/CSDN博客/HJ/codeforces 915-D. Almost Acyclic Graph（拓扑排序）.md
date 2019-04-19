# codeforces 915-D. Almost Acyclic Graph（拓扑排序） - HJ - CSDN博客
2018年04月25日 17:35:56[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：89
个人分类：[====ACM====																[----------图论---------																[拓扑排序																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238159)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/915/D](http://codeforces.com/problemset/problem/915/D)
题目大意：输入数据表示一个有向图，问是否能够至多删除其中的任意一条边，让这个图不存在环？
解题思路：首先拓扑排序判断是否存在环，如果不存在，枚举每一个点，因为删除一条边，实际上就是让这条边的后继点的入度值减一，因此每次枚举继续拓扑排序一下判断是否存在环，如果不存在，则输出YES，否则最后输出NO。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<queue> 
using namespace std;
const int maxn=1e5+10;
vector<int> vec[maxn];
queue<int> q;
int n,m,u,v;
int Indeg[maxn],Rudeg[maxn];
bool topsort()
{
    for(int i=1;i<=n;i++)
      if(!Indeg[i])
        q.push(i);
    int cnt=0;
    while(!q.empty())
    {
        int now=q.front();
        q.pop();
        cnt++;
        for(int i=0;i<vec[now].size();i++)
        {
            if(--Indeg[vec[now][i]]==0)
              q.push(vec[now][i]); 
        }
    }
    return cnt==n;
} 
int main()
{
    while(cin>>n>>m)
    {
        for(int i=1;i<=n;i++)
          vec[i].clear();
        memset(Indeg,0,sizeof(Indeg));
        memset(Rudeg,0,sizeof(Rudeg));
        while(m--)
        {
            cin>>u>>v;
            vec[u].push_back(v);
            Indeg[v]++;
            Rudeg[v]++;
        }
        int flag=0;
        if(topsort())   //首先判断是否存在环 
        {
            cout<<"YES"<<endl;
            continue;
        }
        else
        {
            for(int i=1;i<=n;i++)
            {
                memcpy(Indeg,Rudeg,sizeof(Rudeg));  //将Indeg拷贝到Rudeg中 
                if(Indeg[i]>=1)   //如果出现入度不为0的点，则表示该点在环中 
                {
                    Indeg[i]--;    //假设去掉该点的某一条线段 
                    if(topsort())  //如果topsort判断不再有环 
                    { 
                        flag=1;
                        cout<<"YES"<<endl;  //则符合要求 
                        break;
                    }
                }     //否则继续枚举 
            }
        }
        if(flag==0)
          cout<<"NO"<<endl;
    }
}
```

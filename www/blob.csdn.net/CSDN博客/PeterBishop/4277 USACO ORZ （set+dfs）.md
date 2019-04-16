# 4277 USACO ORZ （set+dfs） - PeterBishop - CSDN博客





2018年07月12日 09:49:13[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：33








```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include<algorithm>
 
using namespace std;
set<pair<int,int> >s;
int a[110],n,b[4],flag[110],co=0;//题意是让用n个小木棍拼一个三角形，所以每一个边可能要用m个小木棍，dfs暴力搜索
void dfs(int t)
{
    if(t==n)
    {
        if(b[0]<=b[1]&&b[1]<=b[2]&&b[0]+b[1]>b[2])
            s.insert(make_pair(b[0],b[1]));//等边去重
        return;
    }
    for(int i=0;i<3;i++)
    {
        b[i]+=a[t];dfs(t+1);b[i]-=a[t];//这里的可能性就是b[i]（三角形的一班）是否要加上a[t]（小木棍长度）
    }
}
int main()
{
    int t;cin>>t;
    while(t--)
    {
        cin>>n;s.clear();
        for(int i=0;i<n;i++)cin>>a[i];
        sort(a,a+n);
        dfs(0);
        cout<<s.size()<<endl;
    }
}
```





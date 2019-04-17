# PAT 甲级-1018 Public Bike Management (30)（好题）（30 分） - 安得广厦千万间的博客 - CSDN博客





2018年06月06日 16:38:42[独-](https://me.csdn.net/qq_41713256)阅读数：271








题目大意：

每个自行车车站的最大容量为一个偶数cmax，如果一个车站里面自行车的数量恰好为cmax / 2，那么称处于完美状态。如果一个车展容量是满的或者空的，控制中心（处于结点0处）就会携带或者从路上手机一定数量的自行车前往该车站，一路上会让所有的车展沿途都达到完美。现在给出cmax，车站的数量n，问题车站sp，m条边，还有距离，求最短路径。如果最短路径有多个，求能带的最少的自行车数目的那条。如果还是有很多条不同的路，那么就找一个从车站带回的自行车数目最少的。带回的时候是不调整的

比较经典的Dijkstra。有优先级的Dijkstra，还要加一个DFS遍历所有的路径，并选出最优的路径。首先在Dijkstra的基础上记录路径，用vector来存，edge【x】表示哪些点到x的距离最短（可能有多个）。最后DFS求出每一条路径需要带出的自行车数量和带回的自行车数量。具体的代码中解释。

这类的题目还是比较多的，好像都是30分的。。。有的不需要DFS，有的需要用map做个映射，代码相对会长一点。



#### 1030 Travel Plan (30)（30 分）

#### 1072 Gas Station (30)（30 分）

#### 1087 All Roads Lead to Rome (30)（30 分）

#### 1111 Online Map (30)（30 分）

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int inf=1e9;
int a[505][505];
int v[505],d[505],vis[505];
vector<int>edge[505],path,tpath;
int C,n,e,m;
int mingo,aback;
void dfs(int x)
{
    tpath.push_back(x);
    if(x==0)
    {
        int len=tpath.size();
        int tgo=0,tback=0;  // 带来和带回的数量。
        for(int i=len-2;i>=0;i--)
        {
           // cout<<tpath[i]<<' ';
            if(v[tpath[i]]+tback>=C/2) //当前站点和要带回的和大于要求的数量。
            {                              
                tback=v[tpath[i]]+tback-C/2;
            }
            else
            {
                tgo+=(C/2-v[tpath[i]]-tback); //当前站点和要带回的和不够要求的数量。
                tback=0;                     // 则必须从0站点带来，带回的更新为0.
            }
        }
       // cout<<"-----";
       // cout<<tgo<<' '<<tback<<endl;
        if(tgo<mingo)   
        {
            path=tpath;
            mingo=tgo;
            aback=tback;
        }
        else if(tgo==mingo&&aback>tback)
        {
            aback=tback;
            path=tpath;
        }
        tpath.pop_back();
        return;
    }
    for(int i=0;i<edge[x].size();i++)
        dfs(edge[x][i]);
    tpath.pop_back();
}
int main()
{
    cin>>C>>n>>e>>m;
    for(int i=1;i<=n;i++) cin>>v[i];
    for(int i=0;i<=n;i++)
    {
        vis[i]=0;
        for(int j=0;j<=n;j++)
         a[i][j]=inf;
    }
    while(m--)
    {
        int x,y,z;
        cin>>x>>y>>z;
        a[x][y]=a[y][x]=min(a[x][y],z);
    }
    for(int i=1;i<=n;i++)
        d[i]=a[0][i];
        d[0]=0;
    for(int j=0;j<=n;j++)
    {
        int tmp=inf,pos;
        for(int i=0;i<=n;i++)
        {
            if(vis[i]==0&&tmp>d[i])
            {
                tmp=d[i];
                pos=i;
            }
        }
        vis[pos]=1;
        for(int i=1;i<=n;i++)
        {
            if(vis[i]==0&&a[pos][i]!=inf)
            {
                if(d[i]>d[pos]+a[pos][i])
                {
                    d[i]=d[pos]+a[pos][i];
                    edge[i].clear();
                    edge[i].push_back(pos); // 如果经过pos到i站点使得距离更小，
                                             //  则更新，把之前的前驱清空
                }
                else if(d[i]==d[pos]+a[pos][i])
                {
                    edge[i].push_back(pos);  //如果经过pos到i站点使得距离与最小的一样，
                                                // 则插入。
                }
            }
        }
    }
    mingo=aback=inf;
    dfs(e);
    cout<<mingo<<' '<<'0';
    for(int i=path.size()-2;i>=0;i--) cout<<"->"<<path[i];
    cout<<' '<<aback;
    return 0;
}
```





# hdu  2544   最短路（Dijkstra） - HJ - CSDN博客
2016年08月22日 19:24:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：279
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最短路](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
在每年的校赛里，所有进入决赛的同学都会获得一件很漂亮的t-shirt。但是每当我们的工作人员把上百件的衣服从商店运回到赛场的时候，却是非常累的！所以现在他们想要寻找最短的从商店到赛场的路线，你可以帮助他们吗？
Input 
输入包括多组数据。每组数据第一行是两个整数N、M（N<=100，M<=10000），N表示成都的大街上有几个路口，标号为1的路口是商店所在地，标号为N的路口是赛场所在地，M则表示在成都有几条路。N=M=0表示输入结束。接下来M行，每行包括3个整数A，B，C（1<=A,B<=N,1<=C<=1000）,表示在路口A与路口B之间有一条路，我们的工作人员需要C分钟的时间走过这条路。 
输入保证至少存在1条商店到赛场的路线。
Output 
对于每组输入，输出一行，表示工作人员从商店走到赛场的最短时间
Sample Input
2 1 
1 2 3 
3 3 
1 2 5 
2 3 5 
3 1 2 
0 0
Sample Output
3 
2
Dijkstra算法：
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
#define INF 0x7ffffff
int d[1005],vis[1005],g[1005][1005];
int n,m,a,b,len;
void Dijkstra()
{
    for(int i=1;i<=n;i++)
      d[i]=g[1][i];
    memset(vis,0,sizeof(vis));
    vis[1]=1;
    for(int i=1;i<=n;i++)
    {
        int mind=INF,mark;
        for(int j=1;j<=n;j++)       //从未选取顶点集中选取距离源点最小的顶点mark 
        {
            if(!vis[j]&&d[j]<mind)
            {
                mind=d[j];
                mark=j;
            }
        }
        vis[mark]=1;       //加入顶点集 
        for(int j=1;j<=n;j++)          //通过mark顶点修改源点到其他顶点的距离，松弛操作 
          if(!vis[j]&&d[mark]+g[mark][j]<d[j])
            d[j]=d[mark]+g[mark][j];
    }
}
int main()
{
    while(cin>>n>>m)
    {
        if(n==0&&m==0)
           break;
        for(int i=1;i<=n;i++)
          for(int j=1;j<=n;j++)
             g[i][j]=INF;
        for(int i=0;i<m;i++)
        {
            cin>>a>>b>>len;
            g[a][b]=g[b][a]=len;
        }
        Dijkstra();
        cout<<d[n]<<endl;
    }
    return 0;
}
```
Floyd算法：
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
#define INF 0x7ffffff
int d[1005][1005],a,b,len,n,m,s,t;
void Floyd()
{
    for(int k=1;k<=n;k++)
      for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
          d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
}
int main()
{
    while(cin>>n>>m)
    {
        if(n==0&&m==0)
           break;
        for(int i=1;i<=n;i++)
          for(int j=1;j<=n;j++)
            d[i][j]=INF;
        for(int i=0;i<m;i++)
        {
            cin>>a>>b>>len;
            d[a][b]=d[b][a]=len;
        }
        Floyd();
        cin>>s>>t;
        if(d[s+1][t+1]!=INF)
          cout<<d[s+1][t+1]<<endl;
        else
          cout<<-1<<endl;
    }
    return 0;
}
```

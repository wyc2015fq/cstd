# hdu  1879  继续畅通工程（Prim） - HJ - CSDN博客
2016年08月22日 19:18:40[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：268
Problem Description 
省政府“畅通工程”的目标是使全省任何两个村庄间都可以实现公路交通（但不一定有直接的公路相连，只要能间接通过公路可达即可）。现得到城镇道路统计表，表中列出了任意两城镇间修建道路的费用，以及该道路是否已经修通的状态。现请你编写程序，计算出全省畅通需要的最低成本。
Input 
测试输入包含若干测试用例。每个测试用例的第1行给出村庄数目N ( 1< N < 100 )；随后的 N(N-1)/2 行对应村庄间道路的成本及修建状态，每行给4个正整数，分别是两个村庄的编号（从1编号到N），此两村庄间道路的成本，以及修建状态：1表示已建，0表示未建。
当N为0时输入结束。
Output 
每个测试用例的输出占一行，输出全省畅通需要的最低成本。
Sample Input
3 
1 2 1 0 
1 3 2 0 
2 3 4 0 
3 
1 2 1 0 
1 3 2 0 
2 3 4 1 
3 
1 2 1 0 
1 3 2 1 
2 3 4 1 
0
Sample Output
3 
1 
0
经典的最小生成树练习题！
Prim算法代码如下：
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>
using namespace std;
#define INF 100000000
int n,u,v,w,tt;
int d[105],map[105][105],vis[105];
void Prim()
{
    int sum=0;
    for(int i=2;i<=n;i++)
      d[i]=map[1][i];
    d[1]=0;
    vis[1]=1;
    for(int i=2;i<=n;i++)
    {
        int mind=INF,mark;
        for(int j=1;j<=n;j++)
        {
            if(!vis[j]&&mind>d[j])
            {
                mind=d[j];
                mark=j;
            }
        }
        vis[mark]=1;
        sum+=mind;
        for(int j=1;j<=n;j++)
          if(!vis[j])
            d[j]=min(d[j],map[mark][j]);
    }
    cout<<sum<<endl;
}
int main()
{
    while(~scanf("%d",&n))
    {
        if(n==0)
          break;
        int m=n*(n-1)/2;
        memset(vis,0,sizeof(vis));
        for(int i=0;i<m;i++)
        {
            scanf("%d %d %d %d",&u,&v,&w,&tt);
            if(tt==1)
              map[u][v]=map[v][u]=0;    //如果tt=1，表示该条路径已经修过，不需要计算，即赋值为0
            else
              map[u][v]=map[v][u]=w;   //否则赋值为输入值w
        }
        Prim();          //类似求从1到n单源最短路的算法，唯一不同的是得到的结果是每个最短路累加和
    }
    return 0;
}
```

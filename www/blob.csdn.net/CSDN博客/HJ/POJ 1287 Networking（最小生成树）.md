# POJ  1287  Networking（最小生成树） - HJ - CSDN博客
2017年01月31日 20:11:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：317
Description 
You are assigned to design network connections between certain points in a wide area. You are given a set of points in the area, and a set of possible routes for the cables that may connect pairs of points. For each possible route between two points, you are given the length of the cable that is needed to connect the points over that route. Note that there may exist many possible routes between two given points. It is assumed that the given possible routes connect (directly or indirectly) each two points in the area. 
Your task is to design the network for the area, so that there is a connection (direct or indirect) between every two points (i.e., all the points are interconnected, but not necessarily by a direct cable), and that the total length of the used cable is minimal.
Input 
The input file consists of a number of data sets. Each data set defines one required network. The first line of the set contains two integers: the first defines the number P of the given points, and the second the number R of given routes between the points. The following R lines define the given routes between the points, each giving three integer numbers: the first two numbers identify the points, and the third gives the length of the route. The numbers are separated with white spaces. A data set giving only one number P=0 denotes the end of the input. The data sets are separated with an empty line. 
The maximal number of points is 50. The maximal length of a given route is 100. The number of possible routes is unlimited. The nodes are identified with integers between 1 and P (inclusive). The routes between two points i and j may be given as i j or as j i.
Output 
For each data set, print one number on a separate line that gives the total length of the cable used for the entire designed network.
Sample Input
1 0
2 3 
1 2 37 
2 1 17 
1 2 68
3 7 
1 2 19 
2 3 11 
3 1 7 
1 3 5 
2 3 89 
3 1 91 
1 2 32
5 7 
1 2 5 
2 3 7 
2 4 8 
4 5 11 
3 5 10 
1 5 6 
4 2 12
0
Sample Output
0 
17 
16 
26
Source 
Southeastern Europe 2002
题目大意： 
给出n个节点，再有m条边，这m条边代表从a节点到b节点电缆的长度，现在要你将所有节点都连起来，并且使长度最小
解题思路： 
这是个标准的最小生成树的问题，用prim的时候需要注意的是他有重边，取边最小的那条加入图里就可以了，但是kruskal可以忽略这个问题
```cpp
#include<iostream>
using namespace std;
#define INF 0x3f3f3f3f
int map[1005][1005];
int d[1005],vis[1005],n,m;
void Prim()
{
    int ans=0;
    for(int i=1;i<=n;i++)
    {
        d[i]=map[1][i];
        vis[i]=0;
    }
    for(int i=1;i<=n;i++)
    {
        int mind=INF,mark;
        for(int j=1;j<=n;j++)
          if(!vis[j]&&mind>d[j])
            mind=d[mark=j];        //两步并做一步实现 
        vis[mark]=1;
        for(int j=1;j<=n;j++)
          if(!vis[j]&&map[mark][j]<d[j])    //松弛操作 
            d[j]=map[mark][j];
    }
    for(int i=1;i<=n;i++)
      ans+=d[i];
    cout<<ans<<endl;    
}
int main()
{
    while(cin>>n)
    {
        if(n==0)
          break;
        cin>>m;
        int u,v,w;
        for(int i=1;i<=n;i++)
          for(int j=1;j<=n;j++)
            if(i==j)
              map[i][j]=0;
            else
              map[i][j]=INF;
        for(int i=0;i<m;i++)
        {
            cin>>u>>v>>w;
            if(map[u][v]>w)
              map[v][u]=map[u][v]=w;     //保存两点之间最短消耗 
        }
        Prim();
    }
    return 0;
}
```

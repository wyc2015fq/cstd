# 最短路径：（Dijkstra & Floyd） - 在思索中前行！ - CSDN博客





2014年09月02日 17:31:22[_Tham](https://me.csdn.net/txl16211)阅读数：799标签：[最短路径																[图论																[Floyd																[dijkstra](https://so.csdn.net/so/search/s.do?q=dijkstra&t=blog)
个人分类：[数据结构																[NOIP 图论](https://blog.csdn.net/txl16211/article/category/2451417)](https://blog.csdn.net/txl16211/article/category/2409063)





## **Dijkstra算法**

1.定义概览

Dijkstra(迪杰斯特拉)算法是典型的单源最短路径算法，用于计算一个节点到其他所有节点的最短路径。主要特点是以起始点为中心向外层层扩展，直到扩展到终点为止。Dijkstra算法是很有代表性的最短路径算法，在很多专业课程中都作为基本内容有详细的介绍，如数据结构，图论，运筹学等等。注意该算法要求图中不存在负权边。

问题描述：在无向图 G=(V,E) 中，假设每条边 E[i] 的长度为 w[i]，找到由顶点 V0 到其余各点的最短路径。（单源最短路径）

2.算法描述

1)算法思想：设G=(V,E)是一个带权有向图，把图中顶点集合V分成两组，第一组为已求出最短路径的顶点集合（用S表示，初始时S中只有一个源点，以后每求得一条最短路径 , 就将加入到集合S中，直到全部顶点都加入到S中，算法就结束了），第二组为其余未确定最短路径的顶点集合（用U表示），按最短路径长度的递增次序依次把第二组的顶点加入S中。在加入的过程中，总保持从源点v到S中各顶点的最短路径长度不大于从源点v到U中任何顶点的最短路径长度。此外，每个顶点对应一个距离，S中的顶点的距离就是从v到此顶点的最短路径长度，U中的顶点的距离，是从v到此顶点只包括S中的顶点为中间顶点的当前最短路径长度。

2)算法步骤：

a.初始时，S只包含源点，即S＝{v}，v的距离为0。U包含除v外的其他顶点，即:U={其余顶点}，若v与U中顶点u有边，则<u,v>正常有权值，若u不是v的出边邻接点，则<u,v>权值为∞。

b.从U中选取一个距离v最小的顶点k，把k，加入S中（该选定的距离就是v到k的最短路径长度）。

c.以k为新考虑的中间点，修改U中各顶点的距离；若从源点v到顶点u的距离（经过顶点k）比原来距离（不经过顶点k）短，则修改顶点u的距离值，修改后的距离值的顶点k的距离加上边上的权。

d.重复步骤b和c直到所有顶点都包含在S中。

执行动画过程如下图

![](http://pic002.cnblogs.com/images/2012/426620/2012073019540660.gif)

3.算法代码实现：



```cpp
const int  MAXINT = 32767;
const int MAXNUM = 10;
int dist[MAXNUM];
int prev[MAXNUM];

int A[MAXUNM][MAXNUM];

void Dijkstra(int v0)
{
  　　bool S[MAXNUM];                                  // 判断是否已存入该点到S集合中
      int n=MAXNUM;
  　　for(int i=1; i<=n; ++i)
 　　 {
      　　dist[i] = A[v0][i];
      　　S[i] = false;                                // 初始都未用过该点
      　　if(dist[i] == MAXINT)    
            　　prev[i] = -1;
 　　     else 
            　　prev[i] = v0;
   　　}
   　 dist[v0] = 0;
   　 S[v0] = true; 　　
 　　 for(int i=2; i<=n; i++)
 　　 {
       　　int mindist = MAXINT;
       　　int u = v0; 　　                            // 找出当前未使用的点j的dist[j]最小值
      　　 for(int j=1; j<=n; ++j)
      　　    if((!S[j]) && dist[j]<mindist)
      　　    {
         　　       u = j;                             // u保存当前邻接点中距离最小的点的号码 
         　 　      mindist = dist[j];
       　　   }
       　　S[u] = true; 
       　　for(int j=1; j<=n; j++)
       　　    if((!S[j]) && A[u][j]<MAXINT)
       　　    {
           　    　if(dist[u] + A[u][j] < dist[j])     //在通过新加入的u点路径找到离v0点更短的路径  
           　    　{
                   　　dist[j] = dist[u] + A[u][j];    //更新dist 
                   　　prev[j] = u;                    //记录前驱顶点 
            　　    }
        　    　}
   　　}
}
```





4.算法实例

先给出一个无向图

![](http://pic002.cnblogs.com/images/2012/426620/2012073019593375.jpg)

用Dijkstra算法找出以A为起点的单源最短路径步骤如下

![](http://pic002.cnblogs.com/images/2012/426620/2012073020014941.jpg)

**Floyd算法**



1.定义概览

**Floyd-Warshall算法**（Floyd-Warshall algorithm）是解决任意两点间的最短路径的一种算法，可以正确处理有向图或负权的最短路径问题，同时也被用于计算有向图的传递闭包。Floyd-Warshall算法的时间复杂度为O(N3)，空间复杂度为O(N2)。

2.算法描述

1)算法思想原理：

Floyd算法是一个经典的动态规划算法。用通俗的语言来描述的话，首先我们的目标是寻找从点i到点j的最短路径。从动态规划的角度看问题，我们需要为这个目标重新做一个诠释（这个诠释正是动态规划最富创造力的精华所在）

      从任意节点i到任意节点j的最短路径不外乎2种可能，1是直接从i到j，2是从i经过若干个节点k到j。所以，我们假设Dis(i,j)为节点u到节点v的最短路径的距离，对于每一个节点k，我们检查Dis(i,k) + Dis(k,j) < Dis(i,j)是否成立，如果成立，证明从i到k再到j的路径比i直接到j的路径短，我们便设置Dis(i,j)
 = Dis(i,k) + Dis(k,j)，这样一来，当我们遍历完所有节点k，Dis(i,j)中记录的便是i到j的最短路径的距离。

2).算法描述：

a.从任意一条单边路径开始。所有两点之间的距离是边的权，如果两点之间没有边相连，则权为无穷大。 　　

b.对于每一对顶点 u 和 v，看看是否存在一个顶点 w 使得从 u 到 w 再到 v 比己知的路径更短。如果是更新它。

3).Floyd算法过程矩阵的计算----十字交叉法

方法：两条线，从左上角开始计算一直到右下角 如下所示

给出矩阵，其中矩阵A是邻接矩阵，而矩阵Path记录u,v两点之间最短路径所必须经过的点

![](http://pic002.cnblogs.com/images/2012/426620/2012073109403649.gif)

相应计算方法如下：

![](http://pic002.cnblogs.com/images/2012/426620/2012073109460084.jpg)

![](http://pic002.cnblogs.com/images/2012/426620/2012073109453085.jpg)

![](http://pic002.cnblogs.com/images/2012/426620/2012073109463549.jpg)

最后A3即为所求结果

3.算法代码实现



```cpp
typedef struct          
{        
    char vertex[VertexNum];                                //顶点表         
    int edges[VertexNum][VertexNum];                       //邻接矩阵,可看做边表         
    int n,e;                                               //图中当前的顶点数和边数         
}MGraph; 

void Floyd(MGraph g)
{
 　　int A[MAXV][MAXV];
 　　int path[MAXV][MAXV];
 　　int i,j,k,n=g.n;
 　　for(i=0;i<n;i++)
    　　for(j=0;j<n;j++)
    　　{ 　　
             A[i][j]=g.edges[i][j];
         　　 path[i][j]=-1;
     　 }
 　　for(k=0;k<n;k++)
 　　{ 
      　　for(i=0;i<n;i++)
         　　for(j=0;j<n;j++)
             　　if(A[i][j]>(A[i][k]+A[k][j]))
             　　{
                   　　A[i][j]=A[i][k]+A[k][j];
                   　　path[i][j]=k;
              　 } 
    　} 
}
```



## **弗洛伊德（Floyd）算法过程：**
１、用D[v][w]记录每一对顶点的最短距离。
２、依次扫描每一个点，并以其为基点再遍历所有每一对顶点D[ ][ ]的值，看看是否可用过该基点让这对顶点间的距离更小。
算法理解：
最短距离有三种情况：
１、两点的直达距离最短。（如下图<v,x>）
２、两点间只通过一个中间点而距离最短。（图<v,u>）
３、两点间用通过两各以上的顶点而距离最短。（图<v,w>）

对于第一种情况：在初始化的时候就已经找出来了且以后也不会更改到。
对于第二种情况：弗洛伊德算法的基本操作就是对于每一对顶点，遍历所有其它顶点，看看可否通过这一个顶点让这对顶点距离更短，也就是遍历了图中所有的三角形（算法中对同一个三角形扫描了九次，原则上只用扫描三次即可，但要加入判断，效率更低）。
对于第三种情况：如下图的五边形，可先找一点（比如x，使<v,u>=2），就变成了四边形问题，再找一点（比如y,使<u,w>=2），可变成三角形问题了（v,u,w），也就变成第二种情况了，由此对于n边形也可以一步步转化成四边形三角形问题。（这里面不用担心哪个点要先找哪个点要后找，因为找了任一个点都可以使其变成（n－1）边形的问题）。

![ddddddddddddd](http://www.itweb2.com/attachments/month_0703/g200739153139.gif)




```cpp
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define impass -1
#define MAX 100
 
typedef struct
{
    int v, e;
    int matrix[MAX][MAX];
}Graph;
 
typedef struct load//用于存储路径所经过的点 从s到e的最短路径所经过的顶点在vex中
{
        int s, e, len;
        int vex[MAX];
}Path;
     
void CreatGraph(Graph *G)
{
    int i;
    int s, e,len;
 
    scanf("%d%d", &G->v, &G->e);
 
    memset(G->matrix, impass, sizeof(int) * (MAX * MAX));
     
    for (i = 0; i < G->e; i++)
    {
        scanf("%d%d%d", &s, &e, &len);
        G->matrix[s][e] = len;
    }
}
 
 
void Floyd(Graph G)
{
     
     
    int Locate(int i, int j, Path path[MAX], int sum);
     
    Path path[MAX];
    int dis[MAX][MAX];
    //int path[MAX][MAX][MAX];//原本想使用path三维数组存储 path[i][j][k] == 1表示从i到j的最短路径经过k
    int i, j, k, p;
    int pos = 0;
    int pos1, pos2, pos3;
     
    memset(path, 0, sizeof(int) * (MAX * MAX));
    memset(path, 0, sizeof(int) * (MAX * MAX));
     
    for (i = 0; i < G.v; i++)//初始化 
    {
        for (j = 0; j < G.v; j++)
        {
            dis[i][j] = G.matrix[i][j];
            path[pos].s = i;
            path[pos].e = j;
            if (dis[i][j] != impass)
            {
                path[pos].vex[i] = 1;
                path[pos].vex[j] = 1;
            }
            pos++;
        }
    }
 
     
    for (i = 0; i < G.v; i++)
        for (j = 0; j < G.v; j++)
            for (k = 0; k < G.v; k++)
            {
                if (i == j)
                    break;
                if (dis[i][k] != impass && dis[k][j] != impass)
                    if (dis[i][j] > dis[i][k] + dis[k][j] || dis[i][j] == impass)//从i到k再到j的路径更短
                    {
                        dis[i][j] = dis[i][k] + dis[k][j];
                        pos1 = Locate(i, j, path, G.v);
                        pos2 = Locate(i, k, path, G.v);
                        pos3 = Locate(k, j, path, G.v);
                        for (p = 0; p < G.v; p++)
                        {
                            path[pos1].vex[p] = (path[pos2].vex[p] || path[pos3].vex[p]);
                        }
                    }
            }
 
    for (i = 0; i < G.v; i++)//输出最短路径
        for (j = 0; j < G.v; j++)
        {
            if (i == j)
                continue;
            printf("The ShortPath From %d to %d:\n", i, j);
            if (dis[i][j] == impass)
                printf("No Path\n");
            else
                printf("%d\n", dis[i][j]);
        }
 
}
 
int Locate(int i, int j, struct load path[MAX], int sum)
{
    int k;
     
    for (k = 0; k < sum; k++)
        if (path[k].s == i && path[k].e == j)
            return k;
    return impass;
}
 
int main()
{
    Graph G;
    CreatGraph(&G);
    Floyd(G);
    return 0;
}
```


单纯的Dijkstra实现





```cpp
void Dijkstra(int n, int v, int *dist, int *prev, int c[maxnum][maxnum])
　
{
    　　bool s[maxnum]; // 判断是否已存入该点到S集合中
    　　for(int i=1; i<=n; ++i)
        　　{
            　　dist[i] = c[v][i];
            　　s[i] = 0; // 初始都未用过该点
            　　if(dist[i] == maxint)
                　　prev[i] = 0;
            　　else
                　　prev[i] = v;
        　　
            }
    　　dist[v] = 0;
    　　s[v] = 1;
    　　// 依次将未放入S集合的结点中，取dist[]最小值的结点，放入结合S中
    　　// 一旦S包含了所有V中顶点，dist就记录了从源点到所有其他顶点之间的最短路径长度
    　　// 注意是从第二个节点开始，第一个为源点
    　　for(int i=2; i<=n; ++i)
        　　{
            　　int tmp = maxint;
            　　int u = v;
            　　// 找出当前未使用的点j的dist[j]最小值
            　　for(int j=1; j<=n; ++j)
                　　if((!s[j])/*unkonw*/ && dist[j]<tmp)
                    　　{
                        　　u = j; // u保存当前邻接点中距离最小的点的号码
                        　　tmp = dist[j];
                        }
            　　s[u] = 1; // 表示u点已存入S集合中
            　　// 更新dist
            　　for(int j=1; j<=n; ++j)//寻找u的邻接，并更亲dist
                　　if((!s[j]) && c[u][j]<maxint)
                    　　{
                        　　int newdist = dist[u] + c[u][j];
                        　　if(newdist < dist[j])
                            　　{
                                　　dist[j] = newdist;
                                　　prev[j] = u;
                                }
                        }
            }
    　　
}
```


## **<<数据结构>>(C语言版 严蔚敏 ) 中dijkstra算法的实现**



```cpp
/*
测试数据 教科书 P189 G6 的邻接矩阵 其中 数字 1000000 代表无穷大
6
1000000 1000000 10 100000 30 100
1000000 1000000 5 1000000 1000000 1000000
1000000 1000000 1000000 50 1000000 1000000
1000000 1000000 1000000 1000000 1000000 10
1000000 1000000 1000000 20 1000000 60
1000000 1000000 1000000 1000000 1000000 1000000
结果：
D[0]   D[1]   D[2]   D[3]   D[4]   D[5]
 0   1000000   10     50     30     60
*/
#include <iostream>
#include <cstdio>
#define MAX 1000000
using namespace std;
int arcs[10][10];//邻接矩阵
int D[10];//保存最短路径长度
int p[10][10];//路径
int final[10];//若final[i] = 1则说明 顶点vi已在集合S中
int n = 0;//顶点个数
int v0 = 0;//源点
int v,w;
void ShortestPath_DIJ()
{
     for (v = 0; v < n; v++) //循环 初始化
     {
          final[v] = 0; D[v] = arcs[v0][v];
          for (w = 0; w < n; w++) p[v][w] = 0;//设空路径
          if (D[v] < MAX) {p[v][v0] = 1; p[v][v] = 1;}
     }
     D[v0] = 0; final[v0]=0; //初始化 v0顶点属于集合S
     //开始主循环 每次求得v0到某个顶点v的最短路径 并加v到集合S中
     for (int i = 1; i < n; i++)
     {
          int min = MAX;
          for (w = 0; w < n; w++)
          {
               //我认为的核心过程--选点
               if (!final[w]) //如果w顶点在V-S中
               {
                    //这个过程最终选出的点 应该是选出当前V-S中与S有关联边
                    //且权值最小的顶点 书上描述为 当前离V0最近的点
                    if (D[w] < min) {v = w; min = D[w];}
               }
          }
          final[v] = 1; //选出该点后加入到合集S中
          for (w = 0; w < n; w++)//更新当前最短路径和距离
          {
               /*在此循环中 v为当前刚选入集合S中的点
               则以点V为中间点 考察 d0v+dvw 是否小于 D[w] 如果小于 则更新
               比如加进点 3 则若要考察 D[5] 是否要更新 就 判断 d(v0-v3) + d(v3-v5) 的和是否小于D[5]
               */
               if (!final[w] && (min+arcs[v][w]<D[w]))
               {
                    D[w] = min + arcs[v][w];
                   // p[w] = p[v];
                    p[w][w] = 1; //p[w] = p[v] +　[w]
               }
          }
     }
}
 
 
int main()
{
    cin >> n;
    for (int i = 0; i < n; i++)
    {
         for (int j = 0; j < n; j++)
         {
              cin >> arcs[i][j];
         }
    }
    ShortestPath_DIJ();
    for (int i = 0; i < n; i++) printf("D[%d] = %d\n",i,D[i]);
    return 0;
}
```
转载来自：[华山大师兄，数据结构与算法：http://www.cnblogs.com/biyeymyhjob/category/395206.html](http://www.cnblogs.com/biyeymyhjob/category/395206.html)](https://so.csdn.net/so/search/s.do?q=Floyd&t=blog)](https://so.csdn.net/so/search/s.do?q=图论&t=blog)](https://so.csdn.net/so/search/s.do?q=最短路径&t=blog)





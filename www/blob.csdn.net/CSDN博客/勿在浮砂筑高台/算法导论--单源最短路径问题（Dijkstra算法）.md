# 算法导论--单源最短路径问题（Dijkstra算法） - 勿在浮砂筑高台 - CSDN博客





2016年07月15日 18:41:49[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：7559
所属专栏：[算法导论--学习笔记](https://blog.csdn.net/column/details/lsx-099.html)









**转载请注明出处**：勿在浮沙筑高台[http://blog.csdn.net/luoshixian099/article/details/51918844](http://blog.csdn.net/luoshixian099/article/details/51918844)

单源最短路径是指：给定源顶点$s \in V$到分别到其他顶点$v \in V-\{s\}$的最短路径的问题。 

Dijkstra算法采用贪心策略：按路径长度递增的顺序，逐个产生各顶点的最短路径。算法过程中需要维护一个顶点集$S$,此顶点集保存已经找到最短路径的顶点。还需要维护一个距离数组dist, dist[i]表示第i个顶点与源结点s的距离长度。

Dijkstra算法思路：

> - $S$初始化时只包括源节点s; 
$dist[]$初始化：dist[i]= arc[s][i],arc为图的邻接矩阵。 
$V-S$表示未被找到最短的路径的顶点集合；- 把$dist$按递增的顺序，选择一个最短路径，从$V-S$把对应顶点加入到$S$中，每次$S$中加入一个新顶点$u$ , 需要对$dist$更新，即$s$能否通过顶点$u$达到其他顶点更近。 

  即若dist[u] + arc[u][v] < dist[v],则更新

$  dist[v]=dist[u] + arc[u][v]$
- 重复上述步骤，直到$S=V$
![这里写图片描述](https://img-blog.csdn.net/20160715180431746)
![这里写图片描述](https://img-blog.csdn.net/20160715182535666)
![这里写图片描述](https://img-blog.csdn.net/20160715182546197)
**代码：**

```cpp
/************************************************************************
CSDN 勿在浮沙筑高台 http://blog.csdn.net/luoshixian099算法导论--最短路径（Dijkstra算法）2016年7月15日
************************************************************************/
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
#define INFINITE 0xFFFFFFFF   
#define VertexData unsigned int  //顶点数据
#define UINT  unsigned int
#define vexCounts 6  //顶点数量
char vextex[] = { 'A', 'B', 'C', 'D', 'E', 'F' };

void AdjMatrix(unsigned int adjMat[][vexCounts])  //邻接矩阵表示法
{
    for (int i = 0; i < vexCounts; i++)   //初始化邻接矩阵
        for (int j = 0; j < vexCounts; j++)
        {
           adjMat[i][j] = INFINITE;
        }
    adjMat[0][1] = 50; adjMat[0][2] = 10; adjMat[0][4] = 45;
    adjMat[1][2] = 15; adjMat[1][4] = 10; 
    adjMat[2][0] = 20; adjMat[2][3] = 15; 
    adjMat[3][1] = 20; adjMat[3][4] = 35; adjMat[3][5] = 3;
    adjMat[4][3] = 30; 
}

void ShortestPath_DJS(unsigned int adjMat[][vexCounts],unsigned int s)
{
    vector<VertexData> vexset; //已经找到最短路径的顶点集
    vector<UINT> dist(vexCounts); //没有被找的最短路径的顶点距离信息
    vector<vector<VertexData> > path(vexCounts); //每个顶点的最短路径信息
    for (unsigned int i = 0; i < vexCounts; i++)
    {
        dist[i] = adjMat[s][i];  //初始化距离
        if (dist[i] != INFINITE)//s是否有路径到达i
        {
            path[i].push_back(s);  //把保存最短路径
            path[i].push_back(i);
        }
    }
    vexset.push_back(s); //初始把顶点s加入vexset

    for (unsigned int n = 1; n <= vexCounts-1; n++) 
    {
        UINT min = INFINITE;
        UINT k = INFINITE;
        for (int i = 0; i < vexCounts;i++) //寻找下一条最短路径
        {
            if (find(vexset.rbegin(),vexset.rend(),i) == vexset.rend() && dist[i] < min)
            {
                k = i;
                min = dist[i];
            }
        }
        vexset.push_back(k); //把最短路径顶点加入vexset中
        for (int i = 0; i < vexCounts;i++)  //更新dist
        {
            /*检测vexset中是否已经有顶点i,即i的最短路径是否已经找到。
            如果没有找到，则判定是否需要更新最短路径*/
            if (find(vexset.rbegin(), vexset.rend(), i) == vexset.rend() 
                && adjMat[k][i] != INFINITE && dist[k]+ adjMat[k][i] < dist[i])
            {
                dist[i] = dist[k] + adjMat[k][i];
                path[i] = path[k];
                path[i].push_back(i);
            }
        };
        /*便于观察，输出每个顶点的最短路径经过的所有其他顶点及其距离*/
        cout << "Path: ";
        for (int i = 0; i < path[k].size();i++)
        {
            cout << vextex[path[k][i]] << ",";
        }
        cout << "     距离="<<dist[k]<<endl;

    }
}

int main()
{
    unsigned int  adjMat[vexCounts][vexCounts] = { 0 };
    AdjMatrix(adjMat);   //邻接矩阵
    cout << "Dijkastra : A" << endl;  //计算顶点A的最短路径
    ShortestPath_DJS(adjMat, 0); //Djikstra算法，A的序号为0.
    return 0;
}
```

![这里写图片描述](https://img-blog.csdn.net/20160715183815921)

***Reference:***

数据结构-耿国华 

算法导论-第三版







# C++生成一个随机网络 - 小平子的专栏 - CSDN博客





2015年05月01日 11:27:41[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：1136








![版權聲明](https://img-blog.csdn.net/20160117172627181)

这是社会网络管理与分析课程的作业。老师要求模拟随机网络的连接过程，画出小世界态变（大陆说的是质变？）的曲线。
在随机网络生成过程中，最大连通分支结点数占所有结点数的比例会随着所有结点平均连接边数而变化。最开始，这种变化非常不明显，但是当所有结点平均连接边数达到1时，最大连通分支结点数占所有结点数的比例会突然增大。效果如下图所示。横轴表示平均连接边数，纵轴表示最大连通分支结点数与总结点数的比例。该图使用Python的Matplotlib包绘制。 
![这里写图片描述](https://img-blog.csdn.net/20160322010941508)
作业使用C++简单地模拟了一个随机网络的生成过程。网络使用邻接表存储。结点数为100。建立一个while循环，每次循环产生两个不重复且之前没有相连的两个伪随机数作为两个将要相连的顶点，直到整个图成为一个连通图为止。每次连接了两个结点都要遍历整个图判断是否形成了连通图，并计算所有结点的平均连接边数和最大连通分支结点占所有结点的比例两个指标。连通图通过深度优先遍历来判断。代码如下。急急忙忙写的程序，不知道会不会有问题，不过有问题老师也不会发现的。写的时候labmate问我为什么不用python？！我说python还不熟悉啊！deadline前一晚才开始写，懒得查python了……

```cpp
// SocialHW_Graph.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include<time.h> 
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define Random(x) (rand() % x) //通过取余取得指定范围的随机数
int const MAXVEX = 100;
int const DIS = MAXVEX; // 用於產生0-DIS之間的隨機數
bool bVisited[MAXVEX]; // 遍歷標志數組
int nMaxConnectedVex = 0; // 連通子圖的最大頂點數
int nSumConnectedVex = 0;
int nConnectedVex = 0; // 連通子圖的頂點數
typedef int VertexType;
typedef int EdgeType;

// 邊表
typedef struct EdgeNode
{
    int adjvex; // 鄰接點域，存儲該頂點對應的下標
    EdgeType weight;
    struct EdgeNode *next;
}EdgeNode;

// 頂點表
typedef struct VertexNode
{
    VertexType data;
    EdgeNode *firstedge;
    bool visited;

}VertexNode, AdjList[MAXVEX];

typedef struct GraphList
{
    AdjList adjList;
    int numVertexes; // 圖中頂點數
    int numEdges; // 圖中邊數
    int numDegree; // 圖中總度數
}GraphList;

int RandomNum(int const dis = DIS)
{
    return Random(dis);
}

// 判斷兩個頂點之間是否相連
// v1和v2是頂點表中的序號
bool IsConnected(GraphList *g, int v1, int v2)
{
    bool bConnected = false;
    EdgeNode *p = NULL;
    if (g->adjList[v1].firstedge != NULL)
    {
        p = g->adjList[v1].firstedge;
        while (p != NULL)
        {
            if (p->adjvex != v2)
            {
                p = p->next;
            }
            else
            {
                bConnected = true;
                break;
            }
        }
    }
    return bConnected;
}

// 判斷該圖是否是完全圖
bool IsCompleteGraph(GraphList *g)
{
    bool bComplete = false;
    if (NULL == g)
    {
        return bComplete;
    }

    if (g->numEdges == (MAXVEX * (MAXVEX - 1)) / 2)
    {
        bComplete = true;
    }

    return bComplete;
}


// 計算最大連通分支的頂點數量
int CalMaxConnectedVexNum(GraphList *g)
{
    int nVexNum = 0;
    int nTemp = 0;
    if (NULL == g)
    {
        return nVexNum;
    }
}

// 深度優先遍歷
// i是頂點序號
void DFS(GraphList g, int i)
{
    EdgeNode *p = NULL;
    bVisited[i] = true;
    nConnectedVex++;
    p = g.adjList[i].firstedge;
    while (p)
    {
        if (!bVisited[p->adjvex])
        {
            DFS(g, p->adjvex);
        }
        p = p->next;
    }
}

// 深度優先遍歷操作
// 返回連通分量個數
int DFSTraverse(GraphList g)
{
    int nCount = 0;
    for (size_t i = 0; i < MAXVEX; i++)
    {
        if (!bVisited[i])
        {
            nConnectedVex = 0;
            DFS(g, i);
            nCount++;
            if (nConnectedVex > 1)
            {
                nSumConnectedVex += nConnectedVex;
            }
            nSumConnectedVex += nConnectedVex;
            if (nConnectedVex > nMaxConnectedVex)
            {
                nMaxConnectedVex = nConnectedVex;
                cout << "The max number of vertex of a connected component for now is: " << nMaxConnectedVex << endl;
            }
        }
    }
    return nCount;
}

// 初始化遍歷標志
void InitVisted()
{
    for (size_t i = 0; i < MAXVEX; i++)
    {
        bVisited[i] = false;
    }
}

// 初始化一個空圖
GraphList* InitGraph(int nVex = MAXVEX)
{
    if (nVex > MAXVEX)
    {
        return NULL;
    }
    GraphList *g = new GraphList();
    // 頂點表分配數據
    // 頂點的值和頂點表中的序號是一樣的
    for (int i = 0; i < MAXVEX; i++)
    {
        g->adjList[i].data = i;
        g->adjList[i].firstedge = NULL;
        g->adjList[i].visited = false;
        bVisited[i] = false;
    }
    g->numEdges = 0;
    g->numVertexes = 0;
    g->numDegree = 0;
    return g;
}

// 根據隨機數構造一個圖
bool GenGraphByRandNum(GraphList *g)
{
    //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
    ofstream inFile;
    ofstream inVertexFile;
    inFile.open("inFile_edge.csv", ios::trunc);
    inVertexFile.open("VertexFile.csv", ios::trunc);

    if (NULL == g)
    {
        return false;
    }
    g->numEdges = 0;
    g->numVertexes = 0;

    double dAvgEdge = 0; // 每個頂點的平均邊數
    double dConnctedVexRate = 0; // 最大連通分支的頂點比例
    int v1 = 0;
    int v2 = 0;
    int nCount = 0; // 從初始圖到完全圖循環的次數
    int nRandomCount = 0; // 選取兩個隨機數的總次數
    bool bCompleteGraph = false;
    // 當出現完連通圖時，停止構造
    while (dConnctedVexRate < 1.0)
    {
        v1 = 0;
        v2 = 0;

        // 產生兩個不相等的隨機數
        // 且兩個頂點沒有邊鏈接
        // 且該圖不是一個完全圖
        //bCompleteGraph = IsCompleteGraph(g);
        if (dConnctedVexRate == 1) // 完全圖，停止
        {
            break;
        }
        while (v1 == v2 || IsConnected(g, v1, v2)) 
        {
            srand((int)time(NULL));
            v1 = RandomNum();
            v2 = RandomNum();
            nRandomCount++;
        }

        nCount++;

        cout << "Vertex No.: " << v1 << "," << v2 << endl;
        inVertexFile << v1 << "," << v2 << endl;

        // 將頂點和邊加入圖中
        EdgeNode *e = new EdgeNode();
        e->adjvex = v2;
        e->next = g->adjList[v1].firstedge;
        g->adjList[v1].firstedge = e;

        EdgeNode *f = new EdgeNode();
        f->adjvex = v1;
        f->next = g->adjList[v2].firstedge;
        g->adjList[v2].firstedge = f;

        // 更新頂點數
        // 先判斷頂點是否已經在圖中
        if (!g->adjList[v1].visited)
        {
            g->numVertexes++;
        }
        if (!g->adjList[v2].visited)
        {
            g->numVertexes++;
        }
        g->adjList[v1].visited = true;
        g->adjList[v2].visited = true;
        g->numEdges++;
        g->numDegree += 2;

        int nComponentNum = DFSTraverse(*g);
        cout << "The number of connected component for now is: " << nComponentNum << endl;
        dAvgEdge = (double)g->numEdges / (double)MAXVEX;
        dConnctedVexRate = (double)nMaxConnectedVex / (double)MAXVEX;
        cout << "Average edge: " << dAvgEdge << endl;
        cout << "Connected vertex rate: " << dConnctedVexRate << endl;
        cout << "-------------------------------------------------" << endl;
        inFile << dAvgEdge << "," << dConnctedVexRate << endl;
        InitVisted();
    }
    inFile.close();
    inVertexFile.close();
}


int _tmain(int argc, _TCHAR* argv[])
{
    /*ofstream inFile;
    inFile.open("randomFile.txt", ios::trunc);
    int x = 0;
    int y = 0;
    for (size_t i = 0; i < 100; i++)
    {
        x = RandomNum(960)+1;
        y = RandomNum(500)+1;
        inFile << x << "," << y << endl;
        cout << x << "," << y << endl;
    }
    inFile.close();
    return 0;*/
    GraphList *g = InitGraph();
    GenGraphByRandNum(g);
    return 0;
}
```






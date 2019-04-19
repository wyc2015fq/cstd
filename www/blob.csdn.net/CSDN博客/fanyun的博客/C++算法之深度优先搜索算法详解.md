# C++算法之深度优先搜索算法详解 - fanyun的博客 - CSDN博客
2018年09月28日 20:23:41[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1580
1.深度优先搜索算法
         深度优先搜索是一种在开发爬虫早期使用较多的方法。它的目的是要达到被搜索结构的叶结点(即那些不包含任何超链的HTML文件) 。在一个HTML文件中，当一个超链被选择后，被链接的HTML文件将执行深度优先搜索，即在搜索其余的超链结果之前必须先完整地搜索单独的一条链。深度优先搜索沿着HTML文件上的超链走到不能再深入为止，然后返回到某一个HTML文件，再继续选择该HTML文件中的其他超链。当不再有其他超链可选择时，说明搜索已经结束。
2.实现原理
        深度优先搜索属于图算法的一种，英文缩写为DFS即Depth First Search.其过程简要来说是对每一个可能的分支路径深入到不能再深入为止，而且每个节点只能访问一次.
        举例说明之：下图是一个无向图，如果我们从A点发起深度优先搜索（以下的访问次序并不是唯一的，第二个点既可以是B也可以是C,D），则我们可能得到如下的一个访问过程：A->B->E（没有路了！回溯到A)->C->F->H->G->D（没有路，最终回溯到A,A也没有未访问的相邻节点，本次搜索结束）.简要说明深度优先搜索的特点：每次深度优先搜索的结果必然是图的一个连通分量.深度优先搜索可以从多点发起.如果将每个节点在深度优先搜索过程中的"结束时间"排序（具体做法是创建一个list，然后在每个节点的相邻节点都已被访问的情况下，将该节点加入list结尾，然后逆转整个链表)，则我们可以得到所谓的"拓扑排序",即topological sort. 
![](https://img-blog.csdn.net/2018052721385611)
深度优先遍历图的方法是，从图中某顶点v出发：
（1）访问顶点v；
（2）依次从v的未被访问的邻接点出发，对图进行深度优先遍历；直至图中和v有路径相通的顶点都被访问；
（3）若此时图中尚有顶点未被访问，则从一个未被访问的顶点出发，重新进行深度优先遍历，直到图中所有顶点均被访问过为止。当然，当人们刚刚掌握深度优先搜索的时候常常用它来走迷宫.事实上我们还有别的方法，那就是广度优先搜索(BFS).
3.算法框架及说明
        所有的搜索算法从其最终的算法实现上来看，都可以划分成两个部分──控制结构和产生系统。正如前面所说的，搜索算法简而言之就是穷举所有可能情况并找到合适的答案，所以最基本的问题就是罗列出所有可能的情况，这其实就是一种产生式系统。我们将所要解答的问题划分成若干个阶段或者步骤，当一个阶段计算完毕，下面往往有多种可选选择，所有的选择共同组成了问题的解空间，对搜索算法而言，将所有的阶段或步骤画出来就类似是树的结构（如图）。从根开始计算，到找到位于某个节点的解，回溯法（深度优先搜索）作为最基本的搜索算法，其采用了一种“一只向下走，走不通就掉头”的思想（体会“回溯”二字），相当于采用了先根遍历的方法来构造搜索树。
4.深度优先算法代码实现
```cpp
bool visited[MaxVnum];
void DFS(Graph G,int v)
{
    visited[v]= true; //从V开始访问，flag它
    printf("%d",v);    //打印出V
    for(int j=0;j<G.vexnum;j++) 
        if(G.arcs[v][j]==1&&visited[j]== false) //这里可以获得V未访问过的邻接点
            DFS(G,j); //递归调用，如果所有节点都被访问过，就回溯，而不再调用这里的DFS
}
void DFSTraverse(Graph G) {
    for (int v = 0; v < G.vexnum; v++)
        visited[v] = false; //刚开始都没有被访问过
    for (int v = 0; v < G.vexnum; ++v)
        if (visited[v] == false) //从没有访问过的第一个元素来遍历图
            DFS(G, v);
}
```
5.深度优先算法的应用
(1).应用1
         基于深度优先遍历算法的应用实现.
```cpp
/*
基于深度优先遍历算法的应用。
假设图G采用邻接矩阵存储：
（1）判断图G中从顶点u到v是否存在简单路径；
（2）输出图G中从顶点u到v的一条简单路径（假设至少存在一条路径）；
（3）输出从图G中从顶点u到v的所有简单路径（假设至少存在一条路径）；
（4）输出从图G中从顶点u到v的长度为s的所有简单路径；
（5）求图中通过某顶点k的所有简单回路（若存在）。
*/
#include <iostream>
#include <stdio.h>
#include <malloc.h>
#define MAXV 50
using namespace std;
int visited[MAXV];
//邻接表类型
typedef struct ANode
{
    int adjvex;             //该边的编号
    struct ANode *nextarc;  //指向下一条边的信息
} ArcNode;                  //边节点的类型
 
typedef struct Vnode
{
    ArcNode *firstarc;      //指向第一条边
} VNode;                    //邻接表头节点类型
 
typedef VNode AdjList[MAXV];
typedef struct
{
    AdjList adjlist;       //邻接表
    int n,e;               //图中顶点数n和边数e
} ALGraph;                 //完整的图邻接表类型
 
void ArrayToList(int *Arr, int n, ALGraph *&G)  //用普通数组构造图的邻接表
{
    int i,j,count=0;  //count用于统计边数，即矩阵中非0元素个数
    ArcNode *p;
    G=(ALGraph *)malloc(sizeof(ALGraph));
    G->n=n;
    for (i=0; i<n; i++)                 //给邻接表中所有头节点的指针域置初值
        G->adjlist[i].firstarc=NULL;
    for (i=0; i<n; i++)                 //检查邻接矩阵中每个元素
        for (j=n-1; j>=0; j--)
            if (Arr[i*n+j]!=0)      //存在一条边，将Arr看作n×n的二维数组，Arr[i*n+j]即是Arr[i][j]
            {
                p=(ArcNode *)malloc(sizeof(ArcNode));   //创建一个节点*p
                p->adjvex=j;
                p->nextarc=G->adjlist[i].firstarc;      //采用头插法插入*p
                G->adjlist[i].firstarc=p;
            }
 
    G->e=count;
}
 
void DispAdj(ALGraph *G)   //输出邻接表G
{
    int i;
    ArcNode *p;
    for (i=0; i<G->n; i++)
    {
        p=G->adjlist[i].firstarc;
        printf("%3d: ",i);
        while (p!=NULL)
        {
            printf("-->%d ",p->adjvex);
            p=p->nextarc;
        }
        printf("\n");
    }
}
 
/*
判断图G中从顶点u到v是否存在简单路径:
在深度优先遍历的基础上增加has和v两个形参，其中has表示顶点u到v是否有路径，其初值为false，
当顶点u遍历到顶点v后，置has为true并返回。
*/
void ExistPath(ALGraph *G,int u,int v,bool &has)
{
    int w;
    ArcNode *p;
    visited[u]=1;              //置初始顶点为已访问标记
    if(u==v)                   //找到一条路径
    {
        has=true;
        return ;
    }
    p=G->adjlist[u].firstarc;  //p指向顶点u的第一个邻接点
    while(p!=NULL)
    {
        w=p->adjvex;           //w为u的相邻顶点
        if(visited[w]==0)
            ExistPath(G,w,v,has);
        p=p->nextarc;          //p指向顶点u的下一个相邻点
    }
}
 
/*
输出图G中从顶点u到v的一条简单路径:
在深度优先遍历的基础上增加v、path和d三个形参，其中path存放顶点u到v的路径，d表示path中的路径长度，初值为-1
当顶点u遍历到顶点v后，输出path并返回。
*/
void FindaPath(ALGraph *G,int u,int v,int path[],int d)
{
    int w,i;
    ArcNode *p;
    visited[u]=1;              //置初始顶点为已访问标记
    d++;
    path[d]=u;                 //路径长度d增1，顶点u加入到路径中
    if(u==v)                   //找到一条路径，输出并返回
    {
        cout<<"输出从u到v的一条简单路径：";
        for(i=0; i<=d; i++)
            cout<<path[i];
        cout<<endl;
        return;
    }
    p=G->adjlist[u].firstarc;  //p指向顶点u的第一个邻接点
    while(p!=NULL)
    {
        w=p->adjvex;           //w为u的相邻顶点
        if(visited[w]==0)
            FindaPath(G,w,v,path,d);
        p=p->nextarc;          //p指向顶点u的下一个相邻点
    }
}
 
/*
输出从图G中从顶点u到v的所有简单路径:
在深度优先遍历的基础上增加v、path和d三个形参，其中path存放顶点u到v的路径，d表示path中的路径长度，初值为-1
当从顶点u出发遍历时，先将visited[u]置为1，并将u加入到路径path中，若满足顶点u就是终点的v的条件时，
则找到了一个从顶点u到v的一条路径，则输出path并继续；再从顶点u找一个未访问过的相邻顶点w，若存在这样的顶点w，
则从w出发继续进行，若不存在这样的顶点w，则说明从顶点u再往下找找不到路径，所以置visited[u]为0，以便顶点u作为
其他路径上的顶点。
*/
void FindPath(ALGraph *G,int u,int v,int path[],int d)
{
    int w,i;
    ArcNode *p;
    d++;
    path[d]=u;                 //路径长度d增1，顶点u加入到路径中
    visited[u]=1;              //置初始顶点为已访问标记
    if(u==v&&d>=1)                   //找到一条路径则输出
    {
        for(i=0; i<=d; i++)
            cout<<path[i];
        cout<<endl;
    }
    p=G->adjlist[u].firstarc;  //p指向顶点u的第一个邻接点
    while(p!=NULL)
    {
        w=p->adjvex;           //w为u的相邻顶点
        if(visited[w]==0)
            FindPath(G,w,v,path,d);
        p=p->nextarc;          //p指向顶点u的下一个相邻点
    }
    visited[u]=0;              //恢复环境，使该顶点可重新使用
}
 
/*
输出从图G中从顶点u到v的长度为s的所有简单路径；
只需将路径输出条件改为u==v且d==s。
*/
void PathAll(ALGraph *G,int u,int v,int s,int path[],int d)
{
    int w,i;
    ArcNode *p;
    visited[u]=1;
    d++;
    path[d]=u;                 //路径长度d增1，顶点u加入到路径中
    if(u==v&&d==s)                   //找到一条路径则输出
    {
        for(i=0; i<=d; i++)
            cout<<path[i];
        cout<<endl;
    }
    p=G->adjlist[u].firstarc;  //p指向顶点u的第一个邻接点
    while(p!=NULL)
    {
        w=p->adjvex;           //w为u的相邻顶点
        if(visited[w]==0)
            PathAll(G,w,v,s,path,d);
        p=p->nextarc;          //p指向顶点u的下一个相邻点
    }
    visited[u]=0;              //恢复环境，使该顶点可重新使用
}
 
/*
求图中通过某顶点k的所有简单回路（若存在）：
利用深度优先搜索方法，从顶点u开始搜索与之相邻的顶点w，若w等于顶点v（其初值为u），且路径长度大于0，表示找到了一条回路，
输出path数组，然后继续搜索顶点u的未访问的相邻点查找其它通路。
*/
void DFSPath(ALGraph *G,int u,int v,int path[],int d)
{
    int w,i;
    ArcNode *p;
    visited[u]=1;
    d++;
    path[d]=u;                 //路径长度d增1，顶点u加入到路径中
    p=G->adjlist[u].firstarc;  //p指向顶点u的第一个邻接点
    while(p!=NULL)
    {
        w=p->adjvex;             //w为顶点u的相邻点
        if(w==v&&d>0)                   //找到一条路径则输出
        {
            for(i=0; i<=d; i++)
                cout<<path[i];
            cout<<v<<endl;
        }
        if(visited[w]==0)        //若w未访问，则递归访问之
            DFSPath(G,w,v,path,d);
        p=p->nextarc;          //p指向顶点u的下一个相邻点
    }
    visited[u]=0;              //恢复环境，使该顶点可重新使用
}
 
int main()
{
    int i,path[MAXV];
    bool f;
    ALGraph *G,*G1,*G2;
    int A[5][5]=
    {
        {0,1,1,0,0},
        {0,0,1,0,0},
        {0,0,0,1,1},
        {0,0,0,0,1},
        {1,0,0,0,0}
    };
    int B[5][5]=
    {
        {0,1,0,1,1},
        {1,0,1,1,0},
        {0,1,0,1,1},
        {1,1,1,0,1},
        {1,0,1,1,0}
    };
    int C[5][5]=
    {
        {0,1,1,0,0},
        {0,0,1,0,0},
        {0,0,0,1,1},
        {0,0,0,0,1},
        {1,0,0,0,0}
    };
    ArrayToList(A[0], 5, G);
    ArrayToList(B[0], 5, G1);
    ArrayToList(C[0], 5, G2);
    for (i=0; i<G->n; i++)
        visited[i]=0; //访问标志数组初始化
    printf("有向图G的邻接表:\n");
    DispAdj(G);
    cout<<endl;
    for (i=0; i<G->n; i++)
        visited[i]=0; //访问标志数组初始化
    ExistPath(G,1,4,f);
    cout<<"是否存在一条u到v的路径？";
    if(f)
        cout<<"存在"<<endl;
    else
        cout<<"不存在"<<endl;
    for (i=0; i<G->n; i++)
        visited[i]=0; //访问标志数组初始化
    cout<<endl;
    FindaPath(G,1,4,path,-1);
    cout<<endl;
    for (i=0; i<G1->n; i++)
        visited[i]=0; //访问标志数组初始化
    printf("无向图G1的邻接表:\n");
    DispAdj(G1);
    cout<<endl;
    for (i=0; i<G1->n; i++)
        visited[i]=0; //访问标志数组初始化
    printf("输出G1从1到4的所有简单路径:\n");
    FindPath(G1,1,4,path,-1);
    cout<<endl;
    for (i=0; i<G1->n; i++)
        visited[i]=0; //访问标志数组初始化
    cout<<"输出从图G中从顶点u到v的长度为s的所有简单路径：\n";
    PathAll(G1,1,4,3,path,-1);
    cout<<endl;
    for (i=0; i<G1->n; i++)
        visited[i]=0; //访问标志数组初始化
    printf("有向图G2的邻接表:\n");
    DispAdj(G2);
    cout<<endl;
    for (i=0; i<G1->n; i++)
        visited[i]=0; //访问标志数组初始化
    cout<<"经过顶点k的所有回路：\n";
    DFSPath(G2,0,0,path,-1);
    cout<<endl;
    return 0;
}
```
程序执行输出：
![](https://img-blog.csdn.net/20180925113608875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

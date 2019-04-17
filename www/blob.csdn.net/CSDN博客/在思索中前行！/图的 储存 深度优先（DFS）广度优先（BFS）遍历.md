# 图的 储存 深度优先（DFS）广度优先（BFS）遍历 - 在思索中前行！ - CSDN博客





2014年08月03日 22:30:58[_Tham](https://me.csdn.net/txl16211)阅读数：1447








图遍历的概念：    

从图中某顶点出发访遍图中每个顶点，且每个顶点仅访问一次，此过程称为**图的遍历**(Traversing
 Graph)。图的遍历算法是求解图的连通性问题、拓扑排序和求关键路径等算法的基础。**图的遍历顺序有两种**：深度优先搜索（DFS）和广度优先搜索（BFS）。对每种搜索顺序，访问各顶点的顺序也不是唯一的。



一、图的存储结构

1.1 邻接矩阵

    图的邻接矩阵存储方式是用两个数组来表示图。一个一维数组存储图中顶点信息，一个二维数组（邻接矩阵）存储图中的边或弧的信息。


    设图G有n个顶点，则邻接矩阵是一个n*n的方阵，定义为：


![](http://blog.chinaunix.net/attachment/201301/28/26548237_1359358455QE9d.png)


    看一个实例，下图左就是一个无向图。


![](http://blog.chinaunix.net/attachment/201301/28/26548237_1359358505GMa9.png)


    从上面可以看出，无向图的边数组是一个对称矩阵。所谓对称矩阵就是n阶矩阵的元满足aij =
 aji。即从矩阵的左上角到右下角的主对角线为轴，右上角的元和左下角相对应的元全都是相等的。


    从这个矩阵中，很容易知道图中的信息。


    （1）要判断任意两顶点是否有边无边就很容易了；


    （2）要知道某个顶点的度，其实就是这个顶点vi在邻接矩阵中第i行或（第i列）的元素之和；


    （3）求顶点vi的所有邻接点就是将矩阵中第i行元素扫描一遍，arc[i][j]为1就是邻接点；

    而有向图讲究入度和出度，顶点vi的入度为1，正好是第i列各数之和。顶点vi的出度为2，即第i行的各数之和。


    若图G是网图，有n个顶点，则邻接矩阵是一个n*n的方阵，定义为：


![](http://blog.chinaunix.net/attachment/201301/28/26548237_1359358929SkA2.png)


    这里的wij表示(vi,vj)上的权值。无穷大表示一个计算机允许的、大于所有边上权值的值，也就是一个不可能的极限值。下面左图就是一个有向网图，右图就是它的邻接矩阵。


![](http://blog.chinaunix.net/attachment/201301/28/26548237_1359359055yEhp.png)


    那么邻接矩阵是如何实现图的创建的呢？代码如下。



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

typedef char VertexType;                //顶点类型应由用户定义
typedef int   EdgeType;                   //边上的权值类型应由用户定义

#define  MAXVEX  100                        //最大顶点数，应由用户定义
#define  INFINITY    65535               //用65535来代表无穷大
#define  DEBUG

typedef struct
{
    VertexType vexs[MAXVEX];            //顶点表
    EdgeType     arc[MAXVEX][MAXVEX];         //邻接矩阵，可看作边
    int numVertexes, numEdges;      //图中当前的顶点数和边数
}Graph;

//定位
int locates(Graph *g, char ch)
{
    int i = 0;
    for(i = 0; i < g->numVertexes; i++)
    {
        if(g->vexs[i] == ch)
        {
            break;
        }
    }
    if(i >= g->numVertexes)
    {
        return -1;
    }

    return i;
}

//建立一个无向网图的邻接矩阵表示
void CreateGraph(Graph *g)
{
    int i, j, k, w;
    printf("输入顶点数和边数:\n");
    scanf("%d%d", &(g->numVertexes), &(g->numEdges));

    #ifdef DEBUG
    printf("%d %d\n", g->numVertexes, g->numEdges);
    #endif

    for(i = 0; i < g->numVertexes; i++)
    {
        g->vexs[i] = getchar();
        while(g->vexs[i] == '\n')
        {
            g->vexs[i] = getchar();
        }
    }

    #ifdef DEBUG
    for(i = 0; i < g->numVertexes; i++)
    {
        printf("%c ", g->vexs[i]);
    }
    printf("\n");
    #endif

    for(i = 0; i < g->numEdges; i++)
    {
        for(j = 0; j < g->numEdges; j++)
        {
            g->arc[i][j] = INFINITY;    //邻接矩阵初始化
        }
    }

    for(k = 0; k < g->numEdges; k++)
    {
        char p, q;
        printf("输入边(vi,vj)上的下标i，下标j和权值:\n");
       //这一段是输入图的顶点符号序号，并查找该顶点符号在顶点符号域中对应的序号
        p = getchar();
        while(p == '\n')
        {
            p = getchar();
        }
        q = getchar();
        while(q == '\n')
        {
            q = getchar();
        }
        scanf("%d", &w);

        int m = -1;
        int n = -1;
        m = locates(g, p);
        n = locates(g, q);
        if(n == -1 || m == -1)
        {
            fprintf(stderr,"there is no this vertex.\n");
            return;
        }
        //getchar();
        g->arc[m][n] = w;
        g->arc[n][m] = g->arc[m][n];  //因为是无向图，矩阵对称
    }
}

//打印图
void printGraph(Graph g)
{
    int i, j;
    for(i = 0; i < g.numVertexes; i++)
    {
        for(j = 0; j < g.numVertexes; j++)
        {
            printf("%d  ", g.arc[i][j]);
        }
        printf("\n");
    }
}

int main(int argc,char** argv)
{
    Graph g;
    //邻接矩阵创建图
    CreateGraph(&g);
    printGraph(g);
    return 0;
}
```



从代码中可以得到，n个顶点和e条边的无向网图的创建，时间复杂度为O(n + n2 +
 e)，其中对邻接矩阵Grc的初始化耗费了O(n2)的时间。




1.2 邻接表

    邻接矩阵是不错的一种图存储结构，但是，对于边数相对顶点较少的图，这种结构存在对存储空间的极大浪费。因此，找到一种数组与链表相结合的存储方法称为邻接表。


    邻接表的处理方法是这样的：


    （1）图中顶点用一个一维数组存储，当然，顶点也可以用单链表来存储，不过，数组可以较容易的读取顶点的信息，更加方便。


    （2）图中每个顶点vi的所有邻接点构成一个线性表，由于邻接点的个数不定，所以，用单链表存储，无向图称为顶点vi的边表，有向图则称为顶点vi作为弧尾的出边表。

    例如，下图就是一个无向图的邻接表的结构。


![](http://blog.chinaunix.net/attachment/201301/28/26548237_1359371746DDZF.png)


    从图中可以看出，顶点表的各个结点由data和firstedge两个域表示，data是数据域，存储顶点的信息，firstedge是指针域，指向边表的第一个结点，即此顶点的第一个邻接点。边表结点由adjvex和next两个域组成。adjvex是邻接点域，存储某顶点的邻接点在顶点表中的下标，next则存储指向边表中下一个结点的指针。


    对于带权值的网图，可以在边表结点定义中再增加一个weight的数据域，存储权值信息即可。如下图所示。


![](http://blog.chinaunix.net/attachment/201301/28/26548237_13593720165ssU.png)


    对于邻接表结构，图的建立代码如下。




```cpp
/* 邻接表表示的图结构 */
#include <stdio.h>
#include<stdlib.h>
 
#define DEBUG
#define MAXVEX 1000              //最大顶点数
typedef char VertexType;         //顶点类型应由用户定义
typedef int  EdgeType;           //边上的权值类型应由用户定义
 
typedef struct EdgeNode         //边表结点
{
    int adjvex;        //邻接点域，存储该顶点对应的下标
    EdgeType weigth;        //用于存储权值，对于非网图可以不需要
    struct EdgeNode *next;      //链域，指向下一个邻接点
}EdgeNode;
 
typedef struct VertexNode       //顶点表结构
{
    VertexType data;        //顶点域，存储顶点信息
    EdgeNode *firstedge;        //边表头指针
}VertexNode, AdjList[MAXVEX];
 
typedef struct
{
    AdjList adjList;
    intnumVertexes, numEdges;  //图中当前顶点数和边数
}GraphList;
 
int Locate(GraphList *g, char ch)
{
    int i;
    for(i = 0; i < MAXVEX; i++)
    {
        if(ch == g->adjList[i].data)
        {
            break;
        }
    }
    if(i >= MAXVEX)
    {
        fprintf(stderr,"there is no vertex.\n");
        return-1;
    }
    returni;
}
 
//建立图的邻接表结构
void CreateGraph(GraphList *g)
{
    int i, j, k;
    EdgeNode *e;
    EdgeNode *f;
    printf("输入顶点数和边数:\n");
    scanf("%d,%d", &g->numVertexes, &g->numEdges);
     
    #ifdef DEBUG
    printf("%d,%d\n", g->numVertexes, g->numEdges);
    #endif
     
    for(i = 0; i < g->numVertexes; i++)
    {
        printf("请输入顶点%d:\n", i);
        g->adjList[i].data = getchar();         //输入顶点信息
        g->adjList[i].firstedge = NULL;          //将边表置为空表
        while(g->adjList[i].data == '\n')
        {
            g->adjList[i].data = getchar();
        }
    }
    //建立边表
    for(k = 0; k < g->numEdges; k++)
    {
        printf("输入边(vi,vj)上的顶点序号:\n");
        char p, q;
        p = getchar();
        while(p == '\n')
        {
            p = getchar();
        }
        q = getchar();
        while(q == '\n')
        {
            q = getchar();
        }
        intm, n;
        m = Locate(g, p);
        n = Locate(g, q);
        if(m == -1 || n == -1)
        {
            return;
        }
        #ifdef DEBUG
        printf("p = %c\n", p);
        printf("q = %c\n", q);
        printf("m = %d\n", m);
        printf("n = %d\n", n);
        #endif
     
        //向内存申请空间，生成边表结点
        e = (EdgeNode *)malloc(sizeof(EdgeNode));
        if(e == NULL)
        {
            fprintf(stderr,"malloc() error.\n");
            return;
        }
        //邻接序号为j
        e->adjvex = n;
        //将e指针指向当前顶点指向的结构
        e->next = g->adjList[m].firstedge;
        //将当前顶点的指针指向e
        g->adjList[m].firstedge = e;
         
        f = (EdgeNode *)malloc(sizeof(EdgeNode));
        if(f == NULL)
        {
            fprintf(stderr,"malloc() error.\n");
            return;
        }
        f->adjvex = m;
        f->next = g->adjList[n].firstedge;
        g->adjList[n].firstedge = f;
    }
}
 
 
void printGraph(GraphList *g)
{
    int i = 0;
    #ifdef DEBUG
    printf("printGraph() start.\n");
    #endif
     
    while(g->adjList[i].firstedge != NULL && i < MAXVEX)
    {
        printf("顶点:%c  ", g->adjList[i].data);
        EdgeNode *e = NULL;
        e = g->adjList[i].firstedge;
        while(e != NULL)
        {
            printf("%d  ", e->adjvex);
            e = e->next;
        }
        i++;
        printf("\n");
    }
}
 
int main(int argc,char **argv)
{
    GraphList g;
    CreateGraph(&g);
    printGraph(&g);
    return 0;
}
```



     对于无向图，一条边对应都是两个顶点，所以，在循环中，一次就针对i和j分布进行插入。





    本算法的时间复杂度，对于n个顶点e条边来说，很容易得出是O(n+e)。

1.3 十字链表

    对于有向图来说，邻接表是有缺陷的。关心了出度问题，想了解入度就必须要遍历整个图才知道，反之，逆邻接表解决了入度却不了解出度情况。下面介绍的这种有向图的存储方法：十字链表，就是把邻接表和逆邻接表结合起来的。


    重新定义顶点表结点结构，如下所示。


![](http://blog.chinaunix.net/attachment/201301/28/26548237_1359379404626B.png)


    其中firstin表示入边表头指针，指向该顶点的入边表中第一个结点，firstout表示出边表头指针，指向该顶点的出边表中的第一个结点。


    重新定义边表结构，如下所示。


![](http://blog.chinaunix.net/attachment/201301/28/26548237_1359379502NBBu.png)


    其中，tailvex是指弧起点在顶点表的下表，headvex是指弧终点在顶点表的下标，headlink是指入边表指针域，指向终点相同的下一条边，taillink是指边表指针域，指向起点相同的下一条边。如果是网，还可以增加一个weight域来存储权值。


    比如下图，顶点依然是存入一个一维数组，实线箭头指针的图示完全与邻接表相同。就以顶点v0来说，firstout指向的是出边表中的第一个结点v3。所以，v0边表结点hearvex
 = 3，而tailvex其实就是当前顶点v0的下标0，由于v0只有一个出边顶点，所有headlink和taillink都是空的。


![](http://blog.chinaunix.net/attachment/201301/28/26548237_1359379826wexn.png)


  重点需要解释虚线箭头的含义。它其实就是此图的逆邻接表的表示。对于v0来说，它有两个顶点v1和v2的入边。因此的firstin指向顶点v1的边表结点中headvex为0的结点，如上图圆圈1。接着由入边结点的headlink指向下一个入边顶点v2，如上图圆圈2。对于顶点v1，它有一个入边顶点v2，所以它的firstin指向顶点v2的边表结点中headvex为1的结点，如上图圆圈3。

    十字链表的好处就是因为把邻接表和逆邻接表整合在一起，这样既容易找到以v为尾的弧，也容易找到以v为头的弧，因而比较容易求得顶点的出度和入度。


    而且除了结构复杂一点外，其实创建图算法的时间复杂度是和邻接表相同的，因此，在有向图应用中，十字链表是非常好的数据结构模型。

    这里就介绍以上三种存储结构，除了第三种存储结构外，其他的两种存储结构比较简单。




二、图的遍历

    图的遍历和树的遍历类似，希望从图中某一顶点出发访遍图中其余顶点，且使每一个顶点仅被访问一次，这一过程就叫图的遍历。


    对于图的遍历来说，如何避免因回路陷入死循环，就需要科学地设计遍历方案，通过有两种遍历次序方案：深度优先遍历和广度优先遍历。


2.1 深度优先遍历

    深度优先遍历，也有称为深度优先搜索，简称DFS。其实，就像是一棵树的前序遍历。


    它从图中某个结点v出发，访问此顶点，然后从v的未被访问的邻接点出发深度优先遍历图，直至图中所有和v有路径相通的顶点都被访问到。若图中尚有顶点未被访问，则另选图中一个未曾被访问的顶点作起始点，重复上述过程，直至图中的所有顶点都被访问到为止。


    我们用邻接矩阵的方式，则代码如下所示。




```cpp
#define MAXVEX  100     //最大顶点数
typedef int Boolean;            //Boolean 是布尔类型，其值是TRUE 或FALSE
Boolean visited[MAXVEX];        //访问标志数组
#define TRUE 1
#define FALSE 0
 
//邻接矩阵的深度优先递归算法
void DFS(Graph g, int i)
{
    int j;
    visited[i] = TRUE;
    printf("%c ", g.vexs[i]);                           //打印顶点，也可以其他操作
    for(j = 0; j < g.numVertexes; j++)
    {
        if(g.arc[i][j] == 1 && !visited[j])
        {
            DFS(g, j);                  //对为访问的邻接顶点递归调用
        }
    }
}
 
//邻接矩阵的深度遍历操作
void DFSTraverse(Graph g)
{
    inti;
    for(i = 0; i < g.numVertexes; i++)
    {
        visited[i] = FALSE;         //初始化所有顶点状态都是未访问过状态
    }
    for(i = 0; i < g.numVertexes; i++)
    {
        if(!visited[i])            //对未访问的顶点调用DFS，若是连通图，只会执行一次
        {
            DFS(g,i);
        }
    }
}

    如果使用的是邻接表存储结构，其DFSTraverse函数的代码几乎是相同的，只是在递归函数中因为将数组换成了链表而有不同，代码如下。

//邻接表的深度递归算法
void DFS(GraphList g, int i)
{
    EdgeNode *p;
    visited[i] = TRUE;
    printf("%c ", g->adjList[i].data);   //打印顶点，也可以其他操作
    p = g->adjList[i].firstedge;
    while(p)
    {
        if(!visited[p->adjvex])
        {
            DFS(g, p->adjvex);           //对访问的邻接顶点递归调用
        }
        p = p->next;
    }
}
 
//邻接表的深度遍历操作
void DFSTraverse(GraphList g)
{
    int i;
    for(i = 0; i < g.numVertexes; i++)
    {
        visited[i] = FALSE;
    }
    for(i = 0; i < g.numVertexes; i++)
    {
        if(!visited[i])
        {
            DFS(g, i);
        }
    }
}
```



    对比两个不同的存储结构的深度优先遍历算法，对于n个顶点e条边的图来说，邻接矩阵由于是二维数组，要查找某个顶点的邻接点需要访问矩阵中的所有元素，因为需要O(n2)的时间。而邻接表做存储结构时，找邻接点所需的时间取决于顶点和边的数量，所以是O(n+e)。显然对于点多边少的稀疏图来说，邻接表结构使得算法在时间效率上大大提高。



2.2 广度优先遍历

    广度优先遍历，又称为广度优先搜索，简称BFS。图的广度优先遍历就类似于树的层序遍历了。

    邻接矩阵做存储结构时，广度优先搜索的代码如下。





```cpp
//邻接矩阵的广度遍历算法
void BFSTraverse(Graph g)
{
    int i, j;
    Queue q;
    for(i = 0; i < g.numVertexes; i++)
    {
        visited[i] = FALSE;
    }
    InitQueue(&q);
    for(i = 0; i < g.numVertexes; i++)//对每个顶点做循环
    {
        if(!visited[i])              //若是未访问过
        {
            visited[i] = TRUE;
            printf("%c ", g.vexs[i]); //打印结点，也可以其他操作
            EnQueue(&q, i);           //将此结点入队列
            while(!QueueEmpty(q))    //将队中元素出队列，赋值给
            {
                intm;
                DeQueue(&q, &m);        
                for(j = 0; j < g.numVertexes; j++)
                {
                    //判断其他顶点若与当前顶点存在边且未访问过
                    if(g.arc[m][j] == 1 && !visited[j])
                    {
                        visited[j] = TRUE;
                        printf("%c ", g.vexs[j]);
                        EnQueue(&q, j);
                    }
                }
            }
        }
    }
}
```









    对于邻接表的广度优先遍历，代码与邻接矩阵差异不大， 代码如下。



```cpp
//邻接表的广度遍历算法
void BFSTraverse(GraphList g)
{
    int i;
    EdgeNode *p;
    Queue q;
    for(i = 0; i < g.numVertexes; i++)
    {
        visited[i] = FALSE;
    }
    InitQueue(&q);
    for(i = 0; i < g.numVertexes; i++)
    {
        if(!visited[i])
        {
            visited[i] = TRUE;
            printf("%c ", g.adjList[i].data);   //打印顶点，也可以其他操作
            EnQueue(&q, i);
            while(!QueueEmpty(q))
            {
                intm;
                DeQueue(&q, &m);
                p = g.adjList[m].firstedge;     找到当前顶点边表链表头指针
                while(p)
                {
                    if(!visited[p->adjvex])
                    {
                        visited[p->adjvex] = TRUE;
                        printf("%c ", g.adjList[p->adjvex].data);
                        EnQueue(&q, p->adjvex);
                    }
                    p = p->next;
                }
            }
        }
    }
}
```






      对比图的深度优先遍历与广度优先遍历算法，会发现，它们在时间复杂度上是一样的，不同之处仅仅在于对顶点的访问顺序不同。可见两者在全图遍历上是没有优劣之分的，只是不同的情况选择不同的算法。




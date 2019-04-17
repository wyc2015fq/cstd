# 【数据结构与算法】自己动手实现图的BFS和DFS（附完整源码） - 在思索中前行！ - CSDN博客





2015年04月10日 16:32:58[_Tham](https://me.csdn.net/txl16211)阅读数：1138








转载请注明出处：[http://blog.csdn.net/ns_code/article/details/19617187](http://blog.csdn.net/ns_code/article/details/19617187)

# 图的存储结构

    本文的重点在于图的深度优先搜索（DFS）和广度优先搜索（BFS），因此不再对图的基本概念做过多的介绍，但是要先大致了解下图的几种常见的存储结构。

##     邻接矩阵

    邻接矩阵既可以用来存储无向图，也可以用来存储有向图。该结构实际上就是用一个二维数组（邻接矩阵）来存储顶点的信息和顶点之间的关系（有向图的弧或无向图的边）。其描述形式如下：





**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- //图的邻接矩阵存储表示
- #define MAX_NUM 20 // 最大顶点个数
- enum GraphKind{GY,GN}; // {有向图,无向图}
- typedefstruct
- {  
-    VRType adj; // 顶点关系类型。对无权图，用1(是)或0(否)表示是否相邻；对带权图，则为权值
-    InfoType *info; // 与该弧或边相关信息的指针(可无)
- }ArcCell,AdjMatrix[MAX_NUM][MAX_NUM]; // 二维数组
- typedefstruct
- {  
-    VertexType vexs[MAX_NUM]; // 顶点向量
-    AdjMatrix arcs; // 邻接矩阵
- int vexnum,arcnum; // 图的当前顶点数和弧（边）数
-    GraphKind kind; // 图的种类标志
- }Graph;  
    我们分别看下面两个图，左边为有向图，右边为无向图





![](https://img-blog.csdn.net/20140221152519250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbnNfY29kZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140221152531000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbnNfY29kZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    上面两个图均为无权图，我们假设存储的时候，V0的序号为0，V1的序号为1，V2的序号为2。。。，且adj为1表示两顶点间没有没有连接，为0时表示有连接。则有向图的邻接矩阵如下图左边的矩阵所示，无向图的邻接矩阵如下图右边的矩阵所示；

![](https://img-blog.csdn.net/20140221155356343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbnNfY29kZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140221154253109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbnNfY29kZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    根据邻接矩阵很容易判断图中任意两个顶点之间连通与否，并可以求出各个顶点的度。

    1、对于无向图，观察右边的矩阵，发现顶点Vi的度即是邻接矩阵中第i行（或第i列）的元素之和。

    2、对于有向图，由于需要分别计算出度和入读，观察左边的矩阵，发现顶点Vi的出度即为邻接矩阵第i行元素之和，入度即为邻接矩阵第i列元素之和，因此顶点Vi的度即为邻接矩阵中第i行元素和第i列元素之和。

**很明显，邻接矩阵所占用的存储空间与图的边数或弧数无关，因此适用于边数或弧数较多的稠密图。**




##     邻接表
    邻接表是图的一种链式存储结构，既适合于存储无向图，也适合于存储有向图。在邻接表中，用一个一维数组存储图中的每个顶点的信息，同时为每个顶点建立一个单链表，链表中的节点保存依附在该顶点上的边或弧的信息。其描述形式如下：




**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- //图的邻接表存储表示
- #define MAX_NUM 20
- enum GraphKind{GY,GN}; // {有向图,无向图}
- typedefstruct
- {  
- int adjvex; // 该弧所指向的顶点或边的另一个顶点的位置
-    ArcNode *nextarc; // 指向下一条弧或边的指针
-    InfoType *info; // 与弧或边相关信息的指针（可无）
- }ArcNode;// 表结点
- typedefstruct
- {  
-    VertexType data; // 顶点信息
-    ArcNode *firstarc; // 第一个表结点的地址,指向第一条依附该顶点的弧或边的指针
- }VNode,AdjList[MAX_NUM]; // 头结点
- struct
- {  
-    AdjList vertices;  
- int vexnum,arcnum; // 图的当前顶点数和弧（边）数
-    GraphKind kind; // 图的种类标志
- }Graph;  

    依然以上面的有向图和无向图为例，采用邻接表来存储，可以得到对应的存储形式如下：

![](https://img-blog.csdn.net/20140221164858453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbnNfY29kZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    在邻接表上容易找到任意一个顶点的第一个邻接点和下一个邻接点，但是要判定任意两个顶点之间是否有边或弧需搜索两个顶点对应的链表，不及邻接矩阵方便。

**很明显，邻接表所占用的存储空间与图的边数或弧数有关，因此适用于边数或弧数较少的稀疏图。**



##     十字链表
    十字链表也是一种链式存储结构，用来表示有向图，它在有向图邻接表的基础上加入了指向弧尾的指针。表示形式如下：




**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- //有向图的十字链表存储表示
- #define MAX_NUM 20
- typedefstruct// 弧结点
- {  
- int tailvex,headvex; // 该弧的尾和头顶点的位置
-   ArcBox *hlink,*tlink; // 分别为弧头相同和弧尾相同的弧的链域
-   InfoType *info; // 该弧相关信息的指针，可指向权值或其他信息（可无）
- }ArcBox;  
- typedefstruct// 顶点结点
- {  
-   VertexType data;  
-   ArcBox *firstin,*firstout; // 分别指向该顶点第一条入弧和出弧
- }VexNode;  
- struct
- {  
-   VexNode xlist[MAX_NUM]; // 表头向量(数组)
- int vexnum,arcnum; // 有向图的当前顶点数和弧数
- }Graph;      

    其思想也很容易理解，这里不再细说。

    在十字链表中，既容易找到以某个顶点为尾的弧，也容易找到以某个顶点为头的弧，因而容易求得顶点的出度和入度。



##     邻接多重表
    邻接多重表也是一种链式存储结构，用来表示无向图，与有向图的十字链表相似，这里也不再细说，直接给出其表示形式：




**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- //无向图的邻接多重表存储表示
-  #define MAX_NUM 20
- typedefstruct
-  {  
-    VisitIf mark; // 访问标记
- int ivex,jvex; // 该边依附的两个顶点的位置
-    EBox *ilink,*jlink; // 分别指向依附这两个顶点的下一条边
-    InfoType *info; // 该边信息指针，可指向权值或其他信息
-  }EBox;  
- typedefstruct
-  {  
-    VertexType data;  
-    EBox *firstedge; // 指向第一条依附该顶点的边
-  }VexBox;  
- typedefstruct
-  {  
-    VexBox adjmulist[MAX_NUM];  
- int vexnum,edgenum; // 无向图的当前顶点数和边数
-  }Graph;  


# 图的遍历

    图的遍历比树的遍历要复杂的多，因为图的任一顶点都有可能与其他的顶点相邻接，因此，我们需要设置一个辅助数组visited[]来标记某个节点是否已经被访问过。图的遍历通常有两种方法：深度优先遍历（BFS）和广度优先遍历（DFS），两种遍历方式的思想都不难理解。下面我们就以如下图所示的例子来说明图的这两种遍历方式的基本思想，并用邻接表作为图的存储结构，给出BFS和DFS的实现代码（下图为无向图，有向图的BFS和DFS实现代码与无向图的相同）：

![](https://img-blog.csdn.net/20140221184754062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbnNfY29kZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    我们以邻接表作为上图的存储结构，并将A、B、C、D、E、F、G、H在顶点数组中的序号分别设为0、1、2、3、4、5、6、7。我们根据上图所包含的信息，精简了邻接表的存储结构，采取如下所示的结构来存储图中顶点和边的相关信息：





**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #define NUM 8          //图中顶点的个数
- 
- /*
- 用邻接表作为图的存储结构
- 在邻接表中，用一个一维数组存储图中的每个顶点的信息，
- 同时为每个顶点建立一个单链表，链表中的节点保存依附在该顶点上的边或弧的信息
- */
- typedefstruct node  
- {   //链表中的每个节点，保存依附在该节点上的边或弧的信息
- int vertex;          //在有向图中表示该弧所指向的顶点（即弧头）的位置，
- //在无向图中表示依附在该边上的另一个顶点的位置
- struct node *pNext;  //指向下一条依附在该顶点上的弧或边
- }Node;  
- typedefstruct head  
- {   //数组中的每个元素，保存图中每个顶点的相关信息
- char data;          //顶点的数据域
-     Node *first;        //在有向图中，指向以该顶点为弧尾的第一条弧
- //在无向图中，指向依附在该顶点上的第一条边
- }Head,*Graph;           //动态分配数组保存每个顶点的相关信息



    那么用邻接表来表示上图中各顶点间的关系，如下图所示：

![](https://img-blog.csdn.net/20140221185624437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbnNfY29kZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



##     深度优先搜索
    深度优先搜索（DFS）类似于树的先序遍历（如尚未掌握图的先序遍历，请移步这里：[**http://blog.csdn.net/ns_code/article/details/12977901**](http://blog.csdn.net/ns_code/article/details/12977901)）。其基本思想是：从图中某个顶点v出发，遍历该顶点，而后依次从v的未被访问的邻接点出发深度优先遍历图中，直到图中所有与v连通的顶点都已被遍历。如果当前图只是需要遍历的非连通图的一个极大连通子图，则另选其他子图中的一个顶点，重复上述遍历过程，直到该非连通图中的所有顶点都被遍历完。很明显，这里要用到递归的思想。


    结合上面的例子来分析，假设从A点开始遍历该图，根据图中顶点的存储关系，会按照下面的步骤来遍历该图：

    1、在访问完顶点A后，选择A的第一个邻接点B进行访问；

    2、而后看B的邻接点，由于B的第一个邻接点A已经被访问过，故选择其下一个邻接点D进行访问；

    3、而后看D的邻接点，由于D的第一个邻接点B已经被访问过，故选择其下一个邻接点H进行访问；

    4、而后看H的邻接点，由于H的第一个邻接点D已经被访问过，故选择其下一个邻接点E进行访问；

    5、而后看E的邻接点，由于E的第一个邻接点B已经被访问过，那么看其第二个邻接点H，也被访问过了，E的邻接点已经全部被访问过了。

    6、这时退回到上一层递归，回到顶点H，同样H的邻接点也都被访问完毕，再退回到D，D的邻接点也已访问完毕，再退回到B，一直退回到A；

    7、由于A的下一个邻接点C还没有被访问，因此访问C；

    8、而后看C的邻接点，由于C的第一个邻接点A已经被访问过，故选择其下一个邻接点F进行访问；

    9、而后看F的邻接点，由于F的第一个邻接点C已经被访问过，故选择其下一个邻接点G进行访问；

    10、而后看G的邻接点，由于G的临界点都已被访问完毕，因此会退到上一层递归，看F顶点；

    11、同理，由F再回退到C，再由C回退到A，这是第一层的递归，A的所有邻接点也已都被访问完毕，遍历到此结束。

    综上，上图的DFS是按照如下顺序进行的：

**A->B->D->H->E->H->D->B->A->C->F->G->F->C->A**

    其中，红色部分代表首次访问，在这部分顶点被访问后，我们便将visited数组的对应元素设为true，表明这些顶点已经被访问过了，因此我们可以得到深度优先搜索得到的顺序为：



**A->B->D->H->E->C->F->G**

    深度优先搜索的代码实现如下：





**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- /*
- 从序号为begin的顶点出发，递归深度优先遍历连通图Gp
- */
- void DFS(Graph Gp, int begin)  
- {  
- //遍历输出序号为begin的顶点的数据域，并保存遍历信息
-     printf("%c ",Gp[begin].data);  
-     visited[begin] = true;   
- 
- //循环访问当前节点的所有邻接点（即该节点对应的链表）
- int i;  
- for(i=first_vertex(Gp,begin); i>=0; i=next_vertex(Gp,begin,i))  
-     {  
- if(!visited[i])  //对于尚未遍历的邻接节点，递归调用DFS
-             DFS(Gp,i);  
-     }  
- }   
- 
- /*
- 从序号为begin的节点开始深度优先遍历图Gp，Gp可以是连通图也可以是非连通图
- */
- void DFS_traverse(Graph Gp,int begin)  
- {  
- int i;  
- for(i=0;i<NUM;i++)    //初始化遍历标志数组
-         visited[i] = false;   
- 
- //先从序号为begin的顶点开始遍历对应的连通图
-     DFS(Gp,begin);  
- //如果是非连通图，该循环保证每个极大连通子图中的顶点都能被遍历到   
- for(i=0;i<NUM;i++)  
-     {         
- if(!visited[i])  
-             DFS(Gp,i);  
-     }  
- }  
    这里调用了first_vertex（）和next_vertex（）两个函数，根据如上图手绘的邻接表的存储结构，这两个函数代码实现及详细注释如下：






**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- /*
- 返回图Gp中pos顶点（序号为pos的顶点）的第一个邻接顶点的序号，如果不存在，则返回-1
- */
- int first_vertex(Graph Gp,int pos)  
- {  
- if(Gp[pos].first)  //如果存在邻接顶点，返回第一个邻接顶点的序号
- return  Gp[pos].first->vertex;  
- else//如果不存在，则返回-1
- return -1;  
- }  
- 
- /*
- cur顶点是pos顶点（cur和pos均为顶点的序号）的其中一个邻接顶点，
- 返回图Gp中，pos顶点的（相对于cur顶点）下一个邻接顶点的序号,如果不存在，则返回-1
- */
- int next_vertex(Graph Gp,int pos,int cur)  
- {  
-     Node *p = Gp[pos].first; //p初始指向顶点的第一个邻接点
- //循环pos节点对应的链表，直到p指向序号为cur的邻接点
- while(p->vertex != cur)  
-         p = p->pNext;  
- 
- //返回下一个节点的序号
- if(p->pNext)  
- return p->pNext->vertex;   
- else
- return -1;  
- }  

##     广度优先搜索



    广度优先搜索（BFS）类似于树的层序遍历（如尚未掌握图的先序遍历，请移步这里：**[http://blog.csdn.net/ns_code/article/details/13169703](http://blog.csdn.net/ns_code/article/details/13169703)**）。其基本思想是：从头图中某个顶点v出发，访问v之后，依次访问v的各个未被访问的邻接点，而后再分别从这些邻接点出发，依次访问它们的邻接点，直到图中所有与v连通的顶点都已被访问。如果当前图只是需要遍历的非连通图的一个极大连通子图，则另选其他子图中的一个顶点，重复上述遍历过程，直到该非连通图中的所有顶点都被遍历完。很明显，跟树的层序遍历一样，图的广度优先搜索要用到队列来辅助实现。

    同样以上面的例子来分析，假设从A点开始遍历该图，根据图中顶点的存储关系，会按照下面的步骤来遍历该图：

    1、在访问完顶点A后，首先访问A的第一个邻接点B，而后访问A的第二个邻接点C；

    2、再根据顺序访问B的未被访问的邻接点，先访问D，后访问E；

    3、再根据顺序访问C的未被访问的邻接点，先访问F，在访问G；

    4、而后一次访问D、E、F、G等顶点的未被访问的邻接点；

    5、D的邻接点中，只有H未被访问，因地访问H；

    6、E、F、G、H等都没有未被访问的邻接点了，遍历到此结束。
    综上，我们可以得到广度优先搜索得到的顺序为：


**A->B->C->D->E->F->G->H**



    深度优先搜索的代码实现如下：





**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- /*
- 从序号为begin的顶点开始，广度优先遍历图Gp，Gp可以是连通图也可以是非连通图
- */
- void BFS_traverse(Graph Gp,int begin)  
- {  
- int i;  
- for(i=0;i<NUM;i++)    //初始化遍历标志数组
-         visited[i] = false;  
- 
- //先从序号为begin的顶点开始遍历对应的连通图
-     BFS(Gp,begin);  
- //如果是非连通图，该循环保证每个极大连通子图中的顶点都能被遍历到   
- for(i=0;i<NUM;i++)  
-     {   if(!visited[i])  
-             BFS(Gp,i);        
-     }  
- }  
- 
- /*
- 从序号为begin的顶点开始，广度优先遍历连通图Gp
- */
- void BFS(Graph Gp,int begin)  
- {  
- //遍历输出序号为begin的顶点的数据域，并保存遍历信息
-     printf("%c ",Gp[begin].data);  
-     visited[begin] = true;   
- 
- int i;  
- int pVertex;  //用来保存从队列中出队的顶点的序号
-     PQUEUE queue = create_queue();  //创建一个空的辅助队列
-     en_queue(queue, begin);          //首先将序号为begin的顶点入队
- while(!is_empty(queue))  
-     {  
-         de_queue(queue,&pVertex);  
- //循环遍历，访问完pVertex顶点的所有邻接顶点,并将访问后的邻接顶点入队
- for(i=first_vertex(Gp,pVertex); i>=0; i=next_vertex(Gp,pVertex,i))  
-         {  
- if(!visited[i])  
-             {  
-                 printf("%c ",Gp[i].data);  
-                 visited[i] = true;  
-                 en_queue(queue,i);  
-             }  
-         }  
-     }  
- //销毁队列，释放其对应的内存
-     destroy_queue(queue);  
- }  




# 遍历结果


    按照上面的例子来构建图，而后采用如下代码测试DFS和BFS的输出结果：





**[cpp]**[view
 plain](http://blog.csdn.net/ns_code/article/details/19617187#)[copy](http://blog.csdn.net/ns_code/article/details/19617187#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- /**********************************
- 图的BFS和DFS
- Author:兰亭风雨 Date:2014-02-20
- Email:zyb_maodun@163.com
- **********************************/
- #include<stdio.h>
- #include<stdlib.h>
- #include "data_structure.h"
- 
- int main()  
- {  
-     Graph Gp = create_graph();  
- 
- //深度优先遍历
-     printf("对图进行深度优先遍历：\n");  
-     printf("从顶点A出发DFS的结果：");  
-     DFS_traverse(Gp,0);  
-     printf("\n");  
-     printf("从顶点H出发DFS的结果：");  
-     DFS_traverse(Gp,7);  
-     printf("\n");  
-     printf("从顶点E出发DFS的结果：");  
-     DFS_traverse(Gp,4);  
-     printf("\n");  
-     printf("\n");  
- 
- //广度优先遍历
-     printf("对图进行深度优先遍历：\n");  
-     printf("从顶点A出发BFS的结果：");  
-     BFS_traverse(Gp,0);  
-     printf("\n");  
-     printf("从顶点H出发BFS的结果：");  
-     BFS_traverse(Gp,7);  
-     printf("\n");  
-     printf("从顶点E出发BFS的结果：");  
-     BFS_traverse(Gp,4);  
-     printf("\n");  
- 
- int i;  
- //释放掉为每个单链表所分配的内存
- for(i=0;i<NUM;i++)  
-     {  
-         free(Gp[i].first);  
-         Gp[i].first = 0;  //防止悬垂指针的产生
-     }  
- 
- //释放掉为顶点数组所分配的内存
-     free(Gp);  
-     Gp = 0;  
- return 0;  
- }  
    测试得到的输出结果如下：


![](https://img-blog.csdn.net/20140221201406000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbnNfY29kZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 完整代码下载

**    完整的C语言实现代码下载地址：[http://download.csdn.net/detail/mmc_maodun/6946859](http://download.csdn.net/detail/mmc_maodun/6946859)**









































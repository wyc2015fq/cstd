# 二之三续、Dijkstra 算法+Heap堆的完整c实现源码 - 结构之法 算法之道 - CSDN博客





2011年03月24日 18:22:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：31706
所属专栏：[经典算法研究](https://blog.csdn.net/column/details/Dijkstra.html)









**二之三续、Dijkstra 算法+Heap堆的完整c实现源码**


作者:JULY、二零一一年三月十八日
出处：[**http://blog.csdn.net/v_JULY_v**](http://blog.csdn.net/v_JULY_v)。
------------------------------------------

**引言：**    此文的写作目的很简单，就一个理由，个人认为：上一篇文章，[**二之再续、Dijkstra 算法+fibonacci堆的逐步c实现**](http://blog.csdn.net/v_JULY_v/archive/2011/03/18/6259576.aspx)，写的不够好，特此再写Dijkstra 算法的一个续集，谓之二之三续。
    鉴于读者理解斐波那契堆的难度，本文，以简单的最小堆为示例。同时，本程序也有参考网友的实现。有任何问题，欢迎指正。


**Dijkstra 算法+Heap堆完整算法思想**    在前一篇文章中，我们已经了解到，Dijkstra 算法如下：

DIJKSTRA(G, w, s)
1  INITIALIZE-SINGLE-SOURCE(G, s)  //1、初始化结点工作
2  S ← Ø
3  Q ← V[G]   //2、初始化队列
4  while Q ≠ Ø
5      do u ← EXTRACT-MIN(Q) //3、从最小队列中，抽取最小结点(在此之前，先建立最小堆)
6         S ← S ∪{u}
7         for each vertex v ∈ Adj[u]
8             do RELAX(u, v, w)  //4、松弛操作。

    如此，咱们不再赘述，直接即可轻松编写如下c/c++源码：

void dijkstra(ALGraph G,int s,int d[],int pi[],int Q[])
{ //Q[]是最小优先队列，Q[1..n]中存放的是图顶点标号,Q[0]中存放堆的大小
 //优先队列中有key的概念，这里key可以从d[]中取得。比如说，Q[2]的大小(key)为 d[ Q[2] ]

 initSingleSource(G,s,d,pi);  //1、初始化结点工作

 //2、初始化队列
 Q[0] = G.vexnum;
 for(int i=1;i<=Q[0];i++)

 {
  Q[i] = i-1;
 }
 Q[1] = s;
 Q[s+1] = 0;

 int u;
 int v;
 while(Q[0]!=0)

 {
  buildMinHeap(Q,d);     //3.1、建立最小堆
  u = extractMin(Q,d);   //3.2、从最小队列中，抽取最小结点
  ArcNode* arcNodePt = G.vertices[u].firstarc;
  while(arcNodePt!=NULL)
 {
   v = arcNodePt->adjvex;
   relax(u,v,G,d,pi);    //4、松弛操作。
   arcNodePt = arcNodePt->nextarc;
  }
 }

}

    ok，接下来，咱们一步一步编写代码来实现此Dijkstra 算法，**先给出第1、初始化结点工作，和4、松弛操作俩个操作的源码：**

void initSingleSource(ALGraph G,int s,int d[],int pi[]) 
{       //1、初始化结点工作
 for(int i=0;i<G.vexnum;i++)

 {
  d[i] = INFINITY;
  pi[i] = NIL;
 }
 d[s] = 0;
}

void relax(int u,int v,ALGraph G,int d[],int pi[])
{ //4、松弛操作。
        //u是新加入集合S的顶点的标号
 if(d[v]>d[u]+getEdgeWeight(G,u,v))

 {
  d[v] = d[u] + getEdgeWeight(G,u,v);
  pi[v] = u;
 }
}

    ok，接下来，咱们具体阐述第3个操作，3、从最小队列中，抽取最小结点(在此之前，先建立最小堆)。


**Heap最小堆的建立与抽取最小结点**    在我的这篇文章[**二、堆排序算法**](http://blog.csdn.net/v_JULY_v/archive/2011/02/21/6198644.aspx)里头，对最大堆的建立有所阐述：
**2.3.1、建堆(O（N）)**

BUILD-MAX-HEAP(A)
1  heap-size[A] ← length[A]
2  for i ← |_length[A]/2_| downto 1
3       do MAX-HEAPIFY(A, i)    
//建堆，怎么建列?原来就是不断的调用MAX-HEAPIFY(A, i)来建立最大堆。

    建最小堆，也是一回事，把上述代码改俩处即可，一，MAX->MIN，二，MAX-HEAPIFY(A, i)->MIN-HEAPIFY(A, i)。如此说来，是不是很简单列，是的，本身就很简单。

**先是建立最小堆的工作：**

void buildMinHeap(int Q[],int d[]) //建立最小堆
{
 for(int i=Q[0]/2;i>=1;i--)
 {
  minHeapify(Q,d,i); //调用minHeapify，以保持堆的性质。
 }
}

    然后，**得编写minHeapify代码，来保持最小堆的性质：**

void minHeapify(int Q[],int d[],int i)
{ //smallest,l,r,i都是优先队列元素的下标，范围是从1 ~ heap-size[Q]
 int l = 2*i;
 int r = 2*i+1;
 int smallest;
 if(l<=Q[0] && d[ Q[l] ] < d[ Q[i] ])

 {
  smallest = l;
 } 
 else
 {
  smallest = i;
 }
 if(r<=Q[0] && d[ Q[r] ] < d[ Q[smallest] ])

 {
  smallest = r;
 }
 if(smallest!=i)
 {
  int temp = Q[i];
  Q[i] = Q[smallest];
  Q[smallest] = temp;  

  minHeapify(Q,d,smallest);
 }
}

你自个比较一下建立最小堆，与建立最大堆的代码，立马看见，如出一辙，不过是改几个字母而已：

MAX-HEAPIFY(A, i)   //建立最大堆的代码
 1 l ← LEFT(i)
 2 r ← RIGHT(i)
 3 if l ≤ heap-size[A] and A[l] > A[i]
 4    then largest ← l
 5    else largest ← i
 6 if r ≤ heap-size[A] and A[r] > A[largest]
 7    then largest ← r
 8 if largest ≠ i
 9    then exchange A[i] <-> A[largest]
10         MAX-HEAPIFY(A, largest) 

    ok，最后，**便是3、从最小队列中，抽取最小结点的工作了**，如下：

int extractMin(int Q[],int d[])   //3、从最小队列中，抽取最小结点
{ //摘取优先队列中最小元素的内容，这里返回图中顶点的标号(0 ~ G.vexnum-1)，
 //这些标号是保存在Q[1..n]中的
 if(Q[0]<1)
 {
  cout<<"heap underflow!"<<endl;
  return -10000;
 }
 int min = Q[1];
 Q[1] = Q[Q[0]];
 Q[0] = Q[0] - 1;
 minHeapify(Q,d,1);
 return min;
}


**ALGraph图的建立**    先定义几个宏，

#define MAX_VERTEX_NUM 20 //图中最大的节点数目
#define INFINITY 10000
#define NIL -1

    再建立几个数据结构：

typedef struct ArcNode  //弧节点，就是邻接链表的表节点
{  
 int adjvex;    //该弧所指向尾节点的位置，其实保存的就是数组的下标
 ArcNode *nextarc;  //指向下一条弧的指针
 int weight;        //权重。
}ArcNode;

typedef struct VNode
{
 ArcNode* firstarc;
}VNode,AdjList[MAX_VERTEX_NUM];

typedef struct
{
 AdjList vertices;
 int vexnum,arcnum;
}ALGraph;

    编写几个功能函数：

void initALGraph(ALGraph* GPt,int vn)   //初始化结点
{
 GPt->arcnum = 0;
 GPt->vexnum = vn;
 for(int i=0;i<vn;i++)

 {
  GPt->vertices[i].firstarc = NULL;
 }
}

void insertArc(ALGraph* GPt,int vhead,int vtail,int w) //增加结点操作
{
 ArcNode* arcNodePt = new ArcNode;
 arcNodePt->nextarc = NULL;
 arcNodePt->adjvex = vtail;
 arcNodePt->weight = w;

 ArcNode* tailPt = GPt->vertices[vhead].firstarc;
 if(tailPt==NULL)
 {
  GPt->vertices[vhead].firstarc = arcNodePt;
 }
 else
 {
  while(tailPt->nextarc!=NULL)
  {
   tailPt = tailPt->nextarc;
  }
  tailPt->nextarc = arcNodePt;
 }
 GPt->arcnum ++;
}

void displayGraph(ALGraph G)  //打印结点
{
 ArcNode* arcNodePt;
 for(int i=0;i<G.vexnum;i++)
 {
  arcNodePt = G.vertices[i].firstarc;
  cout<<"vertex"<<i<<": ";
  while(arcNodePt!=NULL)
  {
   cout<<arcNodePt->adjvex<<"("<<"weight"<<arcNodePt->weight<<")"<<" ";
   arcNodePt = arcNodePt->nextarc;
  }
  cout<<endl;
 }
}

int getEdgeWeight(ALGraph G,int vhead,int vtail)  //求边的权重
{
 ArcNode* arcNodePt = G.vertices[vhead].firstarc;
 while(arcNodePt!=NULL)
 {
  if(arcNodePt->adjvex==vtail)
  {
   return arcNodePt->weight;
  }
  arcNodePt = arcNodePt->nextarc;
 }
 return INFINITY;
}


**主函数测试用例**    最后，便是编写主函数测试本程序：

int main(){

 ALGraph G;
 ALGraph* GPt = &G;
 initALGraph(GPt,5);

 insertArc(GPt,0,1,10);
 insertArc(GPt,0,3,5);
 insertArc(GPt,1,2,1);
 insertArc(GPt,1,3,2);
 insertArc(GPt,2,4,4);
 insertArc(GPt,3,1,3);
 insertArc(GPt,3,2,9);
 insertArc(GPt,3,4,2);
 insertArc(GPt,4,2,4);
 insertArc(GPt,4,0,7);

 cout<<"显示出此构造的图:"<<endl;
 displayGraph(G);
 cout<<endl;

 int d[MAX_VERTEX_NUM];
 int pi[MAX_VERTEX_NUM];
 int Q[MAX_VERTEX_NUM+1]; 
        //Q[]的第一个元素只保存堆的大小，不保存元素。所以定义长度时+1

 dijkstra(G,0,d,pi,Q);

 for(int i=0;i<G.vexnum;i++)
        {
  cout<<"从源点0到点"<<i<<"的最短路径信息:"<<endl;
  cout<<"长度为"<<d[i]<<endl;
  cout<<"路径为";
  printRoute(i,pi);
  cout<<endl;
  if(i==G.vexnum-1)
                {
   cout<<endl;
  }
 }
 return 0; 
}

最后的运行结果，如下所示： 

![](http://hi.csdn.net/attachment/201103/24/8394323_1300961975JjjA.jpg)

全文，到此完。



**版权所有。转载本BLOG内任何文章，请以超链接形式注明出处。否则，一经发现，必定永久谴责+追究法律责任。谢谢，各位。**




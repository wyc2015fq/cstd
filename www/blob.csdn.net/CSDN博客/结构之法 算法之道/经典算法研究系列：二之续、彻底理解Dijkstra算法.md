# 经典算法研究系列：二之续、彻底理解Dijkstra算法 - 结构之法 算法之道 - CSDN博客





2011年02月13日 15:55:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：49675标签：[算法																[insert																[each																[list																[exchange																[delete](https://so.csdn.net/so/search/s.do?q=delete&t=blog)
个人分类：[01.Algorithms（研究）																[02.Algorithms（后续）](https://blog.csdn.net/v_JULY_v/article/category/772685)](https://blog.csdn.net/v_JULY_v/article/category/769275)

所属专栏：[经典算法研究](https://blog.csdn.net/column/details/Dijkstra.html)





**经典算法研究系列：二之续、彻底理解Dijkstra算法**

作者：July   二零一一年二月十三日。
参考代码：introduction to algorithms，Second Edition。
---------------------------------------

了解什么是Dijkstra 算法，请参考：
经典算法研究系列：二、Dijkstra 算法初探
[http://blog.csdn.net/v_JULY_v/archive/2010/12/24/6096981.aspx](http://blog.csdn.net/v_JULY_v/archive/2010/12/24/6096981.aspx)

本文由单源最短路径路径问题开始，而后描述Bellman-Ford算法，到具体阐述Dijkstra算法，
阐述详细剖析Dijkstra算法的每一个步骤，教你彻底理解此Dijkstra算法。 

**一、单源最短路径问题**我们知道，单源最短路径问题：已知图G=(V，E)，要求找出从某个定源顶点s<-V，到每个v<-V的最短路径。
简单来说，就是一个图G中，找到一个定点s，然后以s为起点，要求找出s到图G中其余各个点的最短距离或路径。

此单源最短路径问题有以下几个变形：
I、  单终点最短路径问题：  
每个顶点v到指定终点t的最短路径问题。即单源最短路径问题的相对问题。
II、 单对顶点最短路径问题：
给定顶点u和v，找出从u到v的一条最短路径。

III、每对顶点间最短路径问题：
针对任意每俩个顶点u和v，找出从u到v的最短路径。
最简单的想法是，将每个顶点作为源点，运行一次单源算法即可以解决这个问题。
当然，还有更好的办法，日后在本BOlG内阐述。 



**二、Bellman-Ford 算法**1、回路问题
一条最短路径不能包含负权回路，也不能包含正权回路。
一些最短路径的算法，如Dijkstra 算法，要求图中所有的边的权值都是非负的，如在公路地图上，找一条从定点s到目的顶点v的最短路径问题。

2、Bellman-Ford 算法
而Bellman-Ford 算法，则允许输入图中存在负权边，只要不存在从源点可达的负权回路，即可。
简单的说，图中可以存在负权边，但此条负权边，构不成负权回路，不影响回路的形成。
且，Bellman-Ford 算法本身，便是可判断图中是否存在从源点可达的负权回路，
若存在负权回路，算法返回FALSE,若不存在，返回TRUE。

Bellman-Ford 算法的具体描述
BELLMAN-FORD(G, w, s)
1  INITIALIZE-SINGLE-SOURCE(G, s)   //对每个顶点初始化 ，O(V) 
2  for i ← 1 to |V[G]| - 1
3       do for each edge (u, v) ∈ E[G]
4              do RELAX(u, v, w)    //针对每个顶点(V-1个)，都运用松弛技术O(E)，计为O（(v-1)*E)）
5  for each edge (u, v) ∈ E[G]
6       do if d[v] > d[u] + w(u, v)
7             then return FALSE     //检测图中每条边，判断是否包含负权回路，
//若d[v]>d[u]+w(u，v)，则表示包含，返回FALSE，
8  return TRUE                      //不包含负权回路，返回TRUE

Bellman-Ford 算法的时间复杂度，由上可得为O（V*E）。

3、关于判断图中是否包含负权回路的问题：
根据定理，我们假定，**u**是v的**父辈**，或父母，那么
当G(V,E)是一个有向图或无向图(且不包含任何负权回路)，s<-V，s为G的任意一个顶点，则对任意边(u，v)<-V，有
**d[s，v] <= d[s，u]+1**此定理的详细证明，可参考算法导论一书上，第22章中引理22.1的证明。
或者根据第24章中通过三角不等式论证Bellman-Ford算法的正确性，也可得出上述定理的变形。

即假设图G中不包含负权回路，可证得
   d[v]=$(s，v)
      <=$(s，u)+w(u，v)  //根据三角不等式
      =d[u]+w[u，v]
**所以，在不包含负权回路的图中，是可以得出d[v]<=d[u]+w(u，v)。**

于是，就不难理解，在上述Bellman-Ford 算法中，
 if d[v] > d[u]+w(u，v)，=> 包含负权回路，返回FASLE
 else if =>不包含负权回路，返回TRUE。

ok，咱们，接下来，立马切入Dijkstra 算法。



**三、深入浅出，彻底解剖Dijkstra 算法**I、松弛技术RELAX的介绍
Dijkstra 算法使用了松弛技术，对每个顶点v<-V，都设置一个属性d[v]，用来描述从源点s到v的最短路径上权值的上界，
称为最短路径的估计。

首先，得用O（V）的时间，来对最短路径的估计，和对前驱进行初始化工作。
INITIALIZE-SINGLE-SOURCE(G, s)
1  for each vertex v ∈ V[G]
2       do d[v] ← ∞
3          π[v] ← NIL      //O（V）
4  d[s] 0

RELAX(u, v, w)
1  if d[v] > d[u] + w(u, v)
2     then d[v] ← d[u] + w(u, v)
3          π[v] ← u        //O（E）
图。

II、Dijkstra 算法
此Dijkstra 算法分三个步骤，
INSERT (第3行), EXTRACT-MIN (第5行), 和DECREASE-KEY(第8行的RELAX，调用此减小关键字的操作)。

**DIJKSTRA(G, w, s)**1  INITIALIZE-SINGLE-SOURCE(G, s)    //对每个顶点初始化 ，O(V) 
2  S ← Ø
3  Q ← V[G]            //INSERT，O(1)
4  while Q ≠ Ø
5      do u ← EXTRACT-MIN(Q)        //简单的O(V*V)；**二叉/项堆，和FIB-HEAP的话，则都为O(V*lgV)。**6         S ← S ∪{u}
7         for each vertex v ∈ Adj[u]
8             do RELAX(u, v, w)      //简单方式:O(E)，**二叉/项堆，E*O(lgV)，FIB-HEAP，E*O(1)**。



**四、Dijkstra 算法的运行时间**在继续阐述之前，得先声明一个问题，DIJKSTRA（G，w，s）算法中的第5行，EXTRACT-MIN(Q)，最小优先队列的具体实现。
而Dijkstra 算法的运行时间，则与此最小优先队列的采取何种具体实现，有关。

最小优先队列三种实现方法：
**1、**利用从1至|V| 编好号的顶点，简单地将每一个d[v]存入一个数组中对应的第v项，
如上述DIJKSTRA（G，w，s）所示，Dijkstra 算法的运行时间为**O(V^2+E)。**

**2、**如果是二叉/项堆实现最小优先队列的话，EXTRACT-MIN(Q)的运行时间为O(V*lgV)，
所以，Dijkstra 算法的运行时间为O(V*lgV+E*lgV)，
若所有顶点都是从源点可达的话，O（(V+E)*lgV）=O（E*lgV）。
当是稀疏图时，则E=O(V^2/lgV)，此Dijkstra 算法的运行时间为**O(V^2)。**

**3、**采用**斐波那契堆**实现最小优先队列的话，EXTRACT-MIN(Q)的运行时间为O(V*lgV)，
所以，此Dijkstra 算法的运行时间即为**O(V*lgV+E)。**

综上所述，此最小优先队列的三种实现方法比较如下：
      EXTRACT-MIN + RELAX
I、  简单方式：  O（V*V + E*1）
II、 二叉/项堆： O（V*lgV + |E|*lgV）
       源点可达：O（E*lgV）
       稀疏图时，有E=o（V^2/lgV），
            =>   O（V^2）  
III、斐波那契堆：O（V*lgV + E）

当|V|<<|E|时，采用DIJKSTRA（G，w，s）+ FIB-HEAP-EXTRACT-MIN(Q)，即斐波那契堆实现最小优先队列的话，
优势就体现出来了。


**五、Dijkstra 算法 + FIB-HEAP-EXTRACT-MIN(H)，斐波那契堆实现最小优先队列**由以上内容，我们已经知道，用斐波那契堆来实现最小优先队列，可以将运行时间提升到O（VlgV+E）。
|V|个EXTRACT-MIN 操作，每个平摊代价为O（lgV），|E|个DECREASE-KEY操作的每个平摊时间为O（1）。

下面，重点阐述DIJKSTRA(G, w, s)中，斐波那契堆实现最小优先队列的操作。

由上，我们已经知道，**DIJKSTRA算法包含以下的三个步骤：INSERT (第3行), EXTRACT-MIN (第5行), 和DECREASE-KEY(第8行的RELAX)。**

先直接给出Dijkstra 算法 + FIB-HEAP-EXTRACT-MIN(H)的算法：
DIJKSTRA(G, w, s)
1  INITIALIZE-SINGLE-SOURCE(G, s)
2  S ← Ø
3  Q ← V[G]   //第3行，INSERT操作，O（1）
4  while Q ≠ Ø
5      do u ← EXTRACT-MIN(Q)   //第5行，EXTRACT-MIN操作，V*lgV
6         S ← S ∪{u}
7         for each vertex v ∈ Adj[u]
8             do RELAX(u, v, w)  //第8行，RELAX操作，E*O(1)

**FIB-HEAP-EXTRACT-MIN(H)**//平摊代价为O（lgV）
 1  z ← min[H]
 2  if z ≠ NIL
 3     then for each child x of z
 4              do add x to the root list of H
 5                 p[x] ← NIL
 6          remove z from the root list of H
 7          if z = right[z]
 8             then min[H] ← NIL
 9             else min[H] ← right[z]
10                  CONSOLIDATE(H)   
11          n[H] ← n[H] - 1
12  return z

**--------------------------------------------------------------------------------------**

**六、Dijkstra 算法 +fibonacci堆各项步骤的具体分析**    ok，接下来，具体分步骤阐述以上各个操作：

**第3行的INSERT操作：**FIB-HEAP-INSERT(H, x)   //平摊代价，O（1）.
 1  degree[x] ← 0
 2  p[x] ← NIL
 3  child[x] ← NIL
 4  left[x] ← x
 5  right[x] ← x
 6  mark[x] ← FALSE
 7  concatenate the root list containing x with root list H
 8  if min[H] = NIL or key[x] < key[min[H]]
 9     then min[H] ← x
10  n[H] ← n[H] + 1

**第5行的EXTRACT-MIN操作：**
FIB-HEAP-EXTRACT-MIN(H)  //平摊代价为O（lgV）
 1  z ← min[H]
 2  if z ≠ NIL
 3     then for each child x of z
 4              do add x to the root list of H
 5                 p[x] ← NIL
 6          remove z from the root list of H
 7          if z = right[z]
 8             then min[H] ← NIL
 9             else min[H] ← right[z]
10                  CONSOLIDATE(H)   //CONSOLIDATE算法在下面，给出。
11          n[H] ← n[H] - 1
12  return z

下图是FIB-HEAP-EXTRACT-MIN 的过程示意图：

![](http://hi.csdn.net/attachment/201102/15/8394323_1297759609L5oK.jpg)


**CONSOLIDATE(H)** 1 for i ← 0 to D(n[H])
 2      do A[i] ← NIL
 3 for each node w in the root list of H
 4      do x ← w
 5         d ← degree[x]        //子女数
 6         while A[d] ≠ NIL
 7             do y ← A[d]      
 8                if key[x] > key[y]
 9                   then exchange x <-> y
10                FIB-HEAP-LINK(H, y, x)  //下面给出。
11                A[d] ← NIL
12                d ← d + 1
13         A[d] ← x
14 min[H] ← NIL
15 for i ← 0 to D(n[H])
16      do if A[i] ≠ NIL
17            then add A[i] to the root list of H
18                 if min[H] = NIL or key[A[i]] < key[min[H]]
19                    then min[H] ← A[i]

FIB-HEAP-LINK(H, y, x)   //y链接至 x。
1  remove y from the root list of H
2  make y a child of x, incrementing degree[x]
3  mark[y] ← FALSE

**第8行的RELAX的操作，已上已经给出:**RELAX(u, v, w)
1  if d[v] > d[u] + w(u, v)
2     then d[v] ← d[u] + w(u, v)
3          π[v] ← u        //O（E）

一般来说，在Dijkstra 算法中，DECREASE-KEY的调用次数远多于EXTRACT-MIN的调用，
所以在不增加EXTRACT-MIN 操作的平摊时间前提下，尽量减小DECREASE-KEY操作的平摊时间，都能获得对比二叉堆更快的实现。

**以下，是二叉堆，二项堆，斐波那契堆的各项操作的时间复杂度的比较：**

操作                  二叉堆(最坏)       二项堆(最坏)     斐波那契堆(平摊)

__________________________________
MAKE-HEAP        Θ(1)                  Θ(1)                Θ(1)
INSERT               Θ(lg n)              O(lg n)            Θ(1)
MINIMUM           Θ(1)                  O(lg n)             Θ(1)
EXTRACT-MIN     Θ(lg n)              Θ(lg n)            O(lg n)
UNION               Θ(n)                  O(lg n)            Θ(1)
DECREASE-KEY   Θ(lg n)             Θ(lg n)              Θ(1)
DELETE              Θ(lg n)              Θ(lg n)             O(lg n)

斐波那契堆，日后会在本BLOG内，更进一步的深入与具体阐述。
且同时，此文，会不断的加深与扩展。
完。



**本人July对本博客所有任何文章、内容和资料享有版权。转载务必注明作者本人及出处，并通知本人。谢谢。**

**July、二零一一年二月十三日。**](https://so.csdn.net/so/search/s.do?q=exchange&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=each&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)





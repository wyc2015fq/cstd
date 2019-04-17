# 经典算法研究系列：二、Dijkstra 算法初探 - 结构之法 算法之道 - CSDN博客





2010年12月24日 19:58:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：82072标签：[算法																[扩展																[存储																[网络																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[01.Algorithms（研究）](https://blog.csdn.net/v_JULY_v/article/category/769275)

所属专栏：[经典算法研究](https://blog.csdn.net/column/details/Dijkstra.html)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)




> 
> 
> 
> 
> 
### 经典算法研究系列：二、Dijkstra 算法初探









July   二零一一年一月

本文主要参考：算法导论 第二版、维基百科。



### 一、Dijkstra 算法的介绍

    Dijkstra 算法，又叫迪科斯彻算法（Dijkstra），算法解决的是有向图中单个源点到其他顶点的最短路径问题。举例来说，如果图中的顶点表示城市，而边上的权重表示著城市间开车行经的距离，Dijkstra 算法可以用来找到两个城市之间的最短路径。



### 二、图文解析 Dijkstra 算法

    ok，经过上文有点繁杂的信息，你还并不对此算法了如指掌，清晰透彻。没关系，咱们来幅图，就好了。请允许我再对此算法的概念阐述下，

    Dijkstra算法是典型最短路径算法，用于计算一个节点到其他所有节点的最短路径。不过，针对的是非负值权边。主要特点是以起始点为中心向外层层扩展，直到扩展到终点为止。[Dijkstra算法能得出最短路径的最优解，但由于它遍历计算的节点很多，所以效率低。]

     ok，如下图，设A为源点，求A到其他各所有一一顶点（B、C、D、E、F）的最短路径。线上所标注为相邻线段之间的距离，即权值。

（注：此图为随意所画，其相邻顶点间的距离与图中的目视长度不能一一对等）

               Dijkstra无向图


> 
> 
> 
> 

![](https://img-my.csdn.net/uploads/201204/18/1334751688_7862.jpg)









算法执行步骤如下表：


> 



![](https://img-my.csdn.net/uploads/201204/18/1334751615_3167.jpg)









### 三、Dijkstra 的算法实现

    Dijkstra 算法的输入包含了一个有权重的有向图 G，以及G中的一个来源顶点 S。我们以 V 表示 G 中所有顶点的集合，以 E 表示G 中所有边的集合。
    (u, v) 表示从顶点 u 到 v 有路径相连，而边的权重则由权重函数 w: E → [0, ∞] 定义。因此，w(u, v) 就是从顶点 u 到顶点 v 的非负花费值（cost），边的花费可以想像成两个顶点之间的距离。
任两点间路径的花费值，就是该路径上所有边的花费值总和。

    已知有 V 中有顶点 s 及 t，Dijkstra 算法可以找到 s 到 t 的最低花费路径(例如，最短路径)。这个算法也可以在一个图中，找到从一个顶点 s 到任何其他顶点的最短路径。

好，咱们来看下此算法的具体实现：

Dijkstra 算法的实现一（维基百科）：

u := Extract_Min(Q) 在顶点集合 Q 中搜索有最小的 d[u] 值的顶点 u。这个顶点被从集合 Q 中删除并返回给用户。

 1  function Dijkstra(G, w, s)
 2     for each vertex v in V[G]                        // 初始化
 3           d[v] := infinity
 4           previous[v] := undefined
 5     d[s] := 0
 6     S := empty set
 7     Q := set of all vertices
 8     while Q is not an empty set                      // Dijkstra演算法主體
 9           u := Extract_Min(Q)
10           S := S union {u}
11           for each edge (u,v) outgoing from u
12                  if d[v] > d[u] + w(u,v)             // 拓展边（u,v）
13                        d[v] := d[u] + w(u,v)
14                        previous[v] := u

如果我们只对在 s 和 t 之间寻找一条最短路径的话，我们可以在第9行添加条件如果满足 u = t 的话终止程序。现在我们可以通过迭代来回溯出 s 到 t 的最短路径：

1 s := empty sequence 
2 u := t
3 while defined u                                        
4       insert u to the beginning of S
5       u := previous[u]
现在序列 S 就是从 s 到 t 的最短路径的顶点集. 



Dijkstra 算法的实现二（算法导论）：

DIJKSTRA(G, w, s)
1  INITIALIZE-SINGLE-SOURCE(G, s)
2  S ← Ø
3  Q ← V[G]                                 //V*O（1）
4  while Q ≠ Ø
5      do u ← EXTRACT-MIN(Q)     //EXTRACT-MIN，V*O（V），V*O（lgV）
6         S ← S ∪{u}
7         for each vertex v ∈ Adj[u]
8             do RELAX(u, v, w)       //松弛技术，E*O（1），E*O（lgV）。



因为Dijkstra算法总是在V-S中选择“最轻”或“最近”的顶点插入到集合S中，所以我们说它使用了贪心策略。

（贪心算法会在日后的博文中详细阐述）。

二零一一年二月九日更新：
此Dijkstra 算法的最初的时间复杂度为O（V*V+E），源点可达的话，O（V*lgV+E*lgV）=>O（E*lgV）
当是稀疏图的情况时，E=V*V/lgV，算法的时间复杂度可为O（V^2）。

但我们知道，若是斐波那契堆实现优先队列的话，算法时间复杂度，则为O（V*lgV + E）。



### 四、Dijkstra 算法的执行速度

    我们可以用大O符号将Dijkstra 算法的运行时间表示为边数 m 和顶点数 n 的函数。Dijkstra 算法最简单的实现方法是用一个链表或者数组来存储所有顶点的集合 Q，所以搜索 Q 中最小元素的运算（Extract-Min(Q)）只需要线性搜索 Q 中的所有元素。这样的话算法的运行时间是 O(E^2)。

    对于边数少于 E^2 的稀疏图来说，我们可以用邻接表来更有效的实现迪科斯彻算法。同时需要将一个二叉堆或者斐波纳契堆用作优先队列来寻找最小的顶点（Extract-Min）。

     当用到二叉堆时候，算法所需的时间为O(( V+E )logE)，斐波纳契堆能稍微提高一些性能，让算法运行时间达到O(V+ElogE)。(此处一月十六日修正。)

     开放最短路径优先（OSPF， Open Shortest Path First）算法是迪科斯彻算法在网络路由中的一个具体实现。
与 Dijkstra 算法不同，Bellman-Ford算法可用于具有负数权值边的图，只要图中不存在总花费为负值且从源点 s 可达的环路即可用此算法（如果有这样的环路，则最短路径不存在，因为沿环路循环多次即可无限制的降低总花费）。

    与最短路径问题相关最有名的一个问题是旅行商问题（Traveling salesman problem），此类问题要求找出恰好通过所有标点一次且最终回到原点的最短路径。
    然而该问题为NP-hard的；换言之，与最短路径问题不同，旅行商问题不太可能具有多项式时间解法。如果有已知信息可用来估计某一点到目标点的距离，则可改用A*搜寻算法，以减小最短路径的搜索范围。

二零一一年二月九日更新：
BFS、DFS、Kruskal、Prim、Dijkstra算法时间复杂度的比较：
一般说来，我们知道，BFS，DFS算法的时间复杂度为O（V+E），
最小生成树算法Kruskal、Prim算法的时间复杂度为O（E*lgV）。

而Prim算法若采用斐波那契堆实现的话，算法时间复杂度为O（E+V*lgV），当|V|<<|E|时，E+V*lgV是一个较大的改进。
//|V|<<|E|，=>O（E+V*lgV） << O（E*lgV），对吧。:D

Dijkstra 算法，斐波纳契堆用作优先队列时，算法时间复杂度为O（V*lgV + E）。
//看到了吧，与Prim算法采用斐波那契堆实现时，的算法时间复杂度是一样的。

所以我们，说，BFS、Prime、Dijkstra 算法是有相似之处的，单从各算法的时间复杂度比较看，就可窥之一二。

==============================================

此文，写的实在不怎么样。不过，承蒙大家厚爱，此经典算法研究系列的后续文章，个人觉得写的还行。
所以，还请，各位可关注此算法系列的后续文章。谢谢。

                 二零一一年一月四日。](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)





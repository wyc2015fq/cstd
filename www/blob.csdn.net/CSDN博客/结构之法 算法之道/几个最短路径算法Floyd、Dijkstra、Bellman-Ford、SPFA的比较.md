# 几个最短路径算法Floyd、Dijkstra、Bellman-Ford、SPFA的比较 - 结构之法 算法之道 - CSDN博客





2011年02月12日 18:37:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：76747标签：[算法																[algorithm																[blog																[优化																[网格																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[02.Algorithms（后续）](https://blog.csdn.net/v_JULY_v/article/category/772685)





**      几大最短路径算法比较**



July、二零一一年二月十二日。
----------------------------------- 



**几个最短路径算法的比较：Floyd**

求多源、无负权边的最短路。用矩阵记录图。时效性较差，时间复杂度O(V^3)。
       Floyd-Warshall算法（Floyd-Warshall algorithm）是解决任意两点间的最短路径的一种算法，可以正确处理有向图或负权的最短路径问题。

Floyd-Warshall算法的时间复杂度为O(N^3)，空间复杂度为O(N^2)。

      Floyd-Warshall的原理是动态规划：
设Di,j,k为从i到j的只以(1..k)集合中的节点为中间节点的最短路径的长度。
若最短路径经过点k，则Di,j,k = Di,k,k-1 + Dk,j,k-1； 
若最短路径不经过点k，则Di,j,k = Di,j,k-1。 
因此，Di,j,k = min(Di,k,k-1 + Dk,j,k-1 , Di,j,k-1)。

      在实际算法中，为了节约空间，可以直接在原来空间上进行迭代，这样空间可降至二维。
Floyd-Warshall算法的描述如下：
for k ← 1 to n do
  for i ← 1 to n do
    for j ← 1 to n do
      if (Di,k + Dk,j < Di,j) then
        Di,j ← Di,k + Dk,j;
其中Di,j表示由点i到点j的代价，当Di,j为 ∞ 表示两点之间没有任何连接。



**Dijkstra**

      求单源、无负权的最短路。时效性较好，时间复杂度为O（V*V+E）。
源点可达的话，O（V*lgV+E*lgV）=>O（E*lgV）。
      当是稀疏图的情况时，此时E=V*V/lgV，所以算法的时间复杂度可为**O（V^2）** 。若是斐波那契堆作优先队列的话，算法时间复杂度，则为O（V*lgV + E）。

      更多，请参考：[**二（续）、彻底理解Dijkstra算法**](http://blog.csdn.net/v_JULY_v/archive/2011/02/13/6182419.aspx)，及[**二（再续）、Dijkstra 算法+fibonacci堆的逐步c实现**](http://blog.csdn.net/v_JULY_v/archive/2011/03/18/6259576.aspx)。



**Bellman-Ford**

      求单源最短路，可以判断有无负权回路（若有，则不存在最短路），
时效性较好，时间复杂度O（VE）。此算法日后还会在本BLOG内具体阐述。

Bellman-Ford算法是求解单源最短路径问题的一种算法。

      单源点的最短路径问题是指：
给定一个加权有向图G和源点s，对于图G中的任意一点v，求从s到v的最短路径。

      与Dijkstra算法不同的是，在Bellman-Ford算法中，边的权值可以为负数。
      设想从我们可以从图中找到一个环路（即从v出发，经过若干个点之后又回到v）且这个环路中所有边的权值之和为负。那么通过这个环路，环路中任意两点的最短路径就可以无穷小下去。如果不处理这个负环路，程序就会永远运行下去。 而Bellman-Ford算法具有分辨这种负环路的能力。


**SPFA**

      是Bellman-Ford的队列优化，时效性相对好，时间复杂度O（kE）。（k<<V）。

与Bellman-ford算法类似，SPFA算法采用一系列的松弛操作以得到从某一个节点出发到达图中其它所有节点的最短路径。所不同的是，SPFA算法通过**维护一个队列**，使得一个节点的当前最短路径被更新之后没有必要立刻去更新其他的节点，从而大大减少了重复的操作次数。

      SPFA算法可以用于存在负数边权的图，这与dijkstra算法是不同的。

与Dijkstra算法与Bellman-ford算法**都**不同，SPFA的算法时间效率是不稳定的，即它对于不同的图所需要的时间有很大的差别。

      在最好情形下，每一个节点都只入队一次，则算法实际上变为广度优先遍历，其时间复杂度仅为O(E)。另一方面，存在这样的例子，使得每一个节点都被入队(V-1)次，此时算法退化为Bellman-ford算法，其时间复杂度为O(VE)。

      SPFA算法在负边权图上可以完全取代Bellman-ford算法，另外在稀疏图中也表现良好。但是在非负边权图中，为了避免最坏情况的出现，通常使用效率更加稳定的Dijkstra算法，以及它的使用堆优化的版本。通常的SPFA算法在一类网格图中的表现不尽如人意。

完。](https://so.csdn.net/so/search/s.do?q=网格&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=blog&t=blog)](https://so.csdn.net/so/search/s.do?q=algorithm&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)





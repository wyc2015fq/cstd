# 浅谈最短路径的几个方法（Dijkstra，Bellman-Ford，SPFA，Floyd算法） - Machine Learning with Peppa - CSDN博客





2018年04月07日 21:56:51[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：69








我之前有文章详细介绍了这四种方法并且给出了C代码实现，今天用四句话总结一下几个特点。

最短路径问题的四大金刚:


1. Dijkstra算法

最经典的单源最短路径算法。本质是BFS+贪心，应用最广


2. Bellman-Ford算法

允许负权边的单源最短路径算法，DJ算法的适应版，可以用来检测负权回路


3. SPFA

其实是Bellman-ford+队列优化,缺点是时间复杂度过高，高达 O(VE)


4. Floyd算法

动态规划的方法，经典的多源最短路径算法，复杂度O(N^3)，简单粗暴有效




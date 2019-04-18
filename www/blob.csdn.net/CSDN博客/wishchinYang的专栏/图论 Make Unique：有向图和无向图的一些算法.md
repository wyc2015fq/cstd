# 图论 Make Unique：有向图和无向图的一些算法 - wishchinYang的专栏 - CSDN博客
2018年01月03日 09:48:58[wishchin](https://me.csdn.net/wishchin)阅读数：1336
          计算机科学入门资料之一的《算法与数据结构-C语言版》，覆盖了基础算法的几乎所有分支，其中的一个典型分支为图理论。
          一个简介：[图论基础-图数据结构](http://blog.csdn.net/saltriver/article/details/54428685)基础
          一个简洁的博客：[图论](http://9080.iteye.com/blog/2062310)基础，简列一本书
前言：
         图论〔Graph Theory〕是数学的一个分支。它以[图](https://baike.baidu.com/item/%E5%9B%BE/13018767)为研究对象。图论中的图是由若干给定的点及连接两点的线所构成的图形，这种图形通常用来描述某些事物之间的某种特定关系 ，用点代表事物，用连接两点的线表示相应两个事物间具有这种关系。
         有向图和无向图有严格的集合论定义。在通常状况下，区分图的有向和无向的区别在于边的有向性。以邻接矩阵存储相邻关系的数据描述中，邻接矩阵的对称性是无向图的必要非充分条件。
         下面再次梳理一下两种图的经典算法，不能记混了。
有向图的基本算法：
        拓扑排序（数据结构之[拓扑排序](https://www.cnblogs.com/Braveliu/archive/2013/12/05/3460232.html)）、联通分量、最短路径（[Dijkstra算法和Floyd算法](http://www.baidu.com/link?url=wt2cngRExz9KCQkkS1Ogs0OFWToQHj1IeGMcPM6JWkPIJn9Ncgb2vWJXF-m_LQaVthSeVI9nxr5nKF7MaURwLaXFmGyhnWkd204JmsPuwbe)）。
无向图的基本算法：
        最小生成树（Prime算法。Kruska算法）、DFS、BFS、MFS、最短路径、最大连通图、强联通分量。
环的查找算法：
        查找所有环、亏格（最小子环的个数）的寻找、最大环寻找。
        查找所有环的算法C++： [找出无向图中所有环](http://blog.csdn.net/robin_xu_shuai/article/details/51898847)的算法
曲面三角化算法
        曲面三角化：[三角剖分算法](http://blog.csdn.net/shenziheng1/article/details/54865138)的数学理解


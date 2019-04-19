# 图、图的遍历、DFS生成树、BFS生成树 - KinboSong的博客 - CSDN博客
2017年04月02日 12:27:03[songjinbo3](https://me.csdn.net/KinboSong)阅读数：5403
个人分类：[数据结构与算法](https://blog.csdn.net/KinboSong/article/category/6791889)
UDG：无向图
图分为：无向图（**UDG**）和有向图（**DG**）
图的表示方法：**邻接表**和**邻接矩阵。**邻接表表示参考：[http://blog.csdn.net/linxinyuluo/article/details/6847851](http://blog.csdn.net/linxinyuluo/article/details/6847851)
图的遍历：深度优先遍历（Depth-First Search，**DFS**）和广度优先遍历（Breadth-First Search，**BFS**）。
深度优先遍历类似与树的**先序**遍历，广度优先遍历类似与树的**层次**遍历
深度优先遍历和广度优先遍历参考博客：[https://segmentfault.com/a/1190000002685939](https://segmentfault.com/a/1190000002685939)
深度优先遍历可以通过递归或者非递归，其中非递归要采用**栈**的形式存储结点；广度优先遍历一般通过非递归，需要通过**队列**的形式存储结点。
c++实现：
（1）邻接矩阵无向图的DFS和BFS：[https://github.com/wangkuiwu/datastructs_and_algorithm/blob/master/source/graph/iterator/udg/cplus/MatrixUDG.cpp](https://github.com/wangkuiwu/datastructs_and_algorithm/blob/master/source/graph/iterator/udg/cplus/MatrixUDG.cpp)
（2）邻接表无向图的DFS和BFS：[https://github.com/wangkuiwu/datastructs_and_algorithm/blob/master/source/graph/iterator/udg/cplus/ListUDG.cpp](https://github.com/wangkuiwu/datastructs_and_algorithm/blob/master/source/graph/iterator/udg/cplus/ListUDG.cpp)
（3）邻接矩阵有向图的DFS和BFS：[https://github.com/wangkuiwu/datastructs_and_algorithm/blob/master/source/graph/iterator/dg/cplus/MatrixDG.cpp](https://github.com/wangkuiwu/datastructs_and_algorithm/blob/master/source/graph/iterator/dg/cplus/MatrixDG.cpp)
（4）邻接表有向图的DFS和BFS：[https://github.com/wangkuiwu/datastructs_and_algorithm/blob/master/source/graph/iterator/dg/cplus/ListDG.cpp](https://github.com/wangkuiwu/datastructs_and_algorithm/blob/master/source/graph/iterator/dg/cplus/ListDG.cpp)
图的生成树：
G的生成树是G的极小连通子图，图的生成树不唯一，从不同的顶点出发进行遍历，可以得到不同的生成树。
1、DFS生成树：
2、BFS生成树：BFS生成树在G的所有生成树中的高度是最低的。
参考：[http://blog.csdn.net/coslay/article/details/38716183](http://blog.csdn.net/coslay/article/details/38716183)

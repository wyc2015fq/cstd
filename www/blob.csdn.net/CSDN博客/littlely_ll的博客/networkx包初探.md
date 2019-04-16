# networkx包初探 - littlely_ll的博客 - CSDN博客





2018年08月16日 22:23:18[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：477








networkx是python的一个包，它是用来复杂网络结构的创建、操作和学习。官网上给它设定的目标为：

> - tools for the study of the structure and dynamics of social, biological, and infrastructure networks,
- a standard programming interface and graph implementation that is suitable for many applications,
- a rapid development environment for collaborative, multidisciplinary projects, 

  an interface to existing numerical algorithms and code written in C, C++, and FORTRAN,
- the ability to painlessly slurp in large nonstandard data sets.


## 构建图

简单图的构建比较简单，首先载入包：
`import networkx as nx`
构建一个简单图：

```
nodes = [0,1,2,3,4]
edges = [(0,1),(0,2),(1,2),(1,3),(2,3),(0,4)]
```

`nodes`为一个图的顶点，而`edges`为顶点之间的边，其用一个列表元组表示。 
**构建一个无向图：**

```
G = nx.Graph()
G.add_nodes_from(nodes)
G.add_edges_from(edges)
```

**构建一个有向图：**

```
DG = nx.DiGraph()
DG.add_nodes_from(nodes)
DG.add_edges_from(edges)
```

有向图和无向图构建时的行为相似，构建无向图时是`nx.Graph`， 而构建有向图时是`nx.DiGraph`，添加结点和边时用的同样的方法。 

构建出的有向图和无向图可以绘出：

```python
import matplotlib.pyplot as plt
#绘制无向图
nx.draw(G, with_labels=True, font_weight='bold')
plt.show()

#绘制有向图
nx.draw(DG, with_labels=True, font_weight='bold')
plt.show()
```

![这里写图片描述](https://img-blog.csdn.net/2018081620260433?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![这里写图片描述](https://img-blog.csdn.net/20180816202623316?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从中可以看出，构建有向图时，元组的第一个元素为起始点，也就是弧尾，第二个元素为结束点，也就是弧头。

## 图的遍历

**深度优先遍历**

深度优先遍历有两种方法，一个是`dfs_edges`，另一个是`edge_dfs`，两种方法有所不同，首先看代码 

以有向图为例：
```php
list(nx.dfs_edges(DG,0)) #1
list(nx.edge_dfs(DG,0)) #2
```

结果： 

[(0, 1), (1, 2), (2, 3), (0, 4)] #1 

[(0, 1), (1, 2), (2, 3), (1, 3), (0, 2), (0, 4)]#2
`dfs_edges`和`edge_dfs`都是深度遍历方法，其第一个参数为图，第二个参数为一个源结点，返回的都是一个生成器。**对于`dfs_edges`来说，一旦它遍历了源结点所能到达的所有结点它就停止了，而`edge_dfs`是直到源结点遍历完所有它能到达的边才停止。**对比着结果看上面两张图就明白了。另一个与`dfs_edges`相对应的是广度优先遍历`bfs_edges`。

**找到源结点的所有可达结点：**

有两种方法，`dfs_preorder_nodes`和`dfs_postorder_nodes`，第一个参数是构建的图，第二个参数为源结点， 它们返回的都是一个生成器

```php
list(nx.dfs_preorder_nodes(DG,1))#1
list(nx.dfs_postorder_nodes(DG,1))#2
```

结果： 

[1, 2, 3]#1 

[3, 2, 1]#2
## 获取路径

**获取两结点间所有简单路径**： 

使用的方法为`all_simple_paths`，第一个参数为图，第二个和第三个参数分别是源结点和目标结点，第四个参数为截断长度`cutoff`，默认为`None`，返回的是一个生成器。

```php
list(nx.all_simple_paths(DG,0,3)#1
list(nx.all_simple_paths(DG,0,3,cutoff=3)#2
list(nx.all_simple_paths(DG,0,3,cutoff=2)#3
```

[[0, 1, 2, 3], [0, 1, 3], [0, 2, 3]]#1 

[[0, 1, 2, 3], [0, 1, 3], [0, 2, 3]]#2 

[[0, 1, 3], [0, 2, 3]]#3 
`all_simple_paths`默认返回所有从源结点到目标结点的路径，**而`cutoff`参数设置的截断长度不包括源结点**。
**最短路径：**
`shortest_path(G[, source, target, weight])`返回最短路径，如果没有设定`source`和`target`，返回一个字典，得到的结果为`path[source][target]=[list of nodes in path]`
`nx.shortest_path(DG)`
{0: {0: [0], 1: [0, 1], 2: [0, 2], 3: [0, 1, 3], 4: [0, 4]}, 

 1: {1: [1], 2: [1, 2], 3: [1, 3]}, 

 2: {2: [2], 3: [2, 3]}, 

 3: {3: [3]}, 

 4: {4: [4]}} 

一般使用时都是设定源结点和目标结点，所以返回一个最短路径列表：`nx.shortest_path(DG, 0,3)`
[0, 1, 3] 
**DG有多个最短路径，看起来它只返回一个最短路径。**
`all_shortest_paths`返回所有的最短路径，返回一个生成器，但要设定`source`和`target`。`list(nx.all_shortest_paths(DG, 0,3))`
[[0, 1, 3], [0, 2, 3]]

`has_path`返回布尔值，即是否有从`source`到`target`的路径：
`nx.has_path(DG,0,3)`
True

**dijkstra最短路径**： 
`dijkstra_path(G, source, target, weight='weight')`返回一个最短路径列表：
`nx.dijkstra_path(DG, 0,3)`
[0, 1, 3] 

它也是返回一条路径。

`dijkstra_path_length(G, source, target, weight='weight')`返回最短路径长度，长度不包括源结点：
`nx.dijkstra_path_length(DG, 0,3)`
2

## 绘制图模型

前面介绍了一种绘制方法：

```
nx.draw(DG, with_labels=True, font_weight='bold')
plt.show()
```

带坐标的一种：

```
nx.draw_networkx(DG, with_labels=True, font_weight='bold')
plt.show()
```

![这里写图片描述](https://img-blog.csdn.net/20180816221540289?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

按照圆形布局：
`nx.draw_circular(DG, with_labels=True, font_weight='bold')`
![这里写图片描述](https://img-blog.csdn.net/20180816221744707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

随机布局：
`nx.draw_random(DG, with_labels=True, font_weight='bold')`
![这里写图片描述](https://img-blog.csdn.net/20180816221910116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

还有一些其他的布局或自定义布局等。 

 先到这吧。










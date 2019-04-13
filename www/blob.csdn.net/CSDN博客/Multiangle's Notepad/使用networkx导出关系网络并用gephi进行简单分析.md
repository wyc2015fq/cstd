
# 使用networkx导出关系网络并用gephi进行简单分析 - Multiangle's Notepad - CSDN博客


2016年02月28日 11:21:21[multiangle](https://me.csdn.net/u014595019)阅读数：11681标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[网络																](https://so.csdn.net/so/search/s.do?q=网络&t=blog)[可视化																](https://so.csdn.net/so/search/s.do?q=可视化&t=blog)[微博																](https://so.csdn.net/so/search/s.do?q=微博&t=blog)[gephi																](https://so.csdn.net/so/search/s.do?q=gephi&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=微博&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=可视化&t=blog)个人分类：[社交网络分析																](https://blog.csdn.net/u014595019/article/category/6110760)[python																](https://blog.csdn.net/u014595019/article/category/5636921)[算法																](https://blog.csdn.net/u014595019/article/category/6163865)[
							](https://blog.csdn.net/u014595019/article/category/5636921)
[
																								](https://blog.csdn.net/u014595019/article/category/6110760)

[
		](https://so.csdn.net/so/search/s.do?q=可视化&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=python&t=blog)
之前使用爬虫获取到了微博上粉丝数大于2000的用户，大概在10W左右，并获取了他们的关注用户列表。现在，需要利用这些节点和节点间的关系网来进行社区探测(community detection)，以进行进一步分析。不过在此之前，需要攫取一些小样本进行简单的可视化。
**使用networkx(python)导出gexf格式文件**
这边使用[gephi](https://gephi.org/)来进行分析。而使用gephi需要有gexf或者gephi格式的数据。其中，gephi格式类似于工程文件，不仅储藏了数据，也储藏了整个工程中的所有参数，包括颜色等。而gexf只包含数据。因此，需要将mysql中的数据转化为gexf格式。
这里需要借助networkx包。networkx是python下的复杂网络编程包，具体使用方法这里不具体展开，只表述如何将从mysql中导入的node和edge数据生成为network中的Graph对象，然后再生成gexf文件
目前，假设有两个列表数据，一个是node，里面储存了网络中节点的数据，每个节点可以用int来表示，也可以用字符串来表示。另一个是edge，表示网络中节点之间的关系数据，或者说是边的数据。每条边用一个长为2的数组表示，分别表示该条边两边的节点。
举例例子，node=[‘A’,’B’,’C’],
edge=[ [‘A’,’B’] , [‘B’,’C’] ]
现在需要利用node和edge生成Graph类，并生成gexf文件，其实挺简单的
```python
import
```
```python
networkx
```
```python
as
```
```python
nx
G=nx.Graph()
G.add_nodes_from(node)
G.add_edges_from(edge)
nx.write_gexf(G,
```
```python
'your_file_name.gexf'
```
```python
)
```
此时在根目录下会生成名为your_file_name.gexf的文件
**使用gephi进行可视化和简单分析**
首先打开gephi，然后选择打开图文件，选择之前生成的gexf文件。我这边选用的是前1万名的微博大V以及他们之间的关系网。
![这里写图片描述](https://img-blog.csdn.net/20160228110732968)
在图的类型处选择无向图或者是有向图，取决于网络自身的属性。
![这里写图片描述](https://img-blog.csdn.net/20160228110902877)
打开以后能看到揉在一起的网络，呈方块状，里面是密密麻麻的节点和边。
![这里写图片描述](https://img-blog.csdn.net/20160228111003878)
在左下角有布局选项，选择一个算法，点运行展开(lay out) 我这里选用的是ForceAtlas 2。展开后的结果如下所示
![这里写图片描述](https://img-blog.csdn.net/20160228111241944)
这时需要探测关系网中的子团体，需要运行community detection算法。在右边统计一栏中找到模块化，点击运行
![这里写图片描述](https://img-blog.csdn.net/20160228111424133)
得到的计算结果如下
![这里写图片描述](https://img-blog.csdn.net/20160228111518103)
横轴为社区的编号，纵轴为该社区具有的节点数。由于有大量节点未与其他节点互相关注，因此大部分的社区大小为1。
此时需要对主要社区进行找色，以便于观察。
在左上角的外观-》节点-》数值设定上，选择用modularity class作为渲染方式
![这里写图片描述](https://img-blog.csdn.net/20160228111830456)
最后得到的渲染结果如下
![这里写图片描述](https://img-blog.csdn.net/20160228111854107)
可以看到一种颜色代表了一个社区。是不是很漂亮呢？


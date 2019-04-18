# MapReduce在搜索引擎中一些应用 - Orisun - 博客园







# [MapReduce在搜索引擎中一些应用](https://www.cnblogs.com/zhangchaoyang/articles/2624278.html)





### word-doc矩阵

在进行文件分类或文档检索的时候，我们通常需要建立一个word-doc矩阵，来记录每个词在每篇文档中出现的次数。

class Mapper
	method map(docid id,doc d)
		foreach term in d
			Emit(pair(term,id),1)


一种更高效的方法是在mapper侧进行聚合。

class Mapper
	method map(docid id,doc d)
		H := new AssociativeArray
		foreach term in d
			H{term} := H{term}+1
		foreach term in H
			Emit(pair(term,id),H{term})


### 图的宽度优先遍历

现代的爬虫一般采用宽度优先的规则来遍历网络。还有在[Dijkstra算法](http://www.cnblogs.com/zhangchaoyang/articles/2366869.html)中也是一种宽度优先的规则--每发再一个新的最近点，就更新与之相邻的节点的dist。我们可以用MapReduce迭代的方式来实现Dijkstra算法。每轮迭代运行一次Map-Reduce，最多迭代N次，N是图中节点的个数。用邻接链表来存储图。算法开始时，起始节点的dist赋0，其余的节点dist赋无穷大。先考虑简单的情形--每条边的权值都是1。

class Mapper
	method map(nid n,node N)
		d := N.distance
		Emit(nid n,N)			//pass along graph structure
		foreach nodeid m in N.AdjacencyList
			Emit(nid m,d+1)		//Emit distances to reachable nodes
			
class Reducer
	method reduce(nid m,[d1,d2,...])
		minDist := infinite
		M := NULL
		foreach d in [d1,d2,...]
			if IsNode(d)
				M := d			//recover graph structure
			else if d<minDist	//lookup for shortest distance
				minDist := d
		M.distance := minDist	//update shortest distance
		Emit(nid m,node M)


在迭代式的Map-Reduce程序设计中，需要有一个driver来控制迭代的进行，检查终止条件是否满足。在reducer中如果所有node的minDist都没有更新，更迭代终止。

上面的代码只算出了源节点到其他节点的最短路程，并没有记录下最短路径，所以我们需要在更新minDist的同时记录下它的上一个节点，mapper在输出(nid,dist)时也应该输出与dist对应的是哪个节点。

上面假设任意节点间的距离都是1，当是任意值r时，只需要在mapper中输出(nid m,d+r)就可以了。

跟单进程的Dijkstra算法比起来，这种Map-Reduce版的算法显然计算量大了许多，因为它每次迭代都要去检查每一个节点的邻居的dist是否需要更新；而单进程的Dijkstra算法通过维持一个优先队列，它每次迭代只需要检查队首元素的邻居的dist是否需要更新。在Hadoop中，mapper和reducer通信方式非常受限，我们无法维持一个像优先队列这样一种全局的数据结构。正因如此，Map-Reduce不适合于处理大规则，稠密图。

### Page Rank

在网页排序算法中Page Rank可谓鼎鼎有名--虽然它只是Google对网页进行排序的上千种因素中的一个。该算法认为一个网页的重要度，由它链接到它的的网页的重要度来决定。网页重要度公式为：

![](https://pic002.cnblogs.com/images/2012/103496/2012080522553031.png)

|G|中整个网络当中节点的总个数，α是随机跳转因子，L(n)是所有铰接到网页n的网页的集合，C(m)是网页m的出度。在网页m上的一个冲浪者它以1/C(m)的概率进入网页n。如何理解加式的第1项呢？一个冲浪者在浏览网页时并不总是点击超链接进行下去的，他也有可能在地址栏随机地输入了一个网址，一下子跳转到了一个完全不相关的网页。

Page Rank算法迭代地计算每个网页的重要度，实际上就是一种宽度优先遍历的方法。一个网页从它的所有incoming网页中“收集”重要度，很自然地用reducer来完成。开始时给网络图上的每个节点赋一个服从[0,1]的均匀分布的随机值作为其重要度。Map-Reduce不断地迭代，直到每个网页的重要度都不再变化为止。

下面的代码简化起见，没有考虑公式中的第一项。

class Mapper
	method map(nid n,node N)
		p := N.PageRank/|N.AdjacencyList|
		Emit(nid n,N)			//pass along graph structure
		foreach nodeid m in N.AdjacencyList
			Emit(nid m,p)
			
class Reducer
	method reduce(nid m,[p1,p2,...])
		M := NULL
		s := 0
		foreach p in [p1,p2,...]
			if IsNode(p)
				M := p
				s := M.PageRank
			else
				s := s+p
		M.PageRank := s
		Emit(nid m,node M)


Graph Structure被mapper抛出，又被reducer写入磁盘，这样它从上一次迭代又传递一了下一次迭代。

### HMM算法

当年李开复搞语音识别用的就是隐马尔可夫算法，HMM在[中文分词](http://www.cnblogs.com/zhangchaoyang/articles/2571110.html)和[词性标注](http://www.cnblogs.com/zhangchaoyang/articles/2567610.html)中也大显威力。

[EM算法](http://www.cnblogs.com/zhangchaoyang/articles/2623364.html)也是Machine Learning中的重要算法之一，比如[HMM模型参数的确立](http://www.cnblogs.com/zhangchaoyang/articles/2220398.html)、[高斯混合模型参数的确立](http://www.cnblogs.com/zhangchaoyang/articles/2624882.html)都要用到它。



### K-means算法

[K-means](http://www.cnblogs.com/zhangchaoyang/articles/2181869.html)是一种古老且经典的聚类算法，至今仍广泛使用。

class Mapper
	method setup()
		Centers[K] := ReadFromFile
	method map(docid id,doc d)
		H := new AssociativeArray
		foreach data in d
			Centers[index] := NearestCenterToData
			c := H{index}.left
			s := H{index}.right
			H{index} := Pair(c+1,s+data)
		foreach integer i in H
			Emit(i,H{i})
			
class Reducer
	method reduce(integer i,pairs [p1,p2,...])
		s := 0
		c := 0
		foreach pair in [p1,p2,...]
			s := s+pair.right
			c := c+pair.left
		Emit(i,s/c)














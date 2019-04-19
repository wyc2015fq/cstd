# Tarjan's strongly connected components algorithm的一些想法 - 电影旅行敲代码 - CSDN博客
2018年06月26日 22:27:17[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：392
Tarjan的极大强连通子图（strongly connected components，SCC）算法基于深度优先遍历（DFS）实现。本文就尝试从深度优先遍历的角度思考一下Tarjan的方法是如何找出SCC的。
## 深度优先遍历
深度优先遍历可以说是求SCC比较直观的一个途径。例如对于下图来说，我们从`A`开始遍历，如果能够回到`A`，那么整个路径就是一个连通分量。例如下图中的`A -> B -> D`就是一个连通子图，但不是极大连通子图。我们可以看到单纯的深度优先遍历并不能找出SCC，我们需要将已经找到的连通分量记录下来，例如下图中的路径`A -> B -> D`，然后和后面遍历得到的点结合起来组成极大连通子图。  
![这里写图片描述](https://img-blog.csdn.net/20180625222857687?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*注：该图来源于[http://blog.miskcoo.com/2016/07/tarjan-algorithm-strongly-connected-components](http://blog.miskcoo.com/2016/07/tarjan-algorithm-strongly-connected-components)*
## 深度优先遍历 with 集合
在前一小节中，我们发现普通的深度优先遍历并不能得出极大连通子图，因此需要一个数据结构记录下来深度优先遍历时的路径，由于连通子图中各个节点是平等的，只是遍历顺序不同，所以我们可以尝试使用**集合**保存遍历时的路径。同时我们可以观察到，连通分量可以**“塌缩“**为一个节点`ABD`，任何与`ABD`构成连通的节点，也与`ABD`中所有的子节点连通。如下图所示，我们将`A`，`B`，`D`塌缩成为节点`ABD`。
![这里写图片描述](https://img-blog.csdn.net/20180625223055321?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*注：为了表示一致，我们也将C塌缩成一个节点*
假如在我们遍历完`A`，`B`，`C`，`D`之后，按照先遍历`E`的顺序继续进行图的遍历，那么我们可以观察到，`E`和`F`能与`ABD`进行连通，所以我们可以将`E`，`F`与`ABD`一起塌缩成节点`ABDEF`，如下图所示。
![这里写图片描述](https://img-blog.csdn.net/20180625223625300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在遍历完`E`，`F`之后，我们继续遍历`G`，`H`之后，发现`G`和`H`也可以与`ABDEF`连通，那么我们同样可以将`G`和`H`与`ABDEF`一起塌缩成为`ABDEFGH`，最终得到下图。
![这里写图片描述](https://img-blog.csdn.net/20180626224841457?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如上图所示，我们得到强连通分量`ABDEFGH`，`C`和`I`。由于很多细节没有说明，所以整个过程还是很简单的。
细节：
- 如何表示一个**塌缩**节点？一种有效的方式就是给该塌缩节点中的所有子节点，都分配一个相同的`scc-index`，例如都为其分配节点1。由于是深度优先遍历，我们可以在返回父节点的时候，依次设置其`scc-index`
- 在遍历的过程中，如何判断一个强连通分量已经形成？我们可以在遍历完之后，遍历所有的节点，`scc-index`相同的节点就构成了一个强连通分量
由上我们可以得出如下的一种可行方案：
- 在遍历的过程中，我们可以为每个节点分配一个遍历序号，第一个遍历的为1，第二个遍历的为2等等
- 如果在遍历的过程中，遇到了在同一条遍历链条上的节点（也就是祖先节点），那么说明我们得到了一个连通分量，此时我们该链条上的所有节点的`scc-index`置为该祖先节点的遍历序号。然后将该链条（环）“塌缩“成一个节点
- 在遍历的过程中，如果遇到了两个“塌缩“节点需要合并的情况，那么就选择`scc-index`低的节点作为新的“塌缩“节点的`scc-index`
- 没有形成“塌缩“节点的节点，独自成为一个新的节点
有了如上的一个方案后，我们重新执行一遍深度优先遍历之后，过程如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180625230941422?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*注：此次为了说明两个“塌缩“节点如何合并，将遍历顺序改成了先遍历`G`，`H`，再遍历`F`*
## Tarjan’s算法
Tarjan’s算法的核心如下：
- Tarjan’s算法选择任一点进行深度优先遍历，每个节点只遍历一次
- Tarjan’s算法将遍历过的节点，按照遍历顺序依次存放到栈上。其中：栈维护一个**不变量**，任意一个节点保留在栈上，当且仅当该节点遍历完之后，存在一条指向栈上更早节点的路径
- 对于每个节点`v`，Tarjan’s算法会按照遍历顺序为为其分配一个唯一的遍历序号`v.index`，还会记录该节点能够访问到的遍历序号最小的节点序号（包括`v`自身），记为`v.lowlink`。结合上面的第二点，我们能够推断得出，节点`v`保留在栈上的条件是`v.lowlink` < `v.index`。其中将`v.lowlink` == `v.index`的节点，称为强连通图的root节点。另外，`v.lowlink`的初始值为`v.index`。
*注：上面的内容精简自[https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm](https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm)*
介绍如何更新`v.lowlink`之前，需要了解`tree edge`，`back edge`和`cross edge`，这三种边是[深度优先遍历中生成树](https://en.wikipedia.org/wiki/Depth-first_search#Output_of_a_depth-first_search)中边的概念。`v.lowlink`的更新有如下两种情况：
- **Tree Edge**
If node `v` is not visited already, then after DFS of `v` is complete, then minimum of `u.lowlink` and `v.lowlink` will be updated to `u.lowlink`. 
`u.lowlink = min(u.lowlink, v.lowlink)`
- **Back Edge**
When child `v` is already visited, then minimum of `low[u]` and `Disc[v]` will be updated to `low[u]`. 
`u.lowlink = min(u.lowlink, v.index)`
我们以最开始的图为例，给出Tarjan’s算法的整个过程：
![这里写图片描述](https://img-blog.csdn.net/20180626211452482?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180626211505800?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
整个过程其实和前面介绍的“深度优先遍历with集合“的过程很相似，例如对于上图中的第（4）步，是在遇到了`back-edge`以后，应用`规则2`，将`D.lowlink`设置为`A`的遍历序号`1`，然后应用`规则1`，在回退的过程中将`B`和`A`的`lowlink`设置为`1`。其实这里的`A`，`B`，`D`组成了一个连通分量，可以理解成将这三点塌缩成`ABD`，该连通分量的标号为1（也就是连通分量“根“的遍历序号）。
上图中第（6）步，是在遇到了`back-edge`以后，应用`规则2`，将`F.lowlink`设置为`D`的遍历序号`4`，然后应用`规则1`，在回退的过程中将`F`，`E`的`lowlink`设置为4，这里的`D`，`F`，`E`也是一个连通分量，同样可以理解成一个塌缩节点`EFD`，该塌缩节点的序号为4。同理的还有第（10）步中的连通分量`EGH`，序号为5。这三个连通分量如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180626214412933?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从上图中我们可以看出，`lowlink`可以看作是一系列连通分量的标号，标号是连通分量最早遍历的节点的遍历序号，也就是`root`节点的遍历序号。上图中有三个连通分量，`1`，`4`，`5`，这三个连通分量也是互相连通的，所以最终这三个连通分量最终组成了极大连通子图。注意连通分量`4`的根节点`D`的`index`为4，`lowlink`为1，说明该节点从属于另外一个连通分量1。因此Tarjan’s算法从另一个角度来看，是首先得到了一系列的连通分量，最后通过这些连通分量来构成更大的连通子图。
## Why `u.lowlink = min(u.lowlink, v.index)`
通过这个角度，我们可以解决如下的疑惑，就是规则2中，为什么`u.lowlink = min(u.lowlink, v.index)`，而不是`u.lowlink = min(u.lowlink, v.lowlink)`。**其实这里用`v.index`还是`v.lowlink`对于结果没有任何区别**。 
 - **Back Edge**
   When child `v` is already visited, then minimum of `low[u]` and `Disc[v]` will be updated to `low[u]`. 
`u.lowlink = min(u.lowlink, v.index)`
首先`lowlink`看作是连通分量的序号，如果遇到了回边，说明遇到了一个**新的连通分量**，我们要为这个连通分量分配一个序号，如果使用`v.lowlink`，例如在（5）步中，我们将`F.lowlink`设置为`D.lowlink`，那么就说明我们将`F`合并进了原有连通分量1中。
> 
**也就是说如果使用`u.lowlink = min(u.lowlink, v.lowlink)`，那么节点都是以渐增的方式扩展连通分量的。**
**如果使用`u.lowlink = min(u.lowlink, v.index)`，那么节点是先构成一些列的子连通分量，然后再将这些子连通分量构成更大的连通分量。**
下面我们采用`u.lowlink = min(u.lowlink, v.lowlink)`，再走一遍Tarjan’s算法的过程，如下所示。
![这里写图片描述](https://img-blog.csdn.net/20180626222328552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*注：这是自己一个简单的想法，写了个初步的文档，并没有严格证明。*

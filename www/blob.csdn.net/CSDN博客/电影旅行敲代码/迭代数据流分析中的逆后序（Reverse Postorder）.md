# 迭代数据流分析中的逆后序（Reverse Postorder） - 电影旅行敲代码 - CSDN博客
2016年08月09日 16:54:32[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：1781
## **—————————————-更新———————————-**
Eli Bendersky大神的博客有关于数据流分析各种ordering的分析，大神的 
分析深入浅出，大家请移步[Eli Bendersky’s website](http://eli.thegreenplace.net/2015/directed-graph-traversal-orderings-and-applications-to-data-flow-analysis/#id10)。当然也可以继续看下面的文章
## **—————————————-更新———————————-**
## 迭代数据流分析中的三个要素点
在使用迭代算法进行数据流分析的时候，需要考虑三个方面的问题。
### Termination
这是迭代数据流分析第一个需要考虑的问题，就是迭代过程是否终止。
> 
That combination, monotonicity and bounded size, again guarantees terminitation. -《Engineering a compiler》.p485
上面说明只要数据流问题中的值在迭代过程中是单调的，并且值集合是有限大小，那么迭代算法就会终止。
### Correctness
迭代数据流分析都是按照数据流方程来进行迭代的，正确性也不容怀疑。
### Efficiency
但是迭代数据流分析的效率就没有那么明朗了。迭代过程的顺序不同，效率也会大不相同。
> 
对于很多数据流分析问题而言，我们可以对求值过程进行适当排序，使算法经过很少的迭代就能收敛。-《编译原理》p.425
但是如何安排不同节点的求值过程呢，在《Engineering a Compiler》中给出了一个简单的提示，就是对于前向数据问题（foward-analysis）来讲，逆后序（Reverse Postordering）非常有效。那么什么是逆后序，怎么得到控制流图的逆后序？
## 逆后序
在《Engineering a Compiler》中，逆后序的简单说明如下：
> 
A postorder traversal visits as many of a node’s children as possible, in a consistent order, before visiting the node. (In a cyclic graph, a node’s child may also be its ancestor).
**An reverse postorder(RPO) traversal is the opposite - it visits as many of a node’s predecessors as possible before visiting the node itself.**
按照上面的说法，在前向数据流分析的时候使用逆后序也是可以理解的，在对当前节点（BasicBlock）进行求值之前，当前节点的所有前驱节点（BasicBlock）在本次迭代中都应该求值完成，否则还要放到下次迭代来进行值的更新，这要会增加一次迭代过程。
例如我们要计算下图各节点的支配节点集合（Dominace sets），我们有如下的数据流方程：
![这里写图片描述](https://img-blog.csdn.net/20160809093441952)
CFG如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160809093515479)
Dominance的求解就是一个经典的前向数据流问题，在计算节点n的前驱节点时，需要首先计算出n的前驱节点m的支配集合，然后应用数据流方程进行求值。如果按照节点标号顺序求值的话，求值过程如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160809094342323)
在第一轮中，求得 *B3* 的支配节点集合为 *{0, 1, 2, 3}* ，这个集合得到的值过大，因为在第一轮中没有获得右半部分的信息的约束（节点Dom sets的初始状态，迭代过程就是不断通过前驱节点的intersection，来使结果越来越精确直到得到最精确的保守解）。所以如果我们能够重新安排求值顺序，让 *B5 B6 B8 B7* 在 *B3* 之前进行求值，就能够更快得到正确的信息。
重新安排的求值顺序以及求值过程如下： 
![这里写图片描述](https://img-blog.csdn.net/20160809095505089)
***注：偷个懒就直接copy 《Engineering a Compiler》中的图片了***
注意这里其实只需要一次迭代就足够了，只是需要额外的迭代来确保达到了fixed point，后面我会介绍《编译原理》对这方面的解释。
### 什么是逆后序
刚开始接触逆后序的时候，有一个疑惑，就是逆后序和前序没有区别啊，前序不就是在深度优先遍历的过程中，先执行对父节点的动作，然后执行对子节点的动作。其实这个困惑的原因在于CFG是图而不是树，连DAG图都算不上。关于这个最直观的例子就是R大给出的解释[请教拓扑排序中的一点疑问](https://www.zhihu.com/question/28549004)，R大给出了一个简单的DAG图来阐述原因，并且给出了一个结论。
> 
能够让前序排列满足拓扑排序的单根DAG都是树，多根DAG都是森林（一个子节点有且只有一个父节点）。
并且提到逆后序得到的结果，也是拓扑排序的结果之一，前序不一定满足拓扑排序，所以前序不一定满足逆后序。
### 什么是DAG的拓扑序
[DAG](https://en.wikipedia.org/wiki/Directed_acyclic_graph)的英文全程是Directed Acyclic Graph， 就是有向无环图。DAG在编译原理中有很多应用，例如基本块的优化，依赖性分析（Dependence Analysis）等等。其中也有很多场景需要对DAG进行拓扑排序，例如依赖性分析。
那么什么是DAG的拓扑排序，这里是wiki对DAG拓扑排序的描述。
> 
Every directed acyclic graph has a **toplogical oedering**, an ordering of the vertices such that the **starting endpoint if every edge occurs earlier in the ordering that the ending endpoint of the edge**.
DAG的拓扑排序，就是任意一条边 *n->m*，节点 n 都先于节点 m 。
> 
The existence of such an ordering can be used to characterize DAGs: **a directed graph is a DAG if and only if it has a topological ordering. In general, this ordering is not unique; **
A DAG has a unique topological ordering **if and only if it has a directed path containing all the vertices, in which case the ordering is the same as the order in which the vertices appear in the path.**
上面的描述给出了 两种关于DAG拓扑序的两个结论，其中第二个结论和R大关于DAG拓扑序的部分描述，本质上是相同的。如果DAG有且仅有一条有向路径能够包含所有的节点（其实就是[哈密顿路径](https://en.wikipedia.org/wiki/Hamiltonian_path)），那么该DAG只有一种拓扑序。
#### 依赖性分析（Dependence Analysis）
为了使指令能够更好的利用流水线，编译器通常都有一个阶段就是 “schedule instructions”。在对指令进行排序之前，需要先将指令构造一个DAG图，然后对DAG图进行拓扑排序，得到一个流水线性能最好的拓扑序进行输出。
> 
To schedule instructions to make better use of the pipeline, we first arrange them into a directed acyclic graph(DAG), in which ench node represent an instruction, and each arc represents a *dependence*.
在指令之间存在三种相关，
- 数据相关（真相关，flow dependence）
- 反相关（RAW, anti-dependece，name-dependence 的一种）
- 输出相关（WAW, output-dependence，name-dependence 的一种）
***注：名称相关可以通过寄存器重命名解决，并不表示数据的真实流动。另外还有控制相关，这个对指令的执行效率影响更大。***
指令之间依赖关系构成DAG图，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160809142815678)
右图中的节点标号表示指令标号，虚线表示反相关（anti-dependence），实现表示数据相关，双实线指令中间必须相隔4个时钟周期，为了尽可能减少流水线暂停的周期我们可以在中间插入一下不相关的指令，来填补空闲流水线周期。所以我们可以通过对指令构成的DAG图进行拓扑排序（必须是拓扑排序，否则程序就失去了原有的含义）寻找一种较优的策略。
> 
Any topological sort of a dependence DAG (i.e., any enumeration of the nodes in which each node appears before its children) will represent a correct schedule. **Ideally we should like to choose a sort that minimizes overall delay.**
一种拓扑排序结果如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160809144355521)
其中节点右上角的数字表示拓扑排序后的指令标号，我们可以看出简单的调度就减少了2个周期的delay。
### 如何对DAG的拓扑排序
DAG的拓扑排序有两种比较常见的方法。
第一种，是先找出图中没有入边的顶点，然后将它和它的边从图中删除，然后对剩余部分使用的同样的操作。
第二种，使用深度优先搜索的方法，然后使用逆后序得到拓扑序。
> 
Alternatively, a topological ordering may be constructed by reversing a postorder numbering of a depth-first search graph traversal.
《算法导论》对拓扑排序有介绍，盗图如下：
![这里写图片描述](https://img-blog.csdn.net/20160809150822751)
上图中，后序就是按完成时间来排序，比如虽然 *s* 节点最先访问，但是完成较晚，必须等所有子节点完成之后 *s* 的访问才算完成。那么逆后序就是晚完成的节点排在前面。
## 如何计算CFG的逆后序
在前面我们提到在迭代数据流分析的时候，可以使用逆后序来进行安排求值的过程来使迭代的效率更高。我们在后面的介绍中，提到了逆后序就是一种拓扑序，但是带有环的图是不可能有拓扑排序的，也就是在有环的图中是不能计算拓扑序的。
但是CFG图是带有环的，那么该如何处理图中的环呢？
《编译原理》第九章介绍了如何对CFG进行拓扑排序，但是名字不是拓扑排序而是深度优先排序（*The depth-first ordering*）。
> 
The depth-first ordering of the nodes is the reverse of the order in which we **last visit** the nodes in the preorder traversal.
上面这个描述提到深度优先排序其实逆后序，”last visit”的含义就是节点的完成时间。这篇完整来自于[这里](http://www.cc.gatech.edu/~harrold/6340/cs6340_fall2009/Readings/pp660-680-A.V.%20Aho,%20R.%20Sethi,%20J.D.%20Ullman%20-%20Compilers.%20Principles,%20Techinques,%20and%20Tools.pdf)。
在介绍流图中的循环时，有一句话我觉得特别有意思，如下：
> 
循环也会影响到程序分析所需的时间。如果一个程序不包含任何循环，我们只需要对程序进行一趟扫描就可以得到数据流问题的答案。比如，**一个前向数据流问题只需要按照拓扑次序对所有的节点进行一次访问就可以解决**。
上面的描述隐含的揭示了，在前向数据流问题中如果CFG能够得到拓扑序就可以按照该次序进行一次迭代就可以达到 *fixed poit*。 但是事与愿违，CFG是基本上都是存在环（回边）的，也就是说不存在拓扑排序，所以《Engineering a Compiler》就勉强的使用了逆后序的概念来描述这种较优的顺序。
### 深度优先排序
**一个后序遍历过程，首先递归地从左到右访问一个结点的子结点，然后访问该结点本身。**
**深度优先排序（depth-first ordering），它的顺序正好和后序遍历的顺序相反。也就是说在深度优先排序中，我们首先访问一个结点，然后遍历该节点的最右子节点，然后访问该子节点左边的子节点，依次类推。**
这里会有两个误区： 
误区一：深度优先排序（逆后序）和前序混淆。 
误区二：深度优先排序（逆后序）和广度优先混淆。
误区一产生的原因我们在文章前面已经介绍过了，这里再强调一下。
![这里写图片描述](https://img-blog.csdn.net/20160809154448688)
如上图所示，前序遍历肯定是A B D C 或者 A C D B，因为前序遍历有一个问题就是没有考虑到存在多个“父节点”的情形，只会一股脑的往下冲，可以看到这两个序列都不是拓扑序的。相反后序遍历序列是 D B C A 或者 D C B A，这两个序列的反序是 A C B D 和 A B C D，可以看到这两个都是拓扑序的。
误区二产生的原因，是广度优先比较注重一层一层的展开（使用队列实现），不会考虑到“父子关系”也就是指向关系。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160809155241046)
如果使用广度优先的话，可能的遍历顺序如下： A B C D E G F，注意其中E -> D，但是在广度优先遍历的过程中却没有考虑到这个问题。
但是后序如下：D B F E G C A，逆后序为： A C G E F B D，这是拓扑排序。
关于深度优先排序，《编译原理》给出了一个算法，这个算法可以结算图中带有环的情况。
**算法9.41 深度优先生成树和深度优先排序** （盗用） 
**输入：** 一个流图G 
**输出：** G的一个DFST树T和G中结点的一个深度优先排序 
**方法：**我们使用下图的的递归过程 ***search( n )***。这个算法首先把 ***n***标 “unvisited”，然后调用 ***search( n0 )***，其实 ***n0*** 是入口结点。当它调用 ***search( n )*** 的时候，首先把 ***n*** 标记为 “visited”，以免把 ***n*** 再次加入到树中。它使用 ***c*** 作为计数器，从 ***G*** 的结点总数一直倒计数到1。在算法执行的时候把 ***c*** 的值赋给结点 ***n*** 的深度优先编号 dfn[0]。边的集合 ***T*** 形成了 ***G*** 的深度优先生成树。
![这里写图片描述](https://img-blog.csdn.net/20160809161207526)
算法中，如果判断 ***s*** 已经遍历过了，则不会继续对其进行遍历，这个操作就可以解决循环的问题。另外在算法中，我们是在对结点 n 的所有后继访问过后，才对其赋值的，这也是后序遍历的体现。我们通过下图进行分析：
![这里写图片描述](https://img-blog.csdn.net/20160809162834672)
上边的左侧图是一个CFG图，而右侧是深度优先表示，其中的虚线表示一种回退边，这个回退边是不会显示出来了，因为回退边的结点已经visit过了，所以不会加入到集合中。结点中的标号表示的是排序的结果。
各个结点遍历的顺序如下所示： 
1， 4， 6， 7， 8， 10， 8， 9， 8， 7，6，4，5，3，1， 2，1.
## 迭代数据流算法的收敛速度
至此我们算是搞清楚了迭代数据流分析中逆后序的概念，其实就是深度优先排序，这个顺序在前向数据流问题中可以尽可能合理的安排各个结点的迭代顺序。
如果CFG没有环的话，也就是DAG的话，使用逆后序可以通过一次迭代达到fixed point（算法还需额外的一次迭代来判断是否达到fixed point）。当CFG存在循环的时候，逆后序这个概念在这里就没有意义了，但是“逆后序”也就是深度优先顺序还是可以让数据更快速的收敛。
> 
RPO still guarantees the fastest convergence - in graphs with cycles data-flow analysis is iterative untill fixed point is reached.
另外对于后向数据流问题来说，使用后序来安排各个求值过程即可。
**注：有的资料描述使用应该使用reverse CFG的RPO更合适，[Eli大神](http://eli.thegreenplace.net/2015/directed-graph-traversal-orderings-and-applications-to-data-flow-analysis/)有相关的描述。不好意思，两者的区别我还没有搞懂，所以无能为力。感兴趣的参看知乎上的[提问](https://www.zhihu.com/question/49478107?from=profile_question_card)！**
我们再次给出《编译原理》关于迭代数据流分析的一个描述，
> 
对于很多数据流分析而言，我们可以对求值过程进行适当排序，使算法经过很少的迭代就能收敛。
虽然我们得到了深度优先排序的概念，但是并没有深入了解，循环对数据流分析的影响。“**所有影响一个结点的重要事件都可以通过一个无环的路径到达该点**”，也就是“**带有环的路径不会增加任何内容**”，其中到达定值、可用表达式和活跃变量都具有这个性质。这个特性与具体问题相关，这里我们不再深入挖掘了。

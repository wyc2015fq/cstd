# 决策树vs行为树 - 凯奥斯的注释 - CSDN博客





2017年09月22日 12:40:34[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：5566








原贴地址https://gamedev.stackexchange.com/questions/51693/decision-tree-vs-behavior-tree（可能需要翻墙）






The two are pretty different. The real indicator is in the names. Decision trees are just for making decisions. Behavior trees are for controlling behavior. Allow me to explain. A major difference in the two is the way they are traversed, likewise the way they're laid out and the node 'types' are different.

这俩区别还挺大的。名字里就指出了。决策树就是为了制定决策。行为树是为了控制行为。让我来讲讲。它俩最主要的区别在于遍历的方式，而且布局方式和节点“类型”也不一样。

Decision trees are evaluated from root to leaf, every time. For a decision tree to work properly, the child nodes of each parent must represent all possible decisions for that node. If a node can be answered "Yes, No, Maybe", there must be three children, Yes node, No node and Maybe node. This means there's always some lower node to traverse, until reaching an end node. The traversal is always down. Graphical form:


决策树每一次都从根到叶子分析。为了让决策树正确工作，每个父节点的子节点都要能表达出父节点所有可能的决策。如果一个节点可以应答“是，否，可能”，那么那必须有三个子节点：“是”节点、“否”节点和“可能”节点。这就意味着，在到达结束节点前，总是可以遍历到某个底层节点。这种遍历总是向下的。如图所示：

![](https://img-blog.csdn.net/20170922123850391)


Pretty simple. We start at the root, and based on some evaluation, choose 1, 2 or 3. We choose 3. Then we do some other evaluation and choose B or B... Well I reused the graphic from below, sorry. Pretend the B on the left is magic B.


挺简单的。我们从根节点开始，基于某种分析来选择节点1、2、3。我们选择3，然后再执行另外一种分析，并选择B或B...好吧，这里我重用了下层的图形，抱歉。就当左边的B是个假B吧。




Behavior trees have a different evaluation. The first time they are evaluated (or they're reset) they start from the root (parent nodes act like selectors) and each child is evaluated from left to right. The child nodes are ordered based on their priority. If all of a child node's conditions are met, its behavior is started. When a node starts a behavior, that node is set to 'running', and it returns the behavior. The next time the tree is evaluated, it again checks the highest priority nodes, then when it comes to a 'running' node, it knows to pick up where it left off. The node can have a sequence of actions and conditions before reaching an end state. If any condition fails, the traversal returns to the parent. The parent selector then moves on to the next priority child. I'll attempt a graphical form here:


行为树有不一样的分析方法。第一次分析（或重置后），它们从根节点开始（这里所有的父节点作为选择器），并且从左到右分析每个子节点。这些子节点按优先级排列。如果一个子节点所有的条件都满足，就开始它的行为。当一个节点开始一个行为，这个节点被设置为“运行中”，并返回这个行为。下一次分析这棵树的时候，重新检查最高优先级的节点，当走到“运行中”节点时，它便知道从它暂停的地方继续执行它。在到达结束状态之前，这个节点可以包含一系列的行动和条件。任意条件失败，遍历器便会返回到父节点。父选择器接着移动到下一个优先级的子节点上。我尝试用下图表达：

![](https://img-blog.csdn.net/20170922130032730)







The traversal starts at the root, goes to child 1, checks the child condition (something like "any enemies near by?"). The condition fails, and the traversal moves back up the tree to move on to node two. Node 2 has an action that's performed (maybe something like finding a path). Then a behavior (something like following the path). The following path is set to running and the tree returns its state as running. The nodes that failed or completed are returned to 'Ready'. Then the next time we check, we start again with the highest priority node. It fails again, so we proceed to node two. There we find we have a behavior running. We also find the behavior has completed, so we mark it completed and return that. The tree is then reset and ready to go again.

遍历从根节点开始，走到1号子节点，检查子节点的条件（例如“周围是否有滴人？”）。这个条件失败了，遍历器返回树（根节点）并移动到2号节点。2号节点执行了一个行动（例如寻路）和一个行为（例如沿路线走）。“沿路线走”的行为被设置为“运行中”，并且行为树返回状态“运行中”。失败和完成的节点会返回“待命”。接着下一次我们检查这个树，我们再次从最高优先级的节点开始。它又失败了，所以我们执行到2号节点。在2号节点，我们发现有一个“运行中”的行为。当我们发现这个行为完成了，那么我们把它标记成“完成”并返回。然后这颗行为树就被重置了，并准备好再次开始。

As you can see the behavior trees are more complex. Behavior trees are more powerful and allow for more complex behavior. Decision trees are easy to understand and simple to implement. So, you'd use behavior trees when you want more complex behavior, or more control over the behavior. Decision trees can be used as part of a behavior tree, or used alone for simple AI.

你会发现行为树（比决策树）更复杂。行为树（比决策树）更牛逼并且允许更复杂的行为。决策树易于理解和实现。所以，如果你需要更复杂的行为和更多对行为的控制，那么你可以使用行为树。决策树可以被作为行为树的一部分，或者单独使用来实现简单的AI。

Some good understanding of how behavior trees are parsed can be found [here](http://www.altdevblogaday.com/2011/02/24/introduction-to-behavior-trees/).

对于行为树如何解析的好的解释可以在[这里](http://www.altdevblogaday.com/2011/02/24/introduction-to-behavior-trees/)找到（链接可能已失效）。





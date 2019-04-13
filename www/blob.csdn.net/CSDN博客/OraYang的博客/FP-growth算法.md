
# FP-growth算法 - OraYang的博客 - CSDN博客

2017年11月08日 21:09:50[OraYang](https://me.csdn.net/u010665216)阅读数：344所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



demo 代码：[传送门](https://gitee.com/orayang_admin/FP-growth/tree/master)
## 引言
上次分享[Apriori算法](http://blog.csdn.net/u010665216/article/details/78470307)时，我们有提到Apriori算法在每次增加频繁项集的大小时，会重新扫描整个数据集。当数据集很大时，这会显著降低频繁项集发现的速度。而本次分享的FP-growth（frequent patten）算法就能高效地发现频繁项集。
那么在现实生活中，是否存在应用FP-growth算法的产品呢？答案是存在的，如下图所示：
![why](https://img-blog.csdn.net/20171108185520574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171108185520574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图中，我们在Google搜索引擎里输入why这个单词，搜索引擎会自动给我们一些推荐查询词项，这些查询词项来自互联网上经常出现的词对。查找的方法正是接下来我们要分享的FP-growth算法。
[

](https://img-blog.csdn.net/20171108185520574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## FP-growth与Apriori的区别
[
](https://img-blog.csdn.net/20171108185520574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)我们先从宏观上来看看两个算法的区别：
[
](https://img-blog.csdn.net/20171108185520574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> FP-growth算法只需要对数据库进行两次扫描，而Apriori算法对每个潜在的频繁项集都会扫描数据集判定给定模式是否频繁，因此FP-growth算法的速度要比Apriori算法快。在小规模数据集上，这不是什么问题，但当处理更大数据集时，FP-growth通常性能要比Apriori好两个数量级以上。

> Apriori是发现频繁项集与关键规则的算法，但是FP-growth并不能用于发现关联规则。

> FP-growth基本流程

> FP-growth只会扫描数据集两次，它在发现频繁项集的基本过程如下：

> 1.构建FP树

> 2.从FP树中挖掘频繁项集

> FP树

> 我们先来看个例子，下图有一份数据样例：
![tid-example](https://img-blog.csdn.net/20171108191217613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171108191217613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 移除非频繁项后的数据样例：
![remove](https://img-blog.csdn.net/20171215162245031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171215162245031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 这份数据左侧代表事务ID，右侧代表事务中的元素项。那么根据上述数据样例生成的FP树如下所示：
![fp-tree](https://img-blog.csdn.net/20171108191405033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171108191405033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 与搜索树不同的是，一个元素项可以在一棵FP树中出现多次。其中树节点给出的是集合中单个元素及其在序列中的出现次数，路径会给出该序列出现的次数。带箭头的链接叫做节点链接，表示相似项之间的链接。

> [
](https://img-blog.csdn.net/20171108191405033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 构建FP树的工作流程

> [
](https://img-blog.csdn.net/20171108191405033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 那么我们来详细说下该算法的工作流程。

> 为构建FP树，需要对原始数据集扫描两遍。第一遍对所有元素项的出现次数进行计数。并应用上次分享的Apriori原理，如果某个元素是不频繁的，那么包含该元素的超集也是不频繁的，所以不需要考虑这些超集。因此数据集的第一次扫描是用来统计元素出现的频率的。第二次扫描就只考虑剩余的频繁元素。

> 因此FP树构建部分流程如下所示：
![construct](https://img-blog.csdn.net/20171215162531338?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 构建FP树

> 第二次扫描FP树就需要构建FP树了。因为FP树比较复杂，因此创建一个类来实现保存树的每个节点。

> 代码如下：

> class
> treeNode
> :
> def
> __init__
> (self, nameValue, numOccur, parentNode)
> :
> self.name = nameValue
        self.count = numOccur
        self.nodeLink =
> None
> self.parent = parentNode
> \#needs to be updated
> self.children = {}
> def
> inc
> (self, numOccur)
> :
> self.count += numOccur
> def
> disp
> (self, ind=
> 1
> )
> :
> print(
> '  '
> *ind, self.name,
> ' '
> , self.count)
> for
> child
> in
> self.children.values():
            child.disp(ind+
> 1
> )
> 上面的程序给出了FP树种节点的类的定义。

> 除了上图FP树之外，还需要建立一个头指针表用来快速访问FP树中一个给定类型的所有元素。如下图所示：
![headTable](https://img-blog.csdn.net/20171108193435628?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171108193435628?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 上图中的Header Table即头指针表，这里使用一个字典来作为数据结构存储头指针表。

> 接下来第二次扫描数据集并构建FP树之前，需要将每个集合的元素进行排序，从而保证相同项只会表现一次。

> FP构建函数如下所示：

> def
> createTree
> (dataSet, minSup=
> 1
> )
> :
> \#create FP-tree from dataset but don't mine
> headerTable = {}
> \#go over dataSet twice
> for
> trans
> in
> dataSet:
> \#first pass counts frequency of occurance
> for
> item
> in
> trans:
            headerTable[item] = headerTable.get(item,
> 0
> ) + dataSet[trans]
> for
> k
> in
> headerTable.keys():
> \#remove items not meeting minSup
> if
> headerTable[k] < minSup:
> del
> (headerTable[k])
    freqItemSet = set(headerTable.keys())
> \#print 'freqItemSet: ',freqItemSet
> if
> len(freqItemSet) ==
> 0
> :
> return
> None
> ,
> None
> \#if no items meet min support -->get out
> for
> k
> in
> headerTable:
        headerTable[k] = [headerTable[k],
> None
> ]
> \#reformat headerTable to use Node link
> \#print 'headerTable: ',headerTable
> retTree = treeNode(
> 'Null Set'
> ,
> 1
> ,
> None
> )
> \#create tree
> for
> tranSet, count
> in
> dataSet.items():
> \#go through dataset 2nd time
> localD = {}
> for
> item
> in
> tranSet:
> \#put transaction items in order
> if
> item
> in
> freqItemSet:
                localD[item] = headerTable[item][
> 0
> ]
> if
> len(localD) >
> 0
> :
            orderedItems = [v[
> 0
> ]
> for
> v
> in
> sorted(localD.items(), key=
> lambda
> p: p[
> 1
> ], reverse=
> True
> )]
            updateTree(orderedItems, retTree, headerTable, count)
> \#populate tree with ordered freq itemset
> return
> retTree, headerTable
> \#return tree and header table
> def
> updateTree
> (items, inTree, headerTable, count)
> :
> if
> items[
> 0
> ]
> in
> inTree.children:
> \#check if orderedItems[0] in retTree.children
> inTree.children[items[
> 0
> ]].inc(count)
> \#incrament count
> else
> :
> \#add items[0] to inTree.children
> inTree.children[items[
> 0
> ]] = treeNode(items[
> 0
> ], count, inTree)
> if
> headerTable[items[
> 0
> ]][
> 1
> ] ==
> None
> :
> \#update header table
> headerTable[items[
> 0
> ]][
> 1
> ] = inTree.children[items[
> 0
> ]]
> else
> :
            updateHeader(headerTable[items[
> 0
> ]][
> 1
> ], inTree.children[items[
> 0
> ]])
> if
> len(items) >
> 1
> :
> \#call updateTree() with remaining ordered items
> updateTree(items[
> 1
> ::], inTree.children[items[
> 0
> ]], headerTable, count)
> def
> updateHeader
> (nodeToTest, targetNode)
> :
> \#this version does not use recursion
> while
> (nodeToTest.nodeLink !=
> None
> ):
> \#Do not use recursion to traverse a linked list!
> nodeToTest = nodeToTest.nodeLink
    nodeToTest.nodeLink = targetNode
> 上述代码中包含三个函数。第一个函数createTree()是用来构建FP树。可以从代码中看到两个嵌套for循环代表遍历数据集两次。updateTree()及updateHeader()都是用来更新FP树及头指针表的。

> 实验结果如下：

> simpDat = fpGrowth.loadSimpDat()
simpDat
> [['r', 'z', 'h', 'j', 'p'],
 ['z', 'y', 'x', 'w', 'v', 'u', 't', 's'],
 ['z'],
 ['r', 'x', 'n', 'o', 's'],
 ['y', 'r', 'x', 'z', 'q', 't', 'p'],
 ['y', 'z', 'x', 'e', 'q', 's', 't', 'm']]
> initSet = fpGrowth.createInitSet(simpDat)
initSet
> {frozenset({'z'}): 1,
 frozenset({'h', 'j', 'p', 'r', 'z'}): 1,
 frozenset({'s', 't', 'u', 'v', 'w', 'x', 'y', 'z'}): 1,
 frozenset({'n', 'o', 'r', 's', 'x'}): 1,
 frozenset({'p', 'q', 'r', 't', 'x', 'y', 'z'}): 1,
 frozenset({'e', 'm', 'q', 's', 't', 'x', 'y', 'z'}): 1}
> importlib.reload(fpGrowth)
myFPtree,myHeaderTab = fpGrowth.createTree(initSet,
> 3
> )
myFPtree.disp()
> Null Set   1
     z   5
       r   1
       x   3
         s   2
           t   2
             y   2
         t   1
           r   1
             y   1
     x   1
       s   1
         r   1
> 从一棵FP树中挖掘出频繁项集

> 这里挖掘频繁项集的思路与Apriori算法类似，首先从单元素项开始，然后在此基础上逐步构建更大的集合。因为整个挖掘频繁项集的过程是围绕FP树进行的，因此不再需要原始数据集了。

> 从FP树中抽取频繁项集的三个基本步骤如下：

> 从FP树中获得条件模式基；

> 利用条件模式基，构建一个条件FP树；

> 迭代重复步骤（1）步骤（2），直到树包含一个元素项为止。

> 条件模式基

> 条件模式基是以所查找元素项为结尾的路径集合。每一条路径其实都是一条前缀路径。下图给出了每个频繁项的所有前缀路径。
![prefix-path](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 从上图我们可以发现获得前缀路径的方式：利用先前创建的头指针，来遍历并回溯。

> [

](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 条件FP树

> [
](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 对于每一个频繁项，都要创建一棵条件FP树。然后在构建条件FP的过程中，再将不满足最小支持度的元素项去掉。具体代码如下：

> [

](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> def
> mineTree
> (inTree, headerTable, minSup, preFix, freqItemList)
> :
> bigL = [v[
> 0
> ]
> for
> v
> in
> sorted(headerTable.items(), key=
> lambda
> p: p[
> 1
> ])]
> \#(sort header table)
> for
> basePat
> in
> bigL:
> \#start from bottom of header table
> newFreqSet = preFix.copy()
        newFreqSet.add(basePat)
> \#print 'finalFrequent Item: ',newFreqSet    \#append to set
> freqItemList.append(newFreqSet)
        condPattBases = findPrefixPath(basePat, headerTable[basePat][
> 1
> ])
> \#print 'condPattBases :',basePat, condPattBases
> \#2. construct cond FP-tree from cond. pattern base
> myCondTree, myHead = createTree(condPattBases, minSup)
> \#print 'head from conditional tree: ', myHead
> if
> myHead !=
> None
> :
> \#3. mine cond. FP-tree
> \#print 'conditional tree for: ',newFreqSet
> \#myCondTree.disp(1)
> mineTree(myCondTree, myHead, minSup, newFreqSet, freqItemList)
> [
](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 运行效果：

> [

](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> importlib.reload(fpGrowth)
myFPtree,myHeaderTab = fpGrowth.createTree(initSet,
> 3
> )
myFPtree.disp()
> [
](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> Null Set   1
     z   5
       r   1
       x   3
         s   2
           t   2
             y   2
         t   1
           r   1
             y   1
     x   1
       s   1
         r   1
> [

](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> importlib.reload(fpGrowth)
freqItems = []
fpGrowth.mineTree(myFPtree,myHeaderTab,
> 3
> ,set([]),freqItems)
> [
](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> finalFrequent Item:  {'r'}
condPattBases : r {frozenset({'z'}): 1, frozenset({'x', 's'}): 1, frozenset({'t', 'x', 'z'}): 1}
head from conditional tree:  None
finalFrequent Item:  {'s'}
condPattBases : s {frozenset({'x', 'z'}): 2, frozenset({'x'}): 1}
head from conditional tree:  {'x': [3, <fpGrowth.treeNode object at 0x000001DEEEFA65C0>]}
conditional tree for:  {'s'}
   Null Set   1
     x   3
finalFrequent Item:  {'x', 's'}
condPattBases : x {}
head from conditional tree:  None
finalFrequent Item:  {'t'}
condPattBases : t {frozenset({'x', 'z', 's'}): 2, frozenset({'x', 'z'}): 1}
head from conditional tree:  {'x': [3, <fpGrowth.treeNode object at 0x000001DEEEFA0240>], 'z': [3, <fpGrowth.treeNode object at 0x000001DEEEFA0278>]}
conditional tree for:  {'t'}
   Null Set   1
     x   3
       z   3
finalFrequent Item:  {'t', 'x'}
condPattBases : x {}
head from conditional tree:  None
finalFrequent Item:  {'t', 'z'}
condPattBases : z {frozenset({'x'}): 3}
head from conditional tree:  {'x': [3, <fpGrowth.treeNode object at 0x000001DEEEFA5128>]}
conditional tree for:  {'t', 'z'}
   Null Set   1
     x   3
finalFrequent Item:  {'t', 'x', 'z'}
condPattBases : x {}
head from conditional tree:  None
finalFrequent Item:  {'x'}
condPattBases : x {frozenset({'z'}): 3}
head from conditional tree:  {'z': [3, <fpGrowth.treeNode object at 0x000001DEEEFA5358>]}
conditional tree for:  {'x'}
   Null Set   1
     z   3
finalFrequent Item:  {'x', 'z'}
condPattBases : z {}
head from conditional tree:  None
finalFrequent Item:  {'y'}
condPattBases : y {frozenset({'t', 'x', 'z', 's'}): 2, frozenset({'t', 'x', 'z', 'r'}): 1}
head from conditional tree:  {'t': [3, <fpGrowth.treeNode object at 0x000001DEEEFA5F28>], 'x': [3, <fpGrowth.treeNode object at 0x000001DEEEFA5F60>], 'z': [3, <fpGrowth.treeNode object at 0x000001DEEEFA5F98>]}
conditional tree for:  {'y'}
   Null Set   1
     t   3
       x   3
         z   3
finalFrequent Item:  {'t', 'y'}
condPattBases : t {}
head from conditional tree:  None
finalFrequent Item:  {'y', 'x'}
condPattBases : x {frozenset({'t'}): 3}
head from conditional tree:  {'t': [3, <fpGrowth.treeNode object at 0x000001DEEEFB4080>]}
conditional tree for:  {'y', 'x'}
   Null Set   1
     t   3
finalFrequent Item:  {'t', 'y', 'x'}
condPattBases : t {}
head from conditional tree:  None
finalFrequent Item:  {'y', 'z'}
condPattBases : z {frozenset({'t', 'x'}): 3}
head from conditional tree:  {'t': [3, <fpGrowth.treeNode object at 0x000001DEEEFB4C50>], 'x': [3, <fpGrowth.treeNode object at 0x000001DEEEFB4C88>]}
conditional tree for:  {'y', 'z'}
   Null Set   1
     t   3
       x   3
finalFrequent Item:  {'t', 'y', 'z'}
condPattBases : t {}
head from conditional tree:  None
finalFrequent Item:  {'y', 'z', 'x'}
condPattBases : x {frozenset({'t'}): 3}
head from conditional tree:  {'t': [3, <fpGrowth.treeNode object at 0x000001DEEEF8C860>]}
conditional tree for:  {'y', 'z', 'x'}
   Null Set   1
     t   3
finalFrequent Item:  {'t', 'y', 'z', 'x'}
condPattBases : t {}
head from conditional tree:  None
finalFrequent Item:  {'z'}
condPattBases : z {}
head from conditional tree:  None
> [

](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 总结

> [
](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> FP-growth算法是一种发现数据集中频繁模式的有效方法。可以使用FP-growth在多种文档中查找频繁单词。频繁项集的生成有着许多应用，比如购物交易、医学诊断及大气研究等。

> [            ](https://img-blog.csdn.net/20171108201936418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


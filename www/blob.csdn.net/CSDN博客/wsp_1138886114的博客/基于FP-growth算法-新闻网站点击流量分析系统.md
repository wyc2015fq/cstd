# 基于FP-growth算法-新闻网站点击流量分析系统 - wsp_1138886114的博客 - CSDN博客





2019年03月08日 10:58:59[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：39








![这里写图片描述](https://img-blog.csdn.net/20180612213159200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 从新闻网站点击流量中挖掘

在新闻网站上，一个会用户不断点击和浏览各种新闻报道，最终该用户的这些点击会被记录下来，成为该用户点击的历史记录。而所有用户的点击历史记录，是一个蕴含了巨大价值的数据集。

我们可以从各个角度，使用不同的方法来进行有价值的挖掘，并应用到不同的任务中去。

本项目就是从用户点击的历史记录中，挖掘频繁项集，以便知道哪些新闻报道是经常关联在一起的。从而为网站的新闻版面排版，新闻推荐等提供科学的参考。

以下代码使用 挖掘频繁项集：[关联规则理论请点击](https://blog.csdn.net/wsp_1138886114/article/details/80921905)

数据集下载：博客下载地址：[https://download.csdn.net/download/wsp_1138886114/10606351](https://download.csdn.net/download/wsp_1138886114/10606351)
```python
#!/usr/bin/python
# coding:utf8
print(__doc__)


class treeNode:
    def __init__(self, nameValue, numOccur, parentNode):
        self.name = nameValue
        self.count = numOccur
        self.nodeLink = None
        self.parent = parentNode  # needs to be updated
        self.children = {}

    def inc(self, numOccur):      #inc(对count变量增加给定值)
        self.count += numOccur

    def disp(self, ind=1):        #disp(用于将树以文本形式显示)
        print('  '*ind, self.name, ' ', self.count)
        for child in self.children.values():
            child.disp(ind+1)
            
    def __lt__(self, other):
        return self.count < other.count

def loadSimpDat():
    simpDat = [['r', 'z', 'h', 'j', 'p'],
               ['z', 'y', 'x', 'w', 'v', 'u', 't', 's'],
               ['z'],
               ['r', 'x', 'n', 'o', 's'],
               ['y', 'r', 'x', 'z', 'q', 't', 'p'],
               ['y', 'z', 'x', 'e', 'q', 's', 't', 'm']]
    return simpDat


def createInitSet(dataSet):        #创建初始化集合
    retDict = {}
    for trans in dataSet:
        if frozenset(trans) not in retDict:
            retDict[frozenset(trans)] = 1
        else:
            retDict[frozenset(trans)] += 1
    return retDict


def updateHeader(nodeToTest, targetNode):  #更新头指针
    """updateHeader(更新头指针，建立相同元素之间的关系，
    例如： 左边的r指向右边的r值，就是后出现的相同元素 指向 已经出现的元素)
    从头指针的nodeLink开始，一直沿着nodeLink直到到达链表末尾。这就是链表。
    性能：如果链表很长可能会遇到迭代调用的次数限制。
    Args:
        nodeToTest  满足minSup {所有的元素+(value, treeNode)}
        targetNode  Tree对象的子节点
    """
    while (nodeToTest.nodeLink is not None):
        nodeToTest = nodeToTest.nodeLink
    nodeToTest.nodeLink = targetNode


def updateTree(items, inTree, headerTable, count): #更新树
    """updateTree(更新FP-tree，第二次遍历)
      针对每一行的数据
      最大的key,  添加
    Args:
        items       满足minSup 排序后的元素key的数组（大到小的排序）
        inTree      空的Tree对象
        headerTable 满足minSup {所有的元素+(value, treeNode)}
        count       原数据集中每一组Kay出现的次数

        取出 元素 出现次数最高的
        如果该元素在 inTree.children 这个字典中，就进行累加
        如果该元素不存在 就 inTree.children 字典中新增key，value为初始化的 treeNode 对象
    """
    """
    更新 最大元素，对应的 treeNode 对象的count进行叠加
         如果不存在子节点，我们为该inTree添加子节点
         
    如果满足minSup的dist字典的value值第二位为null， 就设置该元素为本节点对应的tree节点
    如果元素第二位不为null，我们就更新header节点
    
    """
    if items[0] in inTree.children:
        inTree.children[items[0]].inc(count)
    else:
        inTree.children[items[0]] = treeNode(items[0], count, inTree)
        if headerTable[items[0]][1] is None:   # headerTable只记录第一次节点出现的位置
            headerTable[items[0]][1] = inTree.children[items[0]]
        else:                                  # 本质上是修改headerTable的key对应的Tree，的nodeLink值
            updateHeader(headerTable[items[0]][1], inTree.children[items[0]])
    if len(items) > 1:
        # 递归的调用，在items[0]的基础上，添加item0[1]做子节点，
        # count只要循环的进行累计加和而已，统计出节点的最后的统计值。
        updateTree(items[1:], inTree.children[items[0]], headerTable, count)


def createTree(dataSet, minSup=1):
    """createTree(生成FP-tree)
    Args:
        dataSet  dist{行：出现次数}的样本数据
        minSup   最小的支持度
    Returns:
        retTree  FP-tree
        headerTable 满足minSup {所有的元素+(value, treeNode)}

        支持度>=minSup的dist{所有元素：出现的次数}
        循环 dist{行：出现次数}的样本数据
        对所有的行进行循环，得到行里面的所有元素
        统计每一行中，每个元素出现的总次数
        例如： {'ababa': 3}  count(a)=3+3+3=9   count(b)=3+3=6
        删除 headerTable中，元素次数<最小支持度的元素
    """
    headerTable = {}
    for trans in dataSet:
        for item in trans:
            headerTable[item] = headerTable.get(item, 0) + dataSet[trans]
    temp_keys = list(headerTable.keys())
    for k in temp_keys:
        if headerTable[k] < minSup:
            del(headerTable[k])

    freqItemSet = set(headerTable.keys())  # 满足minSup: set(各元素集合)
    if len(freqItemSet) == 0:              # 如果不存在，直接返回None
        return None, None
    for k in headerTable:                  # 格式化：dist{元素key: [元素次数, None]}
        headerTable[k] = [headerTable[k], None]


    """
        开始create tree
        循环 dist{行：出现次数}的样本数据
        print 'tranSet, count=', tranSet, count
        localD = dist{元素key: 元素总出现次数}
        判断是否在满足minSup的集合中
        print 'headerTable[item][0]=', headerTable[item][0], headerTable[item]
        print 'localD=', localD
    """

    retTree = treeNode('Null Set', 1, None)
    for tranSet, count in dataSet.items():
        localD = {}
        for item in tranSet:
            if item in freqItemSet:
                localD[item] = headerTable[item][0]
        """
        p=key,value; 所以是通过value值的大小，进行从大到小进行排序
        orderedItems 表示取出元组的key值，也就是字母本身，但是字母本身是大到小的顺序
        print 'orderedItems=', orderedItems, 'headerTable', headerTable, 
        填充树，通过有序的orderedItems的第一位，进行顺序填充 第一层的子节点。
        """
        if len(localD) > 0:
            orderedItems = [v[0] for v in sorted(localD.items(), key=lambda p: p[1], reverse=True)]
            updateTree(orderedItems, retTree, headerTable, count)
    return retTree, headerTable


def ascendTree(leafNode, prefixPath):
    """ascendTree(如果存在父节点，就记录当前节点的name值)
    Args:
        leafNode   查询的节点对于的nodeTree
        prefixPath 要查询的节点值
    """
    if leafNode.parent is not None:
        prefixPath.append(leafNode.name)
        ascendTree(leafNode.parent, prefixPath)


def findPrefixPath(basePat, treeNode):
    """findPrefixPath 基础数据集
    Args:
        basePat  要查询的节点值
        treeNode 查询的节点所在的当前nodeTree
    Returns:
    condPats 对非basePat的倒叙值作为key,赋值为count数
    """
    """
        对 treeNode的link进行循环
        寻找改节点的父节点，相当于找到了该节点的频繁项集
        避免 单独`Z`一个元素，添加了空节点 
        对非basePat的倒叙值作为key,赋值为count数
        prefixPath[1:] 变frozenset后，字母就变无序了
        condPats[frozenset(prefixPath)] = treeNode.count
        递归，寻找改节点的下一个 相同值的链接节点
        print treeNode
    """
    condPats = {}

    while treeNode is not None:
        prefixPath = []
        ascendTree(treeNode, prefixPath)
        if len(prefixPath) > 1:
            condPats[frozenset(prefixPath[1:])] = treeNode.count
        treeNode = treeNode.nodeLink
    return condPats


def mineTree(inTree, headerTable, minSup, preFix, freqItemList):
    """mineTree(创建条件FP树)
    Args:
        inTree       myFPtree
        headerTable  满足minSup {所有的元素+(value, treeNode)}
        minSup       最小支持项集
        preFix       preFix为newFreqSet上一次的存储记录，一旦没有myHead，就不会更新
        freqItemList 用来存储频繁子项的列表
    """
    """
    通过value进行从小到大的排序， 得到频繁项集的key
    最小支持项集的key的list集合
    循环遍历 最频繁项集的key，从小到大的递归寻找对应的频繁项集
    preFix为newFreqSet上一次的存储记录，一旦没有myHead，就不会更新
    """

    bigL = [v[0] for v in sorted(headerTable.items(), key=lambda p: p[1])]
    print('-----', sorted(headerTable.items(), key=lambda p: p[1]))
    print('bigL=', bigL)

    for basePat in bigL:
        newFreqSet = preFix.copy()
        newFreqSet.add(basePat)
        print('newFreqSet=', newFreqSet, preFix)

        freqItemList.append(newFreqSet)
        print('freqItemList=', freqItemList)
        condPattBases = findPrefixPath(basePat, headerTable[basePat][1])
        print('condPattBases=', basePat, condPattBases)

        # 构建FP-tree
        myCondTree, myHead = createTree(condPattBases, minSup)
        print('myHead=', myHead)
        # 挖掘条件 FP-tree, 如果myHead不为空，表示满足minSup {所有的元素+(value, treeNode)}
        if myHead is not None:
            myCondTree.disp(1)
            print('\n\n\n')
            # 递归 myHead 找出频繁项集
            mineTree(myCondTree, myHead, minSup, newFreqSet, freqItemList)
        print('\n\n\n')



if __name__ == "__main__":
    simpDat = loadSimpDat()          # load样本数据
    initSet = createInitSet(simpDat) #格式化-样本数据,并统计：{行：出现次数}
    print(initSet)

    """
    创建FP树
    输入：dist{行：出现次数}的样本数据  和  最小的支持度
    输出：最终的PF-tree，通过循环获取第一层的节点，
          然后每一层的节点进行递归的获取每一行的字节点，也就是分支。
          然后所谓的指针，就是后来的指向已存在的
    
    抽取条件模式基
    查询树节点的，频繁子项
    """
    myFPtree, myHeaderTab = createTree(initSet, 3)
    myFPtree.disp()

    print('x --->', findPrefixPath('x', myHeaderTab['x'][1]))
    print('z --->', findPrefixPath('z', myHeaderTab['z'][1]))
    print('r --->', findPrefixPath('r', myHeaderTab['r'][1]))

    # 创建条件模式基
    freqItemList = []
    mineTree(myFPtree, myHeaderTab, 3, set([]), freqItemList)
    print(freqItemList)
	
    # 新闻网站点击流中挖掘，例如：文章1阅读过的人，还阅读过什么？
    parsedDat = [line.split() for line in open('kosarak.dat').readlines()]
    initSet = createInitSet(parsedDat)
    myFPtree, myHeaderTab = createTree(initSet, 100000)

    myFreList = []
    mineTree(myFPtree, myHeaderTab, 100000, set([]), myFreList)
    print(myFreList)
```





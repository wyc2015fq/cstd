# Apriori算法解析 - guoziqing506的博客 - CSDN博客





2017年03月09日 10:46:22[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：9053
所属专栏：[机器学习经典算法研究](https://blog.csdn.net/column/details/25189.html)









## 背景介绍

维克多迈尔在《大数据时代》中，提出了大数据时代跟传统的信息时代相比，最本质的三个思维变革：1. 要全体数据，而不仅是样本；2. 要混杂，而不要效率偏低的精确；3. 要相关关系，而不是因果关系。这第三条说的就是数据挖掘中，最基础，最简单，也是最为重要的应用——数据相关关系的挖掘。相关关系，其实是数据中蕴含的最直接的知识，而对这种相关关系的挖掘，如今也早已应用到推荐系统，个性化检索，机器学习，以及很多更加高级的领域。所以说，相关关系的挖掘，第一，它极为重要，它几乎是数据挖掘和传统数据分析侧重点的分水岭，在如今这个数据时代，它是最重要也是最基本的数据技能；第二，它不难，一般的相关关系挖掘，不需要太过精深的理论；第三，它很普及，已经渗入了生活的方方面面。而这个问题入门级的算法，就是本文要说的Apriori算法，也叫“先验算法”。

当然，Apriori算法虽然本身不难，也容易理解，但是还是有必要学习一下它的产生思路。一来能有个更深入的认识，二来，也算是对数据相关关系基本特征有个理解。所以，我将用较大的一个篇幅，说明Apriori的相关背景。这一点，我觉得比学习算法本身，更有意义。

这首先得从生活中最普通的购物篮说起，我们去买东西，经常把一下商品放在一起购买，比如，我去买红酒，可能会连带着酒杯一起，我去买被子，可能会连带着枕头一起。因为，这些东西其实背后是存在着某种关联的。当然，我们不妨像维克多迈尔说的那样，先不用去管这样东西背后到底是为什么关联起来的，这是科学家和哲学家想的事情，作为商店的老板，你需要想的，只是知道什么东西之间存在关联就可以了。这样，就能通过对商店里商品的摆放，提高你的营业额。一个著名的例子是沃尔玛超市的“啤酒”和“尿布”。

购物篮的例子，可以用来说明两个问题：
- 在很多生活实例中，确实事物之间会存在某种关系，或强或弱，而如果我们能通过计算发现这种关联，那就太有用了！
- 发现这种关联的一个前提条件，就是我得知道哪些东西是经常在一起出现的。

上面两点，就是Apriori算法产生的原因。

## 相关概念

下面为了说明方便，会给出一些概念，当然，这些概念我不想给出很精确的定义，因为你在任何一个搜索引擎都能查到。我只是给出最容易理解的解释。
- 项(item): 项指的是具体的一件东西，比如在购物篮的例子中，你的购物篮里面的大米，被子，红酒等等商品都是项；
- 项集(itemset): 顾名思义，项的集合，由一个或多个项组成的一个整体，我们把由$k$个项组成的项集叫$k$项集；
- 事务(transaction)：一个事务，可以看做是发生的一次事件，比如一个人一次的购物清单，用符号$T$表示，一般来说，$T$包含一个它本身的身份——$TID$以及事务中的一个项集。当然，如果我们收集很多这样的清单，构成一个数据库，这个数据库就能作为我们挖掘计算的依据了。这个库，用符号$D$表示。
- 关联规则：表示实体之间相关关系，用蕴含式$A \Rightarrow B$表示A和B之间相关关系（A和B可以是两个项，也可以是两个项集），关联规则包含支持度(support)和置信度(confidence)两个层面指标；
- 支持度(support)和置信度(confidence)：这两个标准在数据相关关系中特别重要，我们判断两个实体之间是否存在相关关系，依据就是这两个标准。下面分开来说。假设现在的问题是判断A和B之间相关关系的强弱； 
- 支持度(support)：说的是所有事务中，A和B同时出现的次数与总的事务数的比例。换个说法，现实数据中，支持A和B这种关联的比例。用以下公式计算：

$\begin{equation}support(A \Rightarrow B) = P(A\cup B)\end{equation}$ 其中，$P(A\cup B)$的意思是事务中同时包含A和B的比例；
- 置信度(confidence)：$A \Rightarrow B$的置信度说的是包含A的事务中，同时也包含B的事务所占的比例。用以下公式计算： 


$\begin{equation}confidence(A \Rightarrow B) = P(B|A)\end{equation}$ 比如说，总共100个事务，有50个包含A，而这50个事务当中，又有20个同时也包含B，那么，$confidence(A \Rightarrow B) = 40\%$

了解了支持度(support)和置信度(confidence)两个概念，那么不妨可以再深入一步，拓展一个概念：绝对支持度；- 支持度计数（绝对支持度）：绝对支持度又叫支持度计数，频度或计数。上面我们定义的支持度其实也可以叫“相对支持度”，而绝对支持度则说的是一个实体出现的次数，比如：support(A) = A在全体事务数据库$D$中出现的次数。这个概念很重要，因为依靠支持度计数我们就能通过$support(A)$和$support(A\cup B)$来计算置信度： 


$\begin{equation}confidence(A \Rightarrow B) = P(B|A) = \frac{support(A\cup B)}{support(A)}\end{equation}$
- 强规则：我们将实体之间的关联规则定义为强规则，如果实体之间的相对支持度(support)和置信度(confidence)满足我们预定义的最小支持度阈值(min_sup)和最小置信度阈值(min_conf)。换句话说，只要我们在上面的概念5中定义的两个指标都满足，那么，实体之间就是极有强（关联）规则的；
- 频繁项集：指的是频繁在事务中出现的项集，所谓“频繁”的标准就是这个项集出现的次数满足最小支持度计数（阈值）。

## 算法原理

好了，到此，将5，6，7，8四个概念统一起来，我们可以得到一个结论：实体间关联规则的强弱可以通过它们的相对支持度和置信度决定，而这两个指标又可以通过绝对支持度：$support(A)$和$support(A\cup B)$来计算。所以说，关联规则挖掘也就可以转化为频繁项集的挖掘。

这样，关联规则的挖掘是一个两步的过程：
- 找出所有的频繁项集：根据相对支持度，置信度的定义可知，任意两个实体之间如果存在强关联规则，那么一定存在于频繁项集之中，反之，如果这两个实体不存在于频繁项集，则一定不会产生强关联规则
- 由频繁项集产生强关联规则：计算支持度和置信度，找到实体间的强规则

显然，当我们确定了要分析的实体之后，第二步的开销就很小了。关键是第一步：挖掘频繁项集。而Apriori算法解决的就是这个问题。

Apriori翻译成中文是“先验”，所以，不难想到，先验性质就是整个Apriori的核心。

**定理1**：先验性质：频繁项集的所有非空子集也一定是频繁的。

**说明**：这个概念很容易理解了，比如一个项集$\{I_1, I_2, I_3\}$是频繁的，那么，说明这三个项同时出现的次数是大于最小支持度计数的，所以，我们可以推知，他的任何非空子集，$\{I_1\}$, $\{I_2, I_3\}$等等的支持度计数也一定比预先定义的阈值要大，故而都是频繁的。

反过来，我们可以换个角度来思考这个问题，如果一个项集$I$是频繁的，那么给这个项集在加一个项$A$，则这个新的项集$\{I\cup A\}$则至少不会比$I$更加频繁，因为加了东西，所以项集中所有项同时出现的次数一定不会增加。

进一步思考可以得到这样一个结论：如果项集$I$是非频繁的，那么无论给它增加什么项，多少项，他都不会变成频繁项集。这种特殊的性质，也叫“反单调性”。我们将这种“反单调性”换个说法，写成下面的定理2：

**定理2**：反单调性：一个项集，如果有至少一个非空子集是非频繁的，那么这个项集一定是非频繁的。

正是利用了上面的定理1，定理2，Apriori被设计出来，它通过逐层搜索的模式，由频繁$k - 1$项集生成频繁$k$项集，从而最终得到全部的频繁项集。

可见，Apriori最核心的部件就是怎样通过频繁$k - 1$项集生成频繁$k$项集。为什么要通过这种方法找频繁$k$项集呢？通过定理1，2，我们知道：如果一个项集是频繁$k$项集，那么它的任意非空子集一定是频繁的，所以说，假如现在我们找到了全部的频繁$k - 1$项集，那么频繁$k$项集则一定是由这些频繁$k - 1$项集组合生成的。可以精简为下面的2个定理：

**定理3**：任何频繁$k$项集都是由频繁$k - 1$项集组合生成的

**定理4**：频繁$k$项集的所有$k - 1$项子集一定全部都是频繁$k - 1$项集

## 算法实现

定理3，4后面有用，暂且先搁这。下面看Apriori的具体步骤：
- 输入：事务数据库$D$；用户子集定义的最小支持度阈值计数阈值$min\_sup$
- 输出：所有频繁项集

这里举一个韩家炜《数据挖掘》中的例子，方便讲解算法，如下表，TID为事务的ID，表右栏的列表为事务包含的项
|TID|商品（项）列表|
|----|----|
|T01|$I_1, I_2, I_5$|
|T02|$I_2, I_4$|
|T03|$I_2, I_3$|
|T04|$I_1, I_2, I_4$|
|T05|$I_1, I_3$|
|T06|$I_2, I_3$|
|T07|$I_1, I_3$|
|T06|$I_1, I_2, I_3, I_5$|
|T07|$I_1, I_2, I_3$|

算法流程如下：

第1步： 生成1项集的集合$C_1$。将所有事务中出现的项组成一个集合，记为$C1$，$C1$可以看做是由所有的1项集组成的集合，比如上面这个例子，$C_1$可以写成下面的形式，每个元素其实都是由一个项构成的集合：
`C1 = [[I1], [I2], [I3], [I4], [I5]]`
第2步： 寻找频繁1项集。统计$C1$中所有元素出现的次数，再与最小支持度计数阈值$min\_sup$比较，筛掉出现小于$min\_sup$的项集（当然此时的项集都是1项集），剩下的都是频繁1项集，这些频繁1项集组成的集合记为$L_1$。此例中，我们设置$min\_sup = 2$，那么经过这一步的筛选，没有项集被淘汰，统计情况以及$L_1$可以写成如下的形式：
|Item|$I_1$|$I_2$|$I_3$|$I_4$|$I_5$|
|----|----|----|----|----|----|
|count|6|7|6|2|2|
`L1 = [[I1], [I2], [I3], [I4], [I5]]`
**1，2步说明**：前两步进行完，我们已经得到频繁1项集的集合$L_1$，现在的目的是要通过$L1$生成$L2$，根据上面讲的定理3，我们必须要用$L1$中的项集组合生成2项集$C2$，再从$C2$中筛选出支持度计数不小于$min\_sup$的，构成频繁2项集$L_2$。这也是Apriori最关键的部分。因为由$L_1$生成$L_2$的步骤与任意$L_{k - 1}$生成$L_k$地步骤是一样的，所以，我在后面就用$L_{k - 1}$和$L_k$代表普遍性。这个工作通过“三步走”来实现：一步连接，一步剪枝，一步筛选。

第3步： 连接。连接的作用就是用两个频繁$k - 1$项集，组成一个$k$项集。具体来说，分为两步，
- 先判断两个频繁$k - 1$项集是否是可连接的，可连接的定义是这样：对于两个频繁$k - 1$项集$l_1, l_2$，先将项集中的项排序（比如字典序），如果$l_1, l_2$的前$k - 2$项都相等，则$l_1, l_2$可连接
- 如果两个频繁$k - 1$项集$l_1, l_2$可连接，则用它们生成一个新的$k$项集：$\{l_1[1], l_1[2], \dots, l_1[k - 2], l_1[k - 1], l_2[k - 1] \}$，其实就是用他们相同的前$k - 2$项加上不同的各自末尾两项，这个过程由式子$l_1 \times l_2$表示。这样，我们只需找到所有的$C_{n}^2$个两两组合($n$为$L_{k - 1}$的长度)，挑出其中可连接的，就能生成所有可能是频繁项集的$k$项集，它们就是频繁$k$项集的候选，这些候选构成的集合记为$C_k$

**3步说明**：经过这样连接起来的$k$项集，至少就有两个$k - 1$子集是频繁的，所以，由定理4，我们知道，这样的$k$项集才有可能是频繁的，这也就在一开始，直接为我们排除了大量不可能的组合（因为我们不是通过找所有项的$k$组合生成的候选），比如上面的例子中，经过连接，由$L_1$生成的$C_2$如下所示：
`C2 = [[I1, I2], [I1, I3], [I1, I4], [I1, I5], [I2, I3], [I2, I4], [I2, I5], [I3, I4], [I3, I5], [I4, I5]]`
**注意：**从$C_2$开始，生成项集时，就按顺序生成，这样可以省去每次连接操作中，对项集的排序工作。后面的代码会有具体说明。

第4步： 剪枝。剪枝是第一步的筛选，因为3步结束之后，所有的频繁$k$项集的候选都在$C_k$里面了，所以现在的任务是对$C_k$筛选，剪枝是第一步。它是这样的一个步骤，对于每个候选$k$项集，找出所有它的$k - 1$项子集，看看是否都在$L_{k - 1}$中，只要有一个不在，那这个$k$项集一定不是频繁的。剪枝的原理就是定理4。经过剪枝，现在$C_k$进一步缩减。这个过程也叫**子集测试**。例子中，因为$C_2$的所有子集都是1项集，都是频繁的，所以，这一步没有筛选掉任何项集。此时的$C_2$还是上面的形式。

第5步： 扫描事务数据库。做进一步筛选。扫描事务数据库$D$，找到所有事务中的项集的所有子集，找出在现在的$C_k$里面的子集，计数，这样能统计出来目前$C_k$当中的所有项集的频数，删去小于$min\_sup$的，得到频繁$k$项集组成的集合$L_k$。比如上面的例子，经过扫描数据库，统计结果如下：
|Item|$[I_1, I_2]$|$[I_1, I_3]$|$[I_1, I_4]$|$[I_1, I_5]$|$[I_2, I_3]$|$[I_2, I_4]$|$[I_2, I_5]$|$[I_3, I_4]$|$[I_3, I_5]$|$[I_4, I_5]$|
|----|----|----|----|----|----|----|----|----|----|----|
|count|4|4|1|2|4|2|2|0|1|0|

筛掉计数小于$min\_sup$(也就是2)的，剩下的就是频繁2项集$L_2$了：
`L2 = [[I1, I2],[I1, I3],[I1, I5],[I2, I3],[I2, I4],[I2, I5]]`
第6步： 重复进行3，4，5步，直到找出的$k$项集$C_k = \varnothing$.比如本文的例子，找到$L_2$后，继续找到$L_3 = [[I_1, I_2, I_3], [I_1, I_2, I_5]]$，由$L_3$找$C_4$时，$C_4 = \varnothing$。至此，已经找到了所有的频繁项集，如下：
`frequentItemSets = [[I1], [I2], [I3], [I4], [I5], [I1, I2],[I1, I3],[I1, I5],[I2, I3],[I2, I4],[I2, I5], [I1, I2, I3], [I1, I2, I5]]`
## 程序设计

明白了Apriori的原理，就可以写出代码了，限于篇幅，我在本文中，只是给出几个关键函数的代码，当然，由于给出的代码只是整个算法的一部分，所以我会对相关没有定义的函数名进行简单说明，方便理解给出部分的代码。全部的实现代码，请参考我的GitHub主页：[Apriori](https://github.com/guoziqingbupt/Apriori)

我用的是Python实现Apriori，先给出用到的相关参量的说明：

```
D:事务数据库，用字典的形式表示。键为TID，值为事务中出现的项的集合，这个集合以列表的形式存储。形式如{TID: [I1, I2]}

min_sup:最小支持度计数阈值，int型，代码中，设定为2
```

再给出一些简单的函数的功能说明，这些函数过于简单，所以，只是说明其实现的功能。不给出具体代码。

```
find_frequent_1_itemsets(D, min_sup):遍历D，根据min_sup找出所有的频繁1项集，结果记为L1，L1是列表型，且其每个元素都是一个长度为1的列表。用于上面第1，2步，找出频繁1项集。

isLinkable(l1, l2):l1, l2为列表型，这个函数的作用是判断两个排好序的项集l1, l2是否是可连接的，用于上面第3步，连接的前提判断。

gen_ksub1_subsets(s):生成集合s的所有长度为k - 1的子集，s为列表型，生成的结果为列表型，且结果的每个元素为列表型。这个函数用于上面第4步——剪枝，判断是否一个k项集的所有k - 1项子集都是频繁的

subsets(S):求出集合S的所有子集，S为列表型，求出的结果result是一个列表，result的每个元素也是列表型，代表S的一个子集。这个函数用于上面第5步的筛选，求出每个事务的子集时用。
```

下面给出一些关键函数的实现代码。首先是连接和剪枝两步（上面第3，4步），我沿用韩家炜先生的用法，将实现这两步的函数叫做`apriori_gen(L_k_subtract_1)`，参数`L_k_subtract_1`表示频繁$k - 1$项集的集合，函数的具体代码如下：

```python
def apriori_gen(L_k_subtract_1):
    """
    There are 2 steps in apriori_gen:
    1. link: execute l1 x l2, generate original Ck;
    2. prune: delete the candidate in Ck who has infrequent subset
    :param L_k_subtract_1: a list, each element is k-1 frequent itemset
    :return: a semi-finished k itemset candidates Lk
    """

    index1 = 0
    k = len(L_k_subtract_1[0]) + 1
    Ck = []

    # while: link process
    while index1 < len(L_k_subtract_1):

        # the itemset l1 that to be linked: l1 x l2
        l1 = L_k_subtract_1[index1]

        # traverse L(k - 1), find the other itemset l2
        for l2 in L_k_subtract_1[index1 + 1:]:

            if isLinkable(l1, l2):

                newItemSet = [item for item in l1[:k - 2]]

                # add tail element with order
                if l1[k - 2] < l2[k - 2]:
                    newItemSet.append(l1[k - 2])
                    newItemSet.append(l2[k - 2])
                else:
                    newItemSet.append(l2[k - 2])
                    newItemSet.append(l1[k - 2])

                Ck.append(newItemSet)

        index1 += 1

    # subSetTest: prune process
    return subSetTest(Ck, L_k_subtract_1)

def subSetTest(Ck, L_k_subtract_1):
    """
    prune process: according to apriori, test if every itemset in Ck is possible frequent
    :param Ck: a list, and each element is also a list
    :param L_k_subtract_1: a list, and each element is also a list
    :return: a semi-finished Lk
    """

    # the cur itemset that to be tested.
    cur = 0
    n = len(Ck)

    semi_finished_Lk = []
    while cur < n:

        # testItemSet: a list
        testItemSet = Ck[cur]

        if not has_infrequent_subset(testItemSet, L_k_subtract_1):
            semi_finished_Lk.append(testItemSet)

        cur += 1

    return semi_finished_Lk

def has_infrequent_subset(testItemSet, L_k_subtract_1):
    """
    :param testItemSet: the candidate k itemset, is a list
    :param L_k_subtract_1: a list
    :return: testItemSet has a infrequent subset or not
    """

    for testSubSet in gen_ksub1_subsets(testItemSet):
        if testSubSet not in L_k_subtract_1:
            return True
    return False
```

可以看出，apriori_gen在此还调用了函数subSetTest，这个函数的功能在于做子集测试，也就是剪枝的过程，对候选集Ck，依照L_k_subtract_1进行剪枝筛选。而subSetTest调用了函数has_infrequent_subset检查每个子集的频繁与否。其它用到的函数，诸如`isLinkable(), gen_ksub1_subsets()`省略。

最后，还有一步就是对整个数据库扫描，二次筛选候选项集，找出频繁项集$L_k$，代码如下：

```python
def scanDataBase(D, min_sup, semi_finished_Lk):
    """
    scan the transaction database, filter the infrequent itemset
    :param D: a dictionary, represents the whole transaction database
    :param min_sup:
    :param semi_finished_Lk: a list, represents semi_finished itemset Lk
    :return: the final frequent itemset Lk
    """

    Lk = []
    k = len(semi_finished_Lk)
    counts = [0 for i in range(len(semi_finished_Lk))]

    for TID in D:

        subSets = subsets(D[TID])

        for subSet in subSets:

            if subSet in semi_finished_Lk:
                counts[semi_finished_Lk.index(subSet)] += 1

    index = 0
    while index < k:
        if counts[index] >= min_sup:
            Lk.append(semi_finished_Lk[index])
        index += 1
    return Lk
```

其用到的函数`subsets()`省略，求子集的函数也是lintcode中非常经典的题目了，容易查到。

根据这些，最终可以给出主函数了：

```python
def miningFrequentItemSet(D, min_sup):

    # initialized
    frequentItemSets = []
    L1 = find_frequent_1_itemsets(D, min_sup)
    frequentItemSets.extend(L1)

    # find frequent itemset Lk, until it is empty
    Lk = L1
    while len(Lk) != 0:

        # here, Ck is also a semi-finished Lk which processed by link and prune
        Ck = apriori_gen(Lk)

        # obtain final frequent itemset Lk
        Lk = scanDataBase(D, min_sup, Ck)

        frequentItemSets.extend(Lk)

    return frequentItemSets
```

找出所有的频繁项集，就可以通过最前面我们说的支持度和置信度的公式，计算得到具有强关联规则的实体（实体可能是项，也可能是项集）。

详细代码参见：[Apriori](https://github.com/guoziqingbupt/Apriori)






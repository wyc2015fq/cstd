# 熵，信息增益，信息增益率，Gini - 数据之美的博客 - CSDN博客
2017年09月13日 09:33:52[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1295
    话说今天《机器学习》上课被很深地打击了，标名为“数据挖掘”专业的我居然连个信息增益的例子都没能算正确。唉，自看书以来，这个地方就一直没有去推算过，每每看到决策树时看完Entropy就直接跳过后面增益计算了。因而，总想找个时间再回过来好好看一下，这不，被逼上了呢。神奇的墨菲定律呢：你担心它发生的，它就一定会发生。
回正题了，这三个指标均是决策树用来划分属性的时候用到的，其中信息增益（Info Gain）用于ID3，Gini用于CART，信息增益率（Info Gain Ratio）用于C4.5。提到前两个指标的计算时，首先要讲到的是关于熵（Entropy）的计算。
1、熵（Entropy）
理论上来说用于决策树的属性选择函数，为方便计算，往往是定义为其属性的不纯性度量，那么必须满足如下三个条件：
- 当结点很纯时，其度量值应为0
- 当不纯性最大时（比如所有类都有同样的可能），其度量值应最大
- 度量应该服从多级特性，这样决策树才能分阶段建立起来
measure([2,3,4])=measure([2,7])+79×measure([3,4])
而熵（Entropy）能够满足以上三点特性。熵（Entropy）是由“信息论之父”香农提出的，更多的各种历史、数学理论请查看参考[1]。接下来，看看熵的计算公式如下：
entropy(p1,p2,…,pn)=–p1log2(p1)–p2log2(p2)–…–pnlog2(pn)
其中，pi为比例值。其实，熵也可以用另外一种意思来解释：
> 
Given a probability distribution, the info required to predict an event is the distribution’s entropy. Entropy gives the information required in bits (this can involve fractions of bits!)
可以简单的理解为“熵”描述了用来预测的信息位数。接下来看个例子：
如下表所述的天气数据，学习目标是预测Play or not play?
表1 天气预报数据集例子
|Outlook|Temperature|Humidity|Windy|Play?|
|----|----|----|----|----|
|sunny|hot|high|false|no|
|sunny|hot|high|true|no|
|overcast|hot|high|false|yes|
|rain|mild|high|false|yes|
|rain|cool|normal|false|yes|
|rain|cool|normal|true|no|
|overcast|cool|normal|true|yes|
|sunny|mild|high|false|no|
|sunny|cool|normal|false|yes|
|rain|mild|normal|false|yes|
|sunny|mild|normal|true|yes|
|overcast|mild|high|true|yes|
|overcast|hot|normal|false|yes|
|rain|mild|high|true|no|
共14个实例，9个正例（yes），5个负例（no）。
这样当前数据的信息量（原始状态）用熵来计算就是：
info(play?)=info([9,5])=entropy(914,514)=–914log2(914)–514log2(514)=0.410+0.530=0.940
有了上面计算熵（信息）的基础，接下来看信息增益了。
2、信息增益（Info Gain）
信息增益，按名称来理解的话，就是前后信息的差值，在决策树分类问题中，即就是决策树在进行属性选择划分前和划分后的信息差值，即可以写成：
gain()=infobeforeSplit()–infoafterSplit()
如上面的例子，通过使用Outlook属性来划分成下图：
![](https://img-my.csdn.net/uploads/201303/29/1364523457_9043.png)
图1 使用Outlook属性划分决策树
在划分后，可以看到数据被分成三份，则各分支的信息计算如下：
info([2,3])=−25log2(25)–35log2(35)=0.971bits
info([4,0])=−44log2(44)–04log2(04)=0bits
，此处虽然log2(0)没有定义，但0×log2(0)仍然计算为0。
info([3,2])=−35log2(35)–25log2(25)=0.971bits
因此，划分后的信息总量应为：
info([2,3],[4,0],[3,2])=514×0.971+414×0+514×0.971=0.693bits
如果把上面的公式整理到一起的话，就是：
$$ info([2,3], [4,0], [3,2]) = \frac{5}{14}\times info([2,3]) + \frac{4}{14}\times info([4,0]) + \frac{5}{14}\times info([3,2])
= 0.347 + 0 + 0.347 = 0.694 bits $$
从上面可以看出，划分后的信息总量计算公式为：
info(S1,…,Sn)=∑in|Si||S|×entropy(Si)
其中，n表示划分后的分支数目，|S|表示划分前的实例个数，|Si|表示划分后某个分支的实例个数。
最后，信息增益
gain(outlook|play?)=infobeforeSplit()–infoafterSplit()=info(play?)–info([3,2],[4,0],[3,2])=0.940–0.694=0.246bits
通过划分其他属性，可得到其他的树如下：
![](https://img-my.csdn.net/uploads/201303/29/1364523464_7002.png)
图2 使用其他属性划分决策树
同样计算，
gain(Temperature|play?)=info([9,5])–info([2,2],[4,2],[3,1])=0.940–(414×info([2,2])+614×info([4,2])+414×info([3,1]))=0.028bits
gain(Humidity|play?)=0.152bits
gain(Windy|play?)=0.048bits
这样，算法会选择最大的信息增益属性来进行划分，在此处为Outlook属性。接下来在Outlook=sunny结点，按照同样的判断方法，选择其他属性进行划分，
![](https://img-my.csdn.net/uploads/201303/29/1364523470_1947.png)
图3 继续划分决策树
计算如下：
infobeforeSplit()=info([2,3])=0.971bits
2个正例，3个负例，计算其熵
gain(Temperature|Outlook,Play?)=0.571bits
gain(Humidity|Outlook,Play?)=0.971bits
gain(Winday|Outlook,Play?)=0.020bits
因而可以得到下一个划分结点是Humidity属性。在这里的Temperature有三个值，因而其info([2,2,1])使用简便方法计算为：
info([0,2],[1,1],[1,0])=25×info([0,2])+25×info([1,1])+15×info([1,0])=0.4bits
因此gain(Temperature|Outlook,Play?)=0.971–0.4=0.571bits。另外，由于要计算的是信息增益最大，在划分前信息总量infobeforeSplit()一定的情况下，我们完全可以直接求划分后信息量最小的特性即可。
3、信息增益率（Info Gain Ratio）
通过上面的例子，想必对于信息增益的计算步骤已经熟悉些了。那么下面来看信息增益存在的一个问题：假设某个属性存在大量的不同值，如ID编号（在上面例子中加一列为ID，编号为a~n），在划分时将每个值成为一个结点，这样就形成了下面的图：
![](https://img-my.csdn.net/uploads/201303/29/1364523479_1485.png)
图4 信息增益偏向
最终计算得到划分后的信息量为：info(S1,…,Sn)=∑ni=1|Si||S|×entropy(Si)=0，因为entropy(Si)=info([1,0]) 或 info([0,1])，只含一个纯结点。这样决策树在选择属性时，将偏向于选择该属性，但这肯定是不正确（导致过拟合）的。因此有必要使用一种更好的方法，那就是C4.5中使用的信息增益率（Info
 Gain Ratio），其考虑了分支数量和尺寸的因素，使用称为内在信息（Intrinsic Information）的概念。
> 
Gain ratio takes number and size of branches into account when choosing an attribute, and corrects the information gain by taking the intrinsic information of a split into account (i.e. how
 much info do we need to tell which branch an instance belongs to).
内在信息（Intrinsic Information），可简单地理解为表示信息分支所需要的信息量，其计算公式如下：
IntrinsicInfo(S,A)=−∑|Si||S|log2|Si||S|
则针对上图中的例子，
info([1,1,…,1])=14×(−114log2114)=3.807bits
信息增益率则计算如下：
gainratio(Attribute)=gain(Attribute)IntrinsicInfo(Attribute)
依然如上例：
gainratio(IDCode)=0.940bits3.807bits=0.246
实际上可以看出，属性的重要性会随着其内在信息（Intrinsic Information）的增大而减小。 信息增益率作为一种补偿（Compensate）措施来解决信息增益所存在的问题，但是它也有可能导致过分补偿，而选择那些内在信息很小的属性，这一点可以尝试：首先，仅考虑那些信息增益超过平均值的属性，其次再比较信息增益。
4、Gini Index
在CART里面划分决策树的条件是采用Gini Index，定义如下：
gini(T)=1−∑j=1np2j
其中，pj是类j在T中的相对频率，当类在T中是倾斜的时，gini(T)会最小。
将T划分为T1（实例数为N1）和T2（实例数为N2）两个子集后，划分数据的Gini定义如下：
ginisplit(T)=N1Ngini(T1)+N2Ngini(T2)
然后选择其中最小的ginisplit(T)作为结点划分决策树。
5、参考
本文大量地参考了如下资料：
[1] [http://en.wikipedia.org/wiki/Entropy](http://en.wikipedia.org/wiki/Entropy)(EN), [http://zh.wikipedia.org/wiki/%E7%86%B5](http://zh.wikipedia.org/wiki/%E7%86%B5)(CN)
[2] Data Mining examples: [www.liacs.nl/home/joost/DM/mod_06_dec_tree_intro.ppt](http://www.liacs.nl/home/joost/DM/mod_06_dec_tree_intro.ppt)
[3] Data Mining – Practical Machine Learning Tools and Techniques with Java
内容写了这么多，就到此为止吧，要不然就没完没了了，下一步考虑分析Weka中的信息增益（weka.attributeSelection.InfoGainAttributeEval）以及信息增益率（weka.attributeSelection.GainRatioAttributeEval）的实现。错误之处，欢迎指正！
原载地址：http://ezale.bokewan.com/info-gain-and-gain-ratio-and-gini/

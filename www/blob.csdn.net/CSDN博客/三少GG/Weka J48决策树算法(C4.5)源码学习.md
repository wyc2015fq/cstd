# Weka J48决策树算法(C4.5)源码学习 - 三少GG - CSDN博客
2013年12月19日 18:03:18[三少GG](https://me.csdn.net/scut1135)阅读数：9153
代码下载：
[http://weka.wikispaces.com/Subversion](http://weka.wikispaces.com/Subversion)
Use WEKA in your Java Code:
[http://www.cs.umb.edu/~ding/history/310_fall_2010/homework/UseWEKA_In_Java_Code.pdf](http://www.cs.umb.edu/~ding/history/310_fall_2010/homework/UseWEKA_In_Java_Code.pdf)

# J48 C4.5决策树算法源码学习 
### TODO: J48 的分类效率分析。
题记： 之前虽然对 J48 用得比较多，是由于它能方便的区别特征的好坏。 工作了，希望自己能更深入， 如是开始了这个算法学习系列。 希望和大家共同进步。
个人对看算法源代码也没有很好的流程，计划先采用 按类Class 做架构介绍；再深入代码具体逻辑的方式展开。 欢迎大家提出好的算法源码阅读流程。
另外，求推荐LR 的比较好的实现代码 ~(^o^)~
## 一、 准备工作。
下载 weka  的工具包，将 weka.jar 和 weka-src.jar 导入eclipse 项目的依赖包，即可查看 到源码。 
也可以将weka-src.jar 解压，在对应的文件夹下建立一个单独的eclipse 项目，这样可以自己修改代码。
相关论文参考： Ross Quinlan (1993). C4.5: Programs for Machine Learning. Morgan Kaufmann Publishers, San Mateo, CA.
因为还是对 E文 不友好，个人看了 统计学习方法 的对应章节就算是把理论先过完啦。
熵用来计算随机量的分布不确定性，或者其表达能力。计算公式：
![](https://img-my.csdn.net/uploads/201212/09/1355055345_7361.jpg)
信息增益 Information Gain 又称为 互信息 Multi Information, 体现了新的属性的出现对类别不确定的弱化的能力。计算公式：
![](https://img-my.csdn.net/uploads/201212/09/1355055603_8655.jpg)
信息增益率在分类问题中体现为：特征A对类别C的 信息增益与特征A的熵之比。能在一定程度上修正某个属性ai由于分布很广而Gain更大的影响；但是也可能导致过分修正而选择那些属性分布很窄的项。可以采用两步判别法：信息增益超过平均值的属性，其次再比较信息增益率。
C4.5 决策树的学习算法：
![](https://img-my.csdn.net/uploads/201212/09/1355059857_7196.jpg)
## 二、代码流解析：
模型的学习程序从 J48.java 开始。
J48.buildClassifier(ins):  选取 C45 决策树模型为例：
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- modSelection = new C45ModelSelection(m_minNumObj, instances);  
- m_root = new C45PruneableClassifierTree(modSelection, !m_unpruned, m_CF,  
-                         m_subtreeRaising, !m_noCleanup);  
- m_root.buildClassifier(instances);  
将C45Pruneable*.buildClassifier(ins) 继续展开：
> 
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- data.deleteWithMissingClass();  
- 
- buildTree(data, m_subtreeRaising || !m_cleanup);  
- collapse();  
- if (m_pruneTheTree) {  
-     prune();  
- }  
- if (m_cleanup) {  
-     cleanup(new Instances(data, 0));  
- }  
 对基类ClassifierTree.**buildTree**()继续展开：
> 
> 
调用 modSelection.selectModel(ins);
> 
modSelection.split(ins).  // 分割数据
> 
m_sons[i] = getNewTree(localInstances[i]);  // 构建子树
将 C45ModelSelection.selectModel(ins) 继续展开：
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- if (Utils.sm(checkDistribution.total(), 2 * m_minNoObj)  
-         || Utils.eq(checkDistribution.total(), checkDistribution.perClass(checkDistribution.maxClass())))  
- return noSplitModel;  
- multiValue = !(attribute.isNumeric() || attribute.numValues() < (0.3 * m_allData.numInstances()));  
- currentModel = new C45Split[data.numAttributes()];  
- sumOfWeights = data.sumOfWeights();  
- 
- // For each attribute.
- for (i = 0; i < data.numAttributes(); i++) {  
- 
- // Apart from class attribute.
- if (i != (data).classIndex()) {  
- 
- // Get models for current attribute.
-         currentModel[i] = new C45Split(i, m_minNoObj, sumOfWeights);  
-         currentModel[i].buildClassifier(data);  
- 
- // ... 省略代码部分： 更新 averageInfoGain的总和
-     } else
-         currentModel[i] = null;  
- }  
- 
- averageInfoGain = averageInfoGain / (double) validModels;  
- 
- // Find "best" attribute to split on.
- minResult = 0;  
- for (i = 0; i < data.numAttributes(); i++) {  
- 
- // Use 1E-3 here to get a closer approximation to the original implementation.
- if ((currentModel[i].infoGain() >= (averageInfoGain - 1E-3))  
-                 && Utils.gr(currentModel[i].gainRatio(), minResult)) {  
-             bestModel = currentModel[i];  
-             minResult = currentModel[i].gainRatio();  
-         }  
- }  
- 
- // ... 省略代码： 将全局数据加载到bestModel. Set the split point analogue to C45 if attribute numeric.
- if (m_allData != null)  
-     bestModel.setSplitPoint(m_allData);  
- return bestModel;  
将C45Split.buildClassifier() 方法继续展开：
> 
> 
> 
按属性为 Enum or Numeric 类型，分别调用handleEnumerateAttribute(ins) or handleNumericAttribute(ins). 这两个方法的具体实现参考下文的讲解。
> 
collapse() 方法： 对比合并树 前后的错误率，不增加则合并。并递归处理孩子节点。
 prune() 方法：对C4.5决策树进行减枝。递归地先从孩子节点开始。 当前节点处理的关键代码：
> 
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- // Compute error if this Tree would be leaf
- errorsLeaf = getEstimatedErrorsForDistribution(localModel()  
-         .distribution());  
- 
- // Compute error for the whole subtree
- errorsTree = getEstimatedErrors();  
- 
- // Decide if leaf is best choice.
- if (Utils.smOrEq(errorsLeaf, errorsTree + 0.1)  
-         && Utils.smOrEq(errorsLeaf, errorsLargestBranch + 0.1)) {  
- // ... 将树设为叶子 
- }  
- 
- // Decide if largest branch is better choice than whole subtree.
- if (Utils.smOrEq(errorsLargestBranch, errorsTree + 0.1)) {  
-     largestBranch = son(indexOfLargestBranch);  
-     m_sons = largestBranch.m_sons;  
-     m_localModel = largestBranch.localModel();  
-     m_isLeaf = largestBranch.m_isLeaf;  
-     newDistribution(m_train);  
-     prune();  
- }  
J48.classifyInstance(data): 使用模型对 测试数据分类。
> 
m_root.classifyInstance(data);
实际调用 ClassifyTree.classifyInstance(ins)： 
> 
> 
调用getProbs(int classIndex, data, 1)计算测试ins在各个类别下的概率，去最大类别的idx返回。
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- if (m_isLeaf) {  
- return weight * localModel().classProb(classIndex, instance, -1);  
- } else {  
- int treeIndex = localModel().whichSubset(instance);  
- if (treeIndex == -1) {  // contained by multi-classLabel.
- double[] weights = localModel().weights(instance);  
- for (int i = 0; i < m_sons.length; i++) {  
- if (!son(i).m_isEmpty) {  
-                 prob += son(i).getProbs(classIndex, instance,  
-                         weights[i] * weight);  
-             }  
-         }  
- return prob;  
-     } else {  
- if (son(treeIndex).m_isEmpty) {  // leaf node.
- return weight * localModel().classProb(classIndex, instance, treeIndex);  
-         } else {  
- return son(treeIndex).getProbs(classIndex, instance, weight);  
-         }  
-     }  
- }  
其中， 有对 ClassSplitModel.classProb() 的调用，策略与上类似，最终概率有 Distribution.prob() 得到。 
classProb() 的代码如下：
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- if (theSubset > -1) {  
- return m_distribution.prob(classIndex, theSubset);  
- } else {  
- double[] weights = weights(instance);  
- if (weights == null) {  
- return m_distribution.prob(classIndex);  
-     } else {  
- double prob = 0;  
- for (int i = 0; i < weights.length; i++) {  
-             prob += weights[i] * m_distribution.prob(classIndex, i);  
-         }  
- return prob;  
-     }  
- }  
> 
## 三、相关代码分析。
### **J48**.java：
 对 决策树 的包装接口类。  主要的属性与方法：(代码自身已经是很好的注释 故文中不再重复)
> 
m_root: 根节点。 以及一些模型学习的参数。
buildClassifier(Ins) 
classifyInstance(ins)
### ClassifierTree.java: 
分类树的数据结构。主要的属性与方法：
> 
m_son. 
m_toSelectModel.
ClassifierSplitModel.
Instances m_train_data.
Distribution m_dis.
buildTree(ins)
classifyInstance(ins) 
distributionForInstance(Instance, boolean): 计算实例在各个类别下的概率（分布）
getProbsLaplace(classIdx, ins, weight): 计算 ins 的class probabilities 
getProbs(...) 同上
子类： 
**C45PruneableClassifierTree**.java：支持基于C4.5 prune 的树。主要的属性与方法：
> 
buildClassifier(ins)
buildTree(ins, boolean)
collapse() Collapses a tree to a node if training error doesn't increase: 将当前错误率(叶子节点之和)与整个distribution的错误率对比
prune()
getEstimatedErrorsForDistribution(Distribution): 估计叶子的错误率：训练错误率+估计值： dis.numIncorrect() + Stats.addErrs(dis.total(), dis.numIncorrect(), m_CF).
C45PruneableClassifierTreeG.java： 比上面的类增加了 grafted on.
> 
doGrafting(ins)
sortInstances(ins, iindex[][], limits[][], subset): 
findGraft(ins, iindex[][], limits[][], ClassifierTree parent, pLaplace, pLeafClass) 
NBTreeClassifierTree.java:  naive bayes tree structure  for classification. 不熟悉，也没看出代码中的独特之处。
PruneableClassifierTree.java：支持prune的结构，主要方法：
> 
prune().
collapse()
### Stats.java: 
对给定数据集估计额外错误estimated extra error, 使用正态normal 分布近视二项binomial 分布. 主要的方法：
> 
addErrs(double N, double e, float CF): 
### ModelSelection.java: 
决策树的特征选择与切分抽象类
子类：
**C45ModelSelection**.java : 基于C45Split 的特征切分方式。主要属性和方法：
> 
selectModel(Instances)
BinC45ModelSelection.java: 基于 BinC45Split 
NBTreeModelSelection.java: 基于NBTreeSplit
ResidualModelSelection.java: 基于ResidualSplit 训练数据上的残差。
### ClassifierSplitModel.java: 
决策树的分支判决 抽象类。主要属性和方法：
> 
m_numSubsets: 整个ins 被分割开的子集数。
Distribution m_dis;
classProb(cidx, ins, bagIdx) && classProbLaplace():  基于m_dis返回 单分布 或 多分布(求和)下的概率。
split(Instances)： 对数据进行分割。基于 抽象方法 weight(ins)  && whichSubset(ins).  部分源码：
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- weights = weights(instance);  
- subset = whichSubset(instance);  
- if (subset > -1)  
-     instances[subset].add(instance);  
- else
- for (j = 0; j < m_numSubsets; j++)  
- if (Utils.gr(weights[j], 0)) {  
-             newWeight = weights[j] * instance.weight();  
-             instances[j].add(instance);  
-             instances[j].lastInstance().setWeight(newWeight);  
-         }  
子类：
NoSplit.java: 对数据集无分割。关键方法： weight(): @ret null; leftSide(): @ret "";  rightSide(): @ret "";
C45Split.java: 对数据集按C4.5算法分割
> 
InfoGainSplitCrit infoGainCrit; @static
GainRatioSplitCrit gainRatioCrit; @static 
m_splitPoint; // 连续属性的分割点
m_attIndex; // Get index of attribute to split on.
m_minNoObj; // 最小叶子节点数*2
m_sumOfWeights; // Set the sum of the weights
buildClassifier(ins): 调用 handleEnumeratedAttribute(ins) or  handleNumericAttribute(ins) 对数据分割。
handleEnumeratedAttribute(ins): 对某个离散属性，生成m_distribution, 计算 IG 与 GR 值。
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- if (m_distribution.check(m_minNoObj)) {  
-     m_numSubsets = m_complexityIndex;  
-     m_infoGain = infoGainCrit.splitCritValue(m_distribution,  
-             m_sumOfWeights);  
-     m_gainRatio = gainRatioCrit.splitCritValue(m_distribution,  
-             m_sumOfWeights, m_infoGain);  
- }  
handleNumericAttribute(ins): 对某个数值属性计算 IG 与 GR 与 m_splitPoint。代码逻辑：
> 
> 
对 m_distribution 赋值。并按 属性的值排序。
> 
确定 minsplit = min(max(0.1*m_dis*_total()/ins.numClasses(), m_minNoObj), 25).
> 
遍历属性， 如果 val(i-1) + 1e-5 < val(i) : 计算按此切分时的IG，并更新全局最大IG 与 分割点下标。
> 
m_infoGain = m_infoGain - (Utils.log2(m_index) / m_sumOfWeights);  // 修正IG
> 
m_splitPoint = (trainInstances.instance(splitIndex + 1).value(m_attIndex) + trainInstances.instance(splitIndex).value(m_attIndex)) / 2;
> 
通过 gainRatioCrit.splitCritValue(Distribution, double, double numerator)计算GR 
> 
> 
> 
> 
weights(ins):  处理missing的属性值
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- if (instance.isMissing(m_attIndex)) {  
-     weights = newdouble[m_numSubsets];  
- for (i = 0; i < m_numSubsets; i++)  
-         weights[i] = m_distribution.perBag(i) / m_distribution.total();  
- return weights;  
- } else {  
- returnnull;  
- }  
whichSubset(ins): 对missing的值返回-1， 枚举值返回idx， 否则按m_splitPoint分割为0|1.
BinC45Split.java: 同上，不同点： m_numSubsets = 2;
GraftSplit.java:Class implementing a split for nodes added to a tree during grafting. 跳过。
NBTreeNoSplit.java:  Naive Bayes Tree的no-split 对象。主要属性和方法：
> 
NaiveBayesUpdateable m_nb;
Discretize m_disc;
m_errors; 
buildClassifier(ins): 
crossValidate(m_nb, ins, Random(1));  // 对模型计算CV-5错误率
NBTreeSplit.java: implementing a NBTree split on an attribute. 主要属性和方法：
> 
m_splitPoint;
C45Split m_c45S;
NBTreeNoSplit m_globalNB;
handleEnumeratedAttribute(ins):  训练 m_c45S, 基于CV计算 m_errors, 更新m_numSubsets.
对某个ins, 决定它在树中的分支： m_C45S 能决定则使用，否则将ins分配到每个分支中。
对各个分支，节点数>5 是基于 NaiveBayesUpdateable 分类器的CV-5计算错误率，否则直接将 weight 相加做为分支的error.
handleNumericAttribute(ins):  同上，处理数值型属性。
ResidualSplit.java: Helper class for logistic model trees, implement the splitting criterion based on residuals of the LogitBoost algorithm. 具体查看wiki.
### EntropyBasedSplitCrit.java: 
决策树分裂准则：基于熵。主要方法：（个人不明白为啥没有log前的概率P(i) 求达人解释！）
> 
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- publicfinaldouble oldEnt(Distribution bags) {  // Computes entropy of distribution before splitting.
- double returnValue = 0;  
- int j;  
- for (j = 0; j < bags.numClasses(); j++)  
-         returnValue = returnValue + logFunc(bags.perClass(j));// log(n)/log(2)
- return logFunc(bags.total()) - returnValue;  
- }  
- /**
-  * Computes entropy of distribution after splitting.
-  */
- publicfinaldouble newEnt(Distribution bags) {  
- double returnValue = 0;  
- int i, j;  
- 
- for (i = 0; i < bags.numBags(); i++) {  
- for (j = 0; j < bags.numClasses(); j++)  
-             returnValue = returnValue + logFunc(bags.perClassPerBag(i, j));  
-         returnValue = returnValue - logFunc(bags.perBag(i));  
-     }  
- return -returnValue;  
- }  
- 
- /**
-  * Computes entropy after splitting without considering the class values.
-  */
- publicfinaldouble splitEnt(Distribution bags) {  
- 
- double returnValue = 0;  
- int i;  
- 
- for (i = 0; i < bags.numBags(); i++)  
-         returnValue = returnValue + logFunc(bags.perBag(i));  
- return logFunc(bags.total()) - returnValue;  
- }  
子类：
InfoGainSplitCrit.java: computing the information gain for a given distribution. 主要方法：
> 
splitCritValue(Distribution): 直接计算IG： bags.total()/[oldEnt(bags)-newEnt(bags)]; //取倒数是为了min splitting criterion's value.
splitCritValue(Distribution bags, double totalNoInst): 实现C4.5 的IG
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- double numerator;  
- double noUnknown;  
- double unknownRate;  
- int i;  
- 
- noUnknown = totalNoInst-bags.total();  // missing Ins's weight-sum.
- unknownRate = noUnknown/totalNoInst;  
- numerator = (oldEnt(bags)-newEnt(bags));  
- numerator = (1-unknownRate)*numerator; // bags.total()/totalNoInst * numerator
- 
- // Splits with no gain are useless.
- if (Utils.eq(numerator,0))  
- return0;  
- 
- return numerator/bags.total();  // oldEnt-newEnt/totalNoInst.
GainRatioSplitCrit.java: computing the gain ratio for a given distribution. 主要方法：
> 
splitCritValue(dis): 计算 GR ，用到了splitEnt()计算分母
splitEnt(Distribution bags, double totalnoInst): 计算基于属性的split entropy. 关键代码：
**[java]**[view
 plain](http://blog.csdn.net/stund/article/details/10286373#)[copy](http://blog.csdn.net/stund/article/details/10286373#)
- noUnknown = totalnoInst - bags.total();  
- if (Utils.gr(bags.total(), 0)) {  
- for (i = 0; i < bags.numBags(); i++)  
-         returnValue = returnValue - logFunc(bags.perBag(i));  
-     returnValue = returnValue - logFunc(noUnknown);  
-     returnValue = returnValue + logFunc(totalnoInst);  
- }  
- return returnValue;  
### **Distribution**.java 
存储instance的权重分布，分 bag(attr-num),
 class(class-num), class +bag, total维度
> 
add(bagIdx)
prob(ins); laplaceProb(int): 平滑前、后的概率 Pc + 1 / total + Nc
shift(int, int, Instance): 移动ins到新的bag
subtract(Distribution): 两个dis中class 部分的weight 相减。
## 参考资料： 
1. 统计学习方法。
2. 另一份 对C4.5 的理论与源码分析 [http://blog.csdn.net/fty8788/article/details/8274730](http://blog.csdn.net/fty8788/article/details/8274730)
3. 正态分布对二项分布的近似： [http://www.cs.uni.edu/~campbell/stat/lectures.html](http://www.cs.uni.edu/~campbell/stat/lectures.html)
***********************************************************************************************************************
### [Weka学习五（ROC简介）](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104545884/)
2009-11-16 10:45
[](http://img.bimg.126.net/photo/vvPG9HRVNknJ5hktuQWe8Q==/4815192426589460072.jpg)[](http://img.bimg.126.net/photo/vvPG9HRVNknJ5hktuQWe8Q==/4815192426589460072.jpg)今天我们来介绍一下ROC（Receiver
 operating characteristics）国内复旦张文彤老师在《
[阅读全文>>](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104545884/)
[分类：默认分类](http://liouwei20051000285.blog.163.com/blog/#m=0&t=1&c=fks_084065081083085066080082074065081084082069083082)|[阅读(1735)](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104545884/)|[评论(0)](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104545884/)
### [Weka学习四（属性选择）](http://liouwei20051000285.blog.163.com/blog/static/2523674200910161045945/)
2009-11-16 10:45
在这一节我们看看属性选择。在数据挖掘的研究中，通常要通过距离来计算样本之间的距离，而样本距离是通过属性值来计算的。我们知道对于不同的属性，它们在样本空间的权重是不一样的，即它们与类别的关联度是不同的，因此有必要筛选一些属性或者对各个属性赋一定的权重。这样属性选择的方法就应运而生了。
在属性选择方面InfoGain
[阅读全文>>](http://liouwei20051000285.blog.163.com/blog/static/2523674200910161045945/)
[分类：默认分类](http://liouwei20051000285.blog.163.com/blog/#m=0&t=1&c=fks_084065081083085066080082074065081084082069083082)|[阅读(1013)](http://liouwei20051000285.blog.163.com/blog/static/2523674200910161045945/)|[评论(1)](http://liouwei20051000285.blog.163.com/blog/static/2523674200910161045945/)
### [Weka学习三（ensemble算法）](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104440417/)
2009-11-16 10:44
前两次我们讲了数据挖掘中比较常见的两类方法。这次我来介绍一下ensemble（集成技术），总的来说，ensemble技术是归类在分类中的。它的主要原理是通过集成多个分类器的效果来达到提高分类效果的目的。简单我们可以通过两张图片来看看集成的效果[](http://img.bimg.126.net/photo/65o8xd97jJqveqokDC-e5Q==/4294745194652592909.jpg)：
[阅读全文>>](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104440417/)
[分类：默认分类](http://liouwei20051000285.blog.163.com/blog/#m=0&t=1&c=fks_084065081083085066080082074065081084082069083082)|[阅读(1125)](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104440417/)|[评论(0)](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104440417/)
### [Weka学习二（聚类算法）](http://liouwei20051000285.blog.163.com/blog/static/25236742009101610445275/)
2009-11-16 10:44
上次我介绍了分类器的使用方法，这次我来介绍一下聚类算法。聚类算法在数据挖掘里面被称之为无监督学习（unsupervised learning），这是与分类算法（supervised
 learning）相对的。在它们两者之间还一种叫做半监督学习（semi-supervised learning
[阅读全文>>](http://liouwei20051000285.blog.163.com/blog/static/25236742009101610445275/)
[分类：默认分类](http://liouwei20051000285.blog.163.com/blog/#m=0&t=1&c=fks_084065081083085066080082074065081084082069083082)|[阅读(1330)](http://liouwei20051000285.blog.163.com/blog/static/25236742009101610445275/)|[评论(1)](http://liouwei20051000285.blog.163.com/blog/static/25236742009101610445275/)
### [Weka学习一（分类器算法）](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104326723/)
2009-11-16 10:43
从前年开始使用weka最数据挖掘方面的研究，到现在有一年半的时间了。看到我们同组的兄弟写了关于weka方面的总结，我也想整理一下。因为网上的资料实在是太少，记得刚接手的时候，真是硬着头皮看代码。不过到现在看来，也积累了很多的代码了。希望能够在这里跟大家分享一下学习weka
[阅读全文>>](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104326723/)
[分类：默认分类](http://liouwei20051000285.blog.163.com/blog/#m=0&t=1&c=fks_084065081083085066080082074065081084082069083082)|[阅读(2169)](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104326723/)|[评论(0)](http://liouwei20051000285.blog.163.com/blog/static/252367420091016104326723/)
### [利用WEKA编写数据挖掘算法](http://liouwei20051000285.blog.163.com/blog/static/25236742009101610174638/)
2009-11-16 10:17
WEKA是由新西兰怀卡托大学开发的开源项目。WEKA是由JAVA编写的，并且限制在GNU通用公众证书的条件下发布，可以运行在所有的操作系统中。WEKA工作平台包含能处理所有标准数据挖掘问题的方法：回归、分类、聚类、关联规则挖掘以及属性选择。作为数据挖掘爱好者自然要对WEKA的源代码进行分析并以及改进，努力写出自己的数据挖掘算法。
下面着重介绍一下如何利用WEKA编写新的数据挖掘算法：
    注意：WEKA的版本有两个版本：稳定版(STABLE)和开发版(DEVELOP)，不同WEKA版本与不同JDK的版本匹配，稳定版WEKA3-4的与JDK1.4.2匹配，而开发版WEK
[阅读全文>>](http://liouwei20051000285.blog.163.com/blog/static/25236742009101610174638/)
[分类：默认分类](http://liouwei20051000285.blog.163.com/blog/#m=0&t=1&c=fks_084065081083085066080082074065081084082069083082)|[阅读(925)](http://liouwei20051000285.blog.163.com/blog/static/25236742009101610174638/)|[评论(1)](http://liouwei20051000285.blog.163.com/blog/static/25236742009101610174638/)
### [Weka开发［-1］——在你的代码中使用Weka](http://liouwei20051000285.blog.163.com/blog/static/252367420091015985102/)
2009-11-15 21:08
无意间在网上看到了：[http://weka.wiki.sourceforge.net/Use+Weka+in+your+Java+code](http://weka.wiki.sourceforge.net/Use+Weka+in+your+Java+co%3Cwbr%3Ede)，相对我写的代码，它的当然更有权威性。翻译完了，第一次翻译，术语的汉语很多不清楚。还没有校对，有什么错误请告诉我。
[阅读全文>>](http://liouwei20051000285.blog.163.com/blog/static/252367420091015985102/)

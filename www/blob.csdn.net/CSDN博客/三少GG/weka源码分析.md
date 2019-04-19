# weka源码分析 - 三少GG - CSDN博客
2013年12月21日 23:45:37[三少GG](https://me.csdn.net/scut1135)阅读数：1681

weka源码获得与导入
1、下载weka3.7，解压其中的weka-src.jar的文件,包含lib,src,META-INF三个文件夹和另外两个文件；
2、在Eclipse下新建java project，命名为weka，在src下新建名为weka的包；
3、在此项目下import-->File System-->选择.../weka/src/main/java/weka，并全部导入；
4、项目导入库文件，biuldpath-->add external jar-->选择lib里的 java-cup.jar  JFlex.jar  junit.jar；
5、运行weka.gui.main成功；
WEKA作为一个公开的数据挖掘工作平台，集合了大量能承担数据挖掘任务的机器学习算法，包括对数据进行预处理，分类，回归、聚类、关联规则以及在新的交互式界面上的可视化。
Weka是一个开源的软件，阅读并理解其源代码可以帮助我们更好的理解算法的细节，同时也能够扩展其算法以获得更广泛的应用。但是现在公开的资料中对weka源代码分析的内容很少，所以本博客推出weka源代码分析系列，将针对weka中实现各种算法分析器实现细节。
本系列根据weka功能的划分主要分析器分类算法、聚类算法、规则挖掘等，分别介绍其中的各个算法，最后介绍如何对weka进行扩展。
本系列主要分析weka3.7.6版本
本文首先概要列出分类、聚类以及规则挖掘等主要实现的算法以及类结构关系，以便有个大致的了解。
分类：
所有的分类器都继承自抽象类AbstractClassifier而AbstractClassifier继承自接口Classifier。集成关系如下图所示：
![](https://img-my.csdn.net/uploads/201303/07/1362667181_4056.jpg)
而类Classifier中主要包含以下接口：
public abstract voidbuildClassifier(Instances data) throws Exception;
该方法主要用于根据样本数据data训练出分类器；
public double classifyInstance(Instanceinstance) throws Exception;
该方法是对样本instance进行分类并返回最有可能的类；
public double[] distributionForInstance(Instanceinstance) throws Exception;
对样本instance进行分类，如果分类成功则返回的数组值包含该类别，否则数组元素全为0；
所有的classifier必须实现classifyInstance或distributionForInstance。
聚类：
所有的聚类算法都集成自AbstractClusterer，而AbstractClusterer继承自接口Clusterer，主要调用关系如下：
![](https://img-my.csdn.net/uploads/201303/07/1362667213_1108.jpg)
Clusterer主要实现的接口如下：
voidbuildClusterer(Instances data) throws Exception;
根据样本数据训练聚类算法；
intclusterInstance(Instance instance) throws Exception;
public double[]distributionForInstance(Instance instance) throws Exception;
为实例归类；
intnumberOfClusters() throws Exception;
返回聚类个数；
同样所有的聚类算法都必须实现clusterInstance或distributionForInstance。
规则挖掘：
关联规则主要实现的算法有Apriori，Fpgrowth等，
类关系如下：
![](https://img-my.csdn.net/uploads/201303/07/1362667226_1264.jpg)
Associator中主要实现
voidbuildAssociations(Instances data) throws Exception;
该方法主要用于生成规则生成器。
对于以上所列的算法的实现细节以及其他的诸如数据预处理、可视化等的代码后面会一一详细列出。
weka分类器-C4.5决策树 
**1.     算法概述**
 C4.5，是机器学习算法中的另一个分类决策树算法，它是决策树(决策树也就是做决策的节点间的组织方式像一棵树，其实是一个倒树)核心算法，是相对于ID3算法的改进，主要改进点有：
1.      用信息增益率来选择属性。C4.5克服了ID3用信息增益选择属性时偏向选择取值多的属性的不足。
2.      在树构造过程中进行剪枝。
3.      对非离散数据也能处理。
4.      能够对不完整数据进行处理。
**2.     算法过程**
统计好样本集S，属性集A，分别求出属性集中每个属性的信息增益率，选中增益率最大的属性P，假设P总有n种情况的取值（连续变量要离散化），那么分别统计好第i种情况时样本集Si和除去P的属性集Pi，递归生成相对应的子树。
**2.1 主要步骤**
1）读取文件信息，统计数目
2）建立决策树
如果样本集为空，则生成一个信息数目都为0的树节点返回
如果样本均为同一类别，则生成一个叶子节点返回
计算节点正负样本的数目
如果属性值只有那个类别的属性，则生成一个叶子节点，并赋值类型索引
如果以上都不是，则选择一个增益率最大的属性（连续属性要用增益率离散化），按那个属性的取值情况从新定义样本集和属性集，建造相关子树
3）事后剪枝（采用悲观错误率估算）
4）输出决策树
5）移除决策时
**3.     算法实现分析**
**3.1 训练分类器**
**publicvoid**buildClassifier(Instances instances) **throws** Exception {
      ModelSelectionmodSelection;
**if** (m_binarySplits)
         modSelection= **new**BinC45ModelSelection(m_minNumObj, instances,
m_useMDLcorrection);
**else**
         modSelection= **new**C45ModelSelection(m_minNumObj, instances,
m_useMDLcorrection);
**if** (!m_reducedErrorPruning)
m_root = **new**C45PruneableClassifierTree(modSelection, !m_unpruned,
m_CF, 
m_subtreeRaising, !m_noCleanup, 
m_collapseTree);
**else**
m_root = **new**PruneableClassifierTree(modSelection, !m_unpruned,
m_numFolds, !m_noCleanup,
m_Seed);
m_root.**buildClassifier**(instances);
   }
第一个if判断是否对nominal属性进行二分裂，第二个if是判断是否对tree进行剪枝。
modSelection后面会被用于选择足有的分裂模型。
m_root是该树的根节点。
上面比较重要的是最后一句m_root.buildClassifier(instances);用于在各节点中构建分类器。下面我们看看其代码：
(C45PruneableClassifierTree.java)
**publicvoid**buildClassifier(Instances data) **throws** Exception {
……
      buildTree(data,m_subtreeRaising);
**if** (m_collapseTheTree){
         collapse();
      }
**if** (m_pruneTheTree) {
         prune();
      }
**if** (m_cleanup) {
         cleanup(**new** Instances(data, 0));
      }
   }
过程为:首选见创建树然后根据需要对数进行节点折叠、剪枝、清理内存空间等。下面我们依次分析：
**3.1.1 创建树buildTree (ClassifierTree.java)**
**publicvoid**buildTree(Instances data, **boolean** keepData)
**throws**Exception {
   Instances [] localInstances;
**if**(keepData) {
m_train= data;
   }
m_test= **null**;
m_isLeaf= **false**;
m_isEmpty= **false**;
m_sons= **null**;
**m_localModel = m_toSelectModel.selectModel(data); //选择最优的分裂模型**
**if**(m_localModel.numSubsets()> 1) {
     localInstances = m_localModel.split(data);  //对训练数据进行分组
     data = **null**;
m_sons= **new**ClassifierTree [m_localModel.numSubsets()]; //分配子树
**for**(**int**i = 0; i <
m_sons.length; i++) {
m_sons[i] = getNewTree(localInstances[i]);  //在子树对应的训练数据中构建树
   localInstances[i]= **null**;
     }
   }**else**{
m_isLeaf= **true**;
**if**(Utils.*eq*(data.sumOfWeights(), 0))
m_isEmpty = **true**;
     data = **null**;
   }
  }
从上述代码中可以看出tree的构建过程为：首先根据instances选择最优的分裂模型，然后根据选择的分裂属性将训练集进行分组，最后在子树中构建树。
在该过程中最重要也是最复杂的是选择分裂模型的过程，即m_toSelectModel.selectModel(data);下面我们分析该过程：
**publicfinal**ClassifierSplitModel selectModel(Instances data) {
**try** {
         checkDistribution= **new**Distribution(data);
         noSplitModel= **new**NoSplit(checkDistribution);
         //这个if语句是判断是否有足够的样本进行分裂，因为分裂至少要生成2个新的节点所以判断checkDistribution.total()是否小于**2***
m_minNoObj，第二个条件是判断是否所有的instance都属于同一个类，若是则checkDistribution.total()会与最大类的instance数量相等。
**if** (Utils.*sm*(checkDistribution.total(),2 *
m_minNoObj)
               || Utils.*eq*(checkDistribution.total(),checkDistribution
                      .perClass(checkDistribution.maxClass())))
**return** noSplitModel;
         //这里判断是否所有的属性都是nominal，第二个条件是是判断该属性是否有足够多的值
**if** (m_allData !=
**null**) {
Enumerationenu = data.enumerateAttributes();
**while** (enu.hasMoreElements()){
                attribute= (Attribute) enu.nextElement();
**if**((attribute.isNumeric())
                      ||(Utils.*sm*((**double**) attribute.numValues(),
                            (0.3* (**double**)m_allData.numInstances())))){
                   multiVal= **false**;
**break**;
                }
            }
         }
         currentModel= **new** C45Split[data.numAttributes()];//currentModel存储了在每个属性上构建出的分裂模型，以便后面挑选最优的一个。
         sumOfWeights= data.sumOfWeights();
// For each attribute.
**for** (i = 0; i <data.numAttributes(); i++) {
// Apart from class attribute.
**if** (i !=(data).classIndex()) {
// Get models for current attribute.
                currentModel[i]= **new**C45Split(i,
m_minNoObj,sumOfWeights,
m_useMDLcorrection);
**currentModel[i].buildClassifier(data); //这里是在第i个属性上构建分裂模型，这也是最重要的一个过程。**
// Check if useful split for current attribute
// exists and check for enumerated attributes with
// a lot of values.
**if**(currentModel[i].checkModel())
**if** (m_allData !=
**null**) {
**if**((data.attribute(i).isNumeric())
                            ||(multiVal || Utils.*sm*((**double**) data.attribute(i).numValues(),
                                  (0.3* (**double**)m_allData.numInstances())))){
                         averageInfoGain= averageInfoGain+ currentModel[i].infoGain();
                         validModels++;
                      }
                   }**else**{
                      averageInfoGain= averageInfoGain
                            +currentModel[i].infoGain();
                      validModels++;
                   }
            }**else**
                currentModel[i]= **null**;
         }
// Check if any useful split was found.
**if** (validModels == 0)
**return** noSplitModel; //如果没有有效的分裂过程则返回为分裂模型
         averageInfoGain= averageInfoGain / (**double**) validModels;
// Find "best" attribute to split on.
         minResult= 0;
**for** (i = 0; i <data.numAttributes(); i++) {
**if** ((i !=(data).classIndex())
                   &&(currentModel[i].checkModel()))
// Use 1E-3 here to get a closer approximation to the
// original
// implementation.
                //对所有的属性都构建完分裂模型后，根据其信息增益率选择一个最大的即为最优模型
**if** ((currentModel[i].infoGain() >= (averageInfoGain - 1E-3))
                      &&Utils.*gr*(currentModel[i].gainRatio(), minResult)) {
                   bestModel= currentModel[i];
                   minResult= currentModel[i].gainRatio();
                }
         }
// Check if useful split was found.
**if** (Utils.*eq*(minResult,0))
**return** noSplitModel;
// Add all Instances with unknown values for thecorresponding
// attribute to the distribution for the model, so that
// the complete distribution is stored with the model.
         bestModel.distribution().addInstWithUnknown(data,bestModel.attIndex());
// Set the split point analogue to C45 ifattribute numeric.
**if** (m_allData !=
**null**)
            bestModel.setSplitPoint(m_allData);
**return** bestModel;
      }**catch**(Exception e) {
         e.printStackTrace();
      }
**returnnull**;
   }
上述过程中最重要的是对一个属性创建分裂模型的过程，即**currentModel[i].buildClassifier(data);**该过程代码如下：
**publicvoid**buildClassifier(Instances trainInstances) **throws** Exception {
// Initialize the remaining instance variables.
m_numSubsets = 0;
m_splitPoint = Double.*MAX_VALUE*;
m_infoGain = 0;
m_gainRatio = 0;
// Different treatment for enumerated and numeric
// attributes.
      //分别处理不同类型的属性
**if**(trainInstances.attribute(m_attIndex).isNominal()) {
m_complexityIndex= trainInstances.attribute(m_attIndex).numValues();
m_index = 
m_complexityIndex;
         handleEnumeratedAttribute(trainInstances);
      }**else**{
m_complexityIndex= 2;
m_index = 0;
         trainInstances.sort(trainInstances.attribute(m_attIndex));
         handleNumericAttribute(trainInstances);
      }
   }
这里handleEnumerateAttribute和handleNumericAttribute是决定到底是哪一个属性分裂(m_attIndex)和分裂出几个子结点的函数(m_numSubsets)。这里的m_ complexityIndex就是指分可以分裂出多少子结点。Nominal类型属性根据其类中值的个数决定，如果是连续属性就是2。
我们首先看一下handleEnumeratedAttribute(trainInstances);
**privatevoid**handleEnumeratedAttribute(InstancestrainInstances)
**throws** Exception {
      Instanceinstance;
      // m_complexityIndex指定应该分裂成几个分支，该值由该类中的值的个数决定
m_distribution = **new** Distribution(m_complexityIndex,trainInstances.numClasses());
// Only Instances with known values are relevant.
      //只考虑无缺失值的样例
Enumerationenu = trainInstances.enumerateInstances();
**while** (enu.hasMoreElements()){
         instance= (Instance) enu.nextElement();
**if** (!instance.isMissing(m_attIndex))
m_distribution.add((**int**) instance.value(m_attIndex), instance);
      }
// Check if minimum number of Instances in at least two
// subsets.
**if** (m_distribution.check(m_minNoObj)) {
m_numSubsets = 
m_complexityIndex;
         //计算信息增益
m_infoGain = *infoGainCrit*.splitCritValue(m_distribution, m_sumOfWeights);
         //计算信息增益率
m_gainRatio = *gainRatioCrit*.splitCritValue(m_distribution, m_sumOfWeights,
m_infoGain);
      }
   }
再来看一下handleNumericAttribute(trainInstances);
**privatevoid**handleNumericAttribute(Instances trainInstances)
**throws** Exception {
// Current attribute is a numeric attribute.
m_distribution = **new** Distribution(**2**, trainInstances.numClasses());
// Only Instances with known values are relevant.
Enumerationenu = trainInstances.enumerateInstances();
      i= 0;
**while** (enu.hasMoreElements()){
         instance= (Instance) enu.nextElement();
**if** (instance.isMissing(m_attIndex))
**break**;
m_distribution.add(1,instance);
         i++;
      }
      //由于前面已经将instances对属性m_attIndex进行排序，所以有缺失值的instance都在后面，也就是firstMiss是在m_attIndex上有确定值的样本个数＋1
      firstMiss= i;
// Compute minimum number of Instances required in each
// subset.
      minSplit= 0.1 * (m_distribution.total()) / ((**double**) trainInstances.numClasses());
**if** (Utils.*smOrEq*(minSplit,m_minNoObj))
         minSplit= m_minNoObj;
**elseif** (Utils.*gr*(minSplit,25))
         minSplit= 25;
// Enough Instances with known values?
**if** (Utils.*sm*((**double**) firstMiss, 2 *minSplit))
**return**;
// Compute values of criteria for all possible split
// indices.
      //计算分裂前的信息增益
      defaultEnt= *infoGainCrit*.oldEnt(m_distribution);
      while(next < firstMiss) {
         //这里if语句是判断两个属性值是否太接近，若是那么选择的分裂点不会有太大的区别，就不进行处理。
**if**(trainInstances.instance(next - 1).value(m_attIndex) + 1e-5 < trainInstances
                .instance(next).value(m_attIndex)) {
// Move class values for all Instances up to next
// possible split point.
            //是把第一个bag中下标从last到next-1的样本移到第0个bag
m_distribution.shiftRange(1, 0,trainInstances, last, next);
// Check if enough Instances in each subset and compute
// values forcriteria.
            //如果在2个bag中都满足最小分裂条件，则计算在当前分裂节点下的信息增益。
**if** (Utils.*grOrEq*(m_distribution.perBag(0),minSplit) && Utils.*grOrEq*(m_distribution.perBag(1),
 minSplit)) {
                currentInfoGain= *infoGainCrit*.splitCritValue(m_distribution,
m_sumOfWeights, defaultEnt);
**if** (Utils.*gr*(currentInfoGain,m_infoGain)) {
                   //记录下所有尝试过分裂节点的最大增益及分裂的节点
m_infoGain = currentInfoGain;
                   splitIndex= next - 1;
                }
m_index++;
            }
            last= next;
         }
         next++;
      }
// Was there any useful split?
**if** (m_index == 0)
**return**;
// Compute modified information gain for best split.
**if** (m_useMDLcorrection){
m_infoGain =
m_infoGain - (Utils.*log2*(m_index) /
m_sumOfWeights);
      }
**if** (Utils.*smOrEq*(m_infoGain, 0))
**return**;
// Set instance variables' values to values for
// best split.
m_numSubsets = 2;
      //分裂节点取为当前分裂点与下一属性值的中间值
m_splitPoint =(trainInstances.instance(splitIndex + 1).value(m_attIndex) +trainInstances.instance(splitIndex).value(m_attIndex)) / 2;
// In case wehave a numerical precision problem we need to choose the
// smaller value
**if** (m_splitPoint ==trainInstances.instance(splitIndex + 1).value(m_attIndex)) {
m_splitPoint =trainInstances.instance(splitIndex).value(m_attIndex);
      }
// Restore distributioN for best split.
      //将instances按照分裂点分组到2个bag中去
m_distribution = **new** Distribution(2,trainInstances.numClasses());
m_distribution.addRange(0,trainInstances, 0, splitIndex + 1);
m_distribution.addRange(1,trainInstances, splitIndex + 1, firstMiss);
// Compute modified gain ratio for best split.
      //计算信息增益率
m_gainRatio = *gainRatioCrit*.splitCritValue(m_distribution, m_sumOfWeights,
m_infoGain);
   }
**3.1.2树节点折叠Collapse**
创建完树结构之后我们可以根据需要对树中节点进行分裂：
**publicfinalvoid**collapse() {
**double** errorsOfSubtree;
**double** errorsOfTree;
**int** i;
**if** (!m_isLeaf) {
         errorsOfSubtree= getTrainingErrors(); //计算以该节点为根的子树的训练误差
         errorsOfTree= localModel().distribution().numIncorrect(); //计算以该节点为根的树的训练误差
         //如果分裂后子树的训练误差更大，则取消对该节点的分裂，将该节点恢复为叶节点。
**if** (errorsOfSubtree >=errorsOfTree - 1E-3) {
// Free adjacent trees
m_sons = **null**;
m_isLeaf = **true**;
// Get NoSplit Model for tree.
m_localModel = **new**NoSplit(localModel().distribution());
         }**else**
**for** (i = 0; i <
m_sons.length; i++)
                son(i).collapse(); //递归的处理子树
      }
   }
**3.1.3树剪枝Prune：**
剪枝过程为从叶节点向根节点进行，采用减少训练误差的方法进行剪枝。
该算法过程为：对任一个非叶节点计算3个误差值：将该节点作为叶节点的训练误差errorsLeaf，取该节点中具有最多样例个数的子树并计算其训练误差errorsLargestBranch，一概节点为根的整棵树的误差errorsTree。如果errorsLeaf最小则将该树剪枝并将该节点设为叶子节点；如果errorsLargestBranch最小则将该子树分支替换该节点；否则不进行剪枝。
下面结合代码分析：
**publicvoid**prune() **throws**Exception {
**double** errorsLargestBranch;
**double** errorsLeaf;
**double** errorsTree;
**int** indexOfLargestBranch;
      C45PruneableClassifierTreelargestBranch;
**int** i;
**if** (!m_isLeaf) {
// Prune all subtrees.
         //递归剪枝子节点，这里可以保证从叶节点向上依次进行
**for** (i = 0; i <
m_sons.length; i++)
            son(i).prune();
// Compute error for largest branch
         //计算包含最多instance的子树分支的训练误差
         indexOfLargestBranch= localModel().distribution().maxBag();
**if** (m_subtreeRaising) {
            errorsLargestBranch= son(indexOfLargestBranch).getEstimatedErrorsForBranch((Instances)
m_train);
         }**else**{
            errorsLargestBranch= Double.*MAX_VALUE*;
         }
// Compute error if this Tree would be leaf
         //计算作为叶节点的训练误差
         errorsLeaf= getEstimatedErrorsForDistribution(localModel().distribution());
// Compute error for the whole subtree
         //计算以以该节点为根的树的训练误差
         errorsTree= getEstimatedErrors();
// Decide if leaf is best choice.
         //判断是否作为叶节点具有更小的误差
**if** (Utils.*smOrEq*(errorsLeaf,errorsTree + 0.1)
                &&Utils.*smOrEq*(errorsLeaf, errorsLargestBranch + 0.1)) {
// Free son Trees
m_sons = **null**;
m_isLeaf = **true**;
// Get NoSplit Model for node.
m_localModel = **new**NoSplit(localModel().distribution());
**return**;
         }
// Decide if largest branch is better choice
// than whole subtree.
         //判断是否以子树取代具有更小的训练误差
**if** (Utils.*smOrEq*(errorsLargestBranch,errorsTree + 0.1)) {
            largestBranch= son(indexOfLargestBranch);
m_sons = largestBranch.m_sons;
m_localModel =largestBranch.localModel();
m_isLeaf = largestBranch.m_isLeaf;
            newDistribution(m_train);
            prune();//递归的对该节点进行剪枝过程
         }
      }
   }
**3.2 样本分类**
对样本分类主要调用方法distributionForInstance(Instance instance)进行，
**publicfinaldouble**[]distributionForInstance(Instance instance)
**throws** Exception {
**return**m_root.distributionForInstance(instance,
m_useLaplace);
   }
这里从根节点开始递归的查找并计算该样例属于每个类的概率，概率最大的类即为该样例所属的类。
过程如下：
**publicfinaldouble**[]distributionForInstance(Instanceinstance,
**boolean** useLaplace)
**throws** Exception {
**double**[] doubles =
**newdouble**[instance.numClasses()];
      //对每个类都要计算概率
**for** (**int** i = 0; i < doubles.length; i++) {
**if** (!useLaplace) {
            doubles[i]= getProbs(i, instance, 1);
         }**else**{
            doubles[i]= getProbsLaplace(i, instance, 1);
         }
      }
**return** doubles;
   }
getProbs主要代码如下：
**privatedouble**getProbs(**int**classIndex, Instance instance,
**double** weight)
**throws** Exception {
**double** prob = 0;
      //如该节点为叶节点的话直接计算其概率
**if** (m_isLeaf) {
**return** weight *localModel().classProb(classIndex, instance, -1);
      }**else**{
         //判断该instance属于哪一个子树
**int**
treeIndex = localModel().whichSubset(instance);
**if** (treeIndex == -1) {
            //这里表示该阳历属于多个子树，则计算其在所有子树中的概率和
**double**[] weights =localModel().weights(instance);
**for** (**int** i = 0; i <
m_sons.length; i++) {
**if** (!son(i).m_isEmpty) {
                   prob+= son(i).getProbs(classIndex, instance, weights[i] * weight);
                }
            }
**return** prob;
         }**else**{
            //找到其对应的子树首先判断节点是否为空，然后计算概率
**if** (son(treeIndex).m_isEmpty) {
**return** weight * localModel().classProb(classIndex, instance,
treeIndex);
            }**else**{
**return** son(treeIndex).getProbs(classIndex,instance, weight);
            }
         }
      }
   }

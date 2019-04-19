# Weka进行数据挖掘简介 - 三少GG - CSDN博客
2013年12月10日 23:12:13[三少GG](https://me.csdn.net/scut1135)阅读数：2352
[使用Weka进行数据挖掘](http://stblog.baidu-tech.com/?p=1918)(2012-7-26 03:07:26)
标签： [Weka](http://stblog.baidu-tech.com/?tag=weka) , [数据挖掘](http://stblog.baidu-tech.com/?tag=%e6%95%b0%e6%8d%ae%e6%8c%96%e6%8e%98)    分类：[](http://stblog.baidu-tech.com/?p=1918#)[数据挖掘](http://stblog.baidu-tech.com/?cat=9)
## [](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420611061.JPG&type=image%2Fjpeg&width=305&height=449)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420611471.JPG&type=image%2Fjpeg&width=713&height=425)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420612311.JPG&type=image%2Fjpeg&width=390&height=362)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420612901.JPG&type=image%2Fjpeg&width=219&height=327)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420614461.JPG&type=image%2Fjpeg&width=550&height=189)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420616561.JPG&type=image%2Fjpeg&width=459&height=309)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420617171.JPG&type=image%2Fjpeg&width=382&height=203)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420617761.JPG&type=image%2Fjpeg&width=223&height=460)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420618181.JPG&type=image%2Fjpeg&width=382&height=299)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420620421.JPG&type=image%2Fjpeg&width=282&height=312)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420621391.JPG&type=image%2Fjpeg&width=284&height=219)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420626861.JPG&type=image%2Fjpeg&width=372&height=273)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420631141.JPG&type=image%2Fjpeg&width=372&height=273)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420631661.JPG&type=image%2Fjpeg&width=372&height=213)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420860961.JPG&type=image%2Fjpeg&width=436&height=311)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420861521.JPG&type=image%2Fjpeg&width=370&height=311)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420862281.JPG&type=image%2Fjpeg&width=489&height=336)[](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420862841.JPG&type=image%2Fjpeg&width=502&height=349)1.简介
数据挖掘、机器学习这些字眼，在一些人看来，是门槛很高的东西。诚然，如果做算法实现甚至算法优化，确实需要很多背景知识。**但事实是，绝大多数数据挖掘工程师，不需要去做算法层面的东西。他们的精力，集中在特征提取，算法选择和参数调优上。**那么，一个可以方便地提供这些功能的工具，便是十分必要的了。而weka，便是数据挖掘工具中的佼佼者。
Weka的全名是怀卡托智能分析环境（Waikato Environment for Knowledge Analysis），是一款免费的，非商业化的，基于JAVA环境下开源的机器学习以及数据挖掘软件。它和它的源代码可在其官方网站下载。有趣的是，该软件的缩写WEKA也是New Zealand独有的一种鸟名，而Weka的主要开发者同时恰好来自新西兰的the University of Waikato。（本段摘自百度百科）。
Weka提供的功能有数据处理，特征选择、分类、回归、聚类、关联规则、可视化等。本文将对Weka的使用做一个简单的介绍，并通过简单的示例，使大家了解使用weka的流程。本文将仅对图形界面的操作做介绍，不涉及命令行和代码层面的东西。
## 2.安装
Weka的官方地址是http://www.cs.waikato.ac.nz/ml/weka/。点开左侧download栏，可以进入下载页面，里面有windows，mac os，linux等平台下的版本，我们以windows系统作为示例。目前稳定的版本是3.6。
如果本机没有安装java，可以选择带有jre的版本。下载后是一个exe的可执行文件，双击进行安装即可。
安装完毕，打开启动weka的快捷方式，如果可以看到下面的界面，那么恭喜，安装成功了。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420609781.JPG&type=image%2Fjpeg&width=327&height=220)
图2.1 weka启动界面
窗口右侧共有4个应用，分别是
1）Explorer
用来进行数据实验、挖掘的环境，它提供了分类，聚类，关联规则，特征选择，数据可视化的功能。（An environment for exploring data with WEKA）
2）Experimentor
用来进行实验，对不同学习方案进行数据测试的环境。（An environment for performing experiments and conducting statistical tests between learning schemes.）
3）KnowledgeFlow
功能和Explorer差不多，不过提供的接口不同，用户可以使用拖拽的方式去建立实验方案。另外，它支持增量学习。（This environment supports essentially the same functions as the Explorer but with a drag-and-drop interface. One advantage is that it supports incremental learning.）
4）SimpleCLI
简单的命令行界面。（Provides a simple command-line interface that allows direct execution of WEKA commands for operating systems that do not provide their own command line interface.）
## 3.数据格式
Weka支持很多种文件格式，包括arff、xrff、csv，甚至有libsvm的格式。其中，arff是最常用的格式，我们在这里仅介绍这一种。
Arff全称是Attribute-Relation File Format，以下是一个arff格式的文件的例子。
%
%  Arff file example
%
@relation ‘labor-neg-data’
@attribute ‘duration’ real
@attribute ‘wage-increase-first-year’ real
@attribute ‘wage-increase-second-year’ real
@attribute ‘wage-increase-third-year’ real
@attribute ‘cost-of-living-adjustment’ {‘none’,'tcf’,'tc’}
@attribute ‘working-hours’ real
@attribute ‘pension’ {‘none’,'ret_allw’,'empl_contr’}
@attribute ’standby-pay’ real
@attribute ’shift-differential’ real
@attribute ‘education-allowance’ {‘yes’,'no’}
@attribute ’statutory-holidays’ real
@attribute ‘vacation’ {‘below_average’,'average’,'generous’}
@attribute ‘longterm-disability-assistance’ {‘yes’,'no’}
@attribute ‘contribution-to-dental-plan’ {‘none’,'half’,'full’}
@attribute ‘bereavement-assistance’ {‘yes’,'no’}
@attribute ‘contribution-to-health-plan’ {‘none’,'half’,'full’}
@attribute ‘class’ {‘bad’,'good’}
@data
1,5,?,?,?,40,?,?,2,?,11,’average’,?,?,’yes’,?,’good’
2,4.5,5.8,?,?,35,’ret_allw’,?,?,’yes’,11,’below_average’,?,’full’,?,’full’,'good’
?,?,?,?,?,38,’empl_contr’,?,5,?,11,’generous’,'yes’,'half’,'yes’,'half’,'good’
3,3.7,4,5,’tc’,?,?,?,?,’yes’,?,?,?,?,’yes’,?,’good’
3,4.5,4.5,5,?,40,?,?,?,?,12,’average’,?,’half’,'yes’,'half’,'good’
2,2,2.5,?,?,35,?,?,6,’yes’,12,’average’,?,?,?,?,’good’
3,4,5,5,’tc’,?,’empl_contr’,?,?,?,12,’generous’,'yes’,'none’,'yes’,'half’,'good’
3,6.9,4.8,2.3,?,40,?,?,3,?,12,’below_average’,?,?,?,?,’good’
2,3,7,?,?,38,?,12,25,’yes’,11,’below_average’,'yes’,'half’,'yes’,?,’good’
1,5.7,?,?,’none’,40,’empl_contr’,?,4,?,11,’generous’,'yes’,'full’,?,?,’good’
3,3.5,4,4.6,’none’,36,?,?,3,?,13,’generous’,?,?,’yes’,'full’,'good’
2,6.4,6.4,?,?,38,?,?,4,?,15,?,?,’full’,?,?,’good’
2,3.5,4,?,’none’,40,?,?,2,’no’,10,’below_average’,'no’,'half’,?,’half’,'bad’
这个例子来自于weka安装目录data文件下的labor.arff文件，来源于加拿大劳资谈判的案例，它根据工人的个人信息，来预测劳资谈判的最终结果。
文件中，“%”开头的是注释。剩余的可以分为两大部分，头信息（header information）和数据信息（data information）。
头信息中，“@relation”开头的行代表关系名称，在整个文件的第一行（除去注释）。格式是
@relation <relation-name>
“@attribute”开头的代表特征，格式是
@attribute <attribute-name> <datatype>
attribute-name是特征的名称，后面是数据类型，常用数据类型有以下几种
1）numeric，数字类型，包括integer（整数）和real（实数）
2）nominal，可以认为是枚举类型，即特征值是有限的集合，可以是字符串或数字。
3）string，字符串类型，值可以是任意的字符串。
从“@data”开始，是实际的数据部分。每一行代表一个实例，可以认为是一个特征向量。各个特征的顺序与头信息中的attribute逐个对应，特征值之间用逗号分割。在有监督分类中，最后一列是标注的结果。
某些特征的数值如果是缺失的，可以用“？”代替。
## 数据挖掘流程
使用weka进行数据挖掘的流程如下图
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420611061.JPG&type=image%2Fjpeg&width=305&height=449)
图4.1 数据挖掘流程图
其中，在weka内进行的是数据预处理，训练，验证这三个步骤。
1）数据预处理
数据预处理包括特征选择，特征值处理（比如归一化），样本选择等操作。
2）训练
训练包括算法选择，参数调整，模型训练。
3）验证
对模型结果进行验证。
本文剩余部分将以这个流程为主线，以分类为示例，介绍使用weka进行数据挖掘的步骤。
## 5. 数据预处理
打开Explorer界面，点“open file”，在weka安装目录下，选择data目录里的“labor.arff”文件，将会看到如下界面。我们将整个区域分为7部分，下面将分别介绍每部分的功能。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420611471.JPG&type=image%2Fjpeg&width=713&height=425)
图5.1 Explorer界面
1）区域1共6个选项卡，用来选择不同的数据挖掘功能面板，从左到右依次是Preprocess（预处理）、Classify（分类）、Cluster（聚类）、Associate（关联规则）、Select attribute（特征选择）和Visualize（可视化）。
2）区域2提供了打开、保存，编辑文件的功能。打开文件不仅仅可以直接从本地选择，还可以使用url和db来做数据源。Generate按钮提供了数据生成的功能，weka提供了几种生成数据的方法。点开Edit，将看到如下界面
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420612311.JPG&type=image%2Fjpeg&width=390&height=362)
图5.2 arff viewer
在这个界面，可以看到各行各列对应的值，右键每一列的名字，可以看到一些编辑数据的功能，这些功能还是比较实用的。
3）区域3名为Filter，有些人可能会联想到特征选择里面的Filter方法，事实上，Filter针对特征（attribute）和样本（instance）提供了大量的操作方法，功能十分强大。
4）在区域4，可以看到当前的特征、样本信息，并提供了特征选择和删除的功能。
5）在区域4用鼠标选择单个特征后，区域5将显示该特征的信息。包括最小值、最大值、期望和标准差。
6）区域6提供了可视化功能，选择特征后，该区域将显示特征值在各个区间的分布情况，不同的类别标签以不同的颜色显示。
7）区域7是状态栏，没有任务时，小鸟是坐着的，任务运行时，小鸟会站起来左右摇摆。如果小鸟站着但不转动，表示任务出了问题。
下面将通过实例介绍Filters的各项功能。
点开Filter下面的choose按钮，可以看到如下界面
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420612901.JPG&type=image%2Fjpeg&width=219&height=327)
图5.3 filter方法选择界面
Filters可分为两大类，supervised和unsupervised。supervised下的方法需要类别标签，而unsupervised则不需要。attribute类别表示对特征做筛选，instance表示对样本做选择。
**1）case 1：特征值归一化**
该项功能与类别无关，且是针对attribute的，我们选择unsupervised -> attribute下面的Normalize。点开Normalize所在的区域，将看到如下界面。左边的窗口，有几个参数可以选择。点击more，将出现右边的窗口，该窗口详细介绍了此功能。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420614461.JPG&type=image%2Fjpeg&width=550&height=189)
图5.4 归一化参数设置界面
使用默认参数，点击ok，回到主窗口。在区域4选好将要归一化的特征，可以是一个或多个，然后点击apply。在可视化区域中，我们可以看到特征值从1到3被归一到了0到1之间。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420616561.JPG&type=image%2Fjpeg&width=459&height=309)
**2）case 2: 分类器特征筛选**
该功能与类别相关，选择supervised -> attribute下面的AttributeSelection。该界面有两个选项，evaluator是评价特征集合有效性的方法，search是特征集合搜索的方法。在这里，我们使用InformationGainAttributeEval作为evaluator，使用Ranker作为search，表示我们将根据特征的信息增益值对特征做排序。Ranker中可以设置阈值，低于这个阈值的特征将被扔掉。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420617171.JPG&type=image%2Fjpeg&width=382&height=203)
图5.7 特征选择参数
点击apply，可以看到在区域4里特征被重新排序，低于阈值的已被删掉。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420617761.JPG&type=image%2Fjpeg&width=223&height=460)
**3）case 3：选择分类器错分的样本**
选择unsupervised -> instance下面的RemoveMisclassified，可以看到6个参数，classIndex用来设置类别标签，classifier用来选择分类器，这里我们选择J48决策树，invert我们选择true，这样保留的是错分样本，numFolds用来设置交叉验证的参数。设置好参数之后，点击apply，可以看到样本的数量从57减少到了7。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420618181.JPG&type=image%2Fjpeg&width=382&height=299)
图5.10 参数设置
## 6. 分类
在Explorer中，打开classifer选项卡，整个界面被分成几个区域。分别是
**1）Classifier**
点击choose按钮，可以选择weka提供的分类器。常用的分类器有
a）bayes下的Naïve Bayes（朴素贝叶斯）和BayesNet（贝叶斯信念网络）。
b）functions下的LibLinear、LibSVM（这两个需要安装扩展包）、Logistic Regression、Linear Regression。
c）lazy下的IB1（1-NN）和IBK（KNN）。
d）meta下的很多boosting和bagging分类器，比如AdaBoostM1。
e）trees下的J48（weka版的C4.5）、RandomForest。
**2）Test options**
评价模型效果的方法，有四个选项。
a）Use training set：使用训练集，即训练集和测试集使用同一份数据，一般不使用这种方法。
b）Supplied test set：设置测试集，可以使用本地文件或者url，测试文件的格式需要跟训练文件格式一致。
c）Cross-validation：交叉验证，很常见的验证方法。N-folds cross-validation是指，将训练集分为N份，使用N-1份做训练，使用1份做测试，如此循环N次，最后整体计算结果。
d）Percentage split：按照一定比例，将训练集分为两份，一份做训练，一份做测试。
在这些验证方法的下面，有一个More options选项，可以设置一些模型输出，模型验证的参数。
**3）Result list**
这个区域保存分类实验的历史，右键点击记录，可以看到很多选项。常用的有保存或加载模型以及可视化的一些选项。
**4）Classifier output**
分类器的输出结果，默认的输出选项有Run information，该项给出了特征、样本及模型验证的一些概要信息；Classifier model，给出的是模型的一些参数，不同的分类器给出的信息不同。最下面是模型验证的结果，给出了   一些常用的一些验证标准的结果，比如准确率（Precision），召回率（Recall），真阳性率（True positive rate），假阳性率（False positive rate），F值（F-Measure），Roc面积（Roc Area）等。Confusion Matrix给出了测试样本的分类情况，通过它，可以很方便地看出正确分类或错误分类的某一类样本的数量。
**Case 1：使用J48对labor文件做分类**
1）打开labor.arff文件，切换到classify面板。
2）选择trees->J48分类器，使用默认参数。
3）Test options选择默认的十折交叉验证，点开More options，勾选Output predictions。
4）点击start按钮，启动实验。
5）在右侧的Classifier output里面，我们看到了实验的结果。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420620421.JPG&type=image%2Fjpeg&width=282&height=312)
图6.1 Run information
上图给出了实验用的分类器以及具体参数，实验名称，样本数量，特征数量以及所用特征，测试模式。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420621391.JPG&type=image%2Fjpeg&width=284&height=219)
图6.2 模型信息
上图给出了生成的决策树，以及叶子节点数、树的节点数、模型训练时间。如果觉得这样不直观，可以在Result list里面右键点击刚刚进行的实验，点击Visualize Tree，可以看到图形界面的决策树，十分直观。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420631141.JPG&type=image%2Fjpeg&width=372&height=273)
图6.3 决策树
再往下是预测结果，可以看到每个样本的实际分类，预测分类，是否错分，预测概率这些信息。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420631661.JPG&type=image%2Fjpeg&width=372&height=213)
图6.4 预测结果
最下面是验证结果，整体的accuracy是73.68%，bad类准确率是60.9%，召回率70.0%，good类准确率是82.4%，召回率75.7%。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420860961.JPG&type=image%2Fjpeg&width=436&height=311)
图6.5 模型效果评估结果
## 7. 可视化
打开Explorer的Visualize面板，可以看到最上面是一个二维的图形矩阵，该矩阵的行和列均为所有的特征（包括类别标签），第i行第j列表示特征i和特征j在二维平面上的分布情况。图形上的每个点表示一个样本，不同的类别使用不同的颜色标识。
下面有几个选项，PlotSize可以调整图形的大小，PointSize可以调整样本点的大小，Jitter可以调整点之间的距离，有些时候点过于集中，可以通过调整Jitter将它们分散开。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420861521.JPG&type=image%2Fjpeg&width=370&height=311)
图7.1 plot matrix二维图
上图是duration和class两个特征的图形，可以看出，duration并不是一个好特征，在各个特征值区间，good和bad的分布差不多。
单击某个区域的图形，会弹出另外一个窗口，这个窗口给出的也是某两个特征之间分布的图形，不同的是，在这里，通过点击样本点，可以弹出样本的详细信息。
可视化还可以用来查看误分的样本，这是非常实用的一个功能。分类结束后，在Result list里右键点击分类的记录，选择Visualize classify errors，会弹出如下窗口。
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420862281.JPG&type=image%2Fjpeg&width=489&height=336)
图7.2 误分样本可视化
这个窗口里面,十字表示分类正确的样本，方块表示分类错误的样本，X轴为实际类别，Y轴为预测类别，蓝色为实际的bad，红色为实际的good。这样，蓝色方块就表示实际为bad，但为误分为good的样本，红色方块表示实际为good，被误分为bad的样本。单击这些点，便可以看到该样本的各个特征值，分析为什么这个样本被误分了。
再介绍一个比较实用的功能，右键点击Result list里的记录，选择Visualize threshold curve，然后选好类别，可以看到如下图形
![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=13420862841.JPG&type=image%2Fjpeg&width=502&height=349)
图7.3 阈值曲线
该图给出的是分类置信度在不同阈值下，分类效果评价标准的对比情况。上图给出的是假阳性比率和真阳性比率在不同阈值下的对比，其实给出的就是ROC曲线。我们可以通过选择颜色，方便地观察不同评价标准的分布情况。如果X轴和Y轴选择的是准确率和召回率，那我们可以通过这个图，在这两个值之间做trade-off，选择一个合适的阈值。
其它的一些可视化功能，不再一一介绍。
## 8. 小结
本文仅仅针对weka的Explorer界面的某些功能做了介绍，Explorer其它的功能，比如聚类、关联规则、特征选择，以及Experimentor和KnowledgeFlow界面使用，可以参考weka的官方文档。
另外，weka支持扩展包，可以很方便地把liblinear、libsvm这样的开源工具放进来。
在Linux下面，可以使用weka的命令行进行实验，具体的使用方法，也请参考weka官方文档。
有这样一款开源、免费、强大的数据挖掘工具，你还在等什么呢？没有用过weka的数据挖掘工程师们，赶紧行动吧.

d
# 用 WEKA 进行数据挖掘，第 1 部分: 简介和回归
数据挖掘是技术界的谈论话题，因为各公司都在生成有关其用户的数百万的数据点并在想方设法将该信息转变为收入的增加。数据挖掘是很多技术的共同术语，用以表达从数据中一点点地收集信息并将其转变成有实际意义的东西。本文将向您介绍开源的数据挖掘软件以及用来解析数据的最为常见的一些技术。
[查看本系列更多内容](http://www.ibm.com/developerworks/cn/views/opensource/libraryview.jsp?search_by=%E7%94%A8+WEKA+%E8%BF%9B%E8%A1%8C%E6%95%B0%E6%8D%AE%E6%8C%96%E6%8E%98%EF%BC%8C)|4![](http://dw1.s81c.com/developerworks/i/v17/dw-cmts-arrow.png)[评论：](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#icomments)
[Michael
 Abernethy](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#authorN10033), 产品开发经理, Optimal Auctions
2010 年 5 月 17 日
- ![+](http://www.ibm.com/i/c.gif)内容

## 简介
什么是 数据挖掘？您会不时地问自己这个问题，因为这个主题越来越得到技术界的关注。您可能听说过像 Google 和 Yahoo! 这样的公司都在生成有关其所有用户的数十亿的数据点，您不禁疑惑，“它们要所有这些信息干什么？”您可能还会惊奇地发现 Walmart 是最为先进的进行数据挖掘并将结果应用于业务的公司之一。现在世界上几乎所有的公司都在使用数据挖掘，并且目前尚未使用数据挖掘的公司在不久的将来就会发现自己处于极大的劣势。
那么，您如何能让您和您的公司跟上数据挖掘的大潮呢？
我们希望能够回答您所有关于数据挖掘的初级问题。我们也希望将一种免费的开源软件 Waikato Environment for Knowledge Analysis (WEKA) 介绍给您，您可以使用该软件来挖掘数据并将您对您用户、客户和业务的认知转变为有用的信息以提高收入。您会发现要想出色地完成挖掘数据的任务并不像您想象地那么困难。
此外，本文还会介绍数据挖掘的第一种技术：回归，意思是根据现有的数据预测未来数据的值。 它可能是挖掘数据最为简单的一种方式，您甚至以前曾经用您喜爱的某个流行的电子数据表软件进行过这种初级的数据挖掘（虽然 WEKA 可以做更为复杂的计算）。本系列后续的文章将会涉及挖掘数据的其他方法，包括群集、最近的邻居以及分类树。（如果您还不太知道这些术语是何意思，没关系。我们将在这个系列一一介绍。）
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## 什么是数据挖掘？
数据挖掘，就其核心而言，是指将大量数据转变为有实际意义的模式和规则。并且，它还可以分为两种类型：直接的和间接的。在 直接的 数据挖掘中，您会尝试预测一个特定的数据点 — 比如，以给定的一个房子的售价来预测邻近地区内的其他房子的售价。
在 间接的 数据挖掘中，您会尝试创建数据组或找到现有数据内的模式 — 比如，创建 “中产阶级妇女”的人群。实际上，每次的美国人口统计都是在进行数据挖掘，政府想要收集每个国民的数据并将它转变为有用信息。
现代的数据挖掘开始于 20 世纪 90 年代，那时候计算的强大以及计算和存储的成本均到达了一种很高的程度，各公司开始可以自己进行计算和存储，而无需再借助外界的计算帮助。
此外，术语数据挖掘是全方位的，可指代诸多查看和转换数据的技术和过程。因为本系列只触及能用数据挖掘实现的功能的一些皮毛。数据挖掘的专家往往是数据统计方面的博士，并在此领域有 10-30 年的研究经验。这会为您留下一种印象，即只有大公司才能负担得起数据挖掘。
我们希望能够清除有关数据挖掘的这些误解并希望弄清楚一点：数据挖掘既不像对一系列数据运行一个电子数据表函数那么简单，也不像有人想的那样难到靠自己根本无法实现。这是 80/20 范型的一个很好的例子 — 甚至更进一步可以是 90/10 范型。您用所谓的数据挖掘专家的 10% 的专业知识就能创建具有 90% 效力的数据挖掘模型。而为了补上模型剩下的 10% 的效力并创建一个完美的模型将需要 90% 额外的时间，甚至长达 20 年。所以除非您立志以数据挖掘为职业方向，否则“足够好”就可以了。从另一个方面看，利用数据挖掘所达到的“足够好”总要比您现在所采用的其他技术要好。
数据挖掘的最终目标就是要创建一个模型，这个模型可改进您解读现有数据和将来数据的方式。由于现在已经有很多数据挖掘技术，因此创建一个好的模型的最主要的步骤是决定要使用哪种技术。而这则极大地依赖于实践和经验以及有效的指导。这之后，需要对模型进行优化以让它更令人满意。在阅读了本系列的文章后 ，您应该能够自己根据自己的数据集正确决定要使用的技术，然后采取必要的步骤对它进行优化。您将能够为您自己的数据创建一个足够好的模型 。
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## WEKA
数据挖掘绝非大公司的专有，也不是多昂贵的软件。实际上，有一种软件可以实现那些价格不菲的软件所能实现的全部功能 — 这个软件就是 WEKA（参见 [参考资料](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#resources)）。WEKA
 诞生于 University of Waikato（新西兰）并在 1997 年首次以其现代的格式实现。它使用了 GNU General Public License (GPL)。该软件以 Java™ 语言编写并包含了一个 GUI 来与数据文件交互并生成可视结果（比如表和曲线）。它还有一个通用 API，所以您可以像嵌入其他的库一样将 WEKA 嵌入到您自己的应用程序以完成诸如服务器端自动数据挖掘这样的任务。
我们继续并安装 WEKA。因为它基于 Java，所以如果您在计算机上没有安装 JRE，那么请下载一个包含 JRE 的 WEKA 版本。
##### 图 1. WEKA 的开始屏
![这个屏幕快照显示了正在打开的 WEKA 屏幕，内含 Explorer、Experimenter、KnowledgeFlow 和 Sinple CLI 选项](http://www.ibm.com/developerworks/cn/opensource/os-weka1/weka-startup1.jpg)
在启动 WEKA 时，会弹出 GUI 选择器，让您选择使用 WEKA 和数据的四种方式。对于本文章系列中的例子，我们只选择了 Explorer 选项。对于我们要在这些系列文章中所需实现的功能，这已经足够。
##### 图 2. WEKA Explorer
![这个屏幕快照显示了具有各种显示和动作按钮的 Explorer 工具](http://www.ibm.com/developerworks/cn/opensource/os-weka1/weka-data.jpg)
在熟悉了如何安装和启动 WEKA 后，让我们来看看我们的第一个数据挖掘技术：回归。
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## 回归
回归是最为简单易用的一种技术，但可能也是最不强大（这二者总是相伴而来，很有趣吧）。此模型可以简单到只有一个输入变量和一个输出变量（在 Excel 中称为 Scatter 图形，或 OpenOffice.org 内的 XYDiagram）。当然，也可以远比此复杂，可以包括很多输入变量。实际上，所有回归模型均符合同一个通用模式。多个自变量综合在一起可以生成一个结果 — 一个因变量。然后用回归模型根据给定的这些自变量的值预测一个未知的因变量的结果。
每个人都可能使用过或看到过回归模型，甚至曾在头脑里创建过一个回归模型。人们能立即想到的一个例子就是给房子定价。房子的价格（因变量）是很多自变量 — 房子的面积、占地的大小、厨房是否有花岗石以及卫生间是否刚重装过等的结果。所以，不管是购买过一个房子还是销售过一个房子，您都可能会创建一个回归模型来为房子定价。这个模型建立在邻近地区内的其他有可比性的房子的售价的基础上（模型），然后再把您自己房子的值放入此模型来产生一个预期价格。
让我们继续以这个房屋定价的回归模型为例，创建一些真实的数据。在我的邻近地区有一些房子出售，我试图找到我自己房子的合理价格。我还需要拿此模型的输出申报财产税。
##### 表 1. 回归模型的房屋值
|房子面积（平方英尺）|占地的大小|卧室|花岗岩|卫生间有无重装？|销售价格|
|----|----|----|----|----|----|
|3529|9191|6|0|0|$205,000|
|3247|10061|5|1|1|$224,900|
|4032|10150|5|0|1|$197,900|
|2397|14156|4|1|0|$189,900|
|2200|9600|4|0|1`|$195,000|
|3536|19994|6|1|1|$325,000|
|2983|9365|5|0|1|$230,000|
|||||||
|3198|9669|5|1|1|????|
好的消息是（也可能是坏消息，取决于您自己的看法）上述对回归模型的简单介绍只触及了一些皮毛，这种触及甚至都不会被真正地注意到。关于回归模型有大学的课程可以选择，那会教授给您有关回归模型的更多信息，甚至多过您想要知道的。但我们的简介让您充分熟悉了这个概念，已足够应付本文中 WEKA 试用。如果对回归模型以及其中的数据统计的细节有更深的兴趣，您可以用自己喜爱的搜索引擎搜索如下的术语：least squares、homoscedasticity、normal distribution、White tests、Lilliefors
 tests、R-squared 和 p-values。
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## 为 WEKA 构建数据集
为了将数据加载到 WEKA，我们必须将数据放入一个我们能够理解的格式。WEKA 建议的加载数据的格式是 Attribute-Relation File Format (ARFF)，您可以在其中定义所加载数据的类型，然后再提供数据本身。在这个文件内，我们定义了每列以及每列所含内容。对于回归模型，只能有`NUMERIC` 或 `DATE` 列。最后，以逗号分割的格式提供每行数据。我们为
 WEKA 使用的 ARFF 文件如下所示。请注意在数据行内，并未包含我的房子。因为我们在创建模型，我房子的价格还不知道，所以我们还不能输入我的房子。
##### 清单 1. WEKA 文件格式
@RELATION house
@ATTRIBUTE houseSize NUMERIC
@ATTRIBUTE lotSize NUMERIC
@ATTRIBUTE bedrooms NUMERIC
@ATTRIBUTE granite NUMERIC
@ATTRIBUTE bathroom NUMERIC
@ATTRIBUTE sellingPrice NUMERIC
@DATA
3529,9191,6,0,0,205000 
3247,10061,5,1,1,224900 
4032,10150,5,0,1,197900 
2397,14156,4,1,0,189900 
2200,9600,4,0,1,195000 
3536,19994,6,1,1,325000 
2983,9365,5,0,1,230000
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## 将数据载入 WEKA
数据创建完成后，就可以开始创建我们的回归模型了。启动 WEKA，然后选择 Explorer。将会出现 Explorer 屏幕，其中 Preprocess 选项卡被选中。选择 Open
 File 按钮并选择在上一节中创建的 ARFF 文件。在选择了文件后，WEKA Explorer 应该类似于图 3 中所示的这个屏幕快照。
##### 图 3. 房屋数据加载后的 WEKA
![这个屏幕快照显示了数据加载后的 WEKA Explorer](http://www.ibm.com/developerworks/cn/opensource/os-weka1/weka-data1.jpg)
在这个视图中，WEKA 允许您查阅正在处理的数据。在 Explorer 窗口的左边，给出了您数据的所有列（Attributes）以及所提供的数据行的数量（Instances）。若选择一列，Explorer 窗口的右侧就会显示数据集内该列数据的信息。比如，通过选择左侧的 houseSize 列（它应该默认选中），屏幕右侧就会变成显示有关该列的统计信息。它显示了数据集内此列的最大值为
 4,032 平方英尺，最小值为 2,200 平方英尺。平均大小为 3,131 平方英尺，标准偏差为 655 平方英尺（标准偏差是一个描述差异的统计量度）。此外，还有一种可视的手段来查看数据，单击 Visualize All 按钮即可。由于在这个数据集内的行数有限，因此可视化的功能显得没有有更多数据点（比如，有数百个）时那么功能强大。
好了，对数据的介绍已经够多了。让我们立即创建一个模型来获得我房子的价格。
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## 用 WEKA 创建一个回归模型
为了创建这个模型，单击 Classify 选项卡。第一个步骤是选择我们想要创建的这个模型，以便 WEKA 知道该如何处理数据以及如何创建一个适当的模型：
- 
单击 Choose 按钮，然后扩展 functions 分支。
- 
选择 LinearRegression 叶。
这会告诉 WEKA 我们想要构建一个回归模型。除此之外，还有很多其他的选择，这说明可以创建的的模型有很多。非常多！这也从另一个方面说明本文只介绍了这个主题的皮毛。有一点值得注意。在同一个分支还有另外一个选项，称为 SimpleLinearRegression 。请不要选择该选项，因为简单回归只能有一个变量，而我们有六个变量。选择了正确的模型后，WEKA
 Explorer 应该类似于图 4。
##### 图 4. WEKA 内的线性回归模型
![这个屏幕快照显示了 WEKA 的线性回归模型内的数据集](http://www.ibm.com/developerworks/cn/opensource/os-weka1/weka-data2.jpg)
## 我能用电子数据表达到同样的目的么？
简单而言：不可以。深思熟虑后的答案是：可以。大多数流行的电子数据表程序都不能轻松完成我们用 WEKA 实现的功能，即定义一个具有多个自变量的线性模型。不过，您可以 十分容易地实现一个 Simple Linear Regression 模型（具有一个自变量）。如果您有足够的勇气，甚至可以进行一个多变量的回归，但是这将非常困难，绝对不如用
 WEKA 来得简单。 本文的 [参考资料](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#resources) 部分有一个 Microsoft®
 Excel® 的示例视频。
现在，选择了想要的模型后，我们必须告诉 WEKA 它创建这个模型应该使用的数据在哪里。虽然很显然我们想要使用在 ARFF 文件内提供的那些数据，但实际上有不同的选项可供选择，有些甚至远比我们将要使用的选项高级。其他的三个选择是：Supplied test set 允许提供一个不同的数据集来构建模型； Cross-validation 让
 WEKA 基于所提供的数据的子集构建一个模型，然后求出它们的平均值来创建最终的模型；Percentage splitWEKA 取所提供数据的百分之一来构建一个最终的模型。这些不同的选择对于不同的模型非常有用，我们在本系列后续文章中会看到这一点。对于回归，我们可以简单地选择 Use
 training set。这会告诉 WEKA 为了构建我们想要的模型，可以使用我们在 ARFF 文件中提供的那些数据。
创建模型的最后一个步骤是选择因变量（即我们想要预测的列）。在本例中指的就是房屋的销售价格，因为那正是我们想要的。在这些测试选项的正下方，有一个组合框，可用它来选择这个因变量。列 sellingPrice 应该默认选中。如果没有，请选择它。
我们准备好创建模型后，单击 Start。图 5 显示了输出结果。
##### 图 5. WEKA 内的房屋价格回归模型
![这个屏幕快照显示了来自 WEKA 的线性回归模型的总结报告](http://www.ibm.com/developerworks/cn/opensource/os-weka1/weka-data3.jpg)
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## 解析这个回归模型
WEKA 可不马虎。它会把这个回归模型径直放在输出，如清单 2 所示。
##### 清单 2. 回归输出
sellingPrice = (-26.6882   * houseSize) + 
               (7.0551     * lotSize) + 
               (43166.0767 * bedrooms) +
               (42292.0901 * bathroom) 
             - 21661.1208
清单 3 显示了结果，其中已经插入了我房子的价格。
##### 清单 3. 使用回归模型的房屋价格
sellingPrice = (-26.6882   * 3198) + 
               (7.0551     * 9669) + 
               (43166.0767 * 5) + 
               (42292.0901 * 1) 
             - 21661.1208
sellingPrice = 219,328
不过，回过头来看看本文的开头部分，我们知道数据挖掘绝不是仅仅是为了输出一个数值：它关乎的是识别模式和规则。它不是严格用来生成一个绝对的数值，而是要创建一个模型来让您探测模式、预测输出并根据这些数据得出结论。让我们更进一步来解读一下我们的模型除了房屋价格之外告诉我们的模式和结论：
- 花岗石无关紧要— WEKA 将只使用在统计上对模型的正确性有贡献的那些列（以 R-squared 量度，但这超出了本文的范围）。它将会抛弃并忽视对创建好的模型没有任何帮助的那些列。所以这个回归模型告诉我们厨房里的花岗石并不会影响房子的价值。
- 卫生间是有关系的— 因我们为卫生间使用了简单的 0 或 1 值，所以我们可以使用来自回归模型的这个系数来决定卫生间的这个值对房屋价值的影响。这个模型告诉我们它使房子的价值增加了 $42,292。
- 较大的房子价格反而低— WEKA 告诉我们房子越大，销售价格越低？这可以从 `houseSize` 变量前面负的系数看出来。此模型告诉我们房子每多出一平方英尺都会使房价减少
 $26？这根本没有意义。这是在美国！当然是房子越大越好，特别是在我所在的得克萨斯州。那么我们怎么才能解释这一点呢？这是无用数据入、无用数据出的一个很好的例子。房子的大小并不是一个自变量，它还与卧室变量相关，因为房子大通常卧室也多。所以我们的模型并不完美。但是我们可以修复这个问题。还记得么：在 Preprocess 选项卡，可以从数据集中删除列。对于本例，我们删除houseSize 列并创建另一个模型。那么它会如何影响房子的价格呢？这个新模型又如何更有实际意义？（修改后的我的房子价格是：
 $217,894）。
## 对统计学家的一个提示
这个模型打破了一个常规线性回归模型的几个要求，因为每个列并不是完全独立的，并且这里也没有足够的数据行来生成一个有效的模型。由于本文主要的目的是介绍 WEKA 这个数据挖掘工具，因此我们极大地简化了示例数据。
要想把这个简单的示例提升到一个新的级别，让我们来看一下 WEKA Web 站点上作为回归示例提供给我们的一个数据文件。理论上讲，这要比我们七个房子的简单示例要复杂得多。这个示例数据文件的作用是创建一个能基于汽车的几个特性来推测其油耗（每加仑英里数，MPG）的回归模型（请务必记住，数据取自 1970 至 1982 年）。这个模型包括汽车的如下属性：汽缸、排量、马力、重量、加速度、年份、产地及制造商。此外，这个数据集有 398 行数据，这些数据足以满足我们的多种统计需求，而这在我们的房价模型中是不能实现的。理论上讲，这是一个极为复杂的回归模型，WEKA
 可能需要大量时间才能创建一个具有如此多数据的模型（但我估计您已预见到了 WEKA 能够很好地处理这些数据）。
要用这个数据集生成一个回归模型，我们需要严格地按照处理房子数据的步骤来处理这些数据，所以这里我不再赘述。继续并创建这个回归模型。它将生成如清单 4 所示的输出。
##### 清单 4. MPG 数据回归模型
class (aka MPG) =
     -2.2744 * cylinders=6,3,5,4 +
     -4.4421 * cylinders=3,5,4 +
      6.74   * cylinders=5,4 +
      0.012  * displacement +
     -0.0359 * horsepower +
     -0.0056 * weight +
      1.6184 * model=75,71,76,74,77,78,79,81,82,80 +
      1.8307 * model=77,78,79,81,82,80 +
      1.8958 * model=79,81,82,80 +
      1.7754 * model=81,82,80 +
      1.167  * model=82,80 +
      1.2522 * model=80 +
      2.1363 * origin=2,3 +
      37.9165
在您自已生成这个模型时，您会看到 WEKA 只用了不到一秒的时间就处理好了这个模型。所以，即使要处理的是具有大量数据的功能强大的回归模型，就计算而言，也不是什么问题。这个模型看上去应该比房子数据复杂得多，但事实并非如此。例如，这个回归模型的首行，
```
-2.2744
 * cylinders=6,3,5,4
```
 表示，如果汽车有 6 个缸，就会在此列中放上一个 1，如果汽车有 8 个缸，就会放上一个 0。让我们从这个数据集中取一个示例行（第 10 行）并将这些数值放入回归模型，看看我们这个模型的输出是否与数据集中提供给我们的输出相似。
##### 清单 5. 示例 MPG 数据
data = 8,390,190,3850,8.5,70,1,15
class (aka MPG) =
     -2.2744 * 0 +
     -4.4421 * 0 +
      6.74   * 0 +
      0.012  * 390 +
     -0.0359 * 190 +
     -0.0056 * 3850 +
      1.6184 * 0 +
      1.8307 * 0 +
      1.8958 * 0 +
      1.7754 * 0 +
      1.167  * 0 +
      1.2522 * 0 +
      2.1363 * 0 +
     37.9165
     
Expected Value = 15 mpg
Regression Model Output = 14.2 mpg
因此，当我们用随机选择的测试数据对此模型进行测试时，此模型的表现非常出色，对于一辆实际值为 15 MPG 的车，我们的预测值是 14.2 MPG。
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## 结束语
本文通过向您介绍数据挖掘这个主题的背景以及这个领域的目标力求回答“什么是数据挖掘”这个问题。数据挖掘就是通过创建模型和规则来将大量的不可用信息（通常是分散的数据形式）变成有用的信息。您的目标是使用模型和规则来预测将来的行为，从而改进您的业务，或是解释一些您用其他方法不能解释的事情。这些模型可以帮助您确认您已经有了的某些想法，甚至可能会让您发现数据中您以前不曾意识到的新东西。这里有个有趣的数据挖掘的例子（不知道还存在多少类似的事例），在美国，Walmart 会在周末时把啤酒移到尿布货架的未端，这是因为 Walmart
 的数据挖掘结果显示男士通常会在周末购买尿布，而他们同时也喜欢在周末喝啤酒。
本文还向您介绍了一种免费的开源软件程序 WEKA。当然，市场上还有很多更为复杂的数据挖掘商业软件产品，但对于刚开始进行数据挖掘的人来说，这种开源的解决方案非常有益。请记住，您永远不可能成为数据挖掘方面的专家，除非您打算用 20 年的时间来研究它。WEKA 可以让您步入数据挖掘的大门，同时也能为您遇到的初级问题提供完美的解决方案。如果您以前对数据挖掘接触不多，那么这个非常好的解决方案将能满足您的全部所需。
最后，本文探讨了第一个数据挖掘模型：回归模型（特别是线性回归多变量模型），另外还展示了如何在 WEKA 中使用它。这个回归模型很容易使用，并且可以用于很多数据集。您会发现这个模型是我在本系列文章中所讨论的所有模型中最有用的一个。然而，数据挖掘不仅局限于简单的回归，在不同的数据集及不同的输出要求的情况下，您会发现其他的模型也许是更好的解决方案。
最后，我再重申一下，本文及本系列的后续文章只是对数据统计和数据挖掘领域做了最简单的介绍。花上整整一学期的时间去学习数据统计和数据挖掘的人也只能被称为“初学者”。 我们的目的就是让初学者充分领略这个可用的开源工具的妙处并提高对数据挖掘所能提供的价值的了解和重视。
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-weka1/#ibm-pcon)
## 下载
|描述|名字|大小|
|----|----|----|
|[os-weka1-Examples.zip](http://www.ibm.com/developerworks/apps/download/index.jsp?contentid=490138&filename=os-weka1-Examples.zip&method=http&locale=zh_CN)|6KB| |
## 参考资料
### 学习
- 
WEKA 要求所有关于它的出版物都必须提及这篇题为 “[The
 WEKA Data Mining Software: An Update](http://www.kdd.org/explorations/issues/11-1-2009-07/p2V11n1.pdf)” （作者 Mark Hall、Eibe Frank、Geoffrey Holmes、Bernhard Pfahringer Peter Reutemann 和 Ian H. Witten）的论文。
- 
利用 [YouTube 上的这个视频](http://www.youtube.com/watch?v=YUmYNutmbRM) 了解如何将一个电子数据表用于一个简单的回归模型。
- 
查阅 [WEKA Web 站点](http://www.cs.waikato.ac.nz/ml/WEKA/) 获得此软件的所有文档和一个 FAQ。
- 
在 [Wikipedia 上查阅回归模型](http://en.wikipedia.org/wiki/Regression_analysis)，这里可能有比您想象中还要多的技术细节。
- 
阅读有关 [ARFF](http://weka.wikispaces.com/ARFF+%28book+version%29) 的详细信息，以便您可以将数据加载入 WEKA。
- 
IBM 也有自己的数据挖掘软件， “ [Integrate
 InfoSphere Warehouse data mining with IBM Cognos reporting, Part 1](http://www.ibm.com/developerworks/data/library/techarticle/dm-0810wurst/)” 提供了一个很好的起点。
- 
要收听面向软件开发人员的有趣访谈和讨论，请访问 [developerWorks podcasts](http://www.ibm.com/developerworks/podcast/)。
- 
随时关注 developerWorks [技术活动](http://www.ibm.com/developerworks/cn/offers/techbriefings/)和[网络广播](http://www.ibm.com/developerworks/cn/swi/)。
- 
查阅最近将在全球举办的面向 IBM 开放源码开发人员的研讨会、交易展览、网络广播和其他 [活动](http://www.ibm.com/developerworks/views/opensource/events.jsp)。
- 
访问 developerWorks [Open source 专区](http://www.ibm.com/developerworks/cn/opensource/)获得丰富的
 how-to 信息、工具和项目更新以及[最受欢迎的文章和教程](http://www.ibm.com/developerworks/cn/opensource/best2009/index.html)，帮助您用开放源码技术进行开发，并将它们与
 IBM 产品结合使用。
- [developerWorks 社区](http://www.ibm.com/developerworks/community/) 是流行社区的一个成功典范，包含广泛的主题。
- 
查看免费的 [developerWorks 演示中心](http://www.ibm.com/developerworks/cn/offers/lp/demos/)，观看并了解
 IBM 及开源技术和产品功能。
### 获得产品和技术
- [下载 WEKA](http://www.cs.waikato.ac.nz/ml/WEKA/index_downloading.html) 并在您的系统上运行它。
- 
您可以查看有关 IBM [DB2 Intelligent Miner](http://www-01.ibm.com/software/data/iminer/) 软件的详细信息以便与
 WEKA 进行对比。
- 
使用 [IBM 产品评估试用版软件](http://www.ibm.com/developerworks/cn/downloads/)改进您的下一个开源开发项目，这些软件可以通过下载获得。
- 
下载 [IBM 产品评估试用版软件](http://www.ibm.com/developerworks/cn/downloads/) 或 [IBM
 SOA Sandbox for People](http://www.ibm.com/developerworks/cn/downloads/soasandbox/people/) 并使用来自 DB2®、Lotus®、Rational®、Tivoli® 和 WebSphere® 的应用程序开发工具和中间件产品。
### 讨论
- [参与论坛讨论](http://www.ibm.com/developerworks/forums/dw_forum.jsp?forum=375&cat=5)。
- 
此外，请查阅 My developerWorks 上的这个新的 [Data
 Mining](https://www.ibm.com/developerworks/mydeveloperworks/groups/service/html/communityview?communityUuid=8c11dc9f-dab3-46f1-83ba-1430256bc332) 组。
- 
参与 [developerWorks 博客](http://www.ibm.com/developerworks/blogs) 并加入 developerWorks 社区。


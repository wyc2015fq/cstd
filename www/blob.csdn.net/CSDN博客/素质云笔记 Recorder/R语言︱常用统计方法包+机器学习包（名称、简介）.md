
# R语言︱常用统计方法包+机器学习包（名称、简介） - 素质云笔记-Recorder... - CSDN博客

2016年02月11日 15:52:27[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：14508




# 一、一些函数包大汇总

转载于：http://www.dataguru.cn/thread-116761-1-1.html
时间上有点过期，下面的资料供大家参考
基本的R包已经实现了传统多元统计的很多功能，然而CRNA的许多其它包提供了更深入的多元统计方法，下面要综述的包主要分为以下几个部分：
**1） 多元数据可视化（Visualising multivariate data）：**
绘图方法：
基本画图函数（如：pairs()、coplot()）和

lattice包里的画图函数（xyplot()、splom()）可以画成对列表的二维散点图，3维密度图。car包里的scatterplot.matrix()函数提供更强大的二维散点图的画法。

cwhmisc包集合里的cwhplot包的pltSplomT()函数类似pair()画散点图矩阵，而且可以在对角位置画柱状图或密度估计图。

除此之外，scatterplot3d包可画3维的散点图，aplpack包里bagplot()可画二变量的boxplot，spin3R()可画可旋转的三维点图。misc3d包有可视化密度的函数。

YaleToolkit包提供许多多元数据可视化技术，agsemisc也是这样。

更特殊的多元图包括：aplpack包里的faces()可画Chernoff’s face；MASS包里的parcoord()可画平行坐标图（矩阵的每一行画一条线，横轴表示矩阵的每列）；

graphics包里的stars()可画多元数据的星状图（矩阵的每一行用一个星状图表示）。ade4包里的mstree()和vegan包里的spantree()可画最小生成树。

calibrate包支持双变量图和散点图，chplot包可画convex hull图。

geometry包提供了和qhull库的接口，由convexhulln()可给出相应点的索引。

ellipse包可画椭圆，也可以用plotcorr()可视化相关矩阵。denpro包为多元可视化提供水平集树形结构（level set trees）。graphics包里的mosaicplot()和vcd包里的mosaic()函数画马赛克图（mosaic plot）。

gclus包提供了针对聚类的散点图和平行坐标图。

rggobi包和DescribeDisplay包是GGobi的接口，DescribeDisplay的图可达到出版质量的要求；

xgobi包是XGobi和XGvis的接口，可实现动态交互的图。

最后，iplots包提供强大的动态交互图，尤其是平行坐标图和马赛克图。

seriation包提供seriation方法，能重新排列矩阵和系统树。


**数据预处理：**
AIS包提供多元数据的初步描述函数。

Hmisc包里的summarize()和summary.formula()辅助描述数据，varclus()函数可做聚类，而dataRep()和find.matches()找给定数据集的典型数据和匹配数据。

KnnFinder包里的nn()函数用kd-tree找相似变量的个数。
dprep包为分类提供数据预处理和可视化函数，如：检查变量冗余性、标准化。

base包里的dist()和cluster包里的daisy()函数提供距离计算函数；

proxy包提供更多的距离测度，包括矩阵间的距离。simba包处理已有数据和缺失数据，包括相似性矩阵和重整形。
**2） 假设检验（Hypothesis testing）：**
ICSNP包提供霍特林（Hotellings）T2检验和许多非参检验方法，包括基于marginal ranks的位置检验（location test），计算空间中值和符号，形状估计。

cramer包做两样本的非参检验，SpatialNP可做空间符号和秩检验。
**3） 多元分布（Multivariate distributions）：**
描述统计（Descriptive measures）：
stats包里的cov()和and cor()分别估计协方差和相关系数。

ICSNP包提供几种数据描述方法，如：spatial.median()估计空间中值，其它的函数估计scatter。

MASS包里的cov.rob()提供更健壮的方差/协方差矩阵估计。

covRobust包用最近邻方差估计法估计协方差。

robustbase包的covMCD()估计协方差和covOGK()做Orthogonalized Gnanadesikan-Kettenring。

rrcov包提供可扩展和稳健的估计函数covMcd()、covMest()。

corpcor包可计算大规模的协方差和偏相关矩阵。

**密度估计和模拟（Densities (estimation and simulation)）：**
MASS包的mvrnorm()产生多元正态分布的随机数。
Mvtnorm包有多元t分布和多元正态分布的概率和分位数函数，还可计算多元正态分布的密度函数。
mvtnormpcs包提供基于Dunnett的函数。
mnormt包提供元t分布和多元正态分布的密度和分布函数，并可产生随机数。
sn包提供多元偏t分布和偏正态分布的密度、分布、随机数函数。
delt包提供了许多估计多元密度的函数方法，如：CART和贪婪方法。
CRAN的Cluster任务列表（[http://cran.r-project.org/web/views/Cluster.html](http://cran.r-project.org/web/views/Cluster.html)）有更全面的信息，ks包里的rmvnorm.mixt()和dmvnorm.mixt()函数产生随机数和估计密度，
bayesm包里有多种拟合方法。很多地方都提供了模拟Wishart分布的函数，如：bayesm包里的rwishart()，MCMCpack包里的rwish()，而且MCMCpack包还有密度函数dwish()。KernSmooth 包里的bkde2D()和MASS包的kde2d()做分箱（binned）或不分箱二维核密度估计。
ks包也像ash和GenKern包样可做核平滑（kernel smoothing）。prim包用法找高维多元数据的高密度区域，feature包可计算多元数据的显著特征。
**正态检验（Assessing normality）：**
mvnormtest包提供Shapiro-Wilks检验的多元数据延伸方法，mvoutlier包检测多元离群点（outlier），ICS包可检验多元正态分布。
energy 包里的mvnorm.etest()基于E统计量做正态检验，k.sample()检验多个数据是否来自同一分布。
dprep 包里的mardia()用Mardia检验正态性。stats包里的mauchly.test()可检验Wishart分布的协方差矩阵。
**连接函数（Copulas）：**
copula包提供常规的copula函数的程序，包括：normal, t, Clayton, Frank, Gumbel。fgac包提供generalised archimedian copula，mlCopulaSelection包可做二变量的copula。
**4） 线形模型（Linear models）：**
stats包里的lm()可做多元线形模型，anova.mlm()比较多个多元线形模型，manova()做多元方差分析（MANOVA）。
sn包的msn.mle()和 and mst.mle()可拟合多元偏正态和偏t分布模型。
pls包提供偏最小二乘回归（PLSR）和主成分回归；
ppls包可做惩罚偏最小二乘回归；
dr包提供降维回归方法，如.：片逆回归法(Sliced Inverse Regression)、片平均方差估计(sliced average variance estimation)。plsgenomics包做基于偏最小二乘回归的基因组分析。relaimpo包可评估回归参数的相对重要性。
**5） 投影方法（Projection methods）：**
主成分（Principal components）：
stats包的prcomp()（基于svd()）和princomp()（基于eigen()）能计算主成分。sca包做单分量分析。
nFactors可评价碎石图(Scree plot)，paran包可评估主成分分析得到的主成分和因子分析得到的因子。
pcurve包做主曲线（Principal Curve）分析和可视化。
gmodels包提供适合大矩阵的fast.prcomp()和fast.svd()。
kernlab包里的kpca()用核方法做非线性的主成分分析。
pcaPP包用投影寻踪（projection pursuit）法计算稳健/鲁棒（robust）主成分。
amap包的acpgen()和acprob()函数分别针对广义（generalized）和稳健（robust）主成分分析。
主成分在很多方面也有相应的应用，如：涉及生态的ade4包，感官的SensoMinR包。
psy包里有用于心理学的各种程序，与主成分相关的有：sphpca()用球形直观表示相关矩阵，类似于3D的PCA；
fpca()图形展示主成分分析的结果，而且允许某些变量间有相关性；
scree.plot()图形展示相关或协方差矩阵的特征值。
PTAk包做主张量分析（Principal Tensor Analysis）。
smatr包提供关于异速生长（allometry）的函数。

**典型相关（Canonical Correlation）：**
stats包里的cancor()是做典型相关的函数。
kernlab包提供更稳健的核方法kcca()。
concor包提供了许多concordance methods。
**冗余度分析（Redundancy Analysis）：**
calibrate包里的rda()函数可做冗余度分析和典型相关。fso包提供了模糊集排序（Ordination）方法。
**独立成分（Independent Components）**：
fastICA包用fastICA算法做独立成分分析（ICA）和投影寻踪分析（Projection Pursuit），mlica包提供独立成分分析的最大似然拟合，PearsonICA包用基于互信息的打分函数分离独立信号。ICS包能执行不变坐标系（invariant coordinate system）和独立成分分析（independent components）。JADE包提供就JADE算法的接口，而且可做一些 ICA。
**普鲁克分析（Procrustes analysis）：**
vegan包里的procrustes()可做普鲁克分析，也提供排序（ordination）函数。更一般的普鲁克分析可由FactoMineR包里的GPA()实现。
**6） 主坐标/尺度方法（Principal coordinates / scaling methods）：**
stats包的cmdscale()函数执行传统的多维尺度分析（multidimensional scaling，MDS）（主坐标分析Principal Coordinates Analysis），MASS包的sammon()和isoMDS()函数分别执行Sammon和Kruskal非度量多维尺度分析。
vegan包提供非度量多维尺度分析的包装(wrappers)和后处理程序。
**7） 无监督分类（Unsupervised classification）：**
**聚类分析：**
CRAN的Cluster任务列表全面的综述了R实现的聚类方法。stats里提供等级聚类hclust()和k-均值聚类kmeans()。
cluster包里有大量的聚类和可视化技术，
clv包里则有一些聚类确认程序，
e1071包的classAgreement()可计算Rand index比较两种分类结果。
Trimmed k-means聚类分析可由trimcluster包实现，聚类融合方法（Cluster Ensembles）由clue包实现，clusterSim包能帮助选择最佳的聚类，hybridHclust包提供一些混合聚类方法。
energy包里有基于E统计量的距离测度函数edist()和等级聚类方法hclust.energy()。
LLAhclust包提供基于似然（likelihood linkage）方法的聚类，也有评定聚类结果的指标。
fpc包里有基于Mahalanobis距离的聚类。
clustvarsel包有多种基于模型的聚类。模糊聚类（fuzzy clustering）可在cluster包和hopach包里实现。
Kohonen包提供用于高维谱（spectra）或模式（pattern）的有监督和无监督的SOM算法。
clusterGeneration包帮助模拟聚类。
CRAN的Environmetrics任务列表里也有相关的聚类算法的综述。mclust包实现了基于模型的聚类，MFDA包实现了功能数据的基于模型的聚类。
**树方法：**
CRAN的MachineLearning任务列表有对树方法的细节描述。分类树也常常是重要的多元方法，rpart包正是这样的包，rpart.permutation包还可以做rpart()模型的置换（permutation）检验。
TWIX包的树可以外部剪枝。hier.part包分割多元数据集的方差。
mvpart包可做多元回归树，party包实现了递归分割（recursive partitioning），rrp包实现了随机递归分割。
caret包可做分类和回归训练，进而caretLSF包实现了并行处理。kknn包的k-近邻法可用于回归，也可用于分类。
**8） 有监督分类和判别分析（Supervised classification and discriminant analysis）：**
MASS包里的lda()和qda()分别针对线性和二次判别分析。
mda包的mda() and fda()允许混合和更灵活的判别分析，mars()做多元自适应样条回归（multivariate adaptive regression splines），bruto()做自适应样条后退拟合（adaptive spline backfitting）。
earth包里也有多元自适应样条回归的函数。
rda包可用质心收缩法（shrunken centroids regularized discriminant analysis）实现高维数据的分类。VR的class包的knn()函数执行k-最近邻算法，knncat包里有针对分类变量的k-最近邻算法。
SensoMineR包的FDA()用于因子判别分析。许多包结合了降维（dimension reduction）和分类。
klaR包可以做变量选择，可处理多重共线性，还有可视化函数。
superpc包利用主成分做有监督的分类，classPP包则可为其做投影寻踪（projection pursuit），gpls包用广义偏最小二乘做分类。hddplot包用交叉验证的线性判别分析决定最优的特征个数。
supclust包可以根据芯片数据做基因的监督聚类。ROCR提供许多评估分类执行效果的方法。
predbayescor包可做朴素贝叶斯（na&iuml;ve Bayes）分类。
关于监督分类的更多信息可以看MachineLearning任务列表。
**9） 对应分析（Correspondence analysis）：**
MASS包的corresp()和mca()可以做简单和多重对应分析。ca包提供单一、多重和联合（joint）对应分析。
ade4包的ca()和mca()分别做一般的和多重对应分析。
vegan包里也有类似的函数。cocorresp可实现两个矩阵间的co-correspondence分析。
FactoMineR 包的CA()和MCA()函数也能做类似的简单和多重对应分析，还有画图函数。
homals执行同质分析（homogeneity）。
**10） 前向查找（Forward search）：**
Rfwdmv包执行多元数据的前向查找。
**11） 缺失数据（Missing data）：**
mitools包里有缺失数据的多重估算（multiple imputation)的函数, mice包用chained equations实现了多重估算，mvnmle包可以为多元正态数据的缺失值做最大似然估计（ML Estimation），
norm包提供了适合多元正态数据的估计缺失值的期望最大化算法（EM algorithm），
cat包允许分类数据的缺失值的多重估算，mix包适用于分类和连续数据的混合数据。
pan包可为面版数据(panel data)的缺失值做多重估算。
VIM包做缺失数据的可视化和估算。
Hmisc包的aregImpute()和transcan()提供了其它的估算缺失值方法。
EMV包提供了knn方法估计缺失数据。monomvn包估计单调多元正态数据的缺失值。
**12） 隐变量方法（Latent variable approaches）：**
stats包的factanal()执行最大似然因子分析，MCMCpack包可做贝叶斯因子分析。
GPArotation包提供投影梯度（Gradient Projection）旋转因子法。
FAiR包用遗传算法作因子分析。ifa包可用于非正态的变量。
sem包拟合线形结构方程模型。
ltm包可做隐含式语义分析 (Latent semantic analysis)，eRm包则可拟合Rasch模型(Rasch models)。
FactoMineR包里有很多因子分析的方法，包括：MFA()多元因子分析，HMFA()等级多元因子分析，ADFM()定量和定性数据的多元因子分析。tsfa包执行时间序列的因子分析。
poLCA包针对多分类变量(polytomous variable)做潜类别分析（Latent Class Analysis）。
**13） 非高斯数据建模（Modelling non-Gaussian data）：**
bivpois包建模Poisson分布的二变量。
mprobit包提供了适合二元和顺序响应变量的多元概率模型。
MNP包实现了Bayesian多元概率模型。polycor包可计算多组相关（olychoric correlation）和四分相关（tetrachoric correlation）矩阵。
bayesm包里有多种模型，如：表面非相关回归(Seemingly unrelated Regression)，多元logit/probit模型, 工具变量法(Instrumental Variables)。
VGAM包里有：广义线形和可加模型（Vector Generalised Linear and Additive Models），减秩回归（Reduced Rank regression）。
**14） 矩阵处理（Matrix manipulations）：**
R作为一种基于向量和矩阵的语言，有许多处理矩阵的强有力的工具，由包Matrix和，SparseM实现。
matrixcalc包增加了矩阵微积分的功能。spam包提供了更深入的针对稀疏矩阵的方法。
**15） 其它（Miscellaneous utitlies）：**
**DEA包执行数据包络分析(data envelopment analysis,DEA)。abind包组合多维array**。

Hmisc 包的mApply()扩充了apply()的功能。
除了前面描述的功能，sn包还未偏正态和偏t分布提供边缘化（marginalisation）、仿射变换（affine transformations）等。
SharedHT2包执行芯片数据的Hotelling's T2检验。panel包里有面版数据(panel data)的建模方法。mAr包可做向量自回归模型(vector auto-regression)，
MSBVAR包里有贝叶斯向量自回归模型。
Hmisc包的rm.boot()函数bootstrap重复测量试验（Repeated Measures Models）。compositions包提供复合数据分析（compositional data analysis）。
cramer包为两样本数据做多元非参Cramer检验。
psy里有许多心理学的常用方法。
cwhmisc包集合的cwhmath包里有许多有趣的功能，如各种旋转函数。
desirability包提供了基于密度函数的多变量最优化方法。
geozoo包可以画geozoo包里定义的几何对象。


————————————————————————————————————————————————

# 二、
# [Machine Learning & Statistical Learning](https://cran.r-project.org/web/views/MachineLearning.html)


以下所有资讯都来自与[VRAN task view](https://cran.r-project.org/web/views/)的[Machine Learning & Statistical Learning](https://cran.r-project.org/web/views/MachineLearning.html)


转载于：https://www.douban.com/note/500637572/

[机器学习](http://lib.csdn.net/base/2)是计算机科学和统计学的边缘交叉领域，R关于机器学习的包主要包括以下几个方面：
1）神经网络（Neural Networks）：
nnet包执行单隐层前馈神经网络，nnet是VR包的一部分（[http://cran.rproject.org/web/packages/VR/index.html](http://cran.rproject.org/web/packages/VR/index.html)）。
2）递归拆分（Recursive Partitioning）：
递归拆分利用树形结构模型，来做回归、分类和生存分析，主要在rpart包（[http://cran.r-project.org/web/packages/rpart/index.html](http://cran.r-project.org/web/packages/rpart/index.html)）和tree包（[http://cran.r-project.org/web/packages/tree/index.html](http://cran.r-project.org/web/packages/tree/index.html)）里执行，尤其推荐rpart包。Weka里也有这样的递归拆分法，如：J4.8, C4.5, M5，包Rweka提供了R与Weka的函数的接口（[http://cran.r-project.org/web/packages/RWeka/index.html](http://cran.r-project.org/web/packages/RWeka/index.html)）。
party包提供两类递归拆分[算法](http://lib.csdn.net/base/31)，能做到无偏的变量选择和停止标准：函数ctree()用非参条件推断法检测自变量和因变量的关系；而函数mob()能用来建立参数模型（[http://cran.r-project.org/web/packages/party/index.html](http://cran.r-project.org/web/packages/party/index.html)）。另外，party包里也提供二分支树和节点分布的可视化展示。
mvpart包是rpart的改进包，处理多元因变量的问题（[http://cran.r-project.org/web/packages/mvpart/index.html](http://cran.r-project.org/web/packages/mvpart/index.html)）。rpart.permutation包用置换法（permutation）评估树的有效性（[http://cran.r-project.org/web/packages/rpart.permutation/index.html](http://cran.r-project.org/web/packages/rpart.permutation/index.html)）。knnTree包建立一个分类树，每个叶子节点是一个knn分类器（[http://cran.r-project.org/web/packages/knnTree/index.html](http://cran.r-project.org/web/packages/knnTree/index.html)）。LogicReg包做逻辑回归分析，针对大多数自变量是二元变量的情况（[http://cran.r-project.org/web/packages/LogicReg/index.html](http://cran.r-project.org/web/packages/LogicReg/index.html)）。maptree包（[http://cran.r-project.org/web/packages/maptree/index.html](http://cran.r-project.org/web/packages/maptree/index.html)）和pinktoe包（[http://cran.r-project.org/web/packages/pinktoe/index.html](http://cran.r-project.org/web/packages/pinktoe/index.html)）提供树结构的可视化函数。
3）随机森林（Random Forests）：
randomForest 包提供了用随机森林做回归和分类的函数（[http://cran.r-project.org/web/packages/randomForest/index.html](http://cran.r-project.org/web/packages/randomForest/index.html)）。ipred包用bagging的思想做回归，分类和生存分析，组合多个模型（[http://cran.r-project.org/web/packages/ipred/index.html](http://cran.r-project.org/web/packages/ipred/index.html)）。party包也提供了基于条件推断树的随机森林法（[http://cran.r-project.org/web/packages/party/index.html](http://cran.r-project.org/web/packages/party/index.html)）。varSelRF包用随机森林法做变量选择（[http://cran.r-project.org/web/packages/varSelRF/index.html](http://cran.r-project.org/web/packages/varSelRF/index.html)）。
4）Regularized and Shrinkage Methods：
lasso2包（[http://cran.r-project.org/web/packages/lasso2/index.html](http://cran.r-project.org/web/packages/lasso2/index.html)）和lars包（[http://cran.r-project.org/web/packages/lars/index.html](http://cran.r-project.org/web/packages/lars/index.html)）可以执行参数受到某些限制的回归模型。elasticnet包可计算所有的收缩参数（[http://cran.r-project.org/web/packages/elasticnet/index.html](http://cran.r-project.org/web/packages/elasticnet/index.html)）。glmpath包可以得到广义线性模型和COX模型的L1 regularization path（[http://cran.r-project.org/web/packages/glmpath/index.html](http://cran.r-project.org/web/packages/glmpath/index.html)）。penalized包执行lasso (L1) 和ridge (L2)惩罚回归模型（penalized regression models）（[http://cran.r-project.org/web/packages/penalized/index.html](http://cran.r-project.org/web/packages/penalized/index.html)）。pamr包执行缩小重心分类法(shrunken centroids classifier)（[http://cran.r-project.org/web/packages/pamr/index.html](http://cran.r-project.org/web/packages/pamr/index.html)）。earth包可做多元自适应样条回归（multivariate adaptive regression splines）（[http://cran.r-project.org/web/packages/earth/index.html](http://cran.r-project.org/web/packages/earth/index.html)）。
5）Boosting :
gbm包（[http://cran.r-project.org/web/packages/gbm/index.html](http://cran.r-project.org/web/packages/gbm/index.html)）和boost包（[http://cran.r-project.org/web/packages/boost/index.html](http://cran.r-project.org/web/packages/boost/index.html)）执行多种多样的梯度boosting算法，gbm包做基于树的梯度下降boosting，boost包包括LogitBoost和L2Boost。GAMMoost包提供基于boosting的广义相加模型(generalized additive models)的程序（[http://cran.r-project.org/web/packages/GAMMoost/index.html](http://cran.r-project.org/web/packages/GAMMoost/index.html)）。mboost包做基于模型的boosting（[http://cran.r-project.org/web/packages/mboost/index.html](http://cran.r-project.org/web/packages/mboost/index.html)）。
6）支持向量机（Support Vector Machines）：
e1071包的svm()函数提供R和LIBSVM的接口 （[http://cran.r-project.org/web/packages/e1071/index.html](http://cran.r-project.org/web/packages/e1071/index.html)）。kernlab包为基于核函数的学习方法提供了一个灵活的框架，包括SVM、RVM……([http://cran.r-project.org/web/packages/kernlab/index.html](http://cran.r-project.org/web/packages/kernlab/index.html)) 。klaR 包提供了R和SVMlight的接口（[http://cran.r-project.org/web/packages/klaR/index.html](http://cran.r-project.org/web/packages/klaR/index.html)）。
7）贝叶斯方法（Bayesian Methods）：
BayesTree包执行Bayesian Additive Regression Trees (BART)算法（[http://cran.r-project.org/web/packages/BayesTree/index.html](http://cran.r-project.org/web/packages/BayesTree/index.html)，[http://www-stat.wharton.upenn.edu/~edgeorge/Research_papers/BART%206--06.pdf](http://www-stat.wharton.upenn.edu/~edgeorge/Research_papers/BART%206--06.pdf)）。tgp包做Bayesian半参数非线性回归（Bayesian nonstationary, semiparametric nonlinear regression）（[http://cran.r-project.org/web/packages/tgp/index.html](http://cran.r-project.org/web/packages/tgp/index.html)）。
8）基于遗传算法的最优化（Optimization using Genetic Algorithms）：
gafit包（[http://cran.r-project.org/web/packages/gafit/index.html](http://cran.r-project.org/web/packages/gafit/index.html)）和rgenoud包（[http://cran.r-project.org/web/packages/rgenoud/index.html](http://cran.r-project.org/web/packages/rgenoud/index.html)）提供基于遗传算法的最优化程序。
9）关联规则（Association Rules）：
arules包提供了有效处理稀疏二元数据的[数据结构](http://lib.csdn.net/base/31)，而且提供函数执Apriori和Eclat算法挖掘频繁项集、最大频繁项集、闭频繁项集和关联规则（[http://cran.r-project.org/web/packages/arules/index.html](http://cran.r-project.org/web/packages/arules/index.html)）。
10）模型选择和确认（Model selection and validation）：
e1071包的tune()函数在指定的范围内选取合适的参数（[http://cran.r-project.org/web/packages/e1071/index.html](http://cran.r-project.org/web/packages/e1071/index.html)）。ipred包的errorest()函数用重抽样的方法（交叉验证，bootstrap）估计分类错误率（[http://cran.r-project.org/web/packages/ipred/index.html](http://cran.r-project.org/web/packages/ipred/index.html)）。svmpath包里的函数可用来选取支持向量机的cost参数C（[http://cran.r-project.org/web/packages/svmpath/index.html](http://cran.r-project.org/web/packages/svmpath/index.html)）。ROCR包提供了可视化分类器执行效果的函数，如画ROC曲线（[http://cran.r-project.org/web/packages/](http://cran.r-project.org/web/packages/ROCR/index.html)ROCR[/index.html](http://cran.r-project.org/web/packages/ROCR/index.html)）。caret包供了各种建立预测模型的函数，包括参数选择和重要性量度（[http://cran.r-project.org/web/packages/caret/index.html](http://cran.r-project.org/web/packages/caret/index.html)）。caretLSF包（[http://cran.r-project.org/web/packages/caretLSF/index.html](http://cran.r-project.org/web/packages/caretLSF/index.html)）和caretNWS（[http://cran.r-project.org/web/packages/caretNWS/index.html](http://cran.r-project.org/web/packages/caretNWS/index.html)）包提供了与caret包类似的功能。
11）统计学习基础（Elements of Statistical Learning）：
书《The Elements of Statistical Learning: Data Mining, Inference, and Prediction 》（[http://www-stat.stanford.edu/~tibs/ElemStatLearn/](http://www-stat.stanford.edu/~tibs/ElemStatLearn/)）里的数据集、函数、例子都被打包放在ElemStatLearn包里（[http://cran.r-project.org/web/packages/ElemStatLearn/index.html](http://cran.r-project.org/web/packages/ElemStatLearn/index.html)）。
12）R统计软件的Lars算法的软件包提供了Lasso算法。根据模型改进的需要，数据挖掘工作者可以借助于Lasso算法，利用AIC准则和BIC准则精炼简化统计模型的变量集合，达到降维的目的。因此，Lasso算法是可以应用到数据挖掘中的实用算法。glasso（graphical lasso）是lasso方法的一种扩展，采用加罚的极大似然方法估计变量间协方差矩阵的逆矩阵（这个逆矩阵在图模型中被称为Concentration Matrix或者Precision Matrix），加以适当整理之后，可以得到变量间的稀疏化的偏相关系数矩阵，其中的零元素表示了变量间的条件独立关系。我们可以利用其中的非零元素生成图模型。


![](https://img-blog.csdn.net/20170130161116762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



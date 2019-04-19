# XGBoost中参数调优的完整指南（含Python-3.X代码） - BlueSky - CSDN博客
2017年11月17日 14:47:52[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1070
CSDN：[http://blog.csdn.net/kicilove/article/](http://blog.csdn.net/kicilove/article/)
Github：[https://github.com/zhaohuicici?tab=repositories](https://github.com/zhaohuicici?tab=repositories)
原文链接：[https://www.analyticsvidhya.com/blog/2016/03/complete-guide-parameter-tuning-xgboost-with-codes-python/](https://www.analyticsvidhya.com/blog/2016/03/complete-guide-parameter-tuning-xgboost-with-codes-python/)
- - [引言](http://blog.csdn.net/kicilove/article/details/78413112#引言)
- [关于XGBoost你需要知道什么](http://blog.csdn.net/kicilove/article/details/78413112#关于xgboost你需要知道什么)
- [目录](http://blog.csdn.net/kicilove/article/details/78413112#目录)
- [XGBoost优势是什么](http://blog.csdn.net/kicilove/article/details/78413112#1-xgboost优势是什么)
- [了解XGBoost参数有哪些](http://blog.csdn.net/kicilove/article/details/78413112#2-了解xgboost参数有哪些)- [普遍意义的参数](http://blog.csdn.net/kicilove/article/details/78413112#1-普遍意义的参数)
- [提升器Booster参数在每一步中引导单个的加速器Booster树回归](http://blog.csdn.net/kicilove/article/details/78413112#2-提升器booster参数在每一步中引导单个的加速器booster树回归)
- [带有学习任务的参数指导优化的过程](http://blog.csdn.net/kicilove/article/details/78413112#3带有学习任务的参数指导优化的过程)
- [在实例中使用XGBoost并调参](http://blog.csdn.net/kicilove/article/details/78413112#3-在实例中使用xgboost并调参)
- [参数调优的一般方法](http://blog.csdn.net/kicilove/article/details/78413112#参数调优的一般方法)- [Step 1 Fix learning rate and number of estimators
 for tuning tree-based parameters](http://blog.csdn.net/kicilove/article/details/78413112#step-1-fix-learning-rate-and-number-of-estimators-for-tuning-tree-based-parameters)
- [Step 2 Tune max_depth and min_child_weight](http://blog.csdn.net/kicilove/article/details/78413112#step-2-tune-maxdepth-and-minchildweight)
- [Step 3 Tune gamma](http://blog.csdn.net/kicilove/article/details/78413112#step-3-tune-gamma)
- [Step 4 Tune subsample and colsample_bytree](http://blog.csdn.net/kicilove/article/details/78413112#step-4-tune-subsample-and-colsamplebytree)
- [Step 5 Tuning Regularization Parameters](http://blog.csdn.net/kicilove/article/details/78413112#step-5-tuning-regularization-parameters)
- [Step 6 Reducing Learning Rate](http://blog.csdn.net/kicilove/article/details/78413112#step-6-reducing-learning-rate)
- [End Notes](http://blog.csdn.net/kicilove/article/details/78413112#end-notes)
## 引言
如果其他的预测模型无法满足你的预测需求时，请使用XGboost。 XGBoost算法已经成为许多数据科学家的终极武器。 这是一个足够强大的可以应对各种不寻常数据的高大上的算法。
使用XGBoost构建模型很容易。 但是，使用XGBoost改进模型很困难（至少于我来说还是很挣扎的）。 这个算法使用了很多个参数。 要想对模型进行改进，就必须对参数做出调整。 很多实际的问题，比如说你应该调整哪一个参数集合？要得到最优的输出结果参数的理想值是多少?，这些实际问题都是很难得到答案的。
本文适合与刚接触XGBoost的新人。 在本文中，我们将学习参数调优的技术以及有关XGBoost的一些有用信息。 此外，我们将使用Python中的数据集练习该算法。
![这里写图片描述](https://img-blog.csdn.net/20171101152153626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 关于XGBoost你需要知道什么
* XGBoost（eXtreme Gradient Boosting）*是梯度增强算法（GBM）的高级实现。 由于我在之前的文章[Complete
 Guide to Parameter Tuning in Gradient Boosting (GBM) in Python](https://www.analyticsvidhya.com/blog/2016/02/complete-guide-parameter-tuning-gradient-boosting-gbm-python/)中详细介绍了Gradient Boosting Machine，所以强烈建议您先阅读本文。 这将有助于您加强对GBM的一般升级和参数调优的理解。
特别感谢：就个人而言，我要感谢[Sudalai Rajkumar](https://www.linkedin.com/in/sudalairajkumar)先生（又名SRK）提供的永久支持，目前是[AV
 Rank 2](http://datahack.analyticsvidhya.com/user/profile/SRK)。如果没有他的帮助，这篇文章是不可能的诞生的啦。 他正帮助着成千上万的数据科学家。 非常感谢SRK！
## 目录
- XGBoost优势是什么
- 了解XGBoost参数有哪些
- 在实例中使用XGBoost并调参
## 1. XGBoost优势是什么
我一直钦佩在一个预测型的模型中这种算法注入了提升（boosting）的能力。 当我探索它的表现性能以及高准确性背后的科学道路时，发现了它身上有诸多的优点：
- 正则化： 
- 标准的GBM实现过程中没有像XGBoost一样拥有[正规化](https://www.analyticsvidhya.com/blog/2015/02/avoid-over-fitting-regularization/)，因此XGBoost也有助于减少过度拟合。
- 事实上，XGBoost也被称为“正规化提升”技术。
- 并行处理： 
- XGBoost实现了并行处理，与GBM相比，速度惊人的快。
- 我们知道[提升（boosting）](https://www.analyticsvidhya.com/blog/2015/11/quick-introduction-boosting-algorithms-machine-learning/)是一个顺序也就是串行的过程，XGBoost是怎样并行化的呢？ 注意xgboost的并行不是tree粒度的并行，xgboost也是一次迭代完才能进行下一次迭代的（第t次迭代的代价函数里包含了前面t-1次迭代的预测值）。xgboost的并行是在特征粒度上的。我们知道，决策树的学习最耗时的一个步骤就是对特征的值进行排序（因为要确定最佳分割点），xgboost在训练之前，预先对数据进行了排序，然后保存为block结构，后面的迭代中重复地使用这个结构，大大减小计算量。这个block结构也使得并行成为了可能，在进行节点的分裂时，需要计算每个特征的增益，最终选增益最大的那个特征去做分裂，那么各个特征的增益计算就可以开多线程进行。 
可并行的近似直方图算法。树节点在进行分裂时，我们需要计算每个特征的每个分割点对应的增益，即用贪心法枚举所有可能的分割点。当数据无法一次载入内存或者在分布式情况下，贪心算法效率就会变得很低，所以xgboost还提出了一种可并行的近似直方图算法，用于高效地生成候选的分割点。 我希望你得到我来的地方。查看这个[链接](http://zhanpengfang.github.io/418home.html)，进一步探索。
- XGBoost还支持在Hadoop上实现。
- 高度的灵活性 
- XGBoost允许用户自定义优化目标以及评估的准则。
- 这就为模型的广泛性应用提供了无限的可能，有没有感受到‘为所欲为’。
- 缺失值处理 
- XGBoost具有内置的规则来处理缺失的值，它可以自动学习出它的分裂方向。
- 剪枝 
- GBM在遇到损失为负的分裂节点时会停止这个节点的继续分裂， 因此，它更像是一种贪心的算法。
- 另一方面，当我们指定树的最大深度（max_depth）时，XGBoost就会以这个指定的最大树深为上限去剪枝。具体来说就是，XGBoost先从上到下建立出所有可能的子树，然后根据我们设定的max_depth，从下到上反向进行剪枝，并且也会移除负增益的分割树。
- 当我们遇到一个比如说为-2的负损失分裂点然后它后面又跟着一个为+10的正损失的分裂时， GBM将在遇到-2时停止，而XGBoost将会继续深入分割，并且会看到一个组合出来为+8的分裂效果。
- 内置交叉验证 
- XGBoost允许用户在每一轮boosting过程中的每次迭代中使用交叉验证，因此很容易获得最优boosting迭代次数；
- GBM必须使用网格搜索，因此只能测试有限个值。
- 在之前存在的模型上继续训练 
- 用户可以从上次运行的最后一次迭代开始训练XGBoost模型。 这在特定的应用中优势还是比较明显的。
- GBM在sklearn的实现中也有这个特点。
我希望现在你明白了XGBoost的绝对优势。 请注意，这些只是XGBoost的部分要点。 还待你发现更多？
您可以参考以下网页以获得更深入的了解：
- [XGBoost Guide – Introduction to Boosted Trees](http://xgboost.readthedocs.io/en/latest/model.html)
- [Words from the Author of XGBoost [Video]](https://www.youtube.com/watch?v=X47SGnTMZIU)
## 2. 了解XGBoost参数有哪些
XGBoost的作者把参数整体分为3类：
- 普遍意义的参数：指导整个函数的
- 提升器（Booster）参数：在每一步中引导单个的加速器（Booster）（树/回归）
- 带有学习任务的参数：指导优化的过程
强烈建议阅读这篇与GBM对比的[文章](https://www.analyticsvidhya.com/blog/2016/02/complete-guide-parameter-tuning-gradient-boosting-gbm-python/)。
下面介绍这些参数
### 1. 普遍意义的参数：
下面这些参数指导XGBoost的整体过程。
- 
- 在每次迭代中选择模型的类型，有2个选项： 
- gbtree: 基于树的模型
- gblinear: 基于回归的模型
- 
- 激活Silent mode就设置为1，即正在运行的消息不会被打印。
- 默认为0，好处就是帮助我们理解模型运行状况。
- 
- 这个参数用于并行处理，系统中的核的数量
- 如果想运行所有的核，就不用再输入nthread的值，因为默认情况就是使用所有核。
还有另外两个参数是由XGBoost自动设置的，下面继续探索Booster参数。
### 2. 提升器（Booster）参数：在每一步中引导单个的加速器（Booster）（树/回归）
虽然有两种类型的Booster，这里只讨论树式Booster。
- 
- 与GBM中的eta类似。
- 在每一步中收缩权重使得模型更加稳健。
- 通常设置值为：
- 
- 孩子节点中最小的样本权重和。如果一个叶子节点的样本权重和小于min_child_weight则拆分过程结束。在现行回归模型中，这个参数是指建立每个模型所需要的最小样本数。该成熟越大算法越conservative
- 这与GBM中的min_child_leaf类似，但不完全相同，XGBoost指 min “sum of weights” of observations 而 GBM 为 min “number of observations”。
- 可用于控制过拟合。太高的值可能导致欠拟合，应使用CV进行调参。
- 
- 与GBM一样，定义了一棵树的最大深度。
- 用于控制过拟合，因为较高的深度会使模型对一些样本学习到特定关系，而这种关系又不是泛化的。
- 适合用CV进行调整值的大小。
- 通常设置值为：
- 
- 树中节点或树叶的最大数量。
- 有时可以代替max_depth。 如：二叉树，深度“n”将产生最大2 ^ n个叶。
- 如果这样，GBM可以忽略max_depth。
- 
- 只有当损失函数以正值减少时，节点才会分割。 Gamma指定了进行分割时所需的最小损失的减少量。
- 使算法比较保守。 Gamma值可以根据损失函数调整大小。
- 
- 如果设置为0，表示没有约束。
 可以取正值。
- 这个参数不是必须要设定的。在逻辑回归中，当类别比例非常不平衡时，这个参数很有用。
- 
- 与GBM取子样本一样，都是对总体进行随机采样出子样本占总体的比例。
- 较低的值使算法比较保守，可以防止过度拟合，但太小的值可能会导致欠拟合。
- 通常设置值为：
- 
- 类似于GBM中的max_features。 表示随机抽取的列数占总列数的比例。
- 通常设置值为：
- 
- 表示每个层中用于拆分时的列数占比（相当于选出的列数的再比例）。
- 这个参数不常用，因为subsample和 colsample_bytree可以替代这个参数的作用。
- 
- 对权重正则化（Ridge回归也是）
- 这用于XGBoost的正则化部分。 虽然许多数据科学家一般不用它，但是减少过拟合的时候还是要用一下的。
- 
- 对权重正则化（类似于Lasso回归的）
- 维度较高时使用，可以运行得更快
- 
- 数值大于0，在样本的类非常不均衡时使用有助于快速收敛。
### 3.带有学习任务的参数：指导优化的过程
这些参数用于定义优化的目标，在一步计算的度量。
- 
- 这个参数定义了要最小化的损失函数。 有如下选择： 
- ：用于二分类的逻辑回归，返回值为概率，非类别。
- ：使用softmax目标的多类分类返回预测类（不是概率）。 
- 还需设置一个num_class（number of classes）参数来定义类的数量。
- ：
 与softmax相同，但返回的是每个样本属于每个类的预测概率而不是类别。
- 
- 默认值为rmse用于回归，错误率用于分类。
- 可选值有： 
-  –
 root mean square error
-  –
 mean absolute error
- –
 negative log-likelihood
-  –
 Binary classification error rate (0.5 threshold)
-  –
 Multiclass classification error rate
- –
 Multiclass logloss
- :
 Area under the curve
- 
- 种子随机数。
- 使采样的结果与之前相同以及参数调整。
如果使用Scikit-Learn，这些参数名称可能不太熟悉。 可喜的是，python中的xgboost模块有一个名为XGBClassifier的sklearn封装。 有些参数名称变化如下：
- eta –> learning_rate
- lambda –> reg_lambda
- alpha –> reg_alpha
好像我们已经定义了在XGBoost与GBM中相似的所有参数，除了GBM中的参数。
 其实它出现在XGBClassifier的参数中，在标准xgboost中拟合函数的时候，以”的形式传递。
建议您通过xgboost指南更好地了解参数和代码：
- [XGBoost Parameters (official guide)](http://xgboost.readthedocs.io/en/latest/parameter.html#general-parameters)
- [XGBoost Demo Codes (xgboost GitHub repository)](https://github.com/dmlc/xgboost/tree/master/demo/guide-python)
- [Python API Reference (official guide)](http://xgboost.readthedocs.io/en/latest/python/python_api.html)
## 3. 在实例中使用XGBoost并调参
我们将从Data Hackathon 3.x AV hackathon中获取数据集，与[GBM文章](https://www.analyticsvidhya.com/blog/2016/02/complete-guide-parameter-tuning-gradient-boosting-gbm-python/)中的数据集相同。 细节可以在[此页面](https://datahack.analyticsvidhya.com/contest/data-hackathon-3x/)找到。
 您可以从[这里](https://www.analyticsvidhya.com/blog/2016/03/complete-guide-parameter-tuning-xgboost-with-codes-python/)下载数据集。 执行步骤如下：
- 城市变量类别太多，舍弃
- DOB转换为Age | DOB
- 创建 EMI_Loan_Submitted_Missing，如果EMI_Loan_Submitted是missing则设置为1，否则为0 |原始变量EMI_Loan_Submitted舍弃
- 雇主名称因为类别太多而被删除
- Existing_EMI缺失的用0补（中位数），因为只有111个值是缺失状态
- 如果Interest_Rate缺失，则为Interest_Rate_Missing创建1，否则为0 |原始变量Interest_Rate舍弃
- Lead_Creation_Date舍弃，直觉上认为对结果没影响
- Loan_Amount_Applied，Loan_Tenure_Applied以中位数填充
- 创建Loan_Amount_Submitted_Missing，如果Loan_Amount_Submitted值缺失则取1，否则取0，原始变量Loan_Amount_Submitted舍弃
- 创建Loan_Tenure_Submitted_Missing，如果Loan_Tenure_Submitted值缺失，则取1，否则为 0 |原始变量Loan_Tenure_Submitted舍弃
- LoggedIn，Salary_Account删除
- 创建Processing_Fee_Missing，如果Processing_Fee值缺失，则为1，否则为0 |原始变量Processing_Fee删除
- Source - 第2名保持原样，所有其他组合成不同的类别
- 进行数值和单热编码（One-Hot-Coding）
开始，导入所需的库并加载数据：
```python
#Import libraries:
import pandas as pd
import numpy as np
import xgboost as xgb
from xgboost.sklearn import XGBClassifier
from sklearn import cross_validation, metrics   #Additional scklearn functions
from sklearn.grid_search import GridSearchCV   #Perforing grid search
import matplotlib.pylab as plt
%matplotlib inline
from matplotlib.pylab import rcParams
rcParams['figure.figsize'] = 12, 4
train = pd.read_csv('train_modified.csv')
target = 'Disbursed'
IDcol = 'ID'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
请注意，我已经导入了2种形式的XGBoost：
-  -
 直接使用xgboost库。 我将使用该库中的特定函数“cv”
-  -
 这是XGBoost的sklearn封装。 可以在并行过程中使用与GBM相同sklearn’s Grid Search。
先定义一个函数，帮助我们创建XGBoost模型并执行交叉验证。这个也可以用在你自己的模型中。
```python
def modelfit(alg, dtrain, predictors,useTrainCV=True, cv_folds=5, early_stopping_rounds=50):
    if useTrainCV:
        xgb_param = alg.get_xgb_params()
        xgtrain = xgb.DMatrix(dtrain[predictors].values, label=dtrain[target].values)
        cvresult = xgb.cv(xgb_param, xgtrain, num_boost_round=alg.get_params()['n_estimators'], nfold=cv_folds,
            metrics='auc', early_stopping_rounds=early_stopping_rounds, show_progress=False)
        alg.set_params(n_estimators=cvresult.shape[0])
    #Fit the algorithm on the data
    alg.fit(dtrain[predictors], dtrain['Disbursed'],eval_metric='auc')
    #Predict training set:
    dtrain_predictions = alg.predict(dtrain[predictors])
    dtrain_predprob = alg.predict_proba(dtrain[predictors])[:,1]
    #Print model report:
    print ("\nModel Report")
    print ("Accuracy : %.4g" %) metrics.accuracy_score(dtrain['Disbursed'].values, dtrain_predictions)
    print ("AUC Score (Train): %f" % metrics.roc_auc_score(dtrain['Disbursed'], dtrain_predprob))
    feat_imp = pd.Series(alg.booster().get_fscore()).sort_values(ascending=False)
    feat_imp.plot(kind='bar', title='Feature Importances')
    plt.ylabel('Feature Importance Score')
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
这个代码与用于GBM的代码略有不同。 本文的重点是涵盖概念而不是编码。 注意，xgboost的sklearn封装没有“feature_importances”指标，它使用get_fscore（）函数做相同的事情。
## 参数调优的一般方法
我们将使用类似于GBM的方法。 几个步骤如下：
- 选择相对较高的学习率（high learning rate）。 一般选用0.1，有时0.05到0.3之间也行，看具体问题。 XGBoost有个非常有用的“cv”函数，可以用来确定该学习率下的最优树的颗数（optimum number of trees for this learning rate. ）， 它在每次 boosting迭代时执行交叉验证，返回所需的最优树的颗数。
- 调整树的特定参数（Tune tree-specific parameters）（max_depth，min_child_weight，gamma，subsample，colsample_bytree），以确定学习速率和树的颗数。 请注意，我们可以选择不同的参数来定义一个树，我将在这里介绍一个例子。
- 调整xgboost的正则化参数（regularization parameters ）（lambda，alpha），这有助于降低模型复杂性并提高性能。
- 降低学习率（Lower the learning rate）并决定最佳参数。
让我们来看一个更详细的一步一步的实现方法。
### Step 1: Fix learning rate and number of estimators for tuning tree-based parameters
为了确定boosting参数，我们需要设置其他参数的初始值。 让我们取下列值：
- ：这应该在3-10之间。
 我已经开始使用5，但是您也可以选择不同的数字。 4-6可以。
- ：选择较小的值，因为它会使类非常不平衡，也可能使叶节点组员较少。
- ：也可以选择像0.1-0.2这样较小的值来启动，这个后面会一直调整变动。
- ：这是一个常用的使用起始值。
 常取值介于0.5-0.9之间。
- ：因为类非常不均衡。
请注意，以上只是设定初始值，后面会进行参数调优。 这里默认学习率为0.1，使用xgboost的cv函数检查最优树的颗数，上面定义的函数可以实现这个功能。
```
#Choose all predictors except target & IDcols
predictors = [x for x in train.columns if x not in [target, IDcol]]
xgb1 = XGBClassifier(
 learning_rate =0.1,
 n_estimators=1000,
 max_depth=5,
 min_child_weight=1,
 gamma=0,
 subsample=0.8,
 colsample_bytree=0.8,
 objective= 'binary:logistic',
 nthread=4,
 scale_pos_weight=1,
 seed=27)
modelfit(xgb1, train, predictors)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
![这里写图片描述](https://img-blog.csdn.net/20171101215839036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
正如你所看到的，在学习率为0.1的情况下，得到140为最优的估计量个数。 请注意，根据你运行系统的情况，此值可能太高。 在这种情况下，您可以增加学习率并重新运行命令以获得减少的估计量数。
### Step 2: Tune max_depth and min_child_weight
首先调整对模型输出结果影响最大的参数。 首先，我们先设置较宽的范围，然后再用较小范围执行另一次迭代。
重要提示：我将在本节中进行运行压力较大的网格搜索，根据您的系统，可能需要15-30分钟甚至更多时间才能运行。 您可以根据系统来更改您要测试的值。
```
param_test1 = {
 'max_depth':range(3,10,2),
 'min_child_weight':range(1,6,2)
}
gsearch1 = GridSearchCV(estimator = XGBClassifier( learning_rate =0.1, n_estimators=140, max_depth=5,
 min_child_weight=1, gamma=0, subsample=0.8, colsample_bytree=0.8,
 objective= 'binary:logistic', nthread=4, scale_pos_weight=1, seed=27), 
 param_grid = param_test1, scoring='roc_auc',n_jobs=4,iid=False, cv=5)
gsearch1.fit(train[predictors],train[target])
gsearch1.grid_scores_, gsearch1.best_params_, gsearch1.best_score_
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
![这里写图片描述](https://img-blog.csdn.net/20171101220947336?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里，我们已经运行12个组合。 max_depth的理想值为5，min_child_weight的理想值为5。 让我们深入一步，寻找最佳值。 我们将搜索间隔为1上下的最佳值，因为之前间隔都是2。
```
param_test2 = {
 'max_depth':[4,5,6],
 'min_child_weight':[4,5,6]
}
gsearch2 = GridSearchCV(estimator = XGBClassifier( learning_rate=0.1, n_estimators=140, max_depth=5,
 min_child_weight=2, gamma=0, subsample=0.8, colsample_bytree=0.8,
 objective= 'binary:logistic', nthread=4, scale_pos_weight=1,seed=27), 
 param_grid = param_test2, scoring='roc_auc',n_jobs=4,iid=False, cv=5)
gsearch2.fit(train[predictors],train[target])
gsearch2.grid_scores_, gsearch2.best_params_, gsearch2.best_score_
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
![这里写图片描述](https://img-blog.csdn.net/20171101221239221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这一次，我们得到最佳值为4为max_depth和为6的min_child_weight。 此外，我们可以看到CV得分增加较轻。 请注意，随着模型性能的提高，要想在哪怕是边缘性能的提升都是指数级的困难。 你会注意到，这里我们得到6作为min_child_weight的最优值，但是我们还没有尝试超过6的值。我们可以这样做：
```
param_test2b = {
 'min_child_weight':[6,8,10,12]
}
gsearch2b = GridSearchCV(estimator = XGBClassifier( learning_rate=0.1, n_estimators=140, max_depth=4,
 min_child_weight=2, gamma=0, subsample=0.8, colsample_bytree=0.8,
 objective= 'binary:logistic', nthread=4, scale_pos_weight=1,seed=27), 
 param_grid = param_test2b, scoring='roc_auc',n_jobs=4,iid=False, cv=5)
gsearch2b.fit(train[predictors],train[target])
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
```
modelfit(gsearch3.best_estimator_, train, predictors)
gsearch2b.grid_scores_, gsearch2b.best_params_, gsearch2b.best_score_
```
- 1
- 2
![这里写图片描述](https://img-blog.csdn.net/20171101221659295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们看到6还是最佳值。
### Step 3: Tune gamma
现在可以使用上面已调整的参数来调整gamma值。 Gamma这里尝试5个值。 也可以使用更精确的数值。
```
param_test3 = {
 'gamma':[i/10.0 for i in range(0,5)]
}
gsearch3 = GridSearchCV(estimator = XGBClassifier( learning_rate =0.1, n_estimators=140, max_depth=4,
 min_child_weight=6, gamma=0, subsample=0.8, colsample_bytree=0.8,
 objective= 'binary:logistic', nthread=4, scale_pos_weight=1,seed=27), 
 param_grid = param_test3, scoring='roc_auc',n_jobs=4,iid=False, cv=5)
gsearch3.fit(train[predictors],train[target])
gsearch3.grid_scores_, gsearch3.best_params_, gsearch3.best_score_
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
![这里写图片描述](https://img-blog.csdn.net/20171101221944118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这表明我们的原始伽玛值，即0是最佳值。 在继续之前，一个好想法是重新校准更新参数的 boosting rounds 。
```
xgb2 = XGBClassifier(
 learning_rate =0.1,
 n_estimators=1000,
 max_depth=4,
 min_child_weight=6,
 gamma=0,
 subsample=0.8,
 colsample_bytree=0.8,
 objective= 'binary:logistic',
 nthread=4,
 scale_pos_weight=1,
 seed=27)
modelfit(xgb2, train, predictors)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
![这里写图片描述](https://img-blog.csdn.net/20171101222142522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在这里，我们可以看到得分有所改善。 所以最终的参数是：
- max_depth: 4
- min_child_weight: 6
- gamma: 0
### Step 4: Tune subsample and colsample_bytree
下一步将尝试不同的subsample和colsample_bytree值。 让我们分两个阶段完成，取0.6,0.7,0.8,0.9为他们的初始值。
```
param_test4 = {
 'subsample':[i/10.0 for i in range(6,10)],
 'colsample_bytree':[i/10.0 for i in range(6,10)]
}
gsearch4 = GridSearchCV(estimator = XGBClassifier( learning_rate =0.1, n_estimators=177, max_depth=4,
 min_child_weight=6, gamma=0, subsample=0.8, colsample_bytree=0.8,
 objective= 'binary:logistic', nthread=4, scale_pos_weight=1,seed=27), 
 param_grid = param_test4, scoring='roc_auc',n_jobs=4,iid=False, cv=5)
gsearch4.fit(train[predictors],train[target])
gsearch4.grid_scores_, gsearch4.best_params_, gsearch4.best_score_
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
![这里写图片描述](https://img-blog.csdn.net/20171101222412407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里，我们发现0.8为subsample和colsample_bytree的最优值。 现在我们应该尝试在0.05间隔周围的值。
```
param_test5 = {
 'subsample':[i/100.0 for i in range(75,90,5)],
 'colsample_bytree':[i/100.0 for i in range(75,90,5)]
}
gsearch5 = GridSearchCV(estimator = XGBClassifier( learning_rate =0.1, n_estimators=177, max_depth=4,
 min_child_weight=6, gamma=0, subsample=0.8, colsample_bytree=0.8,
 objective= 'binary:logistic', nthread=4, scale_pos_weight=1,seed=27), 
 param_grid = param_test5, scoring='roc_auc',n_jobs=4,iid=False, cv=5)
gsearch5.fit(train[predictors],train[target])
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
![这里写图片描述](https://img-blog.csdn.net/20171101222518568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这一次得到的最优值和上面相同，因此最优值为：
- subsample: 0.8
- colsample_bytree: 0.8
### Step 5: Tuning Regularization Parameters
下一步是应用正则化来减少过度拟合。 尽管许多人不太使用这些参数，因为gamma提供了一种控制复杂性的实质方法。 但我们应该尝试使用它。 我会在这里调整“reg_alpha”值，并留给你尝试不同的’reg_lambda’值。
```
param_test6 = {
 'reg_alpha':[1e-5, 1e-2, 0.1, 1, 100]
}
gsearch6 = GridSearchCV(estimator = XGBClassifier( learning_rate =0.1, n_estimators=177, max_depth=4,
 min_child_weight=6, gamma=0.1, subsample=0.8, colsample_bytree=0.8,
 objective= 'binary:logistic', nthread=4, scale_pos_weight=1,seed=27), 
 param_grid = param_test6, scoring='roc_auc',n_jobs=4,iid=False, cv=5)
gsearch6.fit(train[predictors],train[target])
gsearch6.grid_scores_, gsearch6.best_params_, gsearch6.best_score_
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
![这里写图片描述](https://img-blog.csdn.net/20171101222758453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们可以看到CV得分小于之前的情况。 但是尝试的值非常广，我们应该尝试更接近于最优的值（0.01），看看是否得到更好的结果。
```
param_test7 = {
 'reg_alpha':[0, 0.001, 0.005, 0.01, 0.05]
}
gsearch7 = GridSearchCV(estimator = XGBClassifier( learning_rate =0.1, n_estimators=177, max_depth=4,
 min_child_weight=6, gamma=0.1, subsample=0.8, colsample_bytree=0.8,
 objective= 'binary:logistic', nthread=4, scale_pos_weight=1,seed=27), 
 param_grid = param_test7, scoring='roc_auc',n_jobs=4,iid=False, cv=5)
gsearch7.fit(train[predictors],train[target])
gsearch7.grid_scores_, gsearch7.best_params_, gsearch7.best_score_
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
![这里写图片描述](https://img-blog.csdn.net/20171101222952415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到我们得到一个更好的CV。 现在我们可以在模型中应用这种正则化，看看影响：
```
xgb3 = XGBClassifier(
 learning_rate =0.1,
 n_estimators=1000,
 max_depth=4,
 min_child_weight=6,
 gamma=0,
 subsample=0.8,
 colsample_bytree=0.8,
 reg_alpha=0.005,
 objective= 'binary:logistic',
 nthread=4,
 scale_pos_weight=1,
 seed=27)
modelfit(xgb3, train, predictors)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
![这里写图片描述](https://img-blog.csdn.net/20171101223229076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再次，我们可以看到得分略有改善。
### Step 6: Reducing Learning Rate
最后，我们应该降低学习率，增加更多的树。 让我们用XGBoost的cv函数再次做这个工作。
```
xgb4 = XGBClassifier(
 learning_rate =0.01,
 n_estimators=5000,
 max_depth=4,
 min_child_weight=6,
 gamma=0,
 subsample=0.8,
 colsample_bytree=0.8,
 reg_alpha=0.005,
 objective= 'binary:logistic',
 nthread=4,
 scale_pos_weight=1,
 seed=27)
modelfit(xgb4, train, predictors)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
![这里写图片描述](https://img-blog.csdn.net/20171101223404626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
现在我们可以看到性能的显着提高，参数调整后的效果更加明显。
最后，我想分享两个关键的想法：
- 通过使用参数调整或稍微更好的模型，很难在结果上获得巨大的飞跃。 GBM的最高分数为0.8487，而XGBoost则为0.8494。 改进是有的但是也并没有达到惊艳。
- 通过特征工程，创建模型集合，比如stacking等其他方法可以获得显着的飞跃。
您还可以从我的[GitHub帐户](https://github.com/aarshayj/Analytics_Vidhya/tree/master/Articles/Parameter_Tuning_XGBoost_with_Example)下载所有这些代码的iPython notebook。 对于R中的实现代码，可以参考[这篇文章](https://www.analyticsvidhya.com/blog/2016/01/xgboost-algorithm-easy-steps/)。
## End Notes
本文实现了端到端的基于XGBoost模型的参数调优。 我们首先讨论为什么XGBoost比GBM具有更好的性能，然后详细讨论了所涉及的各种参数。 我们还定义了一个通用函数，可以重新用于其他模型。
最后，我们讨论了解决XGBoost问题的一般方法，并通过该方法制定了AV Data Hackathon 3.x问题。
我希望你发现这很有用并更有信心应用XGBoost来解决数据科学问题。

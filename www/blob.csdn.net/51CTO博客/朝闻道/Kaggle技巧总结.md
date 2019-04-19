# Kaggle技巧总结-朝闻道-51CTO博客
Kaggle技巧总结
Introduction
首先简单介绍一些关于 Kaggle 比赛的知识：
不同比赛有不同的任务，分类、回归、推荐、排序等。比赛开始后训练集和测试集就会开放下载。
比赛通常持续 2 ~ 3 个月，每个队伍每天可以提交的次数有限，通常为 5 次。
比赛结束前一周是一个 Deadline，在这之后不能再组队，也不能再新加入比赛。所以想要参加比赛请务必在这一 Deadline 之前有过至少一次有效的提交。
一般情况下在提交后会立刻得到得分的反馈。不同比赛会采取不同的评分基准，可以在分数栏最上方看到使用的评分方法。
反馈的分数是基于测试集的一部分计算的，剩下的另一部分会被用于计算最终的结果。所以最后排名会变动。
LB 指的就是在 Leaderboard 得到的分数，由上，有 Public LB 和 Private LB 之分。
自己做的 Cross Validation 得到的分数一般称为 CV 或是 Local CV。一般来说 CV 的结果比 LB 要可靠。
Data preprocessing
数据预处理，就是将数据处理下，为模型输入做准备，其中包括：
处理missing value：主要有以下几种方法
1.如果missing value占总体的比例非常小，那么直接填入平均值或者众数
2.如果missing value所占比例不算小也不算大，那么可以考虑它跟其他特征的关系，如果关系明显，那么直接根据其他特征填入；也可以建立简单的模型，比如线性回归，随机森林等。
3.如果missing value所占比例大，那么直接将miss value当做一种特殊的情况，另取一个值填入
处理Outlier：
1.通过画图，找出异常值
2.通过聚类等找出异常值
处理categorical feature：一般就是通过dummy variable的方式解决，也叫one hot encode，可以通过pandas.get_dummies()或者 sklearn中preprocessing.OneHotEncoder(),
另外在处理categorical feature有两点值得注意：
1.如果特征中包含大量需要做dummy variable处理的，那么很可能导致得到一个稀疏的dataframe，这时候最好用下PCA做降维处理。
2.如果某个特征有好几万个取值，那么用dummy variable就并不现实了，这时候可以用Count-Based Learning.
近期在kaggle成功的案例中发现，对于类别特征，在模型中加入tf-idf(term frequency–inverse document frequency)总是有效果的。
还有个方法叫“Leave-one-out” encoding，也可以处理类别特征种类过多的问题，实测效果不错。
有些 Float 变量可能是从未知的 Int 变量转换得到的，这个过程中发生精度损失会在数据中产生不必要的 Noise，即两个数值原本是相同的却在小数点后某一位开始有不同。这对 Model 可能会产生很负面的影响，需要设法去除或者减弱 Noise。
下面给两个例子:
Outlier
Dummy Variables
对于 Categorical Variable，常用的做法就是 One-hot encoding。即对这一变量创建一组新的伪变量，对应其所有可能的取值。这些变量中只有这条数据对应的取值为 1，其他都为 0。
如下，将原本有 7 种可能取值的 Weekdays 变量转换成 7 个 Dummy Variables。
要注意，当变量可能取值的范围很大（比如一共有成百上千类）时，这种简单的方法就不太适用了。
Feature Engineering
有人总结 Kaggle 比赛是 “Feature 为主，调参和 Ensemble 为辅”，说的很有道理。Feature Engineering 能做到什么程度，取决于对数据领域的了解程度。比如在数据包含大量文本的比赛中，常用的 NLP 特征就是必须的。怎么构造有用的 Feature，是一个不断学习和提高的过程。
一般来说，当一个变量从直觉上来说对所要完成的目标有帮助，就可以将其作为 Feature。至于它是否有效，最简单的方式就是通过图表来直观感受。比如：
在对一些基础的特征进行生成之后，测试的思路我后来发现并不是很好，因为是通过新增加一个或几个feature，如果cv分数上去了，就增加这个feature，如果cv分数没有上去，就舍弃这个feature，也就是相当于贪心验证。这样做的弊处在于，如果之前被舍弃的feature和之后被舍弃的feature联合在一起才会有正面影响，就相当于你错过了两个比较好的feature。因此特征的选择和联合显得非常关键。
总结一下比赛中有哪些“好”的feature，为以后的比赛提供灵感和经验。
数值型feature的简单加减乘除
这个乍一看仿佛没有道理可言，但是事实上却能挖掘出几个feature之间的内在联系，比如在比赛Two Sigma Connect: Rental Listing Inquiries中提供了bathrooms和bedrooms的数量，以及价格price，合租用户可能会更关心每个卧室的价格，即bathrooms / price，也会关心是不是每个房间都会有一个卫生间bathrooms / price ，这些数值型feature之间通过算数的手段建立了联系，从而挖掘出了feature内部的一些价值，分数也就相应地上去了。
高势集类别（High Categorical）进行经验贝叶斯转换成数值feature
什么是High Categorical的特征呢？一个简单的例子就是邮编，有100个城市就会有好几百个邮编，有些房子坐落在同一个邮编下面。很显然随着邮编的数量增多，如果用简单的one-hot编码显然效果不太好，因此有人就用一些统计学思想（经验贝叶斯）将这些类别数据进行一个map，得到的结果是数值数据。在这场比赛中有人分享了一篇paper里面就提到了具体的算法。详细就不仔细讲了，用了这个encoding之后，的确效果提升了很多。那么这场比赛中哪些数据可以进行这样的encoding呢，只要满足下面几点：1. 会重复，2. 根据相同的值分组会分出超过一定数量（比如100）的组。
时间特征
针对于时间数据来讲，提取年、月、日、星期等可能还是不够的，有另外一些points可以去思考，用户的兴趣跟发布时间的久远是否有关系？可以构造如下的feature来进行测试：
```
data["latest"] = (data["created"]- data["created"].min())
data["passed"] = (data["created"].max()- data["created"])
```
另外针对于时间特征还可以用可视化的方式来与其他特征建立联系，比如我们观察其他字段与时间变化到底有怎样的联系
地理位置特征
想到地理位置，就会想到聚类，一个简单的方式将每个位置数据划分到同一块区域中去；除了聚类以外，算出几个中心点坐标，计算曼哈顿距离或者欧式距离可能都会有神奇的效果。
文本特征
文本类数据主要包括提取关键词、情感分析、word embedding聚类之类等方法，也可以采用深度网络方法进行特征提取，分析文本特征与目标字段之间的关系。
图片特征
图片类数据特征还是需要通过CNN方式进行提取。
稀疏特征集
其实就相当于一系列标签，不同标签的个数也是挺多的，本次比赛我只是简单地采用了counterEncoding的方式进行one-hot编码。值得一提的是，有些标签是可以合并的，比如cat allowed 和 dog allowed可以合并成为 pet allowed，我在这场比赛中手工地合并了一些feature数据，最终结果略微有所提升。
特征重要程度（feature importance）
在树结构的分类器比如randomforest、xgboost中最后能够对每个特征在分类上面的重要程度进行一个评估。这时候如果已经选定了一些feature进行训练了之后，查看feature importance的反馈是非常重要的，比如本场比赛制胜的关键是运用manager_id这个feature，而它的feature importance反馈结果也是非常高。通过对重要特征的重新再提取特征，能够发现很多有意思的新特征，这才是用FE打好一场比赛的关键所在。
Feature Selection
总的来说，我们应该生成尽量多的 Feature，相信 Model 能够挑出最有用的 Feature。但有时先做一遍 Feature Selection 也能带来一些好处：
Feature 越少，训练越快。
有些 Feature 之间可能存在线性关系，影响 Model 的性能。
通过挑选出最重要的 Feature，可以将它们之间进行各种运算和操作的结果作为新的 Feature，可能带来意外的提高。
Feature Selection 最实用的方法也就是看 Random Forest 训练完以后得到的 Feature Importance 了。其他有一些更复杂的算法在理论上更加 Robust，但是缺乏实用高效的实现，比如这个。从原理上来讲，增加 Random Forest 中树的数量可以在一定程度上加强其对于 Noisy Data 的 Robustness。
看 Feature Importance 对于某些数据经过脱敏处理的比赛尤其重要。这可以免得你浪费大把时间在琢磨一个不重要的变量的意义上。
Feature Encoding
这里用一个例子来说明在一些情况下 Raw Feature 可能需要经过一些转换才能起到比较好的效果。
假设有一个 Categorical Variable 一共有几万个取值可能，那么创建 Dummy Variables 的方法就不可行了。这时一个比较好的方法是根据 Feature Importance 或是这些取值本身在数据中的出现频率，为最重要（比如说前 95% 的 Importance）那些取值（有很大可能只有几个或是十几个）创建 Dummy Variables，而所有其他取值都归到一个“其他”类里面。
Model Selection
准备好 Feature 以后，就可以开始选用一些常见的模型进行训练了。Kaggle 上最常用的模型基本都是基于树的模型：
Gradient Boosting
Random Forest
Extra Randomized Trees
以下模型往往在性能上稍逊一筹，但是很适合作为 Ensemble 的 Base Model。SVM
Linear Regression
Logistic Regression
Neural Networks
以上这些模型基本都可以通过 sklearn 来使用。
当然，这里不能不提一下 Xgboost。Gradient Boosting 本身优秀的性能加上 Xgboost 高效的实现，使得它在 Kaggle 上广为使用。几乎每场比赛的获奖者都会用 Xgboost 作为最终 Model 的重要组成部分。在实战中，我们往往会以 Xgboost 为主来建立我们的模型并且验证 Feature 的有效性。
Model Training
在训练时，我们主要希望通过调整参数来得到一个性能不错的模型。一个模型往往有很多参数，但其中比较重要的一般不会太多。比如对 sklearn 的 RandomForestClassifier 来说，比较重要的就是随机森林中树的数量 n_estimators 以及在训练每棵树时最多选择的特征数量 max_features。所以我们需要对自己使用的模型有足够的了解，知道每个参数对性能的影响是怎样的。
通常我们会通过一个叫做 Grid Search 的过程来确定一组最佳的参数。其实这个过程说白了就是根据给定的参数候选对所有的组合进行暴力搜索。
```
param_grid = {'n_estimators': [300, 500], 'max_features': [10, 12, 14]}
model = grid_search.GridSearchCV(estimator=rfr, param_grid=param_grid, n_jobs=1, cv=10, verbose=20, scoring=RMSE)
model.fit(X_train, y_train)
```
顺带一提，Random Forest 一般在 max_features 设为 Feature 数量的平方根附近得到最佳结果。
这里要重点讲一下 Xgboost 的调参。通常认为对它性能影响较大的参数有：
oeta：每次迭代完成后更新权重时的步长。越小训练越慢。
onum_round：总共迭代的次数。
osubsample：训练每棵树时用来训练的数据占全部的比例。用于防止 Overfitting。
ocolsample_bytree：训练每棵树时用来训练的特征的比例，类似 RandomForestClassifier 的 max_features。
omax_depth：每棵树的最大深度限制。与 Random Forest 不同，Gradient Boosting 如果不对深度加以限制，最终是会 Overfit 的。
oearly_stopping_rounds：用于控制在 Out Of Sample 的验证集上连续多少个迭代的分数都没有提高后就提前终止训练。用于防止 Overfitting。
Hyperparameter
网格搜索 
网格搜索可能是应用最广泛的超参数搜索算法了，因为它确实很简单。网格搜索通过查找搜索范围内的所有的点，来确定最优值。它返回目标函数的最大值或损失函数的最小值。给出较大的搜索范围，以及较小的步长，网格搜索是一定可以找到全局最大值或最小值的。但是，网格搜索一个比较大的问题是，它十分消耗计算资源，特别是，需要调优的超参数比较多的时候（例如，随机森林里有8个左右
随机搜索 
随机搜索的思想和网格搜索比较相似，只是不再测试上界和下界之间的所有值，只是在搜索范围中随机取样本点。它的理论依据是，如果随即样本点集足够大，那么也可以找到全局的最大或最小值，或它们的近似值。通过对搜索范围的随机取样，随机搜索一般会比网格搜索要快一些。但是和网格搜索的快速版（非自动版）相似，结果也是没法保证的
基于梯度的优化 
贝叶斯优化
贝叶斯优化寻找使全局达到最值的参数时，使用了和网格搜索、随机搜索完全不同的方法。网格搜索和随机搜索在测试一个新的点时，会忽略前一个点的信息。而贝叶斯优化充分利用了这个信息。贝叶斯优化的工作方式是通过对目标函数形状的学习，找到使结果向全局最大值提升的参数。它学习目标函数形状的方法是，根据先验分布，假设一个搜集函数。在每一次使用新的采样点来测试目标函数时，它使用这个信息来更新目标函数的先验分布。然后，算法测试由后验分布给出的，全局最值最可能出现的位置的点。
对于贝叶斯优化，一个主要需要注意的地方，是一旦它找到了一个局部最大值或最小值，它会在这个区域不断采样，所以它很容易陷入局部最值。为了减轻这个问题，贝叶斯优化算法会在勘探和开采(exploration and exploitation)中找到一个平衡点。
Cross Validation
Cross Validation 是非常重要的一个环节。它让你知道你的 Model 有没有 Overfit，是不是真的能够 Generalize 到测试集上。在很多比赛中 Public LB 都会因为这样那样的原因而不可靠。当你改进了 Feature 或是 Model 得到了一个更高的 CV 结果，提交之后得到的 LB 结果却变差了，一般认为这时应该相信 CV 的结果。当然，最理想的情况是多种不同的 CV 方法得到的结果和 LB 同时提高，但这样的比赛并不是太多。
在数据的分布比较随机均衡的情况下，5-Fold CV 一般就足够了。如果不放心，可以提到 10-Fold。但是 Fold 越多训练也就会越慢，需要根据实际情况进行取舍。
很多时候简单的 CV 得到的分数会不大靠谱，Kaggle 上也有很多关于如何做 CV的讨论。比如这个。但总的来说，靠谱的 CV 方法是 Case By Case 的，需要在实际比赛中进行尝试和学习，这里就不再（也不能）叙述了。
Ensemble Generation
Ensemble Learning 是指将多个不同的 Base Model 组合成一个 Ensemble Model 的方法。它可以同时降低最终模型的 Bias 和 Variance（证明可以参考这篇论文，我最近在研究类似的理论，可能之后会写新文章详述)，从而在提高分数的同时又降低 Overfitting 的风险。在现在的 Kaggle 比赛中要不用 Ensemble 就拿到奖金几乎是不可能的。
常见的 Ensemble 方法有这么几种：
oBagging：使用训练数据的不同随机子集来训练每个 Base Model，最后进行每个 Base Model 权重相同的 Vote。也即 Random Forest 的原理。
oBoosting：迭代地训练 Base Model，每次根据上一个迭代中预测错误的情况修改训练样本的权重。也即 Gradient Boosting 的原理。比 Bagging 效果好，但更容易 Overfit。
oBlending：用不相交的数据训练不同的 Base Model，将它们的输出取（加权）平均。实现简单，但对训练数据利用少了。
oStacking：接下来会详细介绍。
从理论上讲，Ensemble 要成功，有两个要素：
oBase Model 之间的相关性要尽可能的小。这就是为什么非 Tree-based Model 往往表现不是最好但还是要将它们包括在 Ensemble 里面的原因。Ensemble 的 Diversity 越大，最终 Model 的 Bias 就越低。
oBase Model 之间的性能表现不能差距太大。这其实是一个 Trade-off，在实际中很有可能表现相近的 Model 只有寥寥几个而且它们之间相关性还不低。但是实践告诉我们即使在这种情况下 Ensemble 还是能大幅提高成绩。
详细ensemble内容参考该博客https://mlwave.com/kaggle-ensembling-guide/
Stacking
相比 Blending，Stacking 能更好地利用训练数据。以 5-Fold Stacking 为例，它的基本原理如图所示：
整个过程很像 Cross Validation。首先将训练数据分为 5 份，接下来一共 5 个迭代，每次迭代时，将 4 份数据作为 Training Set 对每个 Base Model 进行训练，然后在剩下一份 Hold-out Set 上进行预测。同时也要将其在测试数据上的预测保存下来。这样，每个 Base Model 在每次迭代时会对训练数据的其中 1 份做出预测，对测试数据的全部做出预测。5 个迭代都完成以后我们就获得了一个 #训练数据行数 x #Base Model 数量 的矩阵，这个矩阵接下来就作为第二层的 Model 的训练数据。当第二层的 Model 训练完以后，将之前保存的 Base Model 对测试数据的预测（因为每个 Base Model 被训练了 5 次，对测试数据的全体做了 5 次预测，所以对这 5 次求一个平均值，从而得到一个形状与第二层训练数据相同的矩阵）拿出来让它进行预测，就得到最后的输出。
这里是参考代码：
```
class Ensemble(object):
    def __init__(self, n_folds, stacker, base_models):
        self.n_folds = n_folds
        self.stacker = stacker
        self.base_models = base_models
    def fit_predict(self, X, y, T):
        X = np.array(X)
        y = np.array(y)
        T = np.array(T)
        folds = list(KFold(len(y), n_folds=self.n_folds, shuffle=True, random_state=2016))
        S_train = np.zeros((X.shape[0], len(self.base_models)))
        S_test = np.zeros((T.shape[0], len(self.base_models)))
        for i, clf in enumerate(self.base_models):
            S_test_i = np.zeros((T.shape[0], len(folds)))
            for j, (train_idx, test_idx) in enumerate(folds):
                X_train = X[train_idx]
                y_train = y[train_idx]
                X_holdout = X[test_idx]
                # y_holdout = y[test_idx]
                clf.fit(X_train, y_train)
                y_pred = clf.predict(X_holdout)[:]
                S_train[test_idx, i] = y_pred
                S_test_i[:, j] = clf.predict(T)[:]
            S_test[:, i] = S_test_i.mean(1)
        self.stacker.fit(S_train, y)
        y_pred = self.stacker.predict(S_test)[:]
        return y_pred
```
获奖选手往往会使用比这复杂得多的 Ensemble，会出现三层、四层甚至五层，不同的层数之间有各种交互，还有将经过不同的 Preprocessing 和不同的 Feature Engineering 的数据用 Ensemble 组合起来的做法。但对于新手来说，稳稳当当地实现一个正确的 5-Fold Stacking 已经足够了。
*Pipeline
可以看出 Kaggle 比赛的 Workflow 还是比较复杂的。尤其是 Model Selection 和 Ensemble。理想情况下，我们需要搭建一个高自动化的 Pipeline，它可以做到：
模块化 Feature Transform，只需写很少的代码就能将新的 Feature 更新到训练集中。
自动化 Grid Search，只要预先设定好使用的 Model 和参数的候选，就能自动搜索并记录最佳的 Model。
自动化 Ensemble Generation，每个一段时间将现有最好的 K 个 Model 拿来做 Ensemble。
第一点可能意义还不是太大，因为 Feature 的数量总是人脑管理的过来的；第三点问题也不大，因为往往就是在最后做几次 Ensemble。但是第二点还是很有意义的，手工记录每个 Model 的表现不仅浪费时间而且容易产生混乱。
可参考Crowdflower Search Results Relevance 的第一名获得者 Chenglong Chen 公开的pipeline代码https://github.com/ChenglongChen/Kaggle_CrowdFlower
Takeaways
1.比较早的时候就开始做 Ensemble 是对的，这次比赛到倒数第三天我还在纠结 Feature。
2.很有必要搭建一个 Pipeline，至少要能够自动训练并记录最佳参数。
3.Feature 为王。我花在 Feature 上的时间还是太少。
4.可能的话，多花点时间去手动查看原始数据中的 Pattern。
Issues Raised
比赛中遇到的一些问题是很有研究价值的：
1.在数据分布并不 i.i.d. 甚至有 Dependency 时如何做靠谱的 CV。
2.如何量化 Ensemble 中 Diversity vs. Accuracy 的 Trade-off。
3.如何处理 Feature 之间互相影响导致性能反而下降。
SWOT分析法
SWOT分析法让我们可以让我们思路更清晰，专注于利用我们最大的优势（Strengths），了解我们的弱点（Weaknesses），利用现有的机会（Opportunities），警惕潜在的威胁（Threats）。 
SWOT方法使我们可以在正确的方向前进，避开很多令人头疼的事儿。
优势（Strengths）：利用我们已有的优势
利用堆叠技术和Agile过程的经验
协作团队的经验和技能可以相互补充
吸取之前的Kaggle挑战的经验和教训
使用Agile过程处理并行工作的经验
弱势（weaknesses）：我们需要提升的领域
时间有限，限制了探索的深度和广度
对新工具和模型的不熟悉，大大降低了我们的战斗力
边做边学，拖慢了整个进程
对于所使用的新技术，相关的资源十分稀少
机会（opportunities）：可以利用的机会，及实践得到的经验
了解如何制定策略，调整模型、算法和参数，来达到对违约几率的最佳预测
在使用贝叶斯优化算法时，获得实时的经验
尝试使用深度学习（Theano/Keras）来进行违约预测
威胁（threats）：我们需要减轻和控制的危机
数据集比较小，这对模型的泛化有较大的挑战。这影响了模型的构造，最终影响了预测的精度。
对AUC的公差严格到了小数点后10，000位
前5%的目标分数太高了-简直不可行
Agile过程
我们使用‘Agile过程’作为本项目的流程，这是由Bernard发明的。它特别为了机器学习优化过，因为他需要整合很大一块内容，包括数据是如何被处理的，建模、测试的，以及很多更传统的开发生命周期。 
Agile过程利用了‘分块’的概念，以一种快速的方式来完成工作任务。它整合了很多并行的任务，很快地失败，又很快地迭代。它发挥了强大的作用，使产出最大化。

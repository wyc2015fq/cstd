
# R+工业级GBDT︱微软开源 的LightGBM（R包已经开放） - 素质云笔记-Recorder... - CSDN博客

2017年01月05日 14:03:31[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6325


看完一篇介绍文章后，第一个直觉就是这算法已经配得上工业级属性。日前看到微软已经公开了这一算法，而且已经发开python版本，本人觉得等hadoop+Spark这些平台配齐之后，就可以大规模宣传啦~如果R包一发我一定要第一时间学习并更新在本帖下~ 哈哈
看好它是因为支持分布式、GPU运算，而且占用内存小，这几个特制已经足以让她从学界走到工业界，之前的XGboosting更多的使用场景在学术、竞赛。之前我也有写过，感觉局限挺多：
R语言︱XGBoost极端梯度上升以及forecastxgb（预测）+xgboost（回归）双案例解读
---LightGBM的工作还在持续进行，近期将会增加更多的新功能，如：

---R, Julia 等语言支持（目前已原生支持python，R语言正在开发中）

---更多平台(如Hadoop和Spark)的

---支持 GPU加速

---GBDT (Gradient Boosting Decision Tree)

---是机器学习中一个长盛不衰的模型，其主要思想是利用弱分类器（决策树）迭代训练以得到最优模型，该模型具有训练效果好、不易过拟合等优点。GBDT在工业界应用广泛，通常被用于点击率预测，搜索排序等任务。GBDT也是各种数据挖掘竞赛的致命武器，据统计Kaggle上的比赛有一半以上的冠军方案都是基于GBDT。

---Xgboost已经十分完美了，为什么还要追求速度更快、内存使用更小的模型？

---对GBDT算法进行改进和提升的技术细节是什么？

---提出LightGBM的动机

---常用的机器学习算法，例如神经网络等算法，都可以以mini-batch的方式训练，训练数据的大小不会受到内存限制。

---而GBDT在每一次迭代的时候，都需要遍历整个训练数据多次。如果把整个训练数据装进内存则会限制训练数据的大小；如果不装进内存，反复地读写训练数据又会消耗非常大的时间。尤其面对工业级海量的数据，普通的GBDT算法是不能满足其需求的。

---LightGBM提出的主要原因就是为了解决GBDT在海量数据遇到的问题，让GBDT可以更好更快地用于工业实践。

---一、改进的细节

---1.Xgboost是如何工作的？

---目前已有的GBDT工具基本都是基于预排序的方法（pre-sorted）的决策树算法(如 xgboost)。这种构建决策树的算法基本思想是：

---首先，对所有特征都按照特征的数值进行预排序。

---其次，在遍历分割点的时候用O(\#data)的代价找到一个特征上的最好分割点。

---最后，找到一个特征的分割点后，将数据分裂成左右子节点。

---这样的预排序算法的优点是能精确地找到分割点。

---缺点也很明显：

---首先，空间消耗大。这样的算法需要保存数据的特征值，还保存了特征排序的结果（例如排序后的索引，为了后续快速的计算分割点），这里需要消耗训练数据两倍的内存。

---其次，时间上也有较大的开销，在遍历每一个分割点的时候，都需要进行分裂增益的计算，消耗的代价大。

---最后，对cache优化不友好。在预排序后，特征对梯度的访问是一种随机访问，并且不同的特征访问的顺序不一样，无法对cache进行优化。同时，在每一层长树的时候，需要随机访问一个行索引到叶子索引的数组，并且不同特征访问的顺序也不一样，也会造成较大的cache miss。

---2.LightGBM在哪些地方进行了优化？

---基于Histogram的决策树算法

---带深度限制的Leaf-wise的叶子生长策略

---直方图做差加速

---直接支持类别特征(Categorical Feature)

---Cache命中率优化

---基于直方图的稀疏特征优化

---多线程优化

---下面主要介绍Histogram算法、带深度限制的Leaf-wise的叶子生长策略和直方图做差加速。

---参考文章：

---[开源|LightGBM：三天内收获GitHub 1000+ 星](http://mp.weixin.qq.com/s?__biz=MzAwMTA3MzM4Nw==&mid=2649440041&idx=1&sn=615217bcd020618e595b0b642ebadc96&chksm=82c0d6adb5b75fbbd8c4eef92aab203542d0d297f7d2f1e4b0dd86a65a7096a5723e853d8be6&mpshare=1&scene=1&srcid=01051u3UechWUbKzaSSiOUXS#rd)

---来看看LightGBM和XGboosting的差异：

---XGBoost是一款经过优化的分布式梯度提升（Gradient Boosting）库，具有高效，灵活和高可移植性的特点。基于梯度提升框架，XGBoost实现了并行方式的决策树提升(Tree Boosting)，从而能够快速准确地解决各种数据科学问题。

---LightGBM（Light Gradient Boosting Machine）同样是一款基于决策树算法的分布式梯度提升框架。

---速度：速度上xgboost 比LightGBM在慢了10倍

---调用核心效率：随着线程数的增加，比率变小了。这也很容易解释，因为你不可能让线程的利用率是100%，线程的切入切出以及线程有时要等待，这都需要耗费很多时间。保持使用逻辑核心创建一定量的线程，并且不要超过该数。不然反而速度会下降。

---内存占用：xgboost：约 1684 MB；LightGBM： 1425

---MB，LightGBM在训练期间的RAM使用率较低，但是内存中数据的RAM使用量增加

---.

---二、R包中的LigthGBM

---主界面：

---[https://github.com/Microsoft/LightGBM/tree/master/R-package](https://github.com/Microsoft/LightGBM/tree/master/R-package)

---1、下载

---devtools:

---:install_github

---(

---"Microsoft/LightGBM"

---, subdir =

---"R-package"

---)

---devtools你懂得，下载的时候，会出现的问题很多，而且！  不同系统、不同机器，问题都不一样，haha~~譬如：

---警告

---:

---运行命令

---'make -f "Makevars.win" -f "C:/PROGRA~1/R/R-31~1.2/etc/i386/Makeconf" -f "C:/PROGRA~1/R/R-31~1.2/share/make/winshlib.mk" CXX='

---$(

---CXX1X

---)

---$(

---CXX1XSTD

---)

---' CXXFLAGS='

---$(

---CXX1XFLAGS

---)

---' CXXPICFLAGS='

---$(

---CXX1XPICFLAGS

---)

---' SHLIB_LDFLAGS='

---$(

---SHLIB_CXX1XLDFLAGS

---)

---' SHLIB_LD='

---$(

---SHLIB_CXX1XLD

---)

---' SHLIB="lightgbm.dll" '

---的状态是

---2

---ERROR

---:

---compilation failed

---for

---package

---'lightgbm'

---* removing

---'C:/Users/mzheng50/Documents/R/win-library/3.1/lightgbm'

---还有什么R6包加载不对，R6-httr有问题等等等，总之最后是顺利安装了。

---Error

---in

---library(

---R6

---) : Package ‘

---R6

---’ version

---2.1

---.2

---cannot be unloaded

---2、案例

---library

---(lightgbm)

---data

---(

---agaricus

---.

---train

---,

---package

---='

---lightgbm'

---)

---train

---<- agaricus.train

---dtrain

---<- lgb.

---Dataset

---(train$

---data

---, label=train$label)

---params

---<- list(objective=

---"regression"

---, metric=

---"l2"

---)

---model

---<- lgb.cv(params, dtrain,

---10

---, nfold=

---5

---, min_data=

---1

---, learning_rate=

---1

---, early_stopping_rounds=

---10

---)



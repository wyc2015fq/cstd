# OpenCV学习笔记（四十五）——小试随机森林（random forest）算法ml

2012年03月12日 18:38:22

yang_xian521

阅读数：48081

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

对于随机森林算法，原理我想大家都会去看论文，推荐两个老外的网址<http://www.stat.berkeley.edu/users/breiman/RandomForests/>和<https://cwiki.apache.org/MAHOUT/random-forests.html>，第一个网址是提出随机森林方法大牛写的，很全面具体，第二个是我自己找的一个，算是一个简化版的介绍吧。说白了，随机森林分类的过程就是对于每个随机产生的决策树分类器，输入特征向量，森林中每棵树对样本进行分类，根据每个树的权重得到最后的分类结果。所有的树训练都是使用同样的参数，但是训练集是不同的，分类器的错误估计采用的是oob（out  of bag）的办法。如果大家看懂了，接下来就简单咯

还是先介绍一下对应opencv的类和函数吧，之前有笼统介绍过机器学习的类，对于随机森林相关算法类有CvRTParams、CvRTrees。具体再讲解一下：

CvRTParams类包涵了随机森林训练过程中需要设置的全部参数，继承自CvDTParams。其中

max_depth表示单棵树的最大深度

min_sample_count阈值，当节点的样本数比阈值小的时候，节点就不在进行分裂

regression_accuracy回逆树时候的阈值

use_surrogates是否使用代理？（这是神马功能。。。）

max_categories最大分类的阈值

priors先验分类可能性

calc_var_importance变量重要性是否计算标志

nactive_vars每棵树选取特征子集的大小

max_num_of_trees_in_the_forest森林内树的数目上限

forest_accuracy森林训练OOB error的精度

termcrit_type森林训练阈值选取的类型

以上参数的赋值都通过CvRTParams的构造函数实现。

CvRTrees就是随机森林的主体了。包涵了train训练函数、predict预测函数、predict_prob返回预测分类标签、getVarImportance返回变量权重矩阵、get_proximity返回两训练样本之间的相似度、calc_error返回随机森林的预测误差、get_train_error返回训练误差、get_rng返回使用随机数的当前值、get_tree_count返回构造随机森林的树的数目、get_tree返回构造随机森林的其中一棵树。

接下来，结合sample自带的letter_recog.cpp讲解一下如何应用随机森林算法做字母的识别，这里选择的训练样本库是<http://archive.ics.uci.edu/ml/>下的一个训练集，这个网站还有很多其他的资源提供，非常好的机器学习的data库。这个训练文件letter-recognition.data有20000个训练字母，每一字母用16维的特征表示。本程序使用前16000个进行训练，后4000个进行测试。唯一让我很不爽的是这个程序是用老版本的数据结构写的，所以我决定用新结构再写一下，我对原程序进行了简化，删除了很多不相关和不是必须的部分。

其实随机森林使用起来非常简单，两个最重要的步骤无非就是train()和predict()函数，其他的函数都是用来得到测试结果。

这里把我简化并用新结构改写之后的代码下载链接奉上<http://download.csdn.net/detail/yang_xian521/4134557>
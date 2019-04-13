
# 问句识别：基于Xgboost的中文疑问句判断模型 - lpty的博客 - CSDN博客

2018年04月13日 19:11:35[lpty](https://me.csdn.net/sinat_33741547)阅读数：1855所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、前言
关于Xgboost：
> xgboost是大规模并行boosted tree的工具，它是目前最快最好的开源boosted

> tree工具包，比常见的工具包快10倍以上。在数据科学方面，有大量kaggle选手选用它进行数据挖掘比赛，其中包括两个以上kaggle比赛的夺冠方案。在工业界规模方面，xgboost的分布式版本有广泛的可移植性，支持在YARN,

> MPI, Sungrid Engine等各个平台上面运行，并且保留了单机并行版本的各种优化，使得它可以很好地解决于工业界规模的问题。

> 网上有不少介绍的文章，这里就此略过，直奔主题，写一篇炼丹文。

> 二、实战

> 1、语料

> （1）语料格式

> content,label
在么,1
你好,0
...
> 左边为文本，右边为标签，从这里也可以看出来，疑问句判别问题可以简单的作为一个二分类问题来解决。

> （2）语料预处理

> tokenizer = jieba.Tokenizer()
   corpus['tokens'] = corpus.content.apply(lambda x: list(tokenizer.cut(x)))
> 从本地读取语料后，进行简单的分词处理

> （3）特征工程

> vectorizer = TfidfVectorizer(smooth_idf=True,
                                analyzer=analyzer,
                                ngram_range=(1, 1),
                                min_df=1, norm='l1')
   sparse_vector = vectorizer.fit_transform(train.tokens.apply(lambda x: ' '.join(x)).tolist())
> 这里对分词后的语料进行TFIDF特征抽取，构造训练矩阵。

> 2、模型

> （1）模型参数

> 模型参数使用k折交叉验证进行对比，确定

> for i, param in enumerate(param_grid):
       cv_result = xgb.cv(param, self.train_matrix,
                          num_boost_round=self.num_boost_round,  \# max iter round
                          nfold=self.nfold,
                          stratified=self.stratified,
                          metrics=self.metrics,  \# metrics focus on
                          early_stopping_rounds=self.early_stopping_rounds)  \# stop when metrics not get better
> 最终得到表现最好的一组参数及相应的迭代次数等

> （2）模型训练

> _, best_param, best_iter_round = self.model_param_select()
   self.model = xgb.train(dtrain=self.train_matrix, params=best_param, num_boost_round=best_iter_round)
> 与交叉验证寻找最佳参数不用，训练模型时使用全部的数据进行训练

> （3）模型结果

> Param select 0, auc: 0.9793438, iter_round: 207, params: {'objective': u'binary:logistic', 'subsample': 0.5, 'eta': 0.1, 'max_depth': 4, 'silent': 1}, now best auc: 0.9793438
   Param select 1, auc: 0.9799142, iter_round: 350, params: {'objective': u'binary:logistic', 'subsample': 0.7, 'eta': 0.1, 'max_depth': 4, 'silent': 1}, now best auc: 0.9799142
   Param select 2, auc: 0.9802402, iter_round: 280, params: {'objective': u'binary:logistic', 'subsample': 1.0, 'eta': 0.1, 'max_depth': 4, 'silent': 1}, now best auc: 0.9802402
   ...
   Param select 24, auc: 0.97926, iter_round: 694, params: {'objective': u'binary:logistic', 'subsample': 0.5, 'eta': 0.02, 'max_depth': 6, 'silent': 1}, now best auc: 0.980495
   Param select 25, auc: 0.9803058, iter_round: 824, params: {'objective': u'binary:logistic', 'subsample': 0.7, 'eta': 0.02, 'max_depth': 6, 'silent': 1}, now best auc: 0.980495
   Param select 26, auc: 0.980129, iter_round: 880, params: {'objective': u'binary:logistic', 'subsample': 1.0, 'eta': 0.02, 'max_depth': 6, 'silent': 1}, now best auc: 0.980495
> 最终可以看到，模型的auc在0.980495得到最佳的参数，保存模型。

> 3、实例

> from interrogative.api import *
train()
tag = recognize(u'今天 来 点 兔子 吗')
output = '是疑问句' if tag else '不是疑问句'
print(output)
> 三、其他

> 1、Xgboost参数解释：
> [https://www.cnblogs.com/mfryf/p/6293814.html](https://www.cnblogs.com/mfryf/p/6293814.html)

> 2、具体源码可以在我的github上找到：
> [https://github.com/lpty/nlp_base](https://github.com/lpty/nlp_base)



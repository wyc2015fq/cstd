# 集成学习——随机深林（RF） - wsp_1138886114的博客 - CSDN博客





2018年07月31日 13:12:23[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：85









- - - [1 随机深林（RF）概论](#1-随机深林rf概论)
- [2.随机森林 ( 分类 ) 算法：](#2随机森林-分类-算法)
- [3 随机森林（回归）算法：](#3-随机森林回归算法)
- [4 使用grid search调参](#4-使用grid-search调参)





### 1 随机深林（RF）概论

```
是一种集成学习/融合算法，基于Bagging的融合思想
它的基础算法是决策树CART算法。既能做分类/也能做回归
顾名思义---产生若干个决策树群

随机森林的两个 -- 随机
    每一棵决策树训练样本随机有放回抽样（Boosts trap）抽样
    每一棵决策树使用的特征是所有特征的一个随机抽样子集（从M个所有特征中抽取m个特征来构造）

    随机性的引入使得随机森林不容易过拟合，具有很好的抗噪性

随机森林的分类效果：
    森林中任意两棵树的相关性：相关性越大--->错误率越大
    森林中每棵树的分类能力：每棵树分类能力越强-->整个森林的错误率越低

    减少特征选择个数 m ，树的相关性和分类能力降低
    增大 m 两者也随之增大。
    选择最优的 m ---随机森林唯一的参数

袋外错误率（ oob error,out-of-bag error）
    oob 可以作为单独测试集

    优点：
        预测效果好
        能处理高维特征，不需要提前做特征过滤
        不容易过拟合
        模型训练速度块（相比Boosting），容易做并行化处理
        能对特征的重要性排序

    缺点：
        对少量数据集和低维度数据集分类--效果不一定好
```

### 2.随机森林 ( 分类 ) 算法：

**sklearn.ensemble.RandomForestClassifier**

```python
sklearn.ensemble.RandomForestClassifier(
    #Bagging 框架参数
    n_estimastors = 10,     #子模型个数（10-200）-默认10，太小容易欠拟合，反之过拟合
    booststrap = True,      #有放回采样
    oob_score = False,      #袋外样本

    #随机森林决策树参数
    criterion = 'gini',     #CART决策树评价标准：gini,entropy,默认gini
    max_depth = None,       #决策树深度
    min_samples_split = 2,  #内部节点划分最小样本数，默认2
    max_samples_leaf = 1,   #叶子数--默认1
    min_weight_fraction_leaf = 0.0,
    max_features = 'auto',
    max_leaf_nodes = None,  #最大叶子节点数，可以限制，防止过拟合
    max_impurity_decrease = 0.0,
    max_impurity_split = None,  #节点划分最小不纯度

    random_state = None
    verbose = 0,
    warm_start = False,
    class_weight = None
    )
```

### 3 随机森林（回归）算法：

**sklearn.ensemble.RandomForestRegressor**

```python
sklearn.ensemble.RandomForestRegressor(
    #Bagging 框架参数
    n_estimastors = 10,     #子模型个数（10-200）-默认10，太小容易欠拟合，反之过拟合
    booststrap = True,      #有放回采样
    oob_score = False,      #袋外样本

    #随机森林决策树参数
    criterion = 'mse',      #CART决策树回归评价标准: mse,mae,默认 mse
    max_depth = None,       #决策树深度
    min_samples_split = 2,  #内部节点划分最小样本数，默认2
    max_samples_leaf = 1,   #叶子数--默认1
    min_weight_fraction_leaf = 0.0,
    max_features = 'auto',
    max_leaf_nodes = None,  #最大叶子节点数，可以限制，防止过拟合
    max_impurity_decrease = 0.0,
    max_impurity_split = None,  #节点划分最小不纯度

    random_state = None
    verbose = 0,
    warm_start = False
    )
```

### 4 使用grid search调参




# 集成学习——GBDT - wsp_1138886114的博客 - CSDN博客





2018年07月31日 13:15:30[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：125









- - - [一、GBDT算法概况，与AdaBoost的对比](#一gbdt算法概况与adaboost的对比)
- [二、GBDT算法两种描述思路：](#二gbdt算法两种描述思路)- - [2.1 基于残差（真实值-预测值）的版本–积跬步以至千里](#21-基于残差真实值-预测值的版本积跬步以至千里)
- [2.2 基于梯度Gradient 的版本](#22-基于梯度gradient-的版本)


- [三、GBDT算法——代码：](#三gbdt算法代码)





### 一、GBDT算法概况，与AdaBoost的对比

```
1 两者都是Boosting迭代融合思想算法

2 Adaboost可以使用各种不同弱学习器，而GBDT的弱分类器是决策树（CART），
      GBDT = GB（Gradient Boosting）+DT（Dicision Tree）
3 GBDT是Adaboost的扩展，或者说是其一个特例：
      Adaboost就是‘指数损失函数’+‘弱分类器为决策树’的GBDT
```

### 二、GBDT算法两种描述思路：

##### 2.1 基于残差（真实值-预测值）的版本–积跬步以至千里

```
每次模型都只是学习一点，然后一步步的接近最终预测值
```

![这里写图片描述](https://img-blog.csdn.net/20180603101447841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180603101956705?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 2.2 基于梯度Gradient 的版本

```
优点：精度高（boosting共同特点）
      能处理非线性问题，回归和分类 ---（决策树算法的特点）
      能适应多种损失函数（Gradient 梯度的特点）

缺点：计算复杂度大，耗时（Boosting 的共性）
```

### 三、GBDT算法——代码：

```python
sklearn.ensemble.GradientBoostingClassifier

sklearn.ensemble.GradientBoostingClassifier(
    #GBDT Boosting 框架参数
    loss ='deviance',       #损失函数 deviance/exponential
    learning_rate = 0.1,    #学习率：弱学习器权重缩小系数v(罗马字母)/步长
    n_estimastors = 100,    #子模型个数（10-200）-默认10，太小容易欠拟合，反之过拟合
    subsample = 1.0,        #正则化中的子采样，防止过拟合.[0,1]
    init = None,            #初始化的弱学习器，不用管

    #弱分类器（决策树）参数
    criterion = 'friedman_mse',     #分类准则，还可选fridman_mae
    min_samples_split = 2,          #内部节点划分最小样本数，默认2
    max_samples_leaf = 1,           #叶子数--默认1
    min_weight_fraction_leaf = 0.0,
    max_depth = 3,                  #决策树深度
    max_impurity_decrease = 0.0,
    max_impurity_split = None,      #节点划分最小不纯度
    random_state = None,
    max_features = None,

    verbose = 0,
    max_leaf_nodes = None,  #最大叶子节点数，可以限制，防止过拟合
    warm_start = False,
    presort = 'auto'
    )
```





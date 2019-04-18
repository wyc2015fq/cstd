# 集成学习——Adaboost - wsp_1138886114的博客 - CSDN博客





2018年07月16日 16:46:38[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：108










- - - [1. Adaboost算法基本原理](#1-adaboost算法基本原理)
- [2. Adaboost 损失函数](#2-adaboost-损失函数)
- [3. Adaboost 代码](#3-adaboost-代码)






### 1. Adaboost算法基本原理
- Adaboost:Adaptive Boosting(自适应增强)
自适应在于：前一个分类器分错的样本会被用来训练下一个分类器。该方法对噪声数据和异常数据很敏感。 但不容易出现过拟合。每一轮训练都更加关心分类错误的案例，增加分类错误的样例权重，减少分类正确的样例权重
![这里写图片描述](https://img-blog.csdn.net/20180716101447399?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2. Adaboost 损失函数

Adaboost算法，它是前向分步加法算法的特例，是一个加和模型，损失函数就是指数函数。在Adaboost中，经过m此迭代之后，可以得到 $f_m(x):$


$f_m(x) = f_{m-1}(x) + \alpha_mG_m(x)$

Adaboost每次迭代时的目的是为了找到最小化下列式子时的参数$α 和G：$


$arg\min_{\alpha ,G} = \sum_{i=1}^{N}exp[-y_i(f_{m-1}(x_i)+\alpha G(x_i))]$

而指数损失函数(exp-loss）的标准形式如下 


$L(y, f(x)) = \exp[-yf(x)]$
可以看出，Adaboost的目标式子就是指数损失，在给定n个样本的情况下，**Adaboost的损失函数**为：



$L(y, f(x)) = \frac{1}{n}\sum_{i=1}^{n}\exp[-y_if(x_i)]$

![示意图](https://img-blog.csdn.net/20180603093912777?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![定理8.1](https://img-blog.csdn.net/20180801220402850?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
Adaboost 优点：
    很好的利用了弱分类器进行级联。
    可以将不同分类算法作为弱分类器
    Adaboost 具有很好高的精度，不容易过拟合
    相对于 bagging/Random Forest 算法，Adaboost充分考虑到每个分类器的权重

Adaboost 缺点:
    迭代次数也就是弱分类器数目不太好设定。
    训练比较长
    预测效果依赖于弱分类器的选择（Adaboost只是一个算法框架）
```

### 3. Adaboost 代码

```python
sklearn.ensemble.AdaBoostClassifier(
    #弱分类器自身参数
    base_estimator = DcisionTreeClassifier,     #选择一个弱分类器

    #boosting框架参数
    n_estimators = 50,      #弱学习器 最大迭代次数
    learning_rate = 1.0,    #弱学习器 的权重缩减系数（0-1）
    algorithm = 'SAMME.R',  #弱分类器支持概率预测
    random_state = None

    )
```






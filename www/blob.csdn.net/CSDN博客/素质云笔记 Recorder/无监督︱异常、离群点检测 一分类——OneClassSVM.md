# 无监督︱异常、离群点检测 一分类——OneClassSVM - 素质云笔记/Recorder... - CSDN博客





2017年08月03日 19:44:30[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：20234








### OneClassSVM两个功能：异常值检测、解决极度不平衡数据

#### **因为之前一直在做非平衡样本分类的问题**，其中如果有一类比例严重失调，就可以直接用这个方式来做：OneClassSVM；OneClassSVM还有一个功能就是异常值检测。

其他我的相关博客： 

 1、[机器学习︱非平衡数据处理方式与评估](http://blog.csdn.net/sinat_26917383/article/details/75890859)

2、[RFM模型+SOM聚类︱离群值筛选问题](http://blog.csdn.net/sinat_26917383/article/details/51222143)

3、[R语言︱异常值检验、离群点分析、异常值处理](http://blog.csdn.net/sinat_26917383/article/details/51210793)
台湾大学林智仁所设计和实现的库LibSVM（地址：[http://www.csie.ntu.edu.tw/~cjlin/libsvm/](http://www.csie.ntu.edu.tw/~cjlin/libsvm/)），里面包含很多种语言的版本。 R语言的在e1071包中。

sklearn之中有该算法，叫[OneClassSVM](http://scikit-learn.org/stable/modules/generated/sklearn.svm.OneClassSVM.html#sklearn.svm.OneClassSVM)

![这里写图片描述](https://img-blog.csdn.net/20170803192449963?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

来看图，一分类就是左图中一大堆点的区域，当有超过这一堆的就会分到另外一类。
![这里写图片描述](https://img-blog.csdn.net/20170803193731142?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

来源知乎：[什么是一类支持向量机（one class SVM），是指分两类的支持向量机吗？](https://www.zhihu.com/question/22365729)

.
## 一、数据结构

主要参考官网的该案例：[One-class SVM with non-linear kernel (RBF)](http://scikit-learn.org/stable/auto_examples/svm/plot_oneclass.html)

训练数据集：X_train—— 2*2

```
array([[ 1.99965086,  2.15923383],
       [ 1.50571424,  2.12918697],
       [ 1.93707554,  2.14992192],
                   ...
       [-1.76587184, -2.50357511]])
```

跟我们之前的数据集有出入的地方在于，我们**不用喂给分类器label，而是无监督的。**

验证数据集：X_test—— 2*2 

array([[ 1.80383853,  1.58672939], 

       [ 2.01768496,  2.06326541], 

       [ 2.17193985,  1.97028432], 

       [ 2.24551427,  2.1166712 ], 

       [ 2.28101497,  2.33716323], 

       [ 1.71641595,  1.78091046], 

       [ 1.61257108,  1.7564344 ], 

       [ 2.38147731,  1.97159579], 

       [ 1.77266618,  2.04611541], 

       [ 2.32454414,  2.79806359], 

       [ 1.73709171,  2.36242084], 

       …. 

       [-1.76587184, -2.50357511]])
离群值X_outliers—— 2*2

```
array([[-2.60871078, -1.94353134],
       [-3.25360609,  2.66247128],
       [-3.86062278, -3.29186255],
       [ 0.12889109, -0.14897511],
       [ 0.04729524,  3.62353636],
       [-1.1672252 ,  2.63097474],
       [-1.13875221,  1.71308978],
       [ 1.75003901,  3.65887014],
       [-2.97229004, -0.83006677]])
```

预测的结果y_pred_train：

```
array([ 1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1., -1., -1.,  1.,  1.,
        1.,  1.,  1.,  1.,  1.,  1.,  1.,  1., -1.,  1.,  1.,  1.,  1.,
        1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,
        1.,  1.,  1.,  1., -1.,  1., -1., -1.,  1.,  1.,  1.,  1.,  1.,
...
        1.,  1.,  1.,  1., -1.,  1.,  1.,  1.,  1.,  1.,  1., -1.,  1.,
        1.,  1.,  1.,  1.,  1.,  1., -1.,  1.,  1.,  1.,  1.,  1.,  1.,
        1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.,
        1.,  1.,  1.,  1.,  1.])
```

预测的结果为-1 或 1 ，在这个群落中为1，不在为-1。 

.

## 二、sklearn实现：OneClassSVM

. 

主要参考官网的该案例：[One-class SVM with non-linear kernel (RBF)](http://scikit-learn.org/stable/auto_examples/svm/plot_oneclass.html)

整个案例的code：

```
print(__doc__)

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.font_manager
from sklearn import svm

xx, yy = np.meshgrid(np.linspace(-5, 5, 500), np.linspace(-5, 5, 500))
# Generate train data
X = 0.3 * np.random.randn(100, 2)
X_train = np.r_[X + 2, X - 2]
# Generate some regular novel observations
X = 0.3 * np.random.randn(20, 2)
X_test = np.r_[X + 2, X - 2]
# Generate some abnormal novel observations
X_outliers = np.random.uniform(low=-4, high=4, size=(20, 2))

# fit the model
clf = svm.OneClassSVM(nu=0.1, kernel="rbf", gamma=0.1)
clf.fit(X_train)
y_pred_train = clf.predict(X_train)
y_pred_test = clf.predict(X_test)
y_pred_outliers = clf.predict(X_outliers)
n_error_train = y_pred_train[y_pred_train == -1].size
n_error_test = y_pred_test[y_pred_test == -1].size
n_error_outliers = y_pred_outliers[y_pred_outliers == 1].size

# plot the line, the points, and the nearest vectors to the plane
Z = clf.decision_function(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.title("Novelty Detection")
plt.contourf(xx, yy, Z, levels=np.linspace(Z.min(), 0, 7), cmap=plt.cm.PuBu)
a = plt.contour(xx, yy, Z, levels=[0], linewidths=2, colors='darkred')
plt.contourf(xx, yy, Z, levels=[0, Z.max()], colors='palevioletred')

s = 40
b1 = plt.scatter(X_train[:, 0], X_train[:, 1], c='white', s=s)
b2 = plt.scatter(X_test[:, 0], X_test[:, 1], c='blueviolet', s=s)
c = plt.scatter(X_outliers[:, 0], X_outliers[:, 1], c='gold', s=s)
plt.axis('tight')
plt.xlim((-5, 5))
plt.ylim((-5, 5))
plt.legend([a.collections[0], b1, b2, c],
           ["learned frontier", "training observations",
            "new regular observations", "new abnormal observations"],
           loc="upper left",
           prop=matplotlib.font_manager.FontProperties(size=11))
plt.xlabel(
    "error train: %d/200 ; errors novel regular: %d/40 ; "
    "errors novel abnormal: %d/40"
    % (n_error_train, n_error_test, n_error_outliers))
plt.show()
```

直接输入的结果为： 
![这里写图片描述](https://img-blog.csdn.net/20170803193608189?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
## 三、其他的离群检验方法

主要参考官网的：[Outlier detection with several methods](http://scikit-learn.org/stable/auto_examples/covariance/plot_outlier_detection.html#sphx-glr-auto-examples-covariance-plot-outlier-detection-py)

```
classifiers = {
    "One-Class SVM": svm.OneClassSVM(nu=0.95 * outliers_fraction + 0.05,
                                     kernel="rbf", gamma=0.1),
    "Robust covariance": EllipticEnvelope(contamination=outliers_fraction),
    "Isolation Forest": IsolationForest(max_samples=n_samples,
                                        contamination=outliers_fraction,
                                        random_state=rng)}
```

其中三种方法的介绍：

#### **Robust covariance：**

基于协方差的稳健估计，假设数据是高斯分布的，那么在这样的案例中执行效果将优于One-Class SVM；

#### **One-Class SVM：**

利用One-Class SVM，它有能力捕获数据集的形状,因此对于强非高斯数据有更加优秀的效果，例如两个截然分开的数据集。严格来说，一分类的SVM并不是一个异常点监测算法，而是一个奇异点检测算法：它的训练集不能包含异常样本，否则的话，可能在训练时影响边界的选取。但是，对于高维空间中的样本数据集，如果它们做不出有关分布特点的假设，One-class SVM将是一大利器。

#### **Isolation Forest：**

孤立森林是一个高效的异常点监测算法。SKLEARN提供了ensemble.IsolationForest模块。该模块在进行检测时，会随机选取一个特征，然后在所选特征的最大值和最小值随机选择一个分切面。该算法下整个训练集的训练就像一颗树一样，递归的划分。划分的次数等于根节点到叶子节点的路径距离d。所有随机树（为了增强鲁棒性，会随机选取很多树形成森林）的d的平均值，就是我们检测函数的最终结果。 

那些路径d比较小的，都是因为距离主要的样本点分布中心比较远的。也就是说可以通过寻找最短路径的叶子节点来寻找异常点。它的例子也放在后面。 

.

**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


## 参考文献：

[One-class SVM with non-linear kernel (RBF)](http://scikit-learn.org/stable/auto_examples/svm/plot_oneclass.html)
[什么是一类支持向量机（one class SVM），是指分两类的支持向量机吗？](https://www.zhihu.com/question/22365729)
[异常检测用几种方法](http://www.cnblogs.com/Gihub/p/3828940.html)
[sklearn中的异常检测方法](http://blog.csdn.net/hustqb/article/details/75216241)










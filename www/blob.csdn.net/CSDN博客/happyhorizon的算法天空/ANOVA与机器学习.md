# ANOVA与机器学习 - happyhorizon的算法天空 - CSDN博客
2019年02月15日 17:14:32[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：84标签：[anova																[统计																[machine learnning																[](https://so.csdn.net/so/search/s.do?q=&t=blog)](https://so.csdn.net/so/search/s.do?q=machine learnning&t=blog)](https://so.csdn.net/so/search/s.do?q=统计&t=blog)](https://so.csdn.net/so/search/s.do?q=anova&t=blog)
个人分类：[Machine Learning](https://blog.csdn.net/happyhorizion/article/category/7303427)
### 文章目录
- [方差分析ANOVA](#ANOVA_1)
- [组间变异和组内变异](#_9)
- [均方差](#_28)
- [F分布与F值](#FF_41)
- [方差分析的关键条件](#_53)
- [Anova在机器学习中的应用: 特征选择](#Anova__62)
- [总结](#_251)
- [更多阅读:](#_264)
# 方差分析ANOVA
anova, analysis of variance, 方差分析, 又称"变异数分析", 用于两个或两个以上样本均数差别的显著性检验.
由于各种因素的影响, 观测数据呈现波动的形状, 造成波动的原因可以分为:
- 不可控的随机因素
- 可控因素, 这是在研究中施加的控制结果的参数引起的
## 组间变异和组内变异
换个角度理解, 任何观察值的总变异都可以分解为组间变异和组内变异. 假设n为样本总数, m为组数.
- 
总变异(total variation)
所有测量值之间总的变异程度 :
$$ SS_T = \sum_{i=1}^{m} \sum_{j=1}^{m_i} (X_{ij}-\bar{X})^2$$
- 
组间变异
各组均数与总均数的离均差平方和：
$$ SS_b =\sum_{i=1}^{m} m_i(\bar{X_i}-\bar{X})^2$$
组间变异反映了各组均数的变异程度， 组间变异=随机误差+处理因素作用
- 
组内变异
用各个组内测量值$X_{ij}$ 与其所在组的均数差值的平方和来表示
$$SS_w = \sum_{i=1}^{m} \sum_{j=1}^{m_i}(X_{ij}-\bar{X_i}) $$
组内变异$SS_w$反映了随机误差的影响，包括了个体误差和测量误差。
## 均方差
变异程度出了与离均差平方和的大小有关以外，还与其自由度有关，由于各部分的自由度并不相等，因此各个部分的自由度不能直接比较，应该用各个部分的离均差平方和除以自由度，称为均方差（Mean Square, MS）组内均方和组间均方分别为：
- 
组内均方
组内自由度：$dfb = m-1$， 组内均方：
$$ MS_b=SS_b/dfb $$
- 
组间均方
组间自由度：$dfw = n-m$， 组间均方：
$$ MS_w=SS_w/dfw $$
组间均方与组内均方比值越接近1，样本越可能是来自一个样本，比值越大，样本越可能来自不同的样本。
## F分布与F值
如果各组样本都来自同一个总体，控制因素没有起到作用，那么组间变异和组内变异都只是反映了随机误差的大小。组间均方和组内均方的比值就称为F统计量：
$$ F = \frac{MS_w}{MS_b}$$
F分布更为严格的定义为：
若总体$X~N(0,1)$, $(X_1, X_2, ..., X_{n_1})$与$(Y_1,Y_2, ...,Y_{n_2})$为来自X的两个独立样本，那么统计量
$$ F = \frac{\sum_{i=1}^{n_1}X_i^2}{n_1} / \frac{\sum_{i=1}^{n_2}X_i^2}{n_2}$$
服从自由度为$n_1, n_2$的F分布。F分布是一个非对称分布，有两个自由度，且自由度的前后顺序不可互换。
![F分布](https://img-blog.csdnimg.cn/20190215170848253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
通常，假设控制因素没有起作用，即H0（null hypothesis）按照公式计算F值，并查F分布表，得到F分布的单尾临界值$F_{a(n_1,n_2)}$ 。
假设a=0.05， 如果 $F\geq{F_{0.05(n_1,n_2)}}$ 时，$p\leq0.05$,拒绝H0，认为两组间有显著差异。
## 方差分析的关键条件
- 各组服从正态分布
- 各组符合方差齐性
- 独立性
说明： 方差齐性检验
方差齐性检验方法有Bartlett检验法，LeveneF检验，此外，如果最大方差于最小方差之比<3,就可以初步认为方差齐同。
如果数据不符合上述方差分析的条件，那么应该采取方差齐性转换/正态性转换，或者采用非参数检验的方法。
# Anova在机器学习中的应用: 特征选择
利用Anova，可以比较样本间不同特征差异的显著性，在数据进入分类器/回归器训练之前进行特征提取，效果类似于数据降维。
以iris数据集为例，首先在数据集中人为地加入了一些噪声信息，然后分别用Anova方差分析选择具有统计显著性的特征，对比p值和SVM支撑向量系数的分布。根据ANOVA的结果，可以看出在iris数据集中主要是前4个特征非常显著地给出了不同类别之间的差异信息。但是SVM的支撑向量中，除了这4个特征外，还零散地选择了一些其它并不重要的特征。
将anova提取的特征信息作为SVM的输入，
```python
clf_selected = svm.SVC(kernel='linear')
clf_selected.fit(selector.transform(X_train), y_train)
```
可以看到在柱形图中，浅蓝色的SVM weighs after selection就没有那些不重要的特征了。当数据噪声增加的时候，可以提高SVM的表现。
```python
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn import datasets, svm
from sklearn.feature_selection import SelectPercentile, f_classif
# #############################################################################
# Import some data to play with
# The iris dataset
iris = datasets.load_iris()
# Some noisy data not correlated
E = np.random.uniform(0, 1, size=(len(iris.data), 20))
# Add the noisy data to the informative features
X = np.hstack((iris.data, E))
y = iris.target
X_train, X_test, y_train, y_test = train_test_split(X, y,train_size=0.7)
plt.figure(1)
plt.clf()
X_indices = np.arange(X_train.shape[-1])
######################################################
# Univariate feature selection with F-test for feature scoring
# We use the default selection function: the 10% most significant features
selector = SelectPercentile(f_classif, percentile=10)
selector.fit(X_train, y_train)
scores = -np.log10(selector.pvalues_)
scores /= scores.max()
plt.bar(X_indices - .45, scores, width=.2,
        label=r'Univariate score ($-Log(p_{value})$)', color='darkorange',
        edgecolor='black')
####################################################
# Compare to the weights of an SVM
clf = svm.SVC(kernel='linear')
clf.fit(X_train, y_train)
svm_weights = (clf.coef_ ** 2).sum(axis=0)
svm_weights /= svm_weights.max()
plt.bar(X_indices - .25, svm_weights, width=.2, label='SVM weight',
        color='navy', edgecolor='black')
clf_selected = svm.SVC(kernel='linear')
clf_selected.fit(selector.transform(X_train), y_train)
svm_weights_selected = (clf_selected.coef_ ** 2).sum(axis=0)
svm_weights_selected /= svm_weights_selected.max()
plt.bar(X_indices[selector.get_support()] - .05, svm_weights_selected,
        width=.2, label='SVM weights after selection', color='c',
        edgecolor='black')
plt.title("Comparing feature selection")
plt.xlabel('Feature number')
plt.yticks(())
plt.axis('tight')
plt.legend(loc='upper right')
plt.show()
y_pred = clf.predict(X_test)
y_pred_selected = clf_selected.predict(selector.transform(X_test))
print(' less noise ')
print('------------------')
print('only SVM:')
print(classification_report(y_test, y_pred))
print('anova_SVM:')
print(classification_report(y_test, y_pred_selected))
######################################################
# increase noise rate
E = np.random.uniform(0, 1, size=(len(iris.data), 100))
# Add the noisy data to the informative features
X = np.hstack((iris.data, E))
y = iris.target
X_train, X_test, y_train, y_test = train_test_split(X, y,train_size=0.7)
# SVM only
clf = svm.SVC(kernel='linear')
clf.fit(X_train, y_train)
# SVM with anova feature selection
selector = SelectPercentile(f_classif, percentile=10)
selector.fit(X_train, y_train)
clf_selected = svm.SVC(kernel='linear')
clf_selected.fit(selector.transform(X_train), y_train)
y_pred = clf.predict(X_test)
y_pred_selected = clf_selected.predict(selector.transform(X_test))
print('more noise')
print('------------------')
print('only SVM:')
print(classification_report(y_test, y_pred))
print('anova_SVM:')
print(classification_report(y_test, y_pred_selected))
X_sel = selector.transform(X_test)
print(X_sel.shape)
print(X_test.shape)
```
输出：
```python
less noise 
------------------
only SVM:
             precision    recall  f1-score   support
          0       1.00      1.00      1.00        11
          1       1.00      0.89      0.94        18
          2       0.89      1.00      0.94        16
avg / total       0.96      0.96      0.96        45
anova_SVM:
             precision    recall  f1-score   support
          0       1.00      1.00      1.00        11
          1       1.00      0.94      0.97        18
          2       0.94      1.00      0.97        16
avg / total       0.98      0.98      0.98        45
more noise
------------------
only SVM:
             precision    recall  f1-score   support
          0       1.00      1.00      1.00        13
          1       0.78      0.82      0.80        17
          2       0.79      0.73      0.76        15
avg / total       0.84      0.84      0.84        45
anova_SVM:
             precision    recall  f1-score   support
          0       1.00      1.00      1.00        13
          1       0.94      0.94      0.94        17
          2       0.93      0.93      0.93        15
avg / total       0.96      0.96      0.96        45
(45, 11)
(45, 104)
```
上述代码还可以通过构建pipeline实现，例如
```python
from sklearn.svm import SVC
svc = SVC(kernel='linear')
# Define the dimension reduction to be used.
# Here we use a classical univariate feature selection based on F-test,
# namely Anova. When doing full-brain analysis, it is better to use
# SelectPercentile, keeping 5% of voxels
# (because it is independent of the resolution of the data).
from sklearn.feature_selection import SelectPercentile, f_classif
feature_selection = SelectPercentile(f_classif, percentile=5)
# We have our classifier (SVC), our feature selection (SelectPercentile),and now,
# we can plug them together in a *pipeline* that performs the two operations
# successively:
from sklearn.pipeline import Pipeline
anova_svc = Pipeline([('anova', feature_selection), ('svc', svc)])
```
# 总结
假设n为样本总数, m为组数, 方差分析假设不同组的平均数差别来自:
- 实验条件, 也就是说由于不同处理造成的差异, 称为组间差异, 用变量在各个组的均值与总均值偏差的平方和表示, 记作SSb, 组间自由度记做dfb(dfb=m-1)
2.随机误差, 例如测量误差造成的差异,或者个体之间的差异(组内差异), 可以用变量在各组的均值与该组内变量值的偏差平方和表示, 记做SSw, 组内自由度dfw(dfw = n-m)
总偏差的平方和 : SSt = SSb +  SSw
组间均方差: MSw= SSw/dfw
组内均方差: MSb = SSb/dfb
$$ F = \frac{MS_w}{MS_b}$$
F值符合F分布，通过查F分布表，可以得到显著性判断，拒绝H0假设或者接受H0假设。
# 更多阅读:
Anova中的P值F值,正态分布到卡方分布再到F分布:
[https://blog.csdn.net/zhangjipinggom/article/details/82315232](https://blog.csdn.net/zhangjipinggom/article/details/82315232)
详解方差分析: [https://zhuanlan.zhihu.com/p/47175790](https://zhuanlan.zhihu.com/p/47175790)


# sklearn中的朴素贝叶斯分类器 - leofionn的博客 - CSDN博客


2018年05月22日 12:39:16[leofionn](https://me.csdn.net/qq_36142114)阅读数：339


## sklearn中的朴素贝叶斯分类器
之前[理解朴素贝叶斯](http://blog.csdn.net/fontthrone/article/details/78751452)中的结尾对sklearn中的朴素贝叶斯进行了简单的介绍.
此处对sklearn中的则对sklearn中的朴素贝叶斯算法进行比较详细介绍.不过手下还是对朴素贝叶斯本身进行一些补充.
### 朴素贝叶斯算法
朴素贝叶斯算法的数学基础都是围绕贝叶斯定理展开的,因此这一类算法都被称为朴素贝叶斯算法.
朴素贝叶斯的分类原理是通过对象的先验概率,利用贝叶斯公式计算出后验概率.即对象属于某一类的概率.
选择具有后验概率最大的类作为该对象所属的类.同时朴素–’特征为独同分布’,
同时因为先验概率需要我们先假设一个事件分布的概率分布方式(三种),因此也就有了我们在sklearn中对应的三种朴素贝叶斯算法
- 高斯朴素贝叶斯分类器(默认条件概率分布概率符合高斯分布)
- 多项式朴素贝叶斯分类器(条件概率符合多项式分布)
- 伯努利朴素贝叶斯分类器(条件概率符合二项分布)
尽管其假设过于简单，但是在很多实际情况下，朴素贝叶斯工作得很好，特别是文档分类和垃圾邮件过滤。
这些工作都要求一个小的训练集来估计必需参数。
同时相比于其他更复杂的方法，朴素贝叶斯学习器和分类器非常快。
分类条件分布的解耦意味着可以独立单独地把每个特征视为一维分布来估计。这样反过来有助于缓解维度灾难带来的问题。
* 最后总结其特点有以下几个 *
- 属性可以离散可以连续
- 数学基础扎实,分类效率稳定
- 对噪音数据与缺失数据不太敏感
- 属性如果不相关,分类效果很好;如果相关,则不低于决策树
- 假设简单,但是在很多实际情况下表现很好
- 速度较快,一定程度上缓解了维度灾难
```python
%matplotlib inline
```
```python
from
```
```python
sklearn
```
```python
import
```
```python
datasets, model_selection, naive_bayes
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
def
```
```python
load_data
```
```python
(datasets_name=
```
```python
'iris'
```
```python
)
```
```python
:
```
```python
if
```
```python
datasets_name ==
```
```python
'iris'
```
```python
:
        data = datasets.load_iris()
```
```python
# 加载 scikit-learn 自带的 iris 鸢尾花数据集-分类
```
```python
elif
```
```python
datasets_name ==
```
```python
'wine'
```
```python
:
```
```python
# 0.18.2 没有
```
```python
data = datasets.load_wine()
```
```python
# 加载 scikit-learn 自带的 wine 红酒起源数据集-分类
```
```python
elif
```
```python
datasets_name ==
```
```python
'cancer'
```
```python
:
        data = datasets.load_breast_cancer()
```
```python
# 加载 scikit-learn 自带的 乳腺癌数据集-分类
```
```python
elif
```
```python
datasets_name ==
```
```python
'digits'
```
```python
:
        data = datasets.load_digits()
```
```python
# 加载 scikit-learn 自带的 digits 糖尿病数据集-回归
```
```python
elif
```
```python
datasets_name ==
```
```python
'boston'
```
```python
:
        data = datasets.load_boston()
```
```python
# 加载 scikit-learn 自带的 boston 波士顿房价数据集-回归
```
```python
else
```
```python
:
```
```python
pass
```
```python
return
```
```python
model_selection.train_test_split(data.data, data.target,test_size=
```
```python
0.25
```
```python
, random_state=
```
```python
0
```
```python
,stratify=data.target)
```
```python
# 分层采样拆分成训练集和测试集，测试集大小为原始数据集大小的 1/4
```
### 测试sklearn中的朴素贝叶斯算法,sklearn中朴素贝叶斯算法比较简单,只有三种
也正是因为朴素贝叶斯算法比较简单,下面的代码已经基本给出了sklearn中朴素贝叶斯算法的所以有关内容.
你可以运行下面的代码进行测试,同时下面也包括一定的可视化内容.
朴素贝叶斯模型可以解决整个训练集不能导入内存的大规模分类问题。 为了解决这个问题， MultinomialNB, BernoulliNB, 和 GaussianNB 实现了 partial_fit 方法，可以动态的增加数据，使用方法与其他分类器的一样，使用示例见 Out-of-core classification of text documents 。所有的朴素贝叶斯分类器都支持样本权重。
与 fit 方法不同，首次调用 partial_fit 方法需要传递一个所有期望的类标签的列表,之后只需要调用数据即可.
```python
def
```
```python
test_GaussianNB
```
```python
(*data, show=False)
```
```python
:
```
```python
X_train, X_test, y_train, y_test = data
    cls = naive_bayes.GaussianNB()
    cls.fit(X_train, y_train)
```
```python
# print('GaussianNB Training Score: %.2f' % cls.score(X_train, y_train))
```
```python
print(
```
```python
'GaussianNB Testing Score: %.2f'
```
```python
% cls.score(X_test, y_test))
```
```python
def
```
```python
test_MultinomialNB
```
```python
(*data, show=False)
```
```python
:
```
```python
X_train, X_test, y_train, y_test = data
    cls = naive_bayes.MultinomialNB()
    cls.fit(X_train, y_train)
```
```python
# print('MultinomialNB Training Score: %.2f' % cls.score(X_train, y_train))
```
```python
print(
```
```python
'MultinomialNB Testing Score: %.2f'
```
```python
% cls.score(X_test, y_test))
```
```python
def
```
```python
test_MultinomialNB_alpha
```
```python
(*data, show=False)
```
```python
:
```
```python
X_train, X_test, y_train, y_test = data
    alphas = np.logspace(-
```
```python
2
```
```python
,
```
```python
5
```
```python
, num=
```
```python
200
```
```python
)
    train_scores = []
    test_scores = []
```
```python
for
```
```python
alpha
```
```python
in
```
```python
alphas:
        cls = naive_bayes.MultinomialNB(alpha=alpha)
        cls.fit(X_train, y_train)
        train_scores.append(cls.score(X_train, y_train))
        test_scores.append(cls.score(X_test, y_test))
```
```python
if
```
```python
show:
```
```python
## 绘图:MultinomialNB 的预测性能随 alpha 参数的影响
```
```python
fig = plt.figure()
        ax = fig.add_subplot(
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
        ax.plot(alphas, train_scores, label=
```
```python
'Training Score'
```
```python
)
        ax.plot(alphas, test_scores, label=
```
```python
'Testing Score'
```
```python
)
        ax.set_xlabel(
```
```python
r'$\alpha$'
```
```python
)
        ax.set_ylabel(
```
```python
'score'
```
```python
)
        ax.set_ylim(
```
```python
0
```
```python
,
```
```python
1.0
```
```python
)
        ax.set_title(
```
```python
'MultinomialNB'
```
```python
)
        ax.set_xscale(
```
```python
'log'
```
```python
)
        plt.show()
```
```python
# print('MultinomialNB_alpha best train_scores %.2f' % (max(train_scores)))
```
```python
print(
```
```python
'MultinomialNB_alpha best test_scores %.2f'
```
```python
% (max(test_scores)))
```
```python
def
```
```python
test_BernoulliNB
```
```python
(*data, show=False)
```
```python
:
```
```python
X_train, X_test, y_train, y_test = data
    cls = naive_bayes.BernoulliNB()
    cls.fit(X_train, y_train)
```
```python
# print('BernoulliNB Training Score: %.2f' % cls.score(X_train, y_train))
```
```python
print(
```
```python
'BernoulliNB Testing Score: %.2f'
```
```python
% cls.score(X_test, y_test))
```
```python
def
```
```python
test_BernoulliNB_alpha
```
```python
(*data, show=False)
```
```python
:
```
```python
X_train, X_test, y_train, y_test = data
    alphas = np.logspace(-
```
```python
2
```
```python
,
```
```python
5
```
```python
, num=
```
```python
200
```
```python
)
    train_scores = []
    test_scores = []
```
```python
for
```
```python
alpha
```
```python
in
```
```python
alphas:
        cls = naive_bayes.BernoulliNB(alpha=alpha)
        cls.fit(X_train, y_train)
        train_scores.append(cls.score(X_train, y_train))
        test_scores.append(cls.score(X_test, y_test))
```
```python
if
```
```python
show:
```
```python
## 绘图-展示BernoulliNB 的预测性能随 alpha 参数的影响
```
```python
fig = plt.figure()
        ax = fig.add_subplot(
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
        ax.plot(alphas, train_scores, label=
```
```python
'Training Score'
```
```python
)
        ax.plot(alphas, test_scores, label=
```
```python
'Testing Score'
```
```python
)
        ax.set_xlabel(
```
```python
r'$\alpha$'
```
```python
)
        ax.set_ylabel(
```
```python
'score'
```
```python
)
        ax.set_ylim(
```
```python
0
```
```python
,
```
```python
1.0
```
```python
)
        ax.set_title(
```
```python
'BernoulliNB'
```
```python
)
        ax.set_xscale(
```
```python
'log'
```
```python
)
        ax.legend(loc=
```
```python
'best'
```
```python
)
        plt.show()
```
```python
# print('BernoulliNB_alpha best train_scores %.2f' % (max(train_scores)))
```
```python
print(
```
```python
'BernoulliNB_alpha best test_scores %.2f'
```
```python
% (max(test_scores)))
```
```python
def
```
```python
test_BernoulliNB_binarize
```
```python
(*data, show=False)
```
```python
:
```
```python
X_train, X_test, y_train, y_test = data
    min_x = min(np.min(X_train.ravel()), np.min(X_test.ravel())) -
```
```python
0.1
```
```python
max_x = max(np.max(X_train.ravel()), np.max(X_test.ravel())) +
```
```python
0.1
```
```python
binarizes = np.linspace(min_x, max_x, endpoint=
```
```python
True
```
```python
, num=
```
```python
100
```
```python
)
    train_scores = []
    test_scores = []
```
```python
for
```
```python
binarize
```
```python
in
```
```python
binarizes:
        cls = naive_bayes.BernoulliNB(binarize=binarize)
        cls.fit(X_train, y_train)
        train_scores.append(cls.score(X_train, y_train))
        test_scores.append(cls.score(X_test, y_test))
```
```python
if
```
```python
show:
```
```python
## 绘图-展示BernoulliNB 的预测性能随 binarize 参数的影响
```
```python
fig = plt.figure()
        ax = fig.add_subplot(
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
        ax.plot(binarizes, train_scores, label=
```
```python
'Training Score'
```
```python
)
        ax.plot(binarizes, test_scores, label=
```
```python
'Testing Score'
```
```python
)
        ax.set_xlabel(
```
```python
'binarize'
```
```python
)
        ax.set_ylabel(
```
```python
'score'
```
```python
)
        ax.set_ylim(
```
```python
0
```
```python
,
```
```python
1.0
```
```python
)
        ax.set_xlim(min_x -
```
```python
1
```
```python
, max_x +
```
```python
1
```
```python
)
        ax.set_title(
```
```python
'BernoulliNB'
```
```python
)
        ax.legend(loc=
```
```python
'best'
```
```python
)
        plt.show()
```
```python
# print('BernoulliNB_binarize best train_scores %.2f' % (max(train_scores)))
```
```python
print(
```
```python
'BernoulliNB_binarize best test_scores %.2f'
```
```python
% (max(test_scores)))
```
```python
def
```
```python
testFuc
```
```python
(fuc,show = False,no_all = True)
```
```python
:
```
```python
for
```
```python
i
```
```python
in
```
```python
[
```
```python
'iris'
```
```python
,
```
```python
'wine'
```
```python
,
```
```python
'cancer'
```
```python
,
```
```python
'digits'
```
```python
, ]:
            print(
```
```python
'\n====== %s ======\n'
```
```python
% i)          
            X_train, X_test, y_train, y_test = load_data(datasets_name=i)
```
```python
# 产生用于分类问题的数据集
```
```python
if
```
```python
no_all:
                fuc(X_train, X_test, y_train, y_test,show = show)
```
```python
else
```
```python
:
                test_GaussianNB(X_train, X_test, y_train, y_test,show = show)
```
```python
# 调用 test_GaussianNB
```
```python
test_MultinomialNB(X_train,X_test,y_train,y_test,show = show)
```
```python
# 调用 test_MultinomialNB
```
```python
test_MultinomialNB_alpha(X_train, X_test, y_train, y_test,show = show)
```
```python
# 调用 test_MultinomialNB_alpha
```
```python
test_BernoulliNB(X_train,X_test,y_train,y_test,show = show)
```
```python
# 调用 test_BernoulliNB
```
```python
test_BernoulliNB_alpha(X_train, X_test, y_train, y_test,show = show)
```
```python
# 调用 test_BernoulliNB_alpha
```
```python
test_BernoulliNB_binarize(X_train, X_test, y_train, y_test,show = show)
```
```python
# 调用 test_BernoulliNB_binarize
```
* 下面是前面我们定义的用来测试的函数*
`test_GaussianNB(X_train, X_test, y_train, y_test) # 调用 test_GaussianNB`
`test_MultinomialNB(X_train,X_test,y_train,y_test) # 调用 test_MultinomialNB`
`test_MultinomialNB_alpha(X_train, X_test, y_train, y_test) # 调用 test_MultinomialNB_alpha`
`test_BernoulliNB(X_train,X_test,y_train,y_test) # 调用 test_BernoulliNB`
`test_BernoulliNB_alpha(X_train, X_test, y_train, y_test) # 调用 test_BernoulliNB_alpha`
`test_BernoulliNB_binarize(X_train, X_test, y_train, y_test) # 调用 test_BernoulliNB_binarize`

```python
testFuc(test_GaussianNB)
```
1
```python
====== iris ======
GaussianNB Testing Score: 0.97
====== wine ======
GaussianNB Testing Score: 0.96
====== cancer ======
GaussianNB Testing Score: 0.92
====== digits ======
GaussianNB Testing Score: 0.84
```
```python
testFuc(test_MultinomialNB,no_all =
```
```python
False
```
```python
)
```
```python
====== iris ======
GaussianNB Testing Score: 0.97
MultinomialNB Testing Score: 1.00
MultinomialNB_alpha best test_scores 1.00
BernoulliNB Testing Score: 0.32
BernoulliNB_alpha best test_scores 0.32
BernoulliNB_binarize best test_scores 0.92
====== wine ======
GaussianNB Testing Score: 0.96
MultinomialNB Testing Score: 0.82
MultinomialNB_alpha best test_scores 0.84
BernoulliNB Testing Score: 0.40
BernoulliNB_alpha best test_scores 0.40
BernoulliNB_binarize best test_scores 0.69
====== cancer ======
GaussianNB Testing Score: 0.92
MultinomialNB Testing Score: 0.90
MultinomialNB_alpha best test_scores 0.91
BernoulliNB Testing Score: 0.63
BernoulliNB_alpha best test_scores 0.63
BernoulliNB_binarize best test_scores 0.91
====== digits ======
GaussianNB Testing Score: 0.84
MultinomialNB Testing Score: 0.90
MultinomialNB_alpha best test_scores 0.91
BernoulliNB Testing Score: 0.87
BernoulliNB_alpha best test_scores 0.87
BernoulliNB_binarize best test_scores 0.91
```
不同的朴素贝叶斯算法的差异在于其假设的先验概率的不同
数据的先验概率越贴近我们假设的先验概率的时候,我们的模型结果也就越准确
* 对于同一个算法,不同的超参数也有一定的影响,这个你可以通过改变我写的test_fun中的超参数show来进行绘图操作,查看不同超参数的区别 *
## 参考资料
对于朴素贝叶斯原理性的理解可以参考
-[理解朴素贝叶斯](http://blog.csdn.net/fontthrone/article/details/78751452)
-[sklearn中文文档-朴素贝叶斯](http://sklearn.apachecn.org/cn/0.19.0/modules/naive_bayes.html)
- 《python大战机器学习 数据科学家的一个小目标》 华校专,王正林编著

